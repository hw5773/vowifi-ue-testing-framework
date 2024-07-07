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

typedef struct eap_header_t ehdr_t;
struct eap_header_t
{
  uint8_t code;
  uint8_t identifier;
  uint16_t length;
  uint8_t subtype;
  uint16_t reserved;
} __attribute__((__packed__));

int check_instance(instance_t *instance, uint64_t ispi, uint64_t rspi, int update)
{
  int ret;

  if (!instance) 
  {
    ret = 0;
    goto out;
  }
  else if (!(instance->ispi) && !(instance->rspi))
  {
    if (update)
    {
      instance->ispi = ispi;
      instance->rspi = rspi;
      instance->imid = -1;
      instance->rmid = -1;
      ret = 1;
    }
    else
    {
      ret = 0;
    }
  }
  else if (instance->ispi == ispi && instance->rspi == rspi)
    ret = 1;
  else if (instance->ispi == ispi && (instance->pispi == ispi || !instance->pispi) && !rspi)
  {
    ret = 1;
  }
  else
  {
    ret = 0;
  }

  if (!rspi)
  {
    instance->pispi = instance->ispi;
  }

out:
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
    while (!instance->query)
    {
      if (instance->finished) 
        break;

      sleep(1);
    }

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
    while (!query || is_query_name(query, instance->sprev))
    {
      if (instance->finished) 
        break;

      if (query)
      {
        printf("[VoWiFi] instance->query->name: %s\n", instance->query->name);
        printf("[VoWiFi] instance->sprev: %s\n", instance->sprev);
      }

      sleep(1);
      query = get_query(instance);
    }

    ret = query;
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
  }

  return ret;
}

query_t *get_sub_query_by_name(query_t *query, const uint8_t *name)
{
  query_t *ret, *curr;

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

int check_drop_next(instance_t *instance, payload_t *next)
{
  int ret, type, op;
  query_t *query;

  ret = COMPLETED;

  if (!next)
    goto out;

  type = next->get_type(next);

  if ((query = get_query(instance))
      && is_query_name(query, "ike_sa_init_response")
      && (query = get_sub_query_by_name(query, "security_association"))
      && (type == PLV2_SECURITY_ASSOCIATION))
  {
    op = get_query_operator(query);
    if (op == OP_TYPE_DROP)
    {    
      ret = NEED_DROP_NEXT;
    }
  }

  printf("\n\n\n[VoWiFi] check drop next\n\n\n");
  if ((query = get_query(instance))
      && is_query_name(query, "ike_sa_init_response")
      && (query = get_sub_query_by_name(query, "key_exchange"))
      && (type == PLV2_KEY_EXCHANGE))
  {
    printf("\n\n\nkey exchange - drop\n\n\n");
    op = get_query_operator(query);
    if (op == OP_TYPE_DROP)
    {    
      ret = NEED_DROP_NEXT;
    }
  }

  if ((query = get_query(instance))
      && is_query_name(query, "ike_sa_init_response")
      && (query = get_sub_query_by_name(query, "nonce"))
      && (type == PLV2_NONCE))
  {
    op = get_query_operator(query);
    if (op == OP_TYPE_DROP)
    {    
      ret = NEED_DROP_NEXT;
    }
  }

  if ((query = get_query(instance))
      && is_query_name(query, "ike_auth_1_response")
      && (query = get_sub_query_by_name(query, "id_responder"))
      && (type == PLV2_ID_RESPONDER))
  {
    op = get_query_operator(query);
    if (op == OP_TYPE_DROP)
    {    
      ret = NEED_DROP_NEXT;
    }
  }

  if ((query = get_query(instance))
      && is_query_name(query, "ike_auth_1_response")
      && (query = get_sub_query_by_name(query, "extensible_authentication"))
      && (type == PLV2_EAP))
  {
    op = get_query_operator(query);
    if (op == OP_TYPE_DROP)
    {    
      ret = NEED_DROP_NEXT;
    }
  }

  if ((query = get_query(instance))
      && is_query_name(query, "ike_auth_2_response")
      && (query = get_sub_query_by_name(query, "extensible_authentication"))
      && (type == PLV2_EAP))
  {
    op = get_query_operator(query);
    if (op == OP_TYPE_DROP)
    {    
      ret = NEED_DROP_NEXT;
    }
  }

  if ((query = get_query(instance))
      && is_query_name(query, "ike_auth_3_response")
      && (query = get_sub_query_by_name(query, "authentication"))
      && (type == PLV2_AUTH))
  {
    op = get_query_operator(query);
    if (op == OP_TYPE_DROP)
    {    
      ret = NEED_DROP_NEXT;
    }
  }

  if ((query = get_query(instance))
      && is_query_name(query, "ike_auth_3_response")
      && (query = get_sub_query_by_name(query, "ts_initiator"))
      && (type == PLV2_TS_INITIATOR))
  {
    op = get_query_operator(query);
    if (op == OP_TYPE_DROP)
    {    
      ret = NEED_DROP_NEXT;
    }
  }

  if ((query = get_query(instance))
      && is_query_name(query, "ike_auth_3_response")
      && (query = get_sub_query_by_name(query, "ts_responder"))
      && (type == PLV2_TS_RESPONDER))
  {
    op = get_query_operator(query);
    if (op == OP_TYPE_DROP)
    {    
      ret = NEED_DROP_NEXT;
    }
  }

  if ((query = get_query(instance))
      && is_query_name(query, "ike_auth_3_response")
      && (query = get_sub_query_by_name(query, "configuration"))
      && (type == PLV2_CONFIGURATION))
  {
    op = get_query_operator(query);
    if (op == OP_TYPE_DROP)
    {    
      ret = NEED_DROP_NEXT;
    }
  }

out:
  return ret;
}

int check_flag(uint8_t *flag, uint8_t *tmp, int tlen)
{
  size_t len;

  if (strlen(flag) < tlen)
  {
    len = strlen(flag);
  }
  else
  {
    len = tlen;
  }

  return !strncmp(tmp, flag, len);
}

int process_ike_header(instance_t *instance, ike_sa_id_t *ike_sa_id, ike_header_t *ike_header)
{
  int ret, i;
  query_t *query;
  payload_t *payload;
  uint8_t v8;
  uint16_t v16;
  uint32_t v32;
  uint64_t v64;
  uint8_t *tmp;
  int vtype, tlen, op;
  uint16_t size;
  const uint8_t *mname;
  table_item_t **init_hashes_table;
  const uint8_t *messages[30] = 
  {
    "ike_sa_init_response",
    "ike_auth_1_response",
    "ike_auth_2_response",
    "ike_auth_3_response",
    "ike_auth_4_response"
  };

  ret = COMPLETED;
  tmp = NULL;

  for (i=0; i<5; i++)
  {
    mname = messages[i];
    if ((query = get_query(instance))
        && is_query_name(query, mname)
        && (query = get_sub_query_by_name(query, "initiator_spi")))
    {    
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
        && is_query_name(query, mname)
        && (query = get_sub_query_by_name(query, "responder_spi")))
    {    
      vtype = get_query_value_type(query);
      op = get_query_operator(query);
      if ((vtype == VAL_TYPE_UINT64 || vtype == VAL_TYPE_UINT64H) && op == OP_TYPE_UPDATE)
      {    
        tmp = get_query_value(query, &tlen);
        if (vtype == VAL_TYPE_UINT64)
          v64 = char_to_int(tmp, tlen, 10); 
        else if (vtype == VAL_TYPE_UINT64H)
          v64 = char_to_int(tmp, tlen, 16); 
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

        thash = (init_hash_t *)item->value;
        thash->our_spi = v64; 
      }    
    } 

    /*
    if ((query = get_query(instance))
        && is_query_name(query, mname)
        && (query = get_sub_query_by_name(query, "next_payload")))
    {
      printf("[VoWiFi] ike_sa_init_response - next_payload\n");
      vtype = get_query_value_type(query);
      op = get_query_operator(query);
      if ((vtype == VAL_TYPE_UINT8 || vtype == VAL_TYPE_UINT8H) && op == OP_TYPE_UPDATE)
      {
        tmp = get_query_value(query, &tlen);
        if (vtype == VAL_TYPE_UINT8)
          v8 = char_to_int(tmp, tlen, 10);
        else if (vtype == VAL_TYPE_UINT8H)
          v8 = char_to_int(tmp, tlen, 16);
        printf("[VoWiFi] v8: %u\n\n\n", v8);
        payload = (payload_t *)ike_header;
        payload->set_next_type(payload, v8);
      }
    }
    */

    if ((query = get_query(instance))
        && is_query_name(query, mname)
        && (query = get_sub_query_by_name(query, "ike_major_version")))
    {    
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
        && is_query_name(query, mname)
        && (query = get_sub_query_by_name(query, "ike_minor_version")))
    {
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
        && is_query_name(query, mname)
        && (query = get_sub_query_by_name(query, "exchange_type")))
    {
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

    if ((query = get_query(instance))
        && is_query_name(query, mname)
        && (query = get_sub_query_by_name(query, "flags")))
    {
      const uint8_t *flag;
      int len;
      vtype = get_query_value_type(query);
      op = get_query_operator(query);
      if (vtype == VAL_TYPE_STRING && op == OP_TYPE_UPDATE)
      {
        tmp = get_query_value(query, &tlen);
        flag = "responder";
        if (check_flag(flag, tmp, tlen))
          ike_header->set_response_flag(ike_header, 1);

        flag = "no_responder";
        if (check_flag(flag, tmp, tlen))
          ike_header->set_response_flag(ike_header, 0);

        flag = "version";
        if (check_flag(flag, tmp, tlen))
          ike_header->set_version_flag(ike_header, 1);

        flag = "no_version";
        if (check_flag(flag, tmp, tlen))
          ike_header->set_version_flag(ike_header, 0);

        flag = "initiator";
        if (check_flag(flag, tmp, tlen))
          ike_header->set_initiator_flag(ike_header, 1);

        flag = "no_initiator";
        if (check_flag(flag, tmp, tlen))
          ike_header->set_initiator_flag(ike_header, 0);

        flag = "encryption";
        if (check_flag(flag, tmp, tlen))
          ike_header->set_encryption_flag(ike_header, 1);

        flag = "no_encryption";
        if (check_flag(flag, tmp, tlen))
          ike_header->set_encryption_flag(ike_header, 0);

        flag = "commit";
        if (check_flag(flag, tmp, tlen))
          ike_header->set_commit_flag(ike_header, 1);

        flag = "no_commit";
        if (check_flag(flag, tmp, tlen))
          ike_header->set_commit_flag(ike_header, 0);

        flag = "authonly";
        if (check_flag(flag, tmp, tlen))
          ike_header->set_authonly_flag(ike_header, 1);

        flag = "no_authonly";
        if (check_flag(flag, tmp, tlen))
          ike_header->set_authonly_flag(ike_header, 0);
      }
    }

    if ((query = get_query(instance))
        && is_query_name(query, mname)
        && (query = get_sub_query_by_name(query, "message_id")))
    {
      vtype = get_query_value_type(query);
      op = get_query_operator(query);
      if ((vtype == VAL_TYPE_UINT32 || vtype == VAL_TYPE_UINT32H) && op == OP_TYPE_UPDATE)
      {
        tmp = get_query_value(query, &tlen);
        if (vtype == VAL_TYPE_UINT32)
          v32 = char_to_int(tmp, tlen, 10);
        else if (vtype == VAL_TYPE_UINT32H)
          v32 = char_to_int(tmp, tlen, 16);
        ike_header->set_message_id(ike_header, v32);
      }
    }
  }

  if (tmp)
    free(tmp);

  return ret;
}

int process_proposal(instance_t *instance, proposal_t *proposal)
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
  uint16_t *algo, *klen;

  ret = COMPLETED;
  tmp = NULL;

  // encryption related
  algo = (uint16_t *)calloc(1, sizeof(uint16_t));
  klen = (uint16_t *)calloc(1, sizeof(uint16_t));

  // ike_sa_init_response - security_association - proposal - transform - encryption_algorithm
  if ((query = get_query(instance))
      && is_query_name(query, "ike_sa_init_response")
      && (query = get_sub_query_by_name(query, "security_association"))
      && (query = get_sub_query_by_name(query, "proposal"))
      && (query = get_sub_query_by_name(query, "transform"))
      && (query = get_sub_query_by_name(query, "encryption_algorithm")))
  {
    printf("[VoWiFi] ike_sa_init_response - security_association - transform - encryption_algorithm\n");
    proposal->get_algorithm(proposal, ENCRYPTION_ALGORITHM, algo, klen);
    vtype = get_query_value_type(query);
    op = get_query_operator(query);
    if (vtype == VAL_TYPE_UINT16 && op == OP_TYPE_UPDATE)
    {
      tmp = get_query_value(query, &tlen);
      *algo = (uint16_t) char_to_int(tmp, tlen, 10);
    }

    if ((query = get_query(instance))
      && is_query_name(query, "ike_sa_init_response")
      && (query = get_sub_query_by_name(query, "security_association"))
      && (query = get_sub_query_by_name(query, "proposal"))
      && (query = get_sub_query_by_name(query, "transform"))
      && (query = get_sub_query_by_name(query, "encryption_key_length")))
    {
      vtype = get_query_value_type(query);
      op = get_query_operator(query);
      if (vtype == VAL_TYPE_UINT16 && op == OP_TYPE_UPDATE)
      {
        tmp = get_query_value(query, &tlen);
        *klen = (uint16_t) char_to_int(tmp, tlen, 10);
      }
    }
    proposal->set_algorithm(proposal, ENCRYPTION_ALGORITHM, *algo, *klen);
  }

  // ike_sa_init_response - security_association - transform - diffie_hellman_group
  if ((query = get_query(instance))
      && is_query_name(query, "ike_sa_init_response")
      && (query = get_sub_query_by_name(query, "security_association"))
      && (query = get_sub_query_by_name(query, "proposal"))
      && (query = get_sub_query_by_name(query, "transform"))
      && (query = get_sub_query_by_name(query, "diffie_hellman_group")))
  {
    proposal->get_algorithm(proposal, DIFFIE_HELLMAN_GROUP, algo, klen);
    vtype = get_query_value_type(query);
    op = get_query_operator(query);
    if (vtype == VAL_TYPE_UINT16 && op == OP_TYPE_UPDATE)
    {
      tmp = get_query_value(query, &tlen);
      *algo = (uint16_t) char_to_int(tmp, tlen, 10);
    }
    proposal->set_algorithm(proposal, DIFFIE_HELLMAN_GROUP, *algo, *klen);
  }

  // ike_sa_init_response - security_association - transform - pseudo_random_function
  if ((query = get_query(instance))
      && is_query_name(query, "ike_sa_init_response")
      && (query = get_sub_query_by_name(query, "security_association"))
      && (query = get_sub_query_by_name(query, "proposal"))
      && (query = get_sub_query_by_name(query, "transform"))
      && (query = get_sub_query_by_name(query, "pseudo_random_function")))
  {
    proposal->get_algorithm(proposal, PSEUDO_RANDOM_FUNCTION, algo, klen);
    vtype = get_query_value_type(query);
    op = get_query_operator(query);
    if (vtype == VAL_TYPE_UINT16 && op == OP_TYPE_UPDATE)
    {
      tmp = get_query_value(query, &tlen);
      *algo = (uint16_t) char_to_int(tmp, tlen, 10);
    }
    proposal->set_algorithm(proposal, PSEUDO_RANDOM_FUNCTION, *algo, *klen);
  }

  // ike_sa_init_response - security_association - transform - integrity_algorithm
  if ((query = get_query(instance))
      && is_query_name(query, "ike_sa_init_response")
      && (query = get_sub_query_by_name(query, "security_association"))
      && (query = get_sub_query_by_name(query, "proposal"))
      && (query = get_sub_query_by_name(query, "transform"))
      && (query = get_sub_query_by_name(query, "integrity_algorithm")))
  {
    proposal->get_algorithm(proposal, INTEGRITY_ALGORITHM, algo, klen);
    vtype = get_query_value_type(query);
    op = get_query_operator(query);
    if (vtype == VAL_TYPE_UINT16 && op == OP_TYPE_UPDATE)
    {
      tmp = get_query_value(query, &tlen);
      *algo = (uint16_t) char_to_int(tmp, tlen, 10);
    }
    proposal->set_algorithm(proposal, INTEGRITY_ALGORITHM, *algo, *klen);
  }

  if ((query = get_query(instance))
      && is_query_name(query, "ike_sa_init_response")
      && (query = get_sub_query_by_name(query, "security_association"))
      && (query = get_sub_query_by_name(query, "proposal"))
      && (query = get_sub_query_by_name(query, "proposal_number")))
  {
    vtype = get_query_value_type(query);
    op = get_query_operator(query);
    if (vtype == VAL_TYPE_UINT8 && op == OP_TYPE_UPDATE)
    {
      tmp = get_query_value(query, &tlen);
      v8 = char_to_int(tmp, tlen, 10);

      proposal->set_number(proposal, v8);
    }
  }

  if ((query = get_query(instance))
      && is_query_name(query, "ike_sa_init_response")
      && (query = get_sub_query_by_name(query, "security_association"))
      && (query = get_sub_query_by_name(query, "proposal"))
      && (query = get_sub_query_by_name(query, "spi")))
  {
    vtype = get_query_value_type(query);
    op = get_query_operator(query);
    if (vtype == VAL_TYPE_UINT64H && op == OP_TYPE_UPDATE)
    {
      tmp = get_query_value(query, &tlen);
      v64 = char_to_int(tmp, tlen, 16);

      proposal->set_spi(proposal, v64);
    }
  }

  free(algo);
  free(klen);

out:
  return ret;
}

int process_notify(instance_t *instance, ike_sa_id_t *ike_sa_id, notify_payload_t *notify)
{
  int ret;
  query_t *query;
  uint8_t v8;
  uint16_t v16;
  uint32_t v32;
  uint64_t v64;
  uint8_t *tmp, *hval;
  int vtype, tlen, op;
  uint16_t size;

  ret = COMPLETED;
  tmp = NULL;
  hval = NULL;
 
  switch (notify->get_notify_type(notify))
  {
    case NAT_DETECTION_DESTINATION_IP:
      if ((query = get_query(instance)) 
          && is_query_name(query, "ike_sa_init_response")
          && (query = get_sub_query_by_name(query, "nat_detection_destination_ip")))
      {
        vtype = get_query_value_type(query);
        op = get_query_operator(query);
        if (vtype == VAL_TYPE_STRING && op == OP_TYPE_UPDATE)
        {
          tmp = get_query_value(query, &tlen);
          if (tlen >= 8
              && !strncmp(tmp, "received", 8))
          {
            notify->set_notification_data(notify, instance->rcvd_dst_hash); 
          }
          else if (tlen >= 20)
          {
            hval = calloc(20, sizeof(uint8_t));
            memcpy(hval, tmp, 20);
            notify->set_notification_data(notify, chunk_create(hval, 20));
          }
          else
          {
            hval = calloc(20, sizeof(uint8_t));
            memcpy(hval, tmp, tlen); 
            notify->set_notification_data(notify, chunk_create(hval, 20));
          }
        }
      }
      break;

    case NAT_DETECTION_SOURCE_IP:
      if ((query = get_query(instance)) 
          && is_query_name(query, "ike_sa_init_response")
          && (query = get_sub_query_by_name(query, "nat_detection_source_ip")))
      {
        vtype = get_query_value_type(query);
        op = get_query_operator(query);
        if (vtype == VAL_TYPE_STRING && op == OP_TYPE_UPDATE)
        {
          tmp = get_query_value(query, &tlen);
          if (tlen >= 8
              && !strncmp(tmp, "received", 8))
          {
            notify->set_notification_data(notify, instance->rcvd_src_hash); 
          }
          else if (tlen >= 20)
          {
            hval = calloc(20, sizeof(uint8_t));
            memcpy(hval, tmp, 20);
            notify->set_notification_data(notify, chunk_create(hval, 20));
          }
          else
          {
            hval = calloc(20, sizeof(uint8_t));
            memcpy(hval, tmp, tlen); 
            notify->set_notification_data(notify, chunk_create(hval, 20));
          }
        }
      }
      break;

    case SIGNATURE_HASH_ALGORITHMS:
      if ((query = get_query(instance))
          && is_query_name(query, "ike_sa_init_response")
          && (query = get_sub_query_by_name(query, "signature_hash_algorithms")))
      {
        vtype = get_query_value_type(query);
        op = get_query_operator(query);
        if (vtype == VAL_TYPE_STRING && op == OP_TYPE_UPDATE)
        {
          tmp = get_query_value(query, &tlen);
          notify->set_notification_data(notify, chunk_create(tmp, tlen));
        }
      }
      break;

    case CHILDLESS_IKEV2_SUPPORTED:
      if ((query = get_query(instance))
          && is_query_name(query, "ike_sa_init_response")
          && (query = get_sub_query_by_name(query, "childless_ikev2_supported")))
      {
        vtype = get_query_value_type(query);
        op = get_query_operator(query);
        if (vtype == VAL_TYPE_STRING && op == OP_TYPE_UPDATE)
        {
          tmp = get_query_value(query, &tlen);
          notify->set_notification_data(notify, chunk_create(tmp, tlen));
        }
      }
      break;

    default:
      break;
  }

  if (tmp)
    free(tmp);
  
out:
  return ret;
}

int process_key_exchange(instance_t *instance, ike_sa_id_t *ike_sa_id, ke_payload_t *ke)
{
  int ret;
  query_t *query;
  uint8_t v8;
  uint16_t v16;
  uint32_t v32;
  uint64_t v64;
  uint8_t *tmp;
  uint8_t *data;
  int i, vtype, tlen, dlen, op;
  uint16_t size, dhgroup;
  uint16_t *algo, *klen;

  ret = COMPLETED;

  // ike_sa_init_response - key_exchange - diffie_hellman_group
  if ((query = get_query(instance))
      && is_query_name(query, "ike_sa_init_response")
      && (query = get_sub_query_by_name(query, "key_exchange"))
      && (query = get_sub_query_by_name(query, "diffie_hellman_group")))
  {
    printf("[VoWiFi] ike_sa_init_response - key_exchange - diffie_hellman_group\n");
    vtype = get_query_value_type(query);
    op = get_query_operator(query);
    if (vtype == VAL_TYPE_UINT16 && op == OP_TYPE_UPDATE)
    {    
      tmp = get_query_value(query, &tlen);
      v16 = char_to_int(tmp, tlen, 10);
      ke->set_dh_group_number(ke, v16);
    }
  }

  // ike_sa_init_response - key_exchange - key_exchange_data
  if ((query = get_query(instance))
      && is_query_name(query, "ike_sa_init_response")
      && (query = get_sub_query_by_name(query, "key_exchange"))
      && (query = get_sub_query_by_name(query, "key_exchange_data")))
  {
    printf("[VoWiFi] ike_sa_init_response - key_exchange - key_exchange_data\n");
    vtype = get_query_value_type(query);
    op = get_query_operator(query);
    if (vtype == VAL_TYPE_STRING && op == OP_TYPE_UPDATE)
    {    
      tmp = get_query_value(query, &tlen);
      dhgroup = ke->get_dh_group_number(ke);

      switch (dhgroup)
      {
        case MODP_NONE:
          dlen = 0;
          break;
        case MODP_768_BIT:
          dlen = 96;
          break;
        case MODP_1024_BIT:
          dlen = 128;
          break;
        case MODP_1536_BIT:
          dlen = 192;
          break;
        case MODP_2048_BIT:
          dlen = 256;
          break;
        case MODP_3072_BIT:
          dlen = 384;
          break;
        case MODP_4096_BIT:
          dlen = 512;
          break;
        case MODP_6144_BIT:
          dlen = 768;
          break;
        case MODP_8192_BIT:
          dlen = 1024;
          break;
        case ECP_256_BIT:
        case ECP_256_BP:
        case MODP_2048_256:
        case CURVE_25519:
        case NTRU_256_BIT:
          dlen = 32;
          break;
        case ECP_384_BIT:
        case ECP_384_BP:
          dlen = 48;
          break;
        case ECP_521_BIT:
        case ECP_512_BP:
          dlen = 64;
          break;
        case MODP_1024_160:
          dlen = 20;
          break;
        case MODP_2048_224:
          dlen = 28;
          break;
        case ECP_192_BIT:
        case NTRU_192_BIT:
          dlen = 24;
          break;
        case ECP_224_BIT:
        case ECP_224_BP:
        case CURVE_448:
          dlen = 28;
          break;
        case NTRU_112_BIT:
          dlen = 14;
          break;
        default:
          dlen = 256;
      }

      if (tlen == 3
          && !strncmp(tmp, "max", tlen))
      {
        data = (uint8_t *)calloc(1, dlen);
        for (i=0; i<dlen; i++)
          data[i] = 0xFF;
      }
      else if (tlen == 3
          && !strncmp(tmp, "min", tlen))
      {
        data = (uint8_t *)calloc(1, dlen);
        for (i=0; i<dlen; i++)
          data[i] = 0;
      }
      ke->set_key_exchange_data(ke, chunk_create(data, dlen));
    }
  }

out:
  return ret;
}

int process_nonce(instance_t *instance, ike_sa_id_t *ike_sa_id, nonce_payload_t *np)
{
  int ret;
  query_t *query;
  uint8_t v8;
  uint16_t v16;
  uint32_t v32;
  uint64_t v64;
  uint8_t *tmp, *data;
  int i, vtype, tlen, dlen, op;

  ret = COMPLETED;

  // ike_sa_init_response - nonce - nonce_data
  if ((query = get_query(instance))
      && is_query_name(query, "ike_sa_init_response")
      && (query = get_sub_query_by_name(query, "nonce")))
  {
    printf("[VoWiFi] ike_sa_init_response - nonce\n");
    vtype = get_query_value_type(query);
    op = get_query_operator(query);
    if (vtype == VAL_TYPE_STRING && op == OP_TYPE_UPDATE)
    {    
      tmp = get_query_value(query, &tlen);
      if (tlen == 3
          && !strncmp(tmp, "max", tlen))
      {
        data = (uint8_t *)calloc(1, NONCE_SIZE);
        for (i=0; i<NONCE_SIZE; i++)
          data[i] = 0xFF;
      }
      else if (tlen == 3
          && !strncmp(tmp, "min", tlen))
      {
        data = (uint8_t *)calloc(1, NONCE_SIZE);
        for (i=0; i<NONCE_SIZE; i++)
          data[i] = 0;
      }
      np->set_nonce(np, chunk_create(data, NONCE_SIZE));
    }
  }

  return ret;
}

int process_identification_responder(instance_t *instance, ike_sa_id_t *ike_sa_id, id_payload_t *id)
{
  int ret;
  query_t *query;
  uint8_t v8;
  uint16_t v16;
  uint32_t v32;
  uint64_t v64;
  uint8_t *tmp, *data;
  int i, vtype, tlen, dlen, op;

  ret = COMPLETED;

  // ike_auth_1_response - identification_responder - id_type
  if ((query = get_query(instance))
      && is_query_name(query, "ike_auth_1_response")
      && (query = get_sub_query_by_name(query, "identification_responder"))
      && (query = get_sub_query_by_name(query, "id_type")))

  {
    printf("[VoWiFi] ike_auth_1_response - identification_responder - id_type\n");
    vtype = get_query_value_type(query);
    op = get_query_operator(query);
    if (vtype == VAL_TYPE_UINT8 && op == OP_TYPE_UPDATE)
    {    
      tmp = get_query_value(query, &tlen);
      v8 = char_to_int(tmp, tlen, 10);
      id->set_id_type(id, v8);
    }
  }

  // ike_auth_1_response - identification_responder - identification_data
  if ((query = get_query(instance))
      && is_query_name(query, "ike_auth_1_response")
      && (query = get_sub_query_by_name(query, "identification_responder"))
      && (query = get_sub_query_by_name(query, "identification_data")))

  {
    printf("[VoWiFi] ike_auth_1_response - identification_responder - identification_data\n");
    vtype = get_query_value_type(query);
    op = get_query_operator(query);
    if (vtype == VAL_TYPE_STRING && op == OP_TYPE_UPDATE)
    {    
      tmp = get_query_value(query, &tlen);
      id->set_id_data(id, chunk_create(tmp, tlen));
    }
  }

  return ret;
}

int check_exceptional_case(instance_t *instance, ike_sa_id_t *ike_sa_id, notify_payload_t *notify)
{
  int i, ret;
  query_t *query;
  const uint8_t *mname;
  const uint8_t *messages[5] =
  {
    "ike_sa_init_response",
    "ike_auth_1_response",
    "ike_auth_2_response",
    "ike_auth_3_response",
    "ike_auth_4_response",
  };

  ret = FALSE;

  printf("\n\n\n[VoWiFi] in check_exceptional_case()\n");
  switch (notify->get_notify_type(notify))
  {
    case AUTHENTICATION_FAILED:
      for (i=0; i<5; i++)
      {
        if ((query = get_query(instance)) 
            && is_query_name(query, mname)
            && (query = get_sub_query_by_name(query, "authentication_failed")))
        {
          ret = TRUE;
          break;
        }
      }
      printf("[VoWiFi] Authentication failed: %d\n", ret);
      break;
    default:
      ret = FALSE;
  }

  return ret;
}

int process_extensible_authentication(instance_t *instance, ike_sa_id_t *ike_sa_id, eap_payload_t *eap)
{
  int ret;
  query_t *query;
  uint8_t v8, alen, abytes, type, clen;
  uint16_t v16;
  uint32_t v32;
  uint64_t v64;
  uint8_t *tmp, *data, *e, *p;
  int i, idx, vtype, tlen, dlen, plen, op;
  chunk_t payload;
  const uint8_t *mname;
  const uint8_t *messages[2] = 
  {
    "ike_auth_1_response",
    "ike_auth_2_response",
  };

  ret = COMPLETED;
  mname = NULL;


  printf("[VoWiFi] in process_extensible_authentication()\n");
  query = get_query(instance);
  print_query(query);
  for (idx=0; idx<2; idx++)
  {
    mname = messages[idx];
    printf("[VoWiFi] mname: %s\n", mname);
    // ike_auth_1_response/ike_auth_2_response - extensible_authentication - code
    if ((query = get_query(instance))
        && is_query_name(query, mname)
        && (query = get_sub_query_by_name(query, "extensible_authentication"))
        && (query = get_sub_query_by_name(query, "code")))

    {
      vtype = get_query_value_type(query);
      op = get_query_operator(query);
      if (vtype == VAL_TYPE_UINT8 && op == OP_TYPE_UPDATE)
      {    
        tmp = get_query_value(query, &tlen);
        v8 = char_to_int(tmp, tlen, 10);

        eap->set_code(eap, v8);
      }
    }

    // ike_auth_1_response/ike_auth_2_response - extensible_authentication - id
    if ((query = get_query(instance))
        && is_query_name(query, mname)
        && (query = get_sub_query_by_name(query, "extensible_authentication"))
        && (query = get_sub_query_by_name(query, "id")))

    {
      vtype = get_query_value_type(query);
      op = get_query_operator(query);
      if (vtype == VAL_TYPE_UINT8 && op == OP_TYPE_UPDATE)
      {    
        tmp = get_query_value(query, &tlen);
        v8 = char_to_int(tmp, tlen, 10);

        eap->set_identifier(eap, v8);
      }
    }

    // ike_auth_1_response/ike_auth_2_response - extensible_authentication - type
    if ((query = get_query(instance))
        && is_query_name(query, mname)
        && (query = get_sub_query_by_name(query, "extensible_authentication"))
        && (query = get_sub_query_by_name(query, "type")))

    {
      vtype = get_query_value_type(query);
      op = get_query_operator(query);
      if (vtype == VAL_TYPE_UINT8 && op == OP_TYPE_UPDATE)
      {    
        tmp = get_query_value(query, &tlen);
        v8 = char_to_int(tmp, tlen, 10);

        eap->set_type(eap, NULL, v8);
      }
    }

    // ike_auth_1_response/ike_auth_2_response - extensible_authentication - eap_aka_attribute - at_rand
    if ((query = get_query(instance))
        && is_query_name(query, mname)
        && (query = get_sub_query_by_name(query, "extensible_authentication"))
        && (query = get_sub_query_by_name(query, "eap_aka_attribute"))
        && (query = get_sub_query_by_name(query, "at_rand")))

    {
      payload = eap->get_data(eap);
      vtype = get_query_value_type(query);
      op = get_query_operator(query);
      tmp = get_query_value(query, &tlen);
      e = p = (uint8_t *) payload.ptr;
      plen = (int) payload.len;
      p += sizeof(ehdr_t);
      plen -= sizeof(ehdr_t);

      while (plen > 0)
      {
        type = *(p++);
        plen--;
        alen = *(p++);
        plen--;
        abytes = 4 * alen - 2;

        if (type != AKA_TYPE_AT_RAND)
        {
          p += abytes;
          plen -= abytes;
          continue;
        }
        else
        {
          break;
        }
      }

      if (plen > 0)
      {
        if (vtype == VAL_TYPE_STRING && op == OP_TYPE_UPDATE)
        {
          if (tlen >= 3 && !strncmp(tmp, "min", 3))
          {
            for (i=2; i<abytes; i++)
            {
              p[i] = 0x00;
            }
          }
          else if (tlen >= 3 && !strncmp(tmp, "max", 3))
          {
            for (i=2; i<abytes; i++)
            {
              p[i] = 0xff;
            }
          }
          else if (tlen >= 3 && !strncmp(tmp, "median", 6))
          {
            for (i=2; i<abytes; i++)
            {
              p[i] = 0x88;
            }
          }
          else
          {
            for (i=2; i<abytes; i++)
            {
              p[i] = 0x00;
              if (tlen > 0)
              {
                p[i] = char_to_int(tmp[2*(i-2)], 2, 16);
                tlen -= 2;
              }
            }
          }
        }
        else if (op == OP_TYPE_DROP)
        {
          clen = payload.len - alen * 4;
          e += 2;
          e[0] = (clen >> 8) & 0xff;
          e[1] = clen & 0xff;
          payload.len = clen;
        }
      }
    }

    // ike_auth_1_response/ike_auth_2_response - extensible_authentication - eap_aka_attribute - at_autn
    if ((query = get_query(instance))
        && is_query_name(query, mname)
        && (query = get_sub_query_by_name(query, "extensible_authentication"))
        && (query = get_sub_query_by_name(query, "eap_aka_attribute"))
        && (query = get_sub_query_by_name(query, "at_autn")))

    {
      payload = eap->get_data(eap);
      vtype = get_query_value_type(query);
      op = get_query_operator(query);
      tmp = get_query_value(query, &tlen);
      e = p = (uint8_t *) payload.ptr;
      plen = (int) payload.len;
      p += sizeof(ehdr_t);
      plen -= sizeof(ehdr_t);

      while (plen > 0)
      {
        type = *(p++);
        plen--;
        alen = *(p++);
        plen--;
        abytes = 4 * alen - 2;

        if (type != AKA_TYPE_AT_AUTN)
        {
          p += abytes;
          plen -= abytes;
          continue;
        }
        else
        {
          break;
        }
      }

      if (plen > 0)
      {
        if (vtype == VAL_TYPE_STRING && op == OP_TYPE_UPDATE)
        {
          if (tlen >= 3 && !strncmp(tmp, "min", 3))
          {
            for (i=2; i<abytes; i++)
            {
              p[i] = 0x00;
            }
          }
          else if (tlen >= 3 && !strncmp(tmp, "max", 3))
          {
            for (i=2; i<abytes; i++)
            {
              p[i] = 0xff;
            }
          }
          else if (tlen >= 3 && !strncmp(tmp, "median", 6))
          {
            for (i=2; i<abytes; i++)
            {
              p[i] = 0x88;
            }
          }
          else
          {
            for (i=2; i<abytes; i++)
            {
              p[i] = 0x00;
              if (tlen > 0)
              {
                p[i] = char_to_int(tmp[2*(i-2)], 2, 16);
                tlen -= 2;
              }
            }
          }
        }
        else if (op == OP_TYPE_DROP)
        {
          clen = payload.len - alen * 4;
          e += 2;
          e[0] = (clen >> 8) & 0xff;
          e[1] = clen & 0xff;
          payload.len = clen;
        }
      }
    }

    // ike_auth_1_response/ike_auth_2_response - extensible_authentication - eap_aka_attribute - at_mac
    if ((query = get_query(instance))
        && is_query_name(query, mname)
        && (query = get_sub_query_by_name(query, "extensible_authentication"))
        && (query = get_sub_query_by_name(query, "eap_aka_attribute"))
        && (query = get_sub_query_by_name(query, "at_mac")))

    {
      printf("\n\n\n\n\n[VoWiFi] extensible_authentication / eap_aka_attribute / at_mac\n\n\n\n\n");
      payload = eap->get_data(eap);
      vtype = get_query_value_type(query);
      op = get_query_operator(query);
      tmp = get_query_value(query, &tlen);
      e = p = (uint8_t *) payload.ptr;
      plen = (int) payload.len;
      p += sizeof(ehdr_t);
      plen -= sizeof(ehdr_t);

      while (plen > 0)
      {
        type = *(p++);
        plen--;
        alen = *(p++);
        plen--;
        abytes = 4 * alen - 2;

        if (type != AKA_TYPE_AT_MAC)
        {
          p += abytes;
          plen -= abytes;
          continue;
        }
        else
        {
          break;
        }
      }

      if (plen > 0)
      {
        printf("[VoWiFi] ike_auth_1_response / extensible_authentication / eap_aka_attribute / at_mac - value update\n");
        if (vtype == VAL_TYPE_STRING && op == OP_TYPE_UPDATE)
        {
          printf("[VoWiFi] minimum value is going to be assigned\n");
          if (tlen >= 3 && !strncmp(tmp, "min", 3))
          {
            for (i=2; i<abytes; i++)
            {
              p[i] = 0x00;
            }
          }
          else if (tlen >= 3 && !strncmp(tmp, "max", 3))
          {
            for (i=2; i<abytes; i++)
            {
              p[i] = 0xff;
            }
          }
          else if (tlen >= 3 && !strncmp(tmp, "median", 6))
          {
            for (i=2; i<abytes; i++)
            {
              p[i] = 0x88;
            }
          }
          else
          {
            for (i=2; i<abytes; i++)
            {
              p[i] = 0x00;
              if (tlen > 0)
              {
                p[i] = char_to_int(tmp[2*(i-2)], 1, 16);
                tlen -= 2;
              }
            }
          }
        }
        else if (op == OP_TYPE_DROP)
        {
          clen = payload.len - alen * 4;
          e += 2;
          e[0] = (clen >> 8) & 0xff;
          e[1] = clen & 0xff;
          payload.len = clen;
        }
      }
    }

    // ike_auth_1_response/ike_auth_2_response - extensible_authentication - eap_aka_attribute - at_mac
    if ((query = get_query(instance))
        && is_query_name(query, mname)
        && (query = get_sub_query_by_name(query, "extensible_authentication"))
        && (query = get_sub_query_by_name(query, "eap_aka_attribute"))
        && (query = get_sub_query_by_name(query, "at_checkcode")))

    {
      payload = eap->get_data(eap);
      vtype = get_query_value_type(query);
      op = get_query_operator(query);
      tmp = get_query_value(query, &tlen);
      e = p = (uint8_t *) payload.ptr;
      plen = (int) payload.len;
      p += sizeof(ehdr_t);
      plen -= sizeof(ehdr_t);

      while (plen > 0)
      {
        type = *(p++);
        plen--;
        alen = *(p++);
        plen--;
        abytes = 4 * alen - 2;

        if (type != AKA_TYPE_AT_CHECKCODE)
        {
          p += abytes;
          plen -= abytes;
          continue;
        }
        else
        {
          break;
        }
      }

      if (plen > 0)
      {
        if (vtype == VAL_TYPE_STRING && op == OP_TYPE_UPDATE)
        {
          if (tlen >= 3 && !strncmp(tmp, "min", 3))
          {
            for (i=2; i<abytes; i++)
            {
              p[i] = 0x00;
            }
          }
          else if (tlen >= 3 && !strncmp(tmp, "max", 3))
          {
            for (i=2; i<abytes; i++)
            {
              p[i] = 0xff;
            }
          }
          else if (tlen >= 3 && !strncmp(tmp, "median", 6))
          {
            for (i=2; i<abytes; i++)
            {
              p[i] = 0x88;
            }
          }
          else
          {
            for (i=2; i<abytes; i++)
            {
              p[i] = 0x00;
              if (tlen > 0)
              {
                p[i] = char_to_int(tmp[2*(i-2)], 1, 16);
                tlen -= 2;
              }
            }
          }
        }
        else if (op == OP_TYPE_DROP)
        {
          clen = payload.len - alen * 4;
          e += 2;
          e[0] = (clen >> 8) & 0xff;
          e[1] = clen & 0xff;
          payload.len = clen;
        }
      }
    }
  }

  return ret;
}

int process_query(instance_t *instance, ike_sa_id_t *ike_sa_id, payload_t *payload, payload_t *next)
{
  int ret;
  ike_header_t *ike_header;
  ke_payload_t *ke;
  nonce_payload_t *np;
  notify_payload_t *notify;
  id_payload_t *id;
  eap_payload_t *eap;
  sa_payload_t *sa;

  ret = NOT_SET;
  ike_header = NULL;
  ke = NULL;
  np = NULL;

  if ((ret = check_drop_next(instance, next)))
  {
    goto out;
  }

  printf("[VoWiFi] in process_query(): %p\n", payload);

  switch (payload->get_type(payload))
  {
    case PL_HEADER:
      printf("[VoWiFi] before process_ike_header()\n");
      ike_header = (ike_header_t *)payload;
      ret = process_ike_header(instance, ike_sa_id, ike_header);
      break;

    case PLV2_KEY_EXCHANGE:
      printf("[VoWiFi] before process_key_exchange()\n");
      ke = (ke_payload_t *)payload;
      ret = process_key_exchange(instance, ike_sa_id, ke);
      break;

    case PLV2_NONCE:
      printf("[VoWiFi] before process_nonce()\n");
      np = (nonce_payload_t *)payload;
      ret = process_nonce(instance, ike_sa_id, np);
      break;

    case PLV2_NOTIFY:
      printf("[VoWiFi] before process_notify()\n");
      notify = (notify_payload_t *)payload;

      if (!check_exceptional_case(instance, ike_sa_id, notify))
      {
        ret = process_notify(instance, ike_sa_id, notify);
      }
      else
      {
        // TODO: need to assign an appropriate value below
        ret = COMPLETED; 
      }
      break;

    case PLV2_ID_RESPONDER:
      printf("[VoWiFi] before process_identification_responder()\n");
      id = (id_payload_t *)payload;
      ret = process_identification_responder(instance, ike_sa_id, id);
      break;

    case PLV2_EAP:
      printf("[VoWiFi] before process_extensible_authentication()\n");
      eap = (eap_payload_t *)payload;
      ret = process_extensible_authentication(instance, ike_sa_id, eap);
      break;

    default:
      break;
  }

out:
  return ret;
}
/////////////////////////
