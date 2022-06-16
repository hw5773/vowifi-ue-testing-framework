///// Added for VoWiFi /////
#ifndef IKE_SA_INSTANCE_H__
#define IKE_SA_INSTANCE_H__

#ifndef MAX_MESSAGE_LEN
#define MAX_MESSAGE_LEN 256
#endif /* MAX_MESSAGE_LEN */

#ifndef MAX_QUEUE_LEN
#define MAX_QUEUE_LEN 10
#endif /* MAX_QUEUE_LEN */

#define MSG_TYPE_ATTRIBUTE 0
#define MSG_TYPE_IKE_MESSAGE_START 1
#define MSG_TYPE_IKE_MESSAGE_END 2
#define MSG_TYPE_IKE_PAYLOAD_START 3
#define MSG_TYPE_IKE_PAYLOAD_END 4

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct msg_st
{
  uint8_t type;
  uint64_t ispi;
  uint64_t rspi;
  uint8_t msg[MAX_MESSAGE_LEN];
  int len;
} msg_t;

typedef struct instance_st 
{
  int asock;

  int slast;
  msg_t *sendq[MAX_QUEUE_LEN];
  int (*add_message_to_send_queue)(struct instance_st *instance, msg_t *msg);
  msg_t *(*fetch_message_from_send_queue)(struct instance_st *instance);

  int rlast;
  msg_t *recvq[MAX_QUEUE_LEN];
  int (*add_message_to_recv_queue)(struct instance_st *instance, msg_t *msg);
  msg_t *(*fetch_message_from_recv_queue)(struct instance_st *instance);

  int running;
} instance_t;

int _add_message_to_send_queue(instance_t *instance, msg_t *msg);
msg_t *_fetch_message_from_send_queue(instance_t *instance);
int _add_message_to_recv_queue(instance_t *instance, msg_t *msg);
msg_t *_fetch_message_from_recv_queue(instance_t *instance, msg_t *msg);
msg_t *init_message(int type, uint64_t ispi, uint64_t rspi, uint8_t *msg, int len);
void free_message(msg_t *msg);
instance_t *init_instance(int asock);
void free_instance(instance_t *instance);
#endif /* IKE_SA_INSTANCE_H__ */
///////////////////////
