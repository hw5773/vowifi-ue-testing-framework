///// Added for VoWiFi /////
#include <inttypes.h>
#include <unistd.h>
#include "ike_sa_instance.h"

typedef struct table_item_t table_item_t;
struct table_item_t {
  void *value;
  table_item_t *next;
};

typedef struct init_hash_t init_hash_t;
struct init_hash_t {
  chunk_t hash;
  uint64_t our_spi;
};

int check_instance(instance_t *instance, uint64_t ispi, uint64_t rspi, int update)
{
  int ret;
  //printf("[check_instance] this->instance: %p\n", instance);

  //if (instance)
  //  printf("[check_instance] instance->ispi: %.16"PRIx64", instance->rspi: %.16"PRIx64", ispi: %.16"PRIx64", rspi: %.16"PRIx64"\n", instance->ispi, instance->rspi, ispi, rspi);

  if (!instance) 
    ret = 0;
  else if (!(instance->ispi) && !(instance->rspi))
  {
    if (update)
    {
      instance->ispi = ispi;
      instance->rspi = rspi;
      ret = 1;
    }
    else
    {
      ret = 0;
    }
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
    memcpy(ret->key, key, strlen(key));
    ret->klen = strlen(key);
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
  ret->initiated = false;
  ret->finished = false;
  if (pthread_mutex_init(&(ret->slock), NULL) != 0)
  {
    printf("[VoWiFi] initializing a mutex for the send queue failed\n");
  }

  ret->running = 1;
  ret->imid = -1;
  ret->rmid = -1;

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
  void *val;

  for (i=0; i<depth; i++)
    printf("  ");
  
  p = buf;
  tmp = get_query_name(query, &tlen);
  if (tlen > 0)
  {
    snprintf(p, tlen+1, "%s", tmp);
    p += tlen;
  }

  val = get_query_value(query, &tlen);
  if (tlen > 0)
  {
    memcpy(p, ":", 1);
    p += 1;
    snprintf(p, tlen+1, "%s", val);
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
  int nlen = (int) strlen(name);
  if (name[nlen-1] == '\n')
    nlen -= 1;
  printf("[VoWiFi] set_query_name()> name: %s, nlen: %d\n", name, nlen);
  query->name = (uint8_t *)calloc(nlen+1, sizeof(uint8_t));
  memcpy(query->name, name, nlen);
  printf("[VoWiFi] set_query_name()> query->name: %s\n", query->name);
  query->nlen = nlen;
}

int get_query_operator(query_t *query)
{
  return query->op;
}

void set_query_operator(query_t *query, int op)
{
  printf("[VoWiFi] set_query_operator()> name: %s, op: %d\n", query->name, op);
  query->op = op;
}

int get_query_value_type(query_t *query)
{
  return query->vtype;
}

void set_query_value_type(query_t *query, uint8_t *vtstr)
{
  int vtlen = (int) strlen(vtstr);
  query->vtype = char_to_int(vtstr, vtlen, 10);
}

uint8_t *get_query_value(query_t *query, int *vlen)
{
  (*vlen) = query->vlen;
  return query->value;
}

void set_query_value(query_t *query, uint8_t *value)
{
  int vlen = (int) strlen(value);
  printf("[VoWiFi] set_query_value()> value: %s, vlen: %d\n", value, vlen);
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
  query_t *ret;

  ret = NULL;
  if (instance->initiated && !instance->finished)
  {
    //printf("[VoWiFi] before the while loop\n");
    while (!instance->query)
    {
      //printf("[VoWiFi] instance->query: %p\n", instance->query);

      if (instance->finished) 
        break;

      sleep(1);
    }
    //printf("[VoWiFi] after the while loop\n");

    if (instance->finished)
      ret = NULL;
    else
      ret = instance->query;
  }

  return ret;
}

query_t *get_next_query(instance_t *instance)
{
  query_t *ret;
  query_t *query;
  ret = NULL;

  if (instance->initiated && !instance->finished)
  {
    query = get_query(instance);
    while (is_query_name(query, instance->sprev))
    {
      printf("[VoWiFi] instance->query->name: %s\n", instance->query->name);
      printf("[VoWiFi] instance->sprev: %s\n", instance->sprev);

      if (instance->finished) 
        break;

      sleep(1);
      query = get_query(instance);
    }

    if (instance->finished)
      ret = NULL;
    else
      ret = instance->query;
  }

  return ret;
}

bool is_query_name(query_t *query, const uint8_t *name)
{
  int qlen, nlen;
  uint8_t *qname;
  bool ret;

  ret = false;
  if (query && name)
  {
    qname = query->name;
    qlen = query->nlen;
    nlen = (int) strlen(name);

    if ((qlen == nlen) && !strncmp(qname, name, qlen))
      ret = true;

    //printf("[VoWiFi] qname (%d bytes): %s, name (%d bytes): %s, ret: %d\n", qlen, qname, nlen, name, ret);
  }

  return ret;
}

query_t *get_sub_query_by_name(query_t *query, const uint8_t *name)
{
  query_t *ret, *curr;

  //printf("[VoWiFi] ike_sa_instance.c: get_sub_query_by_name(): query: %p, name: %s\n", query, name);
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

  if (!base)
  {
    if (str[0] == '0' && str[1] == 'x')
    {
      str = str + 2;
      base = 16;
    }
    else
    {
      base = 10;
    }
  }

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

/*
int check_drop_next(instance_t *instance, payload_t *payload)
{
  int ret;
  ret = COMPLETE;

  return ret;
}
*/

int process_ike_header(instance_t *instance, ike_sa_id_t *ike_sa_id, ike_header_t *ike_header)
{
  int ret;
  query_t *query;
  uint8_t v8;
  uint16_t v16;
  uint32_t v32;
  uint64_t v64;
  uint8_t *tmp;
  int vtype, tlen, op;
  uint16_t size;
  table_item_t **init_hashes_table;

  ret = NOT_SET;

  if ((query = get_query(instance))
      && is_query_name(query, "ike_sa_init_response")
      && (query = get_sub_query_by_name(query, "initiator_spi")))
  {    
    printf("\n\n[VoWiFI] ike_sa_init_response - initiator_spi\n\n");
    vtype = get_query_value_type(query);
    op = get_query_operator(query);
    if ((vtype == VAL_TYPE_UINT64 || vtype == VAL_TYPE_UINT64H) && op == OP_TYPE_UPDATE)
    {    
      tmp = get_query_value(query, &tlen);
      if (vtype == VAL_TYPE_UINT64)
        v64 = char_to_int(tmp, tlen, 10); 
      else if (vtype == VAL_TYPE_UINT64H)
        v64 = char_to_int(tmp, tlen, 16); 
      ike_header->set_initiator_spi(ike_header, v64);
      ike_sa_id->set_initiator_spi(ike_sa_id, v64);
    }    
  }    

  if ((query = get_query(instance))
      && is_query_name(query, "ike_sa_init_response")
      && (query = get_sub_query_by_name(query, "responder_spi")))
  {    
    printf("\n\n[VoWiFI] ike_sa_init_response - responder_spi\n\n");
    vtype = get_query_value_type(query);
    op = get_query_operator(query);
    printf("[VoWiFi] vtype: %d, VAL_TYPE_UINT64: %d, VAL_TYPE_UINT64H: %d, op: %d, OP_TYPE_UPDATE: %d\n",
        vtype, VAL_TYPE_UINT64, VAL_TYPE_UINT64H, op, OP_TYPE_UPDATE);
    if ((vtype == VAL_TYPE_UINT64 || vtype == VAL_TYPE_UINT64H) && op == OP_TYPE_UPDATE)
    {    
      tmp = get_query_value(query, &tlen);
      if (vtype == VAL_TYPE_UINT64)
        v64 = char_to_int(tmp, tlen, 10); 
      else if (vtype == VAL_TYPE_UINT64H)
        v64 = char_to_int(tmp, tlen, 16); 
      printf("v64: 0x%.16"PRIx64"\n", v64);
      ike_header->set_responder_spi(ike_header, v64);
      ike_sa_id->set_responder_spi(ike_sa_id, v64);
      init_hashes_table = (table_item_t **)(instance->init_hashes_table);

      u_int row; 
      table_item_t *item;
      init_hash_t *thash;

      row = chunk_hash(instance->init_hash) & instance->table_mask;
      item = init_hashes_table[row];
      while (item)
      {
        init_hash_t *current = item->value;

        if (chunk_equals(instance->init_hash, current->hash))
          break;
      }

      printf("here 1\n");
      thash = (init_hash_t *)item->value;
      printf("here 2: v64: %d, thahs->our_spi: %d\n", v64, thash->our_spi);
      thash->our_spi = v64; 
      printf("here 3: thash->our_spi: %d\n", thash->our_spi);
    }    
  } 

  if ((query = get_query(instance))
      && is_query_name(query, "ike_sa_init_response")
      && (query = get_sub_query_by_name(query, "ike_major_version")))
  {    
    printf("\n\n[VoWiFI] ike_sa_init_response - ike_major_version\n\n");
    vtype = get_query_value_type(query);
    op = get_query_operator(query);
    if ((vtype == VAL_TYPE_UINT8 || vtype == VAL_TYPE_UINT8H) && op == OP_TYPE_UPDATE)
    {    
      tmp = get_query_value(query, &tlen);
      if (vtype == VAL_TYPE_UINT8)
        v8 = char_to_int(tmp, tlen, 10); 
      else if (vtype == VAL_TYPE_UINT8H)
        v8 = char_to_int(tmp, tlen, 16);
      ike_header->set_maj_version(ike_header, v8);
    }
  }

  if ((query = get_query(instance))
      && is_query_name(query, "ike_sa_init_response")
      && (query = get_sub_query_by_name(query, "ike_minor_version")))
  {
    printf("\n\n[VoWiFI] ike_sa_init_response - ike_minor_version\n\n");
    vtype = get_query_value_type(query);
    op = get_query_operator(query);
    if ((vtype == VAL_TYPE_UINT8 || vtype == VAL_TYPE_UINT8H) && op == OP_TYPE_UPDATE)
    {
      tmp = get_query_value(query, &tlen);
      if (vtype == VAL_TYPE_UINT8)
        v8 = char_to_int(tmp, tlen, 10);
      else if (vtype == VAL_TYPE_UINT8H)
        v8 = char_to_int(tmp, tlen, 16);
      ike_header->set_min_version(ike_header, v8);
    }
  }

  if ((query = get_query(instance))
      && is_query_name(query, "ike_sa_init_response")
      && (query = get_sub_query_by_name(query, "exchange_type")))
  {
    printf("\n\n[VoWiFI] ike_sa_init_response - exchange_type\n\n");
    vtype = get_query_value_type(query);
    op = get_query_operator(query);
    if ((vtype == VAL_TYPE_UINT8 || vtype == VAL_TYPE_UINT8H) && op == OP_TYPE_UPDATE)
    {
      tmp = get_query_value(query, &tlen);
      if (vtype == VAL_TYPE_UINT8)
        v8 = char_to_int(tmp, tlen, 10);
      else if (vtype == VAL_TYPE_UINT8H)
        v8 = char_to_int(tmp, tlen, 16);
      ike_header->set_exchange_type(ike_header, v8);
    }
  }

  return ret;
}


int process_query(instance_t *instance, ike_sa_id_t *ike_sa_id, payload_t *payload, payload_t *next)
{
  int ret;
  ike_header_t *ike_header;

  printf("\n\n\n\n\n[VoWiFi] Process Query!!!!!\n\n\n\n\n");
  ret = NOT_SET;
  ike_header = NULL;

  /*
  if ((ret = check_drop_next(instance, next)))
  {
    goto out;
  }
  */

  switch (payload->get_type(payload))
  {
    case PL_HEADER:
      ike_header = (ike_header_t *)payload;
      ret = process_ike_header(instance, ike_sa_id, ike_header);
      break;

    default:
      break;
  }

out:
  return ret;
}
/////////////////////////
