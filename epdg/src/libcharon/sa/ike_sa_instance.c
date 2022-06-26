///// Added for VoWiFi /////
#include "ike_sa_instance.h"

int _add_message_to_send_queue(instance_t *instance, msg_t *msg)
{
  int ret;
  ret = -1;

  // TODO: add a lock for the send queue
  if (instance->slast >= MAX_QUEUE_LEN)
    return ret;

  instance->sendq[instance->slast++] = msg;
  // until here

  ret = 1;
  return ret;
}

msg_t *_fetch_message_from_send_queue(instance_t *instance)
{
  int i;
  msg_t *ret;
  ret = NULL;

  // TODO: add a lock for the send queue
  if (instance->slast > 0)
  {
    ret = instance->sendq[0];
    for (i=1; i<instance->slast; i++)
      instance->sendq[i-1] = instance->sendq[i];
    instance->slast--;
  }
  // until here

  return ret;
}

int _add_message_to_recv_queue(instance_t *instance, msg_t *msg)
{
  int ret;
  ret = -1;

  // TODO: add a lock for the receive queue
  if (instance->rlast >= MAX_QUEUE_LEN)
    return ret;

  instance->recvq[instance->rlast++] = msg;
  // until here

  ret = 1;
  return ret;
}

msg_t *_fetch_message_from_recv_queue(instance_t *instance, msg_t *msg)
{
  int i;
  msg_t *ret;
  ret = NULL;

  // TODO: add a lock for the receive queue
  if (instance->rlast > 0)
  {
    ret = instance->recvq[0];
    for (i=1; i<instance->rlast; i++)
      instance->recvq[i-1] = instance->recvq[i];
    instance->rlast--;
  }
  // until here

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
  ret->add_message_to_recv_queue = _add_message_to_recv_queue;
  ret->fetch_message_from_recv_queue = _fetch_message_from_recv_queue;
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
    
    for (i=0; i<instance->rlast; i++)
    {
      free_message(instance->recvq[i]);
    }

    free(instance);
  }
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
      ret = i;
    tmp /= base;
  }

  ret++;

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
/////////////////////////
