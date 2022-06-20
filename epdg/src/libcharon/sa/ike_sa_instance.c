///// Added for VoWiFi /////
#include "ike_sa_instance.h"

int _add_message_to_send_queue(instance_t *instance, msg_t *msg)
{
  int ret;
  ret = -1;

  if (instance->slast >= MAX_QUEUE_LEN)
    return ret;

  instance->sendq[instance->slast++] = msg;

  ret = 1;
  return ret;
}

msg_t *_fetch_message_from_send_queue(instance_t *instance)
{
  int i;
  msg_t *ret;
  ret = NULL;

  if (instance->slast > 0)
  {
    ret = instance->sendq[0];
    for (i=1; i<instance->slast; i++)
      instance->sendq[i-1] = instance->sendq[i];
    instance->slast--;
  }

  return ret;
}

int _add_message_to_recv_queue(instance_t *instance, msg_t *msg)
{
  int ret;
  ret = -1;

  if (instance->rlast >= MAX_QUEUE_LEN)
    return ret;

  instance->recvq[instance->rlast++] = msg;

  ret = 1;
  return ret;
}

msg_t *_fetch_message_from_recv_queue(instance_t *instance, msg_t *msg)
{
  int i;
  msg_t *ret;
  ret = NULL;

  if (instance->rlast > 0)
  {
    ret = instance->recvq[0];
    for (i=1; i<instance->rlast; i++)
      instance->recvq[i-1] = instance->recvq[i];
    instance->rlast--;
  }

  return ret;
}

msg_t *init_message(int type, uint64_t ispi, uint64_t rspi, uint8_t *msg, int len)
{
  msg_t *ret;
  ret = (msg_t *)calloc(1, sizeof(msg_t));
  ret->type = type;
  ret->ispi = ispi;
  ret->rspi = rspi;
  if (msg && len > 0)
    memcpy(ret->msg, msg, len);
  ret->len = len;
  return ret;
}

void free_message(msg_t *msg)
{
  if (msg)
  {
    printf("msg: %p\n", msg);
    printf("msg->msg (msg->len: %d bytes): %s\n", msg->len, msg->msg);

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
/////////////////////////
