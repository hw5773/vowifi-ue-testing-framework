///// Added for VoWiFi /////
#ifndef __IKE_SA_INSTANCE_H__
#define __IKE_SA_INSTANCE_H__

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
#define RESET_REQUEST "init\n"
#define FIN_REQUEST "fin\n"
#define ACK_RESPONSE "ACK\n"

#define UPDATE  1
#define NON_UPDATE 0

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
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
  uint8_t *name;
  int nlen;

  int vtype;

  uint8_t *value;
  int vlen;

  struct query_st *parent;
  struct query_st *sub;
  struct query_st *next;
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
  void (*set_query)(struct instance_st *instance, query_t *query);
  pthread_mutex_t slock;

  query_t *query;
  query_t *curr;

  int running;
  bool initiated;
  bool finished;

  const uint8_t *rprev;
  const uint8_t *sprev;
} instance_t;

int check_instance(instance_t *instance, uint64_t ispi, uint64_t rspi, int update);

int _add_message_to_send_queue(instance_t *instance, msg_t *msg);
msg_t *_fetch_message_from_send_queue(instance_t *instance);
void _set_query(instance_t *instance, query_t *query);
msg_t *init_message(instance_t *instance, int mtype, const uint8_t *key, 
    int vtype, void *val, int vlen);
void free_message(msg_t *msg);
instance_t *init_instance(int asock);
void free_instance(instance_t *instance);
int int_to_char(int num, uint8_t *str, int base);

query_t *init_query(void);
void free_query(query_t *query);
void print_query(query_t *query);

query_t *add_query_sub_message(query_t *query, int ptype, int mtype);
int has_query_parent(query_t *query);
query_t *get_query_parent(query_t *query);
void set_query_parent(query_t *query, query_t *parent);
uint8_t *get_query_name(query_t *query, int *nlen);
void set_query_name(query_t *query, uint8_t *name);
int get_query_value_type(query_t *query);
void set_query_value_type(query_t *query, uint8_t *vtype);
uint8_t *get_query_value(query_t *query, int *vlen);
void set_query_value(query_t *query, uint8_t *value);

bool has_query(instance_t *instance);
bool wait_query(instance_t *instance);
bool is_query_finished(instance_t *instance);
query_t *get_query(instance_t *instance);
query_t *get_next_query(instance_t *instance);
bool is_query_name(query_t *query, const uint8_t *name);
query_t *get_sub_query_by_name(query_t *query, const uint8_t *name);

void *sender_run(void *data);
void *listener_run(void *data);

int int_to_char(int num, uint8_t *str, int base);
int char_to_int(char *str, int slen, int base);
#endif /* __IKE_SA_INSTANCE_H__ */
///////////////////////
