///// Added for VoWiFi /////
#ifndef IKE_SA_INSTANCE_H__
#define IKE_SA_INSTANCE_H__

#ifndef MAX_MESSAGE_LEN
#define MAX_MESSAGE_LEN 1514
#endif /* MAX_MESSAGE_LEN */

#ifndef MAX_QUEUE_LEN
#define MAX_QUEUE_LEN 10
#endif /* MAX_QUEUE_LEN */

#define MSG_TYPE_ATTRIBUTE 1
#define MSG_TYPE_BLOCK_START 2
#define MSG_TYPE_BLOCK_END 3

#define VAL_TYPE_NONE 1
#define VAL_TYPE_INTEGER  2
#define VAL_TYPE_UINT16 3
#define VAL_TYPE_STRING 4

#define VAL_LENGTH_NONE 0
#define VAL_LENGTH_INTEGER 4
#define VAL_LENGTH_UINT16 2

#define HELLO_REQUEST "Hello\n"
#define HELLO_RESPONSE  "ACK\n"
#define ACK_RESPONSE "ACK\n"

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

typedef struct msg_st
{
  uint8_t mtype;
  uint64_t ispi;
  uint64_t rspi;
  uint8_t key[MAX_MESSAGE_LEN];
  int klen;
  int vtype;
  void *val;
  int vlen;
} msg_t;

typedef struct query_st
{

} query_t;

typedef struct instance_st 
{
  int asock;
  uint64_t ispi;
  uint64_t rspi;

  int slast;
  msg_t *sendq[MAX_QUEUE_LEN];
  int (*add_message_to_send_queue)(struct instance_st *instance, msg_t *msg);
  msg_t *(*fetch_message_from_send_queue)(struct instance_st *instance);
  pthread_mutex_t slock;

  int rlast;
  msg_t *recvq[MAX_QUEUE_LEN];
  int (*add_message_to_recv_queue)(struct instance_st *instance, msg_t *msg);
  msg_t *(*fetch_message_from_recv_queue)(struct instance_st *instance);
  pthread_mutex_t rlock;

  int running;
} instance_t;

int _add_message_to_send_queue(instance_t *instance, msg_t *msg);
msg_t *_fetch_message_from_send_queue(instance_t *instance);
int _add_message_to_recv_queue(instance_t *instance, msg_t *msg);
msg_t *_fetch_message_from_recv_queue(instance_t *instance, msg_t *msg);
msg_t *init_message(instance_t *instance, int mtype, const uint8_t *key, 
    int vtype, void *val, int vlen);
void free_message(msg_t *msg);
instance_t *init_instance(int asock);
void free_instance(instance_t *instance);
int int_to_char(int num, uint8_t *str, int base);

void *sender_run(void *data);
void *listener_run(void *data);
#endif /* IKE_SA_INSTANCE_H__ */
///////////////////////
