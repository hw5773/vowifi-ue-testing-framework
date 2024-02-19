///// Added for VoWiFi /////
#include <inttypes.h>
#include <unistd.h>
//#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/shm.h>

#include "sip_instance.h"
#include "io_wait.h"
#include "dprint.h"

int vowifi = 1;

void *sender_run(void *data)
{
  instance_t *instance;
  key_t kid; 
  int shmid;
  msg_t *msg;
  size_t tbs;
  int asock, offset, sent, tint, tlen;
  uint8_t buf[MAX_MESSAGE_LEN] = {0, };
  uint8_t tmp[MAX_MESSAGE_LEN] = {0, };
  uint8_t *p;

  instance = (instance_t *)data;
  msg = NULL;

  asock = instance->asock;
  LM_INFO("sender_run(): instance: %p, asock: %d\n", instance, instance->asock);

  while (instance->running)
  {
    kid = fetch_message_from_send_queue(instance);
    if (kid >= SHARED_MEMORY_MESSAGE_BASE)
    {
      shmid = shmget((key_t)kid, sizeof(msg_t), 0666);
      if (shmid == -1)
      {
        LM_ERR("[VoWiFi] error in shmid\n");
        continue;
      }
      msg = (msg_t *)shmat(shmid, NULL, 0);
      if (msg)
      {
        LM_INFO("[VoWiFi] (sender_run()) kid: %d, mtype: %d\n", kid, msg->mtype);
        // type (1 byte) || ispi (16 bytes) || rspi (16 bytes) 
        // || key || ":" (if there is a value) || value type || ":" || value (until \n)
        p = buf;

        LM_INFO("[VoWiFi] before mtype\n");
        *(p++) = msg->mtype;
        LM_INFO("[VoWiFi] after mtype: %d\n", msg->mtype);

  	    //printf(">>>>> Initiator SPI: 0x%.16"PRIx64"\n", msg->ispi);
	      //printf(">>>>> Responder SPI: 0x%.16"PRIx64"\n", msg->rspi);

        snprintf((char *)p, 17, "%.16"PRIx64, msg->ispi); 
        p += 16;
        snprintf((char *)p, 17, "%.16"PRIx64, msg->rspi); 
        p += 16;

        LM_INFO("[VoWiFi] before key\n");
        if (msg->klen > 0)
        {
          memcpy(p, msg->key, msg->klen);
          p += msg->klen;
        }
        LM_INFO("[VoWiFi] after key: %s\n", msg->key);

        LM_INFO("[VoWiFi] sender_run 1\n");
        if (msg->vlen > 0)
        {
        LM_INFO("[VoWiFi] sender_run 2\n");
          memcpy(p, ":", 1);
          p += 1;

        LM_INFO("[VoWiFi] sender_run 3\n");
          tlen = int_to_char(msg->vtype, tmp, 10);
          memcpy(p, tmp, tlen);
          p += tlen;

        LM_INFO("[VoWiFi] sender_run 4\n");
          memcpy(p, ":", 1);
          p += 1;

        LM_INFO("[VoWiFi] sender_run 5\n");
          if (msg->vtype == VAL_TYPE_INTEGER)
          {
        LM_INFO("[VoWiFi] sender_run 6\n");
            tint = *((int *)(msg->val));
            tlen = int_to_char(tint, tmp, 10);
            memcpy(p, tmp, tlen);
            p += tlen;
          }
          else if (msg->vtype == VAL_TYPE_UINT16)
          {
        LM_INFO("[VoWiFi] sender_run 7\n");
            tint = *((uint16_t *)(msg->val));
            tlen = int_to_char(tint, tmp, 10);
            //printf("*((uint16_t *)(msg->val): %u, tint: %d, tlen: %d\n", 
            //    *((uint16_t *)(msg->val)), tint, tlen);
            memcpy(p, tmp, tlen);
            p += tlen;
          }
          else if (msg->vtype == VAL_TYPE_STRING)
          {
        LM_INFO("[VoWiFi] sender_run 8\n");
            memcpy(p, msg->val, msg->vlen);
            p += msg->vlen;
          }
        LM_INFO("[VoWiFi] sender_run 9\n");
        }
        LM_INFO("[VoWiFi] sender_run 10\n");

        memcpy(p, "\n", 1);
        p += 1;

        LM_INFO("[VoWiFi] sender_run 11\n");
        tbs = p - buf;
        offset = 0;

        LM_INFO("[VoWiFi] sender_run 12\n");
        LM_INFO("[VoWiFi] To be sent: %d bytes\n", tbs);
        while (offset < tbs)
        {
          sent = write(asock, buf + offset, tbs - offset);
          if (sent > 0)
            offset += sent;
        }
        LM_INFO("[VoWiFi] Sent bytes: %d bytes\n", offset);

        shmdt(msg);
        free_message(kid);
        msg = NULL;
      }
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
      LM_INFO("sent ACK to LogExecutor\n");
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
        LM_INFO("ispi: %s, instance->ispi: %s\n", ispi, spi);
        LM_ERR("ERROR: Initiator's SPIs are different\n");
      }

      memcpy(rspi, p, 16);
      p += 16; offset -= 16;
      snprintf((char *)spi, 17, "%.16"PRIx64, instance->rspi); 
      if (strncmp((const char *)rspi, (const char *)spi, 16))
      {
        LM_INFO("rspi: %s, instance->rspi: %s\n", rspi, spi);
        LM_ERR("ERROR: Responder's SPIs are different\n");
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
        set_query(instance, query);
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
    LM_INFO("\n[check_instance] instance->ispi: %.16"PRIx64", instance->rspi: %.16"PRIx64", ispi: %.16"PRIx64", rspi: %.16"PRIx64"\n\n", instance->ispi, instance->rspi, ispi, rspi);

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

int add_message_to_send_queue(instance_t *instance, key_t kid)
{
  int ret;
  ret = -1;

  pthread_mutex_lock(&(instance->slock));
  if (instance->slast < MAX_QUEUE_LEN)
  {
    instance->sendq[instance->slast++] = kid;
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

int fetch_message_from_send_queue(instance_t *instance)
{
  int i;
  key_t ret;
  ret = 0;

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

key_t init_message(instance_t *instance, int mtype, const uint8_t *key, 
    int vtype, uint8_t *val, int vlen)
{
  int shmid;
  key_t ret;
  uint64_t ispi, rspi;
  msg_t *msg;

  ispi = instance->ispi;
  rspi = instance->rspi;

  ret = SHARED_MEMORY_MESSAGE_BASE + instance->num;
  instance->num++;
  shmid = shmget((key_t)ret, sizeof(msg_t), 0666 | IPC_CREAT);
  if (shmid == -1)
  {
    perror("shmget failed");
    exit(1);
  }

  msg = (msg_t *)shmat(shmid, NULL, 0);
  if (msg == (msg_t *)-1)
  {
    perror("shmat failed");
    exit(1);
  }
  msg->mtype = mtype;
  msg->ispi = ispi;
  msg->rspi = rspi;

  if (key)
  {
    memcpy(msg->key, key, strlen((const char *)key));
    msg->klen = strlen((const char *)key);
  }

  msg->vtype = vtype;
  memcpy(msg->val, val, vlen);
  msg->vlen = vlen;

  LM_INFO("[init_message()] kid: %d, mtype: %d\n", ret, mtype);

  return ret;
}

void free_message(key_t kid)
{
  shmctl(kid, IPC_RMID, NULL);
}

instance_t *init_instance(int asock)
{
  int shmid;
  instance_t *ret;

  ret = (instance_t *)calloc(1, sizeof(instance_t));

  shmid = shmget((key_t)SHARED_MEMORY_INSTANCE_KEY, sizeof(instance_t), 0666 | IPC_CREAT);
  if (shmid == -1)
  {
    perror("shmget failed");
    exit(0);
  }

  ret = (instance_t *)shmat(shmid, NULL, 0);
  if (ret == (instance_t *)-1)
  {
    perror("shmat failed");
    exit(0);
  }

  LM_INFO("[VoWiFi] address of instance: %p\n", ret);

  ret->asock = asock;

  ret->finished = false;
  if (pthread_mutex_init(&(ret->slock), NULL) != 0)
  {
    LM_ERR("initializing a mutex for the send queue failed\n");
  }

  ret->running = 1;

  return ret;
}

void free_instance(instance_t *instance)
{
  shmctl(SHARED_MEMORY_INSTANCE_KEY, IPC_RMID, NULL);
}

query_t *init_query(void)
{
  int shmid;
  query_t *ret;

  shmid = shmget((key_t)SHARED_MEMORY_QUERY_KEY, sizeof(query_t), 0666 | IPC_CREAT);
  if (shmid == -1)
  {
    perror("shmget failed");
    exit(0);
  }
  ret = (query_t *) shmat(shmid, NULL, 0);

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
  LM_INFO("set_query_name()> name: %s, nlen: %d\n", name, nlen);
  query->name = (uint8_t *)calloc(nlen+1, sizeof(uint8_t));
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
  LM_INFO("set_query_value()> value: %s, vlen: %d\n", value, vlen);
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
  LM_INFO("[VoWiFi] ike_sa_instance.c: instance: %p\n", instance);
  LM_INFO("[VoWiFi] ike_sa_instance.c: instance->query: %p\n", instance->query);
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

void parse_sip_message(instance_t *instance, uint8_t *buf, size_t len)
{
  int first_line;
  uint8_t *start, *p;
  const uint8_t *symbol;
  const uint8_t *version;
  uint8_t receiver[1024] = {0, };
  uint8_t attr[1024] = {0, };
  uint8_t key[1024] = {0, };
  uint8_t mem[1024] = {0, };
  msg_t *msg;
  key_t kid;

  int receiver_set;
  int attr_set;
  int key_set;
  int within;

  first_line = 1;
  start = buf;
  p = buf;
  symbol = version = NULL;
  receiver_set = 0;
  attr_set = 0;
  key_set = 0;
  within = 0;
  
  LM_INFO("[VoWiFi] before parsing the message\n");
  while (p - buf < len)
  {
    if (*p == ' ')
    {
      if (first_line)
      {
        if (!symbol)
        {
          memcpy(mem, start, p - start);
          start = p + 1;

          if (!strncmp(mem, "REGISTER", strlen("REGISTER")))
          {
            symbol = "register";
          }
          else
          {
            symbol = "unknown";
          }
        }
        else if (!receiver_set)
        {
          memcpy(receiver, start, p - start);
          start = p + 1;
          receiver_set = 1;
        }
        else if (!version)
        {
          memcpy(mem, start, p - start);
          start = p + 1;

          if (!strncmp(mem, "SIP/2.0", strlen("SIP/2.0")))
          {
            version = "sip/2.0";
          }
          memset(mem, 0, 1024);
        }
      }
    }
    else if (*p == '\n')
    {
      if (first_line)
      {
        if (!version)
        {
          memcpy(mem, start, p - start);
          start = p + 1;

          if (!strncmp(mem, "SIP/2.0", strlen("SIP/2.0")))
          {
            version = "sip/2.0";
          }
          memset(mem, 0, 1024);
        }

        LM_INFO("[VoWiFi] before init_message() type 2 (block start)\n");
        kid = init_message(instance, MSG_TYPE_BLOCK_START, symbol, VAL_TYPE_NONE, NULL, VAL_LENGTH_NONE);
        add_message_to_send_queue(instance, kid);
        LM_INFO("[VoWiFi] after init_message() type 2 (block start): kid: %d\n", kid);
        memset(mem, 0, 1024);

        first_line = 0;
        LM_INFO("%s (%s)\n", symbol, version);
      }
      else
      {
        if (!key_set)
        {
          memcpy(key, "value", strlen("value"));
        }
        memcpy(mem, start, p - start);
        start = p + 1;
        LM_INFO("    %s: %s\n", key, mem);
        memset(attr, 0, sizeof(attr));
        memset(key, 0, sizeof(key));
        memset(mem, 0, sizeof(mem));
        attr_set = 0;
        key_set = 0;
      }
    }
    else if (!within && *p == ':')
    {
      if (!first_line)
      {
        if (!attr_set)
        {
          memcpy(attr, start, p - start);
          start = p + 1;
          attr_set = 1;
          LM_INFO("  %s\n", attr);
        }
      }
    }
    else if (!within && *p == '=')
    {
      if (!key_set)
      {
        memcpy(key, start, p - start);
        start = p + 1;
        key_set = 1;
      }
      else
      {
        perror("error happened in '='");
        exit(1);
      }
    }
    else if (!within && *p == ';')
    {
      if (!key_set)
      {
        memcpy(key, "value", strlen("value"));
      }
      memcpy(mem, start, p - start);
      start = p + 1;
      LM_INFO("    %s: %s\n", key, mem);
      memset(key, 0, sizeof(key));
      memset(mem, 0, sizeof(mem));
      key_set = 0;
    }
    else if (!within && *p == ',')
    {
      if (!key_set)
      {
        memcpy(key, "value", strlen("value"));
      }
      memcpy(mem, start, p - start);
      start = p + 1;
      LM_INFO("    %s: %s\n", key, mem);
      memset(key, 0, sizeof(key));
      memset(mem, 0, sizeof(mem));
      key_set = 0;
    }
    else if (*p == '\"')
    {
      within = (within + 1) % 2;
    }
    else if (*p == '<')
    {
      within = 1;
    }
    else if (*p == '>')
    {
      within = 0;
    }
    else if (*p == '[')
    {
      within = 1;
    }
    else if (*p == ']')
    {
      within = 0;
    }
    p++;
  }
  LM_INFO("[VoWiFi] after parsing the message\n");

  LM_INFO("[VoWiFi] before init_message() type 3 (block end)\n");
  kid = init_message(instance, MSG_TYPE_BLOCK_END, symbol, VAL_TYPE_NONE, NULL, VAL_LENGTH_NONE);
  add_message_to_send_queue(instance, kid);
  LM_INFO("[VoWiFi] after init_message() type 3 (block end): kid: %d\n", kid);
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
