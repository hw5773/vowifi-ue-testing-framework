///// Added for VoWiFi /////
#ifndef __SIP_INSTANCE_H__
#define __SIP_INSTANCE_H__

#ifndef MAX_MESSAGE_LEN
#define MAX_MESSAGE_LEN 1514
#endif /* MAX_MESSAGE_LEN */

#ifndef MAX_VALUE_LEN
#define MAX_VALUE_LEN 256
#endif /* MAX_VALUE_LEN */

#ifndef MAX_QUEUE_LEN
#define MAX_QUEUE_LEN 8
#endif /* MAX_QUEUE_LEN */

#ifndef MAX_NAME_LEN
#define MAX_NAME_LEN 256
#endif /* MAX_NAME_LEN */

#define MSG_TYPE_ATTRIBUTE 1
#define MSG_TYPE_BLOCK_START 2
#define MSG_TYPE_BLOCK_END 3

#define OP_TYPE_UPDATE 1
#define OP_TYPE_DROP 2

#define VAL_TYPE_NONE 1
#define VAL_TYPE_INTEGER  2
#define VAL_TYPE_UINT8 3
#define VAL_TYPE_UINT16 4
#define VAL_TYPE_STRING 5

#define VAL_LENGTH_NONE 0
#define VAL_LENGTH_INTEGER 4
#define VAL_LENGTH_UINT8 1
#define VAL_LENGTH_UINT16 2

#define HELLO_REQUEST "Hello\n"
#define RESET_REQUEST "reset\n"
#define ACK_RESPONSE "ACK\n"

#define UPDATE  1
#define NON_UPDATE 0

#define DEFAULT_IMS_PORT 7779
#define MAX_CLNT_SIZE 10

#define SHARED_MEMORY_INSTANCE_KEY 3000
#define SHARED_MEMORY_QUERY_KEY 4000
#define SHARED_MEMORY_MESSAGE_BASE 5000

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/types.h>

typedef struct msg_st
{
  uint8_t mtype;
  uint64_t ispi;
  uint64_t rspi;
  uint8_t key[MAX_MESSAGE_LEN];
  int klen;
  int vtype;
  uint8_t val[MAX_VALUE_LEN];
  int vlen;
} msg_t;

typedef struct query_st
{
  uint8_t name[MAX_NAME_LEN];
  int nlen;

  int op;
  int vtype;

  uint8_t value[MAX_VALUE_LEN];
  int vlen;

  struct query_st *parent;
  struct query_st *sub;
  struct query_st *next;
} query_t;

typedef struct instance_st 
{
  int asock;
  int num;

  int slast;
  msg_t *sendq[MAX_QUEUE_LEN];
  pthread_mutex_t slock;

  query_t *query;
  query_t *curr;

  pid_t pid;

  const uint8_t *rprev;
  const uint8_t *sprev;

  int running;
  bool finished;
} instance_t;

typedef struct arg_st
{
  int lsock;
  pthread_t *sender;
  pthread_attr_t *attr;
  void *io_h;
} arg_t;

int check_instance(instance_t *instance);

msg_t *add_message_to_send_queue(instance_t *instance, msg_t *msg);
msg_t *fetch_message_from_send_queue(instance_t *instance);
void set_query(instance_t *instance, query_t *query);
msg_t *init_message(instance_t *instance, int mtype, const uint8_t *key, 
    int vtype, uint8_t *val, int vlen);
void free_message(msg_t *msg);
instance_t *init_instance(int asock);
instance_t *get_instance(void);
void free_instance(instance_t *instance);
int int_to_char(int num, uint8_t *str, int base);

query_t *init_query();
void free_query(query_t *query);
void print_query(query_t *query);

query_t *add_query_sub_message(query_t *query, int ptype, int mtype);
int has_query_parent(query_t *query);
query_t *get_query_parent(query_t *query);
void set_query_parent(query_t *query, query_t *parent);
int get_query_operator(query_t *query);
void set_query_operator(query_t *query, int op);
uint8_t *get_query_name(query_t *query, int *nlen);
void set_query_name(query_t *query, uint8_t *name);
int get_query_value_type(query_t *query);
void set_query_value_type(query_t *query, uint8_t *vtype);
uint8_t *get_query_value(query_t *query, int *vlen);
void set_query_value(query_t *query, uint8_t *value);

bool has_query(instance_t *instance);
bool wait_query(instance_t *instance);
bool is_query_finished(instance_t *instance);
query_t *get_next_query(instance_t *instance);
query_t *get_query(instance_t *instance);
bool is_query_name(query_t *query, const uint8_t *name);
query_t *get_sub_query_by_name(query_t *query, const uint8_t *name);

void *sender_run(void *data);
void *listener_run(void *data);

int int_to_char(int num, uint8_t *str, int base);
int char_to_int(char *str, int slen, int base);

extern instance_t *instance;
extern int vowifi;
#endif /* __SIP_INSTANCE_H__ */
///////////////////////
