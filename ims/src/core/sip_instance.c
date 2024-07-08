///// Added for VoWiFi /////
#include <inttypes.h>
#include <unistd.h>
//#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "mem/shm.h"
#include "sip_instance.h"
#include "io_wait.h"
#include "dprint.h"

int vowifi = 1;

void *sender_run(void *data)
{
  instance_t *instance;
  msg_t *msg;
  size_t tbs;
  int asock, offset, sent, tint, tlen;
  uint8_t buf[MAX_MESSAGE_LEN] = {0, };
  uint8_t tmp[MAX_MESSAGE_LEN] = {0, };
  uint8_t *p;
  int i;

  instance = (instance_t *)data;
  msg = NULL;

  asock = instance->asock;
  LM_INFO("sender_run(): instance: %p, asock: %d\n", instance, instance->asock);

  while (instance->running)
  {
    msg = fetch_message_from_send_queue(instance);
    if (msg)
    {
      LM_INFO("[VoWiFi] (sender_run()) msg: %p, mtype: %d\n", msg, msg->mtype);
      // type (1 byte) || ispi (16 bytes) || rspi (16 bytes) 
      // || key || ":" (if there is a value) || value type || ":" || value (until \n)
      p = buf;

      LM_INFO("[VoWiFi] before mtype\n");
      *(p++) = msg->mtype;
      LM_INFO("[VoWiFi] after mtype: %d\n", msg->mtype);

  	  //printf(">>>>> Initiator SPI: 0x%.16"PRIx64"\n", msg->ispi);
	    //printf(">>>>> Responder SPI: 0x%.16"PRIx64"\n", msg->rspi);

      // ispi (16 bytes)
      for (i=0; i<16; i++)
        p[i] = '0';
      p += 16;

      // rspi (16 bytes)
      for (i=0; i<16; i++)
        p[i] = '0';
      p += 16;

      LM_INFO("[VoWiFi] before key (length: %d)\n", msg->klen);
      if (msg->klen > 0)
      {
        memcpy(p, msg->key, msg->klen);
        p += msg->klen;
      }
      LM_INFO("[VoWiFi] after key: %s\n", msg->key);

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
          //printf("*((uint16_t *)(msg->val): %u, tint: %d, tlen: %d\n", 
          //    *((uint16_t *)(msg->val)), tint, tlen);
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

      LM_INFO("[VoWiFi] To be sent: %d bytes\n", tbs);
      LM_INFO("[VoWiFi] Message: %s\n", buf);
      while (offset < tbs)
      {
        sent = write(asock, buf + offset, tbs - offset);
        if (sent > 0)
          offset += sent;
      }
      LM_INFO("[VoWiFi] Sent bytes: %d bytes\n", offset);

      LM_INFO("[VoWiFi] before free_message()\n");
      free_message(msg);
      LM_INFO("[VoWiFi] after free_message()\n");
      msg = NULL;
    }
  }

  return NULL;
}

void *listener_run(void *data)
{
  size_t tbs;
  int i, lsock, asock, flags, offset, sent, rcvd, reading, rc, idx;
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
  arg_t *arg;

  query = NULL;
  depth = 0;
  ptype = 0;
  arg = (arg_t *)data;
  lsock = arg->lsock;

  for (i=1; i<65538; i++)
  {
    shmctl(i, IPC_RMID, NULL);
  }

  LM_INFO("running the listener socket thread: this->lsock: %d\n", lsock);

  asock = accept(lsock, (struct sockaddr *)&addr, &len);

  if (asock < 0)
  {
    LM_ERR("socket failure: %d\n", asock);
  }

  LM_INFO("accept the socket: asock: %d\n", asock);

  flags = fcntl(asock, F_GETFL);
  fcntl(asock, F_SETFL, flags | O_NONBLOCK);

  instance = init_instance(asock);
  rc = pthread_create(arg->sender, arg->attr, sender_run, instance);
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
        LM_ERR("socket error happened()\n");
        goto out;
      }
    }

    LM_INFO("received message (%d bytes): %s\n", offset, buf);

    if (offset == strlen(HELLO_REQUEST) 
        && !strncmp((const char *)buf, HELLO_REQUEST, strlen(HELLO_REQUEST)))
    {
      LM_INFO("received Hello from LogExecutor!\n");

      tbs = strlen(ACK_RESPONSE);
      offset = 0;
      memcpy(buf, ACK_RESPONSE, tbs);

      while (offset < tbs)
      {
        sent = write(asock, buf + offset, tbs - offset);
        if (sent > 0)
          offset += sent;
      }
      LM_INFO("sent ACK to LogExecutor\n");
    }
    else if (offset == strlen(RESET_REQUEST)
        && !strncmp((const char *)buf, RESET_REQUEST, strlen(RESET_REQUEST)))
    {
      LM_INFO("receiver reset from LogExecutor!\n");
      if (instance->query)
        free_query(instance->query);
      instance->query = NULL;
      instance->finished = false;
      instance->rprev = "ike_auth_3_request";
      instance->sprev = "ike_auth_3_response";

      tbs = strlen(ACK_RESPONSE);
      offset = 0;
      memcpy(buf, ACK_RESPONSE, tbs);

      while (offset < tbs)
      {
        sent = write(asock, buf + offset, tbs - offset);
        if (sent > 0)
          offset += sent;
      }
      LM_INFO("sent ACK to LogExecutor\n");
    }
    else if (offset == strlen(FIN_REQUEST)
        && !strncmp((const char *)buf, FIN_REQUEST, strlen(FIN_REQUEST)))
    {
      LM_INFO("receive FIN from LogExecutor!\n");
      instance->finished = true;
      if (instance->query)
        free_query(instance->query);
      instance->query = NULL;
      instance->rprev = "ike_auth_3_request";
      instance->sprev = "ike_auth_3_response";
    }
    else if (offset > 0)
    {
      LM_INFO("The offset is larger than 0\n");
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
      memset(spi, '0', 16);
      if (strncmp((const char *)ispi, (const char *)spi, 16))
      {
        LM_ERR("ERROR: Initiator's SPIs are different\n");
      }

      memcpy(rspi, p, 16);
      p += 16; offset -= 16;
      memset(spi, '0', 16);
      if (strncmp((const char *)rspi, (const char *)spi, 16))
      {
        LM_ERR("ERROR: Responder's SPIs are different\n");
      }

      // if offset == 1, it means that there is only '\n'
      if (offset > 1)
      {
        LM_INFO("Block Start: offset > 1: query: %p\n", query);
        token = (uint8_t *)strtok((char *)p, ":");
        idx = 0;
        while (token)
        {
          switch (idx)
          {
            case 0:
              LM_INFO("query: %p, name: %s\n", query, token);
              set_query_name(query, token);
              break;

            case 1:
              LM_INFO("operator: %s\n", token);
              if (strlen(token) >= 6 && !strncmp(token, "update", 6))
                set_query_operator(query, OP_TYPE_UPDATE);
              else if (strlen(token) >= 4 && !strncmp(token, "drop", 4))
                set_query_operator(query, OP_TYPE_DROP);
              LM_INFO("  the operator is set to %d\n", get_query_operator(query));
              break;

            case 2:
              LM_INFO("value type: %s\n", token);
              set_query_value_type(query, token);
              break;

            case 3:
              LM_INFO("value: %s\n", token);
              set_query_value(query, token);
              break;

            default:
              break;
          }

          token = (uint8_t *)strtok(NULL, ":");
          idx++;
        }
        LM_INFO("Block Finish: offset > 1: query: %p\n", query);
      }

      if (!depth)
      {
        LM_INFO("Block Start: depth == 0: query: %p\n", query);
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
        set_query(instance, query);
        query = NULL;
        LM_INFO("Block Finish: depth == 0, query: %p\n", query);
      }
      else
      {
        LM_INFO("Block Start: depth != 0: query: %p\n", query);
        LM_INFO("Block Finish: depth != 0: query: %p\n", query);
      }
    }
  }

out:
  return NULL;
}

msg_t *add_message_to_send_queue(instance_t *instance, msg_t *msg)
{
  int ret;
  ret = -1;

  pthread_mutex_lock(&(instance->slock));
  if (instance->slast < MAX_QUEUE_LEN)
  {
    instance->sendq[instance->slast++] = msg;
    ret = 1;
  }
  int i;
  LM_DBG(">>>>> Queue (after adding) <<<<<\n");
  for (i=0; i<MAX_QUEUE_LEN; i++)
  {
    LM_DBG("  %d: %d\n", i, instance->sendq[i]);
  }
  LM_DBG("=================\n");
  pthread_mutex_unlock(&(instance->slock));

  return ret;
}

msg_t *fetch_message_from_send_queue(instance_t *instance)
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

    for (i=instance->slast; i<MAX_QUEUE_LEN; i++)
      instance->sendq[i] = 0;
  }

  if (ret > 0)
  {
    LM_DBG(">>>>> Queue (after fetch) <<<<<\n");
    for (i=0; i<MAX_QUEUE_LEN; i++)
    {
      LM_DBG("  %d: %d\n", i, instance->sendq[i]);
    }
    LM_DBG("=================\n");
  }

  pthread_mutex_unlock(&(instance->slock));

  return ret;
}

void set_query(instance_t *instance, query_t *query)
{
  instance->query = query;
}

msg_t *init_message(instance_t *instance, int mtype, const uint8_t *key, 
    int vtype, uint8_t *val, int vlen)
{
  uint64_t ispi, rspi;
  msg_t *ret;

  ispi = 0;
  rspi = 0;

  ret = (msg_t *)shm_malloc(sizeof(msg_t));
  if (!ret)
  {
    perror("shm_malloc() failed");
    exit(1);
  }
  memset(ret, 0, sizeof(msg_t));

  ret->mtype = mtype;
  ret->ispi = ispi;
  ret->rspi = rspi;

  if (key)
  {
    memcpy(ret->key, key, strlen(key));
    ret->klen = strlen(key);
  }

  ret->vtype = vtype;

  if (val)
  {
    memcpy(ret->val, val, ret->vlen);
    ret->vlen = vlen;
  }

  LM_INFO("[init_message()] ret: %p, mtype: %d\n", ret, mtype);

  return ret;
}

void free_message(msg_t *msg)
{
  shm_free(msg);
}

instance_t *get_instance()
{
  int shmid;
  instance_t *ret;

  ret = NULL;
  shmid = shmget((key_t)SHARED_MEMORY_INSTANCE_KEY, sizeof(instance_t), 0666);
  if (shmid == -1)
  {
    LM_ERR("[VoWiFi] error in shmget() at get_instance()\n");
  }
  else
  {
    ret = (instance_t *)shmat(shmid, NULL, 0);
    LM_DBG("[VoWiFi] instance at: %p\n", ret);
  }

  return ret;
}

instance_t *init_instance(int asock)
{
  int shmid;
  instance_t *ret;

  shmid = shmget((key_t)SHARED_MEMORY_INSTANCE_KEY, sizeof(instance_t), 0666|IPC_CREAT);
  if (shmid == -1)
  {
    perror("shmget failed");
    exit(0);
  }
  
  ret = (instance_t *)shmat(shmid, NULL, 0);
  if (ret == -1)
  {
    perror("shmat failed");
    exit(0);
  }
  LM_INFO("[VoWiFi] address of instance: %p\n", ret);
  memset(ret, 0, sizeof(instance_t));

  ret->asock = asock;

  ret->finished = false;
  if (pthread_mutex_init(&(ret->slock), NULL) != 0)
  {
    LM_ERR("initializing a mutex for the send queue failed\n");
  }

  ret->rprev = "ike_auth_3_request";
  ret->sprev = "ike_auth_3_response";
  ret->pid = getpid();
  ret->running = 1;

  return ret;
}

void free_instance(instance_t *instance)
{
  shmctl(SHARED_MEMORY_INSTANCE_KEY, IPC_RMID, NULL);
}

int check_instance(instance_t *instance)
{
  int ret;
  pid_t pid;
  ret = 0;
  pid = getpid();

  if (!instance)
    goto out;

  LM_ERR("instance->pid: %d, pid: %d\n", instance->pid, pid);
  if (pid >= (instance->pid - 5) && pid <= (instance->pid + 5))
    ret = 1;

out:
  return ret;
}

query_t *init_query()
{
  LM_INFO("Start: init_query()\n");
  query_t *ret;

  ret = (query_t *)shm_malloc(sizeof(query_t));
  if (!ret)
  {
    perror("shm_malloc() for query failed");
    exit(1);
  }
  memset(ret, 0, sizeof(query_t));

  LM_INFO("Finish: init_query()\n");
  return ret;
}

void free_query(query_t *query)
{
  query_t *curr, *next;

  if (query)
  {
    curr = query->sub;
    
    while (curr)
    {
      next = curr->next;
      free_query(curr);
      curr = next;
    }

    shm_free(query);
  }
}

void _print_query(query_t *query, int depth)
{
  LM_INFO("Start: _print_query(): query: %p, depth: %d\n", query, depth);
  query_t *sub;
  int i, tlen;
  uint8_t buf[MAX_MESSAGE_LEN] = {0, };
  uint8_t *p, *tmp;
  uint8_t *val;

  p = buf;
  for (i=0; i<depth; i++)
    p += 2;
  
  tmp = get_query_name(query, &tlen);
  if (tlen > 0)
  {
    snprintf(p, tlen + 1, "%s", tmp);
    p += tlen;
  }

  val = get_query_value(query, &tlen);
  if (tlen > 0)
  {
    *(p++) = ':';
    snprintf(p, tlen + 1, "%s", val);
    p += tlen;
  }

  LM_INFO("%s\n", buf);

  depth += 1;
  sub = query->sub;
  while (sub) 
  {
    _print_query(sub, depth);
    sub = sub->next;
  }
  depth -= 1;

  memset(buf, 0, MAX_MESSAGE_LEN);
  p = buf;
  for (i=0; i<depth; i++)
    p += 2;
  *(p++) = 'e';
  *(p++) = 'n';
  *(p++) = 'd';
  LM_INFO("%s\n", buf);
  LM_INFO("Finish: _print_query()\n");
}

void print_query(query_t *query)
{
  LM_INFO(">>>>> print_query() <<<<<\n");
  _print_query(query, 0);
  LM_INFO(">>>>>>>>>>>>><<<<<<<<<<<<\n");
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
  LM_INFO("set_query_name()> name: %s, nlen: %d\n", name, nlen);
  memcpy(query->name, name, nlen);
  LM_INFO("set_query_name()> query->name: %s\n", query->name);
  query->nlen = nlen;
}

int get_query_operator(query_t *query)
{
  return query->op;
}

void set_query_operator(query_t *query, int op)
{
  query->op = op;
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
  LM_INFO("_value()> value: %s, vlen: %d\n", value, vlen);
  if (value[vlen-1] == '\n')
    vlen -= 1;
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

query_t *get_next_query(instance_t *instance)
{
  query_t *ret;
  ret = NULL;

  LM_ERR("instance->running: %d, instance->finished: %d\n", instance->running, instance->finished);
  if (instance->running && !instance->finished)
  {
    ret = get_query(instance);
    LM_ERR("query: %p\n", ret);
    while (!ret || is_query_name(ret, instance->sprev))
    {
      if (instance->finished)
        break;

      if (ret)
      {
        LM_INFO("[VoWiFi] query->name: %s\n", ret->name);
        LM_INFO("[VoWiFi] instance->sprev: %s\n", instance->sprev);
      }

      sleep(1);
      ret = get_query(instance);
    }
  }

  return ret;
}

query_t *get_query(instance_t *instance)
{
  LM_INFO("[VoWiFi] instance: %p\n", instance);
  LM_INFO("[VoWiFi] instance->query: %p\n", instance->query);
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

  LM_INFO("[VoWiFi] qname (%d bytes): %s, name (%d bytes): %s, ret: %d\n", qlen, qname, nlen, name, ret);

  return ret;
}

query_t *get_sub_query_by_name(query_t *query, const uint8_t *name)
{
  query_t *ret, *curr;

  LM_INFO("[VoWiFi] ike_sa_instance.c: get_sub_query_by_name(): query: %p, name: %s\n", query, name);
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
