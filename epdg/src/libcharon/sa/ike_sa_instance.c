///// Added for VoWiFi /////
#include <inttypes.h>
#include "ike_sa_instance.h"

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

msg_t *init_message(instance_t *instance, int mtype, const uint8_t *key, 
    int vtype, void *val, int vlen)
{
  msg_t *ret;
  uint8_t *p;
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
  if (pthread_mutex_init(&(ret->slock), NULL) != 0)
  {
    printf("initializing a mutex for the send queue failed\n");
  }

  //ret->set_query = _set_query;
  //ret->get_current_query = _get_current_queue;

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

  for (i=0; i<depth; i++)
    printf("  ");
  
  p = buf;
  tmp = get_query_name(query, &tlen);
  if (tlen > 0)
  {
    snprintf(p, tlen, "%s", tmp);
    p += tlen;
  }

  tmp = get_query_value(query, &tlen);
  if (tlen > 0)
  {
    *(p++) = ':';
    snprintf(p, tlen, "%s", tmp);
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
  _print_query(query, 0);
}

query_t *add_query_sub_message(query_t *query, int mtype)
{
  query_t *sub;
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
  printf("nlen: %d\n", nlen);
  query->name = (uint8_t *)calloc(nlen, sizeof(uint8_t));
  memcpy(query->name, name, nlen);
  query->nlen = nlen;
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
  query->value = (uint8_t *)calloc(vlen, sizeof(uint8_t));
  memcpy(query->value, value, vlen);
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
