///// Added for VoWiFi /////
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "sip_instance.h"

void *sender_run(void *data)
{
  instance_t *instance;
  msg_t *msg;
  size_t tbs;
  int asock, offset, sent, tint, tlen;
  uint8_t buf[MAX_MESSAGE_LEN] = {0, };
  uint8_t tmp[MAX_MESSAGE_LEN] = {0, };
  uint8_t *p;

  instance = (instance_t *)data;
  msg = NULL;

  asock = instance->asock;

  while (instance->running)
  {
    msg = instance->fetch_message_from_send_queue(instance);
    if (msg)
    {
      // type (1 byte) || ispi (16 bytes) || rspi (16 bytes) 
      // || key || ":" (if there is a value) || value type || ":" || value (until \n)
      p = buf;

      *(p++) = msg->mtype;

	    //printf(">>>>> Initiator SPI: 0x%.16"PRIx64"\n", msg->ispi);
	    //printf(">>>>> Responder SPI: 0x%.16"PRIx64"\n", msg->rspi);

      snprintf((char *)p, 17, "%.16"PRIx64, msg->ispi); 
      p += 16;
      snprintf((char *)p, 17, "%.16"PRIx64, msg->rspi); 
      p += 16;

      if (msg->klen > 0)
      {
        memcpy(p, msg->key, msg->klen);
        p += msg->klen;
      }

      if (msg->vlen > 0)
      {
        memcpy(p, ":", 1);
        p += 1;

        tlen = int_to_char(msg->vtype, tmp, 10);
        memcpy(p, tmp, tlen);
        p += tlen;

        memcpy(p, ":", 1);
        p += 1;

        if (msg->vtype == VAL_TYPE_INTEGER)
        {
          tint = *((int *)(msg->val));
          tlen = int_to_char(tint, tmp, 10);
          memcpy(p, tmp, tlen);
          p += tlen;
        }
        else if (msg->vtype == VAL_TYPE_UINT16)
        {
          tint = *((uint16_t *)(msg->val));
          tlen = int_to_char(tint, tmp, 10);
          printf("*((uint16_t *)(msg->val): %u, tint: %d, tlen: %d\n", 
              *((uint16_t *)(msg->val)), tint, tlen);
          memcpy(p, tmp, tlen);
          p += tlen;
        }
        else if (msg->vtype == VAL_TYPE_STRING)
        {
          memcpy(p, msg->val, msg->vlen);
          p += msg->vlen;
        }
      }

      memcpy(p, "\n", 1);
      p += 1;

      tbs = p - buf;
      offset = 0;

      while (offset < tbs)
      {
        sent = write(asock, buf + offset, tbs - offset);
        if (sent > 0)
          offset += sent;
      }
      free_message(msg);
      msg = NULL;
    }
  }

  return NULL;
}

void *listener_run(void *data)
{
  size_t tbs;
  int lsock, asock, flags, offset, sent, rcvd, reading, rc, idx;
  struct sockaddr_in addr;
  socklen_t len = sizeof(addr);
  uint8_t buf[MAX_MESSAGE_LEN];
  uint8_t *p, *token;
  uint8_t ptype, mtype;
  uint8_t ispi[17] = {0, };
  uint8_t rspi[17] = {0, };
  uint8_t spi[17] = {0, };
  int depth;
  instance_t *instance;
  query_t *query;

  query = NULL;
  depth = 0;
  ptype = 0;

  printf("running the listener socket thread: this->lsock: %d\n", lsock);

  asock = accept(lsock, (struct sockaddr *)&addr, &len);

  if (asock < 0)
  {
    perror("socket failure");
  }

  printf("accept the socket: asock: %d\n", asock);

  flags = fcntl(asock, F_GETFL);
  fcntl(asock, F_SETFL, flags | O_NONBLOCK);

  instance = init_instance(asock);
  printf("socket with LogExecutor is set: asock: %d\n", asock);

  rc = pthread_create(listener, attr, sender_run, instance);
  if (rc < 0)
    perror("error in pthread_create");

  while (instance->running)
  {
    reading = 1;
    offset = 0;
    memset(buf, 0, MAX_MESSAGE_LEN);
    while (reading)
    {
      rcvd = read(asock, buf + offset, 1);
      if (rcvd > 0)
      {
        if (buf[offset] == '\n')
          reading = 0;
        offset += rcvd;
      }
      else if (rcvd == 0)
      {
        printf("socket error happened()\n");
        goto out;
      }
    }

    printf("received message (%d bytes): %s\n", offset, buf);

    if (offset == strlen(HELLO_REQUEST) 
        && !strncmp((const char *)buf, HELLO_REQUEST, strlen(HELLO_REQUEST)))
    {
      printf("received Hello from LogExecutor!\n");

      tbs = strlen(ACK_RESPONSE);
      offset = 0;
      memcpy(buf, ACK_RESPONSE, tbs);

      while (offset < tbs)
      {
        sent = write(asock, buf + offset, tbs - offset);
        if (sent > 0)
          offset += sent;
      }
      printf("sent ACK to LogExecutor\n");
    }
    else if (offset == strlen(RESET_REQUEST)
        && !strncmp((const char *)buf, RESET_REQUEST, strlen(RESET_REQUEST)))
    {
      printf("receiver reset from LogExecutor!\n");
      instance->ispi = 0;
      instance->rspi = 0;

      tbs = strlen(ACK_RESPONSE);
      offset = 0;
      memcpy(buf, ACK_RESPONSE, tbs);

      while (offset < tbs)
      {
        sent = write(asock, buf + offset, tbs - offset);
        if (sent > 0)
          offset += sent;
      }
      printf("sent ACK to LogExecutor\n");
    }
    else if (offset > 0)
    {
      p = buf;
      mtype = char_to_int((char *)p, 1, 10);
      p++;

      switch (mtype)
      {
        case MSG_TYPE_ATTRIBUTE:
          query = add_query_sub_message(query, ptype, mtype);
          break;

        case MSG_TYPE_BLOCK_START:
          if (!query)
            query = init_query();
          else
            query = add_query_sub_message(query, ptype, mtype);
          depth++;
          break;

        case MSG_TYPE_BLOCK_END:
          if (has_query_parent(query))
            query = get_query_parent(query);
          depth--;
        default:
          break;
      }
      ptype = mtype;
        
      offset -= 1;
      memcpy(ispi, p, 16);
      p += 16; offset -= 16;
      snprintf((char *)spi, 17, "%.16"PRIx64, instance->ispi); 
      if (strncmp((const char *)ispi, (const char *)spi, 16))
      {
        printf("ispi: %s, instance->ispi: %s\n", ispi, spi);
        printf("ERROR: Initiator's SPIs are different\n");
      }

      memcpy(rspi, p, 16);
      p += 16; offset -= 16;
      snprintf((char *)spi, 17, "%.16"PRIx64, instance->rspi); 
      if (strncmp((const char *)rspi, (const char *)spi, 16))
      {
        printf("rspi: %s, instance->rspi: %s\n", rspi, spi);
        printf("ERROR: Responder's SPIs are different\n");
      }

      // if offset == 1, it means that there is only '\n'
      if (offset > 1)
      {
        token = (uint8_t *)strtok((char *)p, ":");
        idx = 0;
        while (token)
        {
          switch (idx)
          {
            case 0:
              printf("query: %p, name: %s\n", query, token);
              set_query_name(query, token);
              break;

            case 1:
              printf("value type: %s\n", token);
              set_query_value_type(query, token);
              break;

            case 2:
              printf("value: %s\n", token);
              set_query_value(query, token);
              break;

            default:
              break;
          }

          token = (uint8_t *)strtok(NULL, ":");
          idx++;
        }
      }

      if (!depth)
      {
        print_query(query);
        /*
        tbs = strlen(ACK_RESPONSE);
        offset = 0;
        memcpy(buf, ACK_RESPONSE, tbs);

        while (offset < tbs)
        {
          sent = write(asock, buf + offset, tbs - offset);
          if (sent > 0)
            offset += sent;
        }
        printf("sent ACK to LogExecutor\n");
        */
        instance->set_query(instance, query);
      }
    }
  }

out:
  return NULL;
}

int check_instance(instance_t *instance, uint64_t ispi, uint64_t rspi, int update)
{
  int ret;

  if (instance)
    printf("\n[check_instance] instance->ispi: %.16"PRIx64", instance->rspi: %.16"PRIx64", ispi: %.16"PRIx64", rspi: %.16"PRIx64"\n\n", instance->ispi, instance->rspi, ispi, rspi);

  if (!instance) 
    ret = 0;
  else if (!(instance->ispi) && !(instance->rspi))
  {
    if (update)
    {
      instance->ispi = ispi;
      instance->rspi = rspi;
    }
    ret = 1;
  }
  else if (instance->ispi == ispi && instance->rspi == rspi)
    ret = 1;
  else
    ret = 0;

  return ret;
}

int _add_message_to_send_queue(instance_t *instance, msg_t *msg)
{
  int ret;
  ret = -1;

  pthread_mutex_lock(&(instance->slock));
  if (instance->slast < MAX_QUEUE_LEN)
  {
    instance->sendq[instance->slast++] = msg;
    ret = 1;
  }
  pthread_mutex_unlock(&(instance->slock));

  return ret;
}

msg_t *_fetch_message_from_send_queue(instance_t *instance)
{
  int i;
  msg_t *ret;
  ret = NULL;

  pthread_mutex_lock(&(instance->slock));
  if (instance->slast > 0)
  {
    ret = instance->sendq[0];
    for (i=1; i<instance->slast; i++)
      instance->sendq[i-1] = instance->sendq[i];
    instance->slast--;
  }
  pthread_mutex_unlock(&(instance->slock));

  return ret;
}

void _set_query(instance_t *instance, query_t *query)
{
  instance->query = query;
}

msg_t *init_message(instance_t *instance, int mtype, const uint8_t *key, 
    int vtype, void *val, int vlen)
{
  msg_t *ret;
  uint64_t ispi, rspi;

  ispi = instance->ispi;
  rspi = instance->rspi;

  ret = (msg_t *)calloc(1, sizeof(msg_t));
  ret->mtype = mtype;
  ret->ispi = ispi;
  ret->rspi = rspi;

  if (key)
  {
    memcpy(ret->key, key, strlen((const char *)key));
    ret->klen = strlen((const char *)key);
  }

  ret->vtype = vtype;
  ret->val = val;
  ret->vlen = vlen;

  return ret;
}

void free_message(msg_t *msg)
{
  if (msg)
  {
    printf("msg: %p\n", msg);

    if (msg->val)
    {
      free(msg->val);
      msg->val = NULL;
    }

    free(msg);
  }
}

instance_t *init_instance(int asock)
{
  instance_t *ret;
  ret = (instance_t *)calloc(1, sizeof(instance_t));
  ret->asock = asock;

  ret->add_message_to_send_queue = _add_message_to_send_queue;
  ret->fetch_message_from_send_queue = _fetch_message_from_send_queue;
  ret->set_query = _set_query;
  ret->finished = false;
  if (pthread_mutex_init(&(ret->slock), NULL) != 0)
  {
    printf("initializing a mutex for the send queue failed\n");
  }

  ret->running = 1;

  return ret;
}

void free_instance(instance_t *instance)
{
  int i;
  if (instance)
  {
    for (i=0; i<instance->slast; i++)
    {
      free_message(instance->sendq[i]);
    }
    
    pthread_mutex_destroy(&(instance->slock));
    free(instance);
  }
}

query_t *init_query(void)
{
  query_t *ret;
  ret = (query_t *) calloc(1, sizeof(query_t));
  return ret;
}

void free_query(query_t *query)
{
  query_t *curr, *next;

  if (query)
  {
    if (query->name)
      free(query->name);

    if (query->value)
      free(query->value);
    
    curr = query->sub;
    
    while (curr)
    {
      next = curr->next;
      free_query(curr);
      curr = next;
    }

    free(query);
  }
}

void _print_query(query_t *query, int depth)
{
  query_t *sub;
  int i, tlen;
  uint8_t buf[MAX_MESSAGE_LEN] = {0, };
  uint8_t *p, *tmp;
  uint8_t *val;

  for (i=0; i<depth; i++)
    printf("  ");
  
  p = buf;
  tmp = get_query_name(query, &tlen);
  if (tlen > 0)
  {
    snprintf((char *)p, tlen+1, "%s", tmp);
    p += tlen;
  }

  val = get_query_value(query, &tlen);
  if (tlen > 0)
  {
    memcpy(p, ":", 1);
    p += 1;
    snprintf((char *)p, tlen+1, "%s", val);
    p += tlen;
  }

  printf("%s\n", buf);

  depth += 1;
  sub = query->sub;
  while (sub) 
  {
    _print_query(sub, depth);
    sub = sub->next;
  }
  depth -= 1;

  for (i=0; i<depth; i++)
    printf("  ");
  printf("end\n");
}

void print_query(query_t *query)
{
  printf(">>>>> print_query() <<<<<\n");
  _print_query(query, 0);
  printf(">>>>>>>>>>>>><<<<<<<<<<<<\n");
}

query_t *add_query_sub_message(query_t *query, int ptype, int ctype)
{
  query_t *sub;
  if (ctype == MSG_TYPE_ATTRIBUTE && ptype == MSG_TYPE_ATTRIBUTE)
  {
    if (has_query_parent(query))
      query = get_query_parent(query);
    else
    {
      perror("error: there is no parent");
      exit(1);
    }
  }
  sub = init_query();
  set_query_parent(sub, query);
  
  if (query->sub)
    sub->next = query->sub;
  query->sub = sub;

  return sub;
}

int has_query_parent(query_t *query)
{
  int ret;
  ret = 0;

  if (query->parent)
    ret = 1;
  
  return ret;
}

query_t *get_query_parent(query_t *query)
{
  return query->parent;
}

void set_query_parent(query_t *query, query_t *parent)
{
  query->parent = parent;
}

uint8_t *get_query_name(query_t *query, int *nlen)
{
  (*nlen) = query->nlen;
  return query->name;
}

void set_query_name(query_t *query, uint8_t *name)
{
  int nlen = (int) strlen((const char *)name);
  if (name[nlen-1] == '\n')
    nlen -= 1;
  printf("set_query_name()> name: %s, nlen: %d\n", name, nlen);
  query->name = (uint8_t *)calloc(nlen+1, sizeof(uint8_t));
  memcpy(query->name, name, nlen);
  printf("set_query_name()> query->name: %s\n", query->name);
  query->nlen = nlen;
}

int get_query_value_type(query_t *query)
{
  return query->vtype;
}

void set_query_value_type(query_t *query, uint8_t *vtstr)
{
  int vtlen = (int) strlen((const char *)vtstr);
  query->vtype = char_to_int((char *)vtstr, vtlen, 10);
}

uint8_t *get_query_value(query_t *query, int *vlen)
{
  (*vlen) = query->vlen;
  return query->value;
}

void set_query_value(query_t *query, uint8_t *value)
{
  int vlen = (int) strlen((const char *)value);
  printf("set_query_value()> value: %s, vlen: %d\n", value, vlen);
  if (value[vlen-1] == '\n')
    vlen -= 1;
  query->value = (uint8_t *)calloc(vlen+1, sizeof(uint8_t));
  memcpy(query->value, value, vlen);
  query->vlen = vlen;
}

bool has_query(instance_t *instance)
{
  return instance->query != NULL;
}

bool wait_query(instance_t *instance)
{
  volatile bool ret;
  ret = false;

  while (!has_query(instance)) {};

  if (has_query(instance))
    ret = true;
  else if (is_query_finished(instance))
    ret = false;

  return ret;
}

bool is_query_finished(instance_t *instance)
{
  return instance->finished;
}

query_t *get_query(instance_t *instance)
{
  printf("[VoWiFi] ike_sa_instance.c: instance: %p\n", instance);
  printf("[VoWiFi] ike_sa_instance.c: instance->query: %p\n", instance->query);
  return instance->query;
}

bool is_query_name(query_t *query, const uint8_t *name)
{
  int qlen, nlen;
  uint8_t *qname;
  bool ret;

  ret = false;
  qname = query->name;
  qlen = query->nlen;
  nlen = (int) strlen((const char *)name);

  if ((qlen == nlen) && !strncmp((const char *)qname, (const char *)name, qlen))
    ret = true;

  printf("[VoWiFi] qname (%d bytes): %s, name (%d bytes): %s, ret: %d\n", qlen, qname, nlen, name, ret);

  return ret;
}

query_t *get_sub_query_by_name(query_t *query, const uint8_t *name)
{
  query_t *ret, *curr;

  printf("[VoWiFi] ike_sa_instance.c: get_sub_query_by_name(): query: %p, name: %s\n", query, name);
  ret = NULL;
  curr = query->sub;

  while (curr)
  {
    if (is_query_name(curr, name))
    {
      ret = curr;
      break;
    }
    curr = curr->next;
  }

  return ret;
}

int int_to_char(int num, uint8_t *str, int base)
{
  int i, tmp, rem, ret;

  ret = 0;
  tmp = num;
  while (tmp > 0)
  {
    rem = tmp % base;
    if (rem > 0)
      ret++;
    tmp /= base;
  }

  tmp = num;
  for (i=0; i<ret; i++)
  {
    rem = tmp % base;
    if (rem >= 0 && rem <= 9)
      str[ret - i - 1] = rem + 48;
    if (rem >= 10)
      str[ret - i - 1] = rem + 87;
    tmp /= base;
  }

  return ret;
}

int char_to_int(char *str, int slen, int base)
{
  int i;
  int ret = 0;
  char ch;

  if (!slen) goto out;

  for (i=0; i<slen; i++)
  {
    ch = str[i];
    if (ch == ' ')
      break;

    ret = ret * base + (ch - 48);
  }

out:
  return ret;
}
/////////////////////////
