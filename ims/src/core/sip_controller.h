#ifndef __SIP_CONTROLLER_H__
#define __SIP_CONTROLLER_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define SC_SUCCESS 1
#define SC_FAILURE -1

#define SC_TRUE 1
#define SC_FALSE 0

#define SC_KEY_LENGTH 1024
#define SC_ATTR_LENGTH 1024
#define SC_VALUE_LENGTH 16384
#define SC_BUF_LENGTH 16384

#define SC_KEY_IDX 0
#define SC_VALUE_IDX 1

typedef struct sip_message_st sip_message_t;
typedef struct kvp_st kvp_t;
typedef struct vlst_st vlst_t;
typedef struct val_st val_t;

struct val_st
{
  uint8_t *attr;
  int alen;
  uint8_t *val;
  int vlen;
  uint8_t delimiter;
  uint8_t space;
  struct val_st *next;
};

struct vlst_st
{
  int num;
  struct val_st *head;
};

struct kvp_st
{
  uint8_t *key;
  int klen;
  struct vlst_st *vlst;
  struct kvp_st *next;
};

struct sip_message_st
{
  int num;
  struct kvp_st *head;
  struct kvp_st *tail;
};

int read_message(char *input, char *buf, int max);
sip_message_t *init_sip_message(char *buf, int len);
void free_sip_message(sip_message_t *message);
uint8_t *serialize_sip_message(sip_message_t *message, int *len);
void print_sip_message(sip_message_t *message);

kvp_t *init_kvp(uint8_t *key, int klen, uint8_t *value, int vlen);
void free_kvp(kvp_t *kvp);

int is_401_unauthorized_message(sip_message_t *message);
int is_200_ok_message(sip_message_t *message);
int get_num_of_kvps_from_sip_message(sip_message_t *message, uint8_t *key, int klen);
kvp_t *get_kvp_from_sip_message(sip_message_t *message, uint8_t *key, int klen, int idx);
int add_kvp_to_sip_message(sip_message_t *message, kvp_t *kvp, uint8_t *key, int klen, int idx);
void del_kvp_from_sip_message(sip_message_t *message, uint8_t *key, int klen, int idx);

int is_attribute_included(kvp_t *kvp, uint8_t *attr, int alen);
int get_num_of_values_from_kvp(kvp_t *kvp);
uint8_t *get_value_from_kvp_by_idx(kvp_t *kvp, int idx, int *vlen);
uint8_t *get_value_from_kvp_by_name(kvp_t *kvp, uint8_t *attr, int alen, int *vlen);
void change_value_from_kvp_by_idx(kvp_t *kvp, int idx, uint8_t *value, int vlen);
void change_value_from_kvp_by_name(kvp_t *kvp, uint8_t *attr, int alen, uint8_t *value, int vlen);
void del_value_from_kvp_by_idx(kvp_t *kvp, int idx);
void del_value_from_kvp_by_name(kvp_t *kvp, uint8_t *attr, int alen);

#endif /* __SIP_CONTROLLER_H__ */
