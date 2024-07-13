#include <time.h>
#include <stdlib.h>
#include "mem/shm.h"
#include "sip_instance.h"
#include "sip_controller.h"

#define SIP_VERSION_TOKEN "SIP/2.0"
#define SIP_VERSION_TOKEN_LENGTH 7

uint8_t *serialize_value(kvp_t *kvp, int *vlen);

void to_lower(uint8_t *str)
{
  int i;
  i = 0;

  while (str[i])
  {
    if (str[i] >= 'A' && str[i] <= 'Z')
      str[i] = str[i] + 32;
    i++;
  }
}

void parse_sip_first_line(sip_message_t *sip, uint8_t *line)
{
  uint8_t *tok, *p;
  size_t len, tlen;
  len = strlen(line);

  p = line;
  while (*p == ' ')
    p++;
  tok = p;
  tlen = 0;

  // determine whether a message is request or response
  while ((*p != '\n') && (p - line < len))
  {
    if (*p == ' ')
    {
      tlen = p - tok;
    
      if ((tlen == SIP_VERSION_TOKEN_LENGTH)
          && !strncmp(tok, SIP_VERSION_TOKEN, tlen))
      {
        sip->version = SIP_VERSION_TOKEN;
        sip->mtype = SC_SIP_RESPONSE;
      }
      else
      {
        sip->version = SIP_VERSION_TOKEN;
        sip->mtype = SC_SIP_REQUEST;

        sip->mname = (uint8_t *)shm_malloc(tlen + 1);
        if (!(sip->mname))
        {
          LM_ERR("[VoWiFi] parse_sip_first_line(): shm_malloc for sip->mname failed\n");
          exit(1);
        }
        memset(sip->mname, 0, tlen + 1);
        memcpy(sip->mname, tok, tlen);
        LM_ERR("[VoWiFi] before to_lower(): %s\n", sip->mname);
        to_lower(sip->mname);
        LM_ERR("[VoWiFi] after to_lower(): %s\n", sip->mname);
        sip->mlen = tlen;
      }
      break;
    }
    p++;
  }

  // next element
  while (*p == ' ' && (p - line < len))
    p++;
  tok = p;
  tlen = 0;

  while ((*p != '\n') && (p - line < len))
  {
    if (*p == ' ')
    {
      tlen = p - tok;
      sip->additional = (uint8_t *)shm_malloc(tlen);
      if (!(sip->additional))
      {
        LM_ERR("[VoWiFi] parse_sip_first_line(): shm_malloc for sip->additional failed\n");
        exit(1);
      }
      memcpy(sip->additional, tok, tlen);
      sip->alen = tlen;
      break;
    }
    p++;
  }

  // last element
  if (sip->mtype == SC_SIP_RESPONSE)
  {
    while (*p == ' ')
      p++;

    tok = p;
    while (*p != '\r' && *p != '\n' && (p - line < len))
      p++;

    tlen = p - tok;
    sip->mname = (uint8_t *)shm_malloc(tlen + 1);
    if (!(sip->mname))
    {
      LM_ERR("[VoWiFi] parse_sip_first_line(): shm_malloc for sip->mname failed\n");
      exit(1);
    }
    memset(sip->mname, 0, tlen + 1);
    memcpy(sip->mname, tok, tlen);
    to_lower(sip->mname);
    sip->mlen = tlen;
  }
}

kvp_t *parse_key_value_line(sip_message_t *sip, uint8_t *line)
{
  kvp_t *ret;
  uint8_t *p, *key, *value;
  int len, klen, vlen;

  ret = NULL;
  len = strlen(line);
  p = line;

  while ((*p != '\n') || (p - line < len))
  {
    if (*p == ':')
      break;
    p++;
  }

  klen = p - line;
  LM_ERR("key (%d bytes): %.*s\n", klen, klen, line);
  key = (uint8_t *)shm_malloc(klen);
  if (!key)
  {
    LM_ERR("[VoWiFi] parse_key_value_line(): shm_malloc for key failed (klen: %d)\n", klen);
    exit(1);
  }
  memcpy(key, line, klen);

  p += 1;
  while (*p == ' ')
    p++;
  vlen = len - (p - line);
  LM_DBG("value (%d bytes): %.*s\n", vlen, vlen, p);

  if (vlen > 0)
  {
    value = (uint8_t *)shm_malloc(vlen);
    if (!value)
    {
      LM_ERR("[VoWiFi] parse_key_value_line(): shm_malloc for value failed (vlen: %d)\n", vlen);
      exit(1);
    }
    memcpy(value, p, vlen);

    ret = init_kvp(key, klen, value, vlen);
  }
  return ret;
}

sip_message_t *init_sip_message(char *buf, int len)
{
  assert(buf != NULL);
  assert(len > 0);

  sip_message_t *ret;
  kvp_t *kvp;
  uint8_t *value;
  uint8_t *line;
  uint8_t *orig;
  int klen, vlen;

  orig = (char *)shm_malloc(len);
  memset(orig, 0, len);
  memcpy(orig, buf, len);
  ret = (sip_message_t *)shm_malloc(sizeof(sip_message_t));
  if (!ret)
  {
    LM_ERR("[VoWiFi] init_sip_message(): shm_malloc for ret failed\n");
    exit(1);
  }
  memset(ret, 0, sizeof(sip_message_t));
  line = strtok(orig, "\r\n");
  parse_sip_first_line(ret, line);

  if (is_register_message(ret)
      || is_401_unauthorized_message(ret)
      || is_200_ok_message(ret))
  {
    while (line)
    {
      line = strtok(NULL, "\r\n");
      if (!line)
        break;
      LM_DBG("before parse_key_value_line(): %s\n", line);
      kvp = parse_key_value_line(ret, line);
      LM_DBG("after parse_key_value_line()\n");
      LM_DBG("before add_kvp_to_sip_message()\n");
      if (kvp)
        add_kvp_to_sip_message(ret, kvp, NULL, 0, 0);
      LM_DBG("after add_kvp_to_sip_message()\n");
    }
  }
  shm_free(orig);

  return ret;
}

int get_message_type(sip_message_t *message)
{
  return message->mtype;
}

uint8_t *get_message_name(sip_message_t *message, int *mlen)
{
  *mlen = message->mlen;
  return message->mname;
}

uint8_t *serialize_sip_message(sip_message_t *message, int *len)
{
  assert(message != NULL);
  assert(len != NULL);

  uint8_t *ret, *p, *vtmp;
  uint8_t tmp[SC_BUF_LENGTH];
  int vlen;
  kvp_t *curr;

  p = tmp;

  if (message->mtype == SC_SIP_REQUEST)
  {
    memcpy(p, message->mname, message->mlen);
    p += message->mlen;
    *(p++) = ' ';
    
    memcpy(p, message->additional, message->alen);
    p += message->alen;
    *(p++) = ' ';

    memcpy(p, SIP_VERSION_TOKEN, SIP_VERSION_TOKEN_LENGTH);
    p += SIP_VERSION_TOKEN_LENGTH;
    *(p++) = '\r';
    *(p++) = '\n';
  }
  else
  {
    memcpy(p, SIP_VERSION_TOKEN, SIP_VERSION_TOKEN_LENGTH);
    p += SIP_VERSION_TOKEN_LENGTH;
    *(p++) = ' ';
    
    memcpy(p, message->additional, message->alen);
    p += message->alen;
    *(p++) = ' ';

    memcpy(p, message->mname, message->mlen);
    p += message->mlen;
    *(p++) = '\r';
    *(p++) = '\n';
  }

  curr = message->head;

  while (curr)
  {
    memcpy(p, curr->key, curr->klen);
    p += curr->klen;

    memcpy(p, ": ", 2);
    p += 2;
    vtmp = serialize_value(curr, &vlen);
    LM_INFO("[VoWiFi] (serialize_sip_message()) val (%d bytes): %s\n", vlen, vtmp);
    memcpy(p, vtmp, vlen);
    p += vlen;
    *(p++) = '\r';
    *(p++) = '\n';
    curr = curr->next;
  }
  
  *(p++) = '\r';
  *(p++) = '\n';
  *len = p - tmp;
  ret = (uint8_t *)shm_malloc(*len);
  if (!ret)
  {
    LM_ERR("[VoWiFi] serialize_sip_message(): shm_malloc for ret failed\n");
    exit(1);
  }
  memcpy(ret, tmp, *len);

  printf("serialized (%d bytes): %.*s\n", *len, *len, ret);
  return ret;
}

void print_sip_message(sip_message_t *message)
{
  assert(message != NULL);

  kvp_t *curr;
  uint8_t *value;
  int vlen;
  curr = message->head;

  printf(">>> # of kvps: %d\n", message->num);
  while (curr)
  {
    value = serialize_value(curr, &vlen);
    printf("key (%d bytes): %.*s, value (%d bytes): %.*s\n", curr->klen, curr->klen, curr->key, vlen, vlen, value);
    curr = curr->next;
  }
}

void free_sip_message(sip_message_t *message)
{
  kvp_t *curr, *next;

  if (message)
  {
    curr = message->head;
    while (curr)
    {
      next = curr->next;
      free_kvp(curr);
      curr = next;
    }
  }
  shm_free(message);
}

val_t *init_val(uint8_t *val, int len, uint8_t delimiter, uint8_t space)
{
  val_t *ret;
  int tlen;
  uint8_t *p, *t;
  uint8_t tmp[SC_BUF_LENGTH] = {0, };
  uint8_t inside;

  //printf("  init_val(): val: %.*s, len: %d, delimiter: %d, space: %d\n", len, val, len, delimiter, space);

  ret = (val_t *)shm_malloc(sizeof(val_t));
  if (!ret)
  {
    LM_ERR("[VoWiFi] init_val(): shm_malloc for ret failed\n");
    exit(1);
  }
  memset(ret, 0, sizeof(val_t));
  p = val;
  t = tmp;
  inside = 0;
  while (p - val <= len)
  {
    if (!inside && *p == '=')
    {
      tlen = t-tmp;
      ret->attr = (uint8_t *)shm_malloc(tlen);
      if (!(ret->attr))
      {
        LM_ERR("[VoWiFi] init_val(): shm_malloc for ret->attr failed\n");
        exit(1);
      }
      memcpy(ret->attr, tmp, tlen);
      ret->alen = tlen;
      //printf("    attr: %.*s, alen: %d\n", ret->alen, ret->attr, ret->alen);
      t = tmp;
    }
    else if (!inside && (p - val == len))
    {
      tlen = t-tmp;
      ret->val = (uint8_t *)shm_malloc(tlen);
      if (!(ret->val))
      {
        LM_ERR("[VoWiFi] init_val(): shm_malloc for ret->val failed\n");
        exit(1);
      }
      memcpy(ret->val, tmp, tlen);
      ret->vlen = tlen;
      //printf("    val: %.*s, vlen: %d\n", ret->vlen, ret->val, ret->vlen);
    }
    else
    {
      if (*p == '\"')
        inside = (inside + 1) % 2;
      *(t++) = *p;
    }

    p++;
  }
  ret->delimiter = delimiter;
  ret->space = space;

  return ret;
}

void free_val(val_t *val)
{
  if (val)
  {
    if (val->attr)
      shm_free(val->attr);
    val->alen = 0;

    if (val->val)
      shm_free(val->val);
    val->vlen = 0;
  }
}

uint8_t *serialize_value(kvp_t *kvp, int *vlen)
{
  assert(kvp != NULL);
  assert(vlen != NULL);

  uint8_t *ret, *p;
  uint8_t tmp[SC_BUF_LENGTH] = {0, };
  vlst_t *vlst;
  val_t *val;

  vlst = kvp->vlst;
  p = tmp;

  val = vlst->head;
  while (val)
  {
    if (val->attr)
    {
      memcpy(p, val->attr, val->alen);
      p += val->alen;
      *(p++) = '=';
    }
    memcpy(p, val->val, val->vlen);
    p += val->vlen;
    
    if (val->delimiter)
      *(p++) = val->delimiter;

    if (val->space)
      *(p++) = ' ';

    val = val->next;
  }
  *vlen = p-tmp;
  ret = (uint8_t *)shm_malloc(*vlen);
  if (!ret)
  {
    LM_ERR("[VoWiFi] serialize_value(): shm_malloc for ret failed\n");
    exit(1);
  }
  memcpy(ret, tmp, *vlen);

  return ret;
}

void add_val_to_vlst(vlst_t *vlst, val_t *val)
{
  assert(vlst != NULL);
  assert(val != NULL);

  val_t *curr;
  curr = vlst->head;

  if (!curr)
  {
    vlst->head = val;
  }
  else
  {
    while (curr->next)
    {
      curr = curr->next;
    }
    curr->next = val;
  }
}

vlst_t *init_vlst(uint8_t *value, int vlen)
{
  vlst_t *ret;
  val_t *val;
  int i, tlen;
  uint8_t space, inside;
  uint8_t *p, *q;
  uint8_t tmp[SC_BUF_LENGTH] = {0, };
  
  ret = (vlst_t *)shm_malloc(sizeof(vlst_t));
  if (!ret)
  {
    LM_ERR("[VoWiFi] init_vlst(): shm_malloc() failed\n");
    exit(1);
  }
  memset(ret, 0, sizeof(vlst_t));

  p = value;
  q = tmp;
  space = 0;
  inside = 0;
  while (p - value <= vlen)
  {
    if (!inside && (*p == ';'))
    {
      tlen = q - tmp;
      val = init_val(tmp, tlen, *p, 0);
      add_val_to_vlst(ret, val);
      ret->num += 1;
      q = tmp;
      memset(tmp, 0, SC_BUF_LENGTH);
    }
    else if (!inside && (p - value == vlen))
    {
      tlen = q - tmp;
      val = init_val(tmp, tlen, 0, 0);
      add_val_to_vlst(ret, val);
      ret->num += 1;
      q = tmp;
      memset(tmp, 0, SC_BUF_LENGTH);
    }
    else if (!inside && *p == ' ')
    {
      tlen = q - tmp;
      val = init_val(tmp, tlen, 0, 1);
      add_val_to_vlst(ret, val);
      ret->num += 1;
      q = tmp;
      memset(tmp, 0, SC_BUF_LENGTH);
    }
    else if (!inside && *p == ',')
    {
      tlen = q - tmp;
      if (*(p+1) == ' ')
      {
        p++;
        space = 1;
      }
      else
      {
        space = 0;
      }
      val = init_val(tmp, tlen, ',', space);
      add_val_to_vlst(ret, val);
      ret->num += 1;
      q = tmp;
      memset(tmp, 0, SC_BUF_LENGTH);
    }
    else
    {
      if (*p == '<')
        inside = 1;
      if (*p == '>')
        inside = 0;
      if (*p == '\"')
        inside = (inside + 1) % 2;
      *(q++) = *p;
    }
    p++;
  }

  return ret;
}

void free_vlst(vlst_t *vlst)
{
  val_t *curr, *next;

  if (vlst)
  {
    curr = vlst->head;

    while (curr)
    {
      next = curr->next;
      free_val(curr);
      curr = next;
    }
    shm_free(vlst);
  }
}

kvp_t *init_kvp(uint8_t *key, int klen, uint8_t *value, int vlen)
{
  assert(key != NULL);
  assert(klen > 0);
  assert(value != NULL);
  assert(vlen > 0);

  kvp_t *ret;
  ret = (kvp_t *)shm_malloc(sizeof(kvp_t));
  if (!ret)
  {
    LM_ERR("[VoWiFi] init_kvp(): shm_malloc() for ret failed\n");
    exit(1);
  }
  memset(ret, 0, sizeof(kvp_t));

  ret->key = (uint8_t *)shm_malloc(klen);
  if (!(ret->key))
  {
    LM_ERR("[VoWiFi] init_kvp(): shm_malloc() for ret->key failed\n");
    exit(1);
  }
  memcpy(ret->key, key, klen);
  ret->klen = klen;

  ret->vlst = init_vlst(value, vlen);
  return ret;
}

void free_kvp(kvp_t *kvp)
{
  if (kvp)
  {
    if (kvp->key)
      shm_free(kvp->key);
    kvp->klen = 0;
    
    if (kvp->vlst)
      free_vlst(kvp->vlst);
  }
}

int is_status_code_message(sip_message_t *message, const char *code)
{
  assert(message != NULL);

  int ret, mtype, clen;
  uint8_t *mcode;

  ret = SC_FALSE;
  mtype = get_message_type(message);
  
  if (mtype != SC_SIP_RESPONSE)
    goto out;

  mcode = get_additional_info(message, &clen);
 
  if (clen != strlen(code))
    goto out;

  if (!strncmp(code, mcode, clen))
    ret = SC_TRUE;

out:
  return ret;
}

int is_register_message(sip_message_t *message)
{
  assert(message != NULL);
  const char *reg = "register";
  int ret;

  ret = SC_FALSE;
  if (message->mlen != strlen(reg))
    goto out;

  if (!strncmp(message->mname, reg, message->mlen))
    ret = SC_TRUE;

out:
  return ret;
}

int is_401_unauthorized_message(sip_message_t *message)
{
  assert(message != NULL);
  return is_status_code_message(message, "401");
}

int is_200_ok_message(sip_message_t *message)
{
  assert(message != NULL);
  return is_status_code_message(message, "200");
}

uint8_t *get_additional_info(sip_message_t *message, int *alen)
{
  *alen = message->alen;
  return message->additional;
}

int get_num_of_kvps_from_sip_message(sip_message_t *message, uint8_t *key, int klen)
{
  assert(message != NULL);
  assert(key != NULL);
  assert(klen > 0);

  int ret;
  kvp_t *curr;
  ret = 0;
  curr = message->head;

  while (curr)
  {
    if (curr->klen == klen
        && !strncmp((const char *)(curr->key), (const char *)key, curr->klen))
      ret++;
    curr = curr->next;
  }

  return ret;
}

kvp_t *get_kvp_from_sip_message(sip_message_t *message, const uint8_t *key, int klen, int idx)
{
  assert(message != NULL);
  assert(key != NULL);
  assert(klen > 0);

  kvp_t *ret, *curr;

  ret = NULL;
  curr = message->head;

  while (curr)
  {
    LM_ERR("curr->key: %.*s, key: %.*s\n", curr->klen, curr->key, klen, key);
    if (curr->klen == klen
        && !strncmp((const char *)(curr->key), (const char *)key, curr->klen))
    {
      idx--;
      if (idx < 0)
      {
        ret = curr;
        break;
      }
    }
    curr = curr->next;
  }

  return ret;
}

void del_kvp_from_sip_message(sip_message_t *message, uint8_t *key, int klen, int idx)
{
  assert(message != NULL);
  assert(key != NULL);
  assert(klen > 0);

  kvp_t *res, *prev, *curr;

  res = NULL;
  prev = NULL;
  curr = message->head;

  while (curr)
  {
    if (curr->klen == klen
        && !strncmp((const char *)(curr->key), (const char *)key, curr->klen))
    {
      idx--;

      if (idx < 0)
      {
        res = curr;
        if (!prev)
          message->head = curr->next;
        else
          prev->next = curr->next;

        if (message->tail == curr)
          message->tail = prev;
        break;
      }
    }
    prev = curr;
    curr = curr->next;
  }

  if (res)
  {
    free_kvp(res);
    message->num--;
  }
}

int add_kvp_to_sip_message(sip_message_t *message, kvp_t *kvp, uint8_t *key, int klen, int idx)
{
  assert(message != NULL);
  assert(kvp != NULL);

  kvp_t *prev;
  int ret;

  ret = 0;
  if (!key)
    prev = message->tail;
  else
  {
    prev = get_kvp_from_sip_message(message, key, klen, idx);

    if (!prev)
      prev = message->tail;
  }

  if (prev)
  {
    kvp->next = prev->next;
    prev->next = kvp;
  }
  else
  {
    if (!message->head)
      message->head = kvp;
  }

  if (!prev || (prev == message->tail))
    message->tail = kvp;

  ret = SC_SUCCESS;
  message->num++;
  return ret;
}

int is_attribute_included(kvp_t *kvp, uint8_t *attr, int alen)
{
  assert(kvp != NULL);
  assert(attr != NULL);
  assert(alen > 0);

  int ret;
  vlst_t *vlst;
  val_t *val;

  ret = SC_FALSE;
  vlst = kvp->vlst;
  val = vlst->head;

  while (val)
  {
    if (val->alen == alen 
        && !strncmp(val->attr, attr, alen))
    {
      ret = SC_TRUE;
      break;
    }
    val = val->next;
  }

  return ret;
}

int get_num_of_values_from_kvp(kvp_t *kvp)
{
  assert(kvp != NULL);
  return kvp->vlst->num;
}

uint8_t *get_value_from_kvp_by_idx(kvp_t *kvp, int idx, int *vlen)
{
  assert(kvp != NULL);
  assert(idx >= 0);
  assert(vlen != NULL);

  uint8_t *ret;
  vlst_t *vlst;
  val_t *val;
  int i;

  ret = NULL;
  vlst = kvp->vlst;
  val = vlst->head;

  if (idx >= vlst->num) goto out;

  for (i=0; i<idx; i++)
    val = val->next;
  
  ret = (uint8_t *)shm_malloc(val->vlen);
  if (!ret)
  {
    LM_ERR("[VoWiFi] get_value_from_kvp_by_idx(): shm_malloc for ret failed\n");
    exit(1);
  }
  memcpy(ret, val->val, val->vlen);
  *vlen = val->vlen;

out:
  return ret;
}

uint8_t *get_value_from_kvp_by_name(kvp_t *kvp, uint8_t *attr, int alen, int *vlen)
{
  assert(kvp != NULL);
  assert(attr != NULL);
  assert(alen > 0);
  assert(vlen != NULL);

  uint8_t *ret;
  vlst_t *vlst;
  val_t *val;

  ret = NULL;
  vlst = kvp->vlst;
  val = vlst->head;

  while (val)
  {
    if (val->alen == alen 
        && !strncmp(val->attr, attr, alen))
    {
      ret = (uint8_t *)shm_malloc(val->vlen);
      if (!ret)
      {
        LM_ERR("[VoWiFi] get_value_from_kvp_by_name(): shm_malloc for ret failed\n");
        exit(1);
      }
      memcpy(ret, val->val, val->vlen);
      *vlen = val->vlen;
      break;
    }
    val = val->next;
  }

  return ret;
}

void change_value_from_kvp_by_idx(kvp_t *kvp, int idx, uint8_t *value, int vlen)
{
  assert(kvp != NULL);
  assert(idx >= 0);
  assert(value != NULL);
  assert(vlen > 0);

  vlst_t *vlst;
  val_t *val;
  int i;

  vlst = kvp->vlst;
  val = vlst->head;

  for (i=0; i<idx; i++)
    val = val->next;

  if (val)
  {
    if (val->val)
      shm_free(val->val);
    val->val = (uint8_t *)shm_malloc(vlen);
    if (!(val->val))
    {
      LM_ERR("[VoWiFi] change_value_from_kvp_by_idx(): shm_malloc for val->val failed\n");
      exit(1);
    }
    memcpy(val->val, value, vlen);
    val->vlen = vlen;
  }
}

void change_value_from_kvp_by_name(kvp_t *kvp, const uint8_t *attr, int alen, const uint8_t *value, int vlen)
{
  assert(kvp != NULL);
  assert(attr != NULL);
  assert(alen > 0);
  assert(value != NULL);
  assert(vlen > 0);

  vlst_t *vlst;
  val_t *val;

  vlst = kvp->vlst;
  val = vlst->head;

  while (val)
  {
    if (val->alen == alen 
        && !strncmp(val->attr, attr, alen))
    {
      if (val->val)
        shm_free(val->val);
      val->val = (uint8_t *)shm_malloc(vlen);
      if (!(val->val))
      {
        LM_ERR("[VoWiFi] change_value_from_kvp_by_name(): shm_malloc for val->val failed\n");
        exit(1);
      }
      memcpy(val->val, value, vlen);
      val->vlen = vlen;
    }
    val = val->next;
  }
}

void del_value_from_kvp_by_idx(kvp_t *kvp, int idx)
{
  assert(kvp != NULL);
  assert(idx >= 0);

  vlst_t *vlst;
  val_t *prev, *curr;
  int i;

  vlst = kvp->vlst;
  prev = NULL;
  curr = vlst->head;

  for (i=0; i<idx; i++)
  {
    if (i == idx-1)
      prev = curr;
    curr = curr->next;
  }

  if (prev)
    prev->next = curr->next;
  else
    vlst->head = curr->next;
  free_val(curr);
}

void del_value_from_kvp_by_name(kvp_t *kvp, uint8_t *attr, int alen)
{
  assert(kvp != NULL);
  assert(attr != NULL);
  assert(alen > 0);

  vlst_t *vlst;
  val_t *prev, *curr;

  vlst = kvp->vlst;
  prev = NULL;
  curr = vlst->head;

  while (curr)
  {
    if (curr->alen == alen 
        && !strncmp(curr->attr, attr, alen))
      break;
    prev = curr;
    curr = curr->next;
  }

  if (prev)
    prev->next = curr->next;
  else
    vlst->head = curr->next;
  free_val(curr);
}

void process_query(instance_t *instance, sip_message_t *sip)
{
  query_t *query;
  kvp_t *kvp;
  uint8_t *tmp, *change, *value;
  int i, tlen, vlen, vtype, op, random;
  const uint8_t *key, *attr, *val;
  srand(time(NULL));

  if ((query = get_query(instance))
      && is_query_name(query, "401_unauthorized")
      && (query = get_sub_query_by_name(query, "www_authenticate"))
      && (query = get_sub_query_by_name(query, "nonce")))
  {
    key = "WWW-Authenticate";
    attr = "nonce";
    kvp = get_kvp_from_sip_message(sip, key, strlen(key), 0);
    value = get_value_from_kvp_by_name(kvp, attr, strlen(attr), &vlen);
    vtype = get_query_value_type(query);
    op = get_query_operator(query);

    if ((vtype == VAL_TYPE_STRING) && op == OP_TYPE_UPDATE)
    {
      tmp = get_query_value(query, &tlen);
      change = (uint8_t *)shm_malloc(vlen);

      if (tlen == strlen("max")
          && (!strncmp(tmp, "max", tlen)))
      {
        memset(change, 'F', vlen);
      }
      else if (tlen == strlen("min")
          && (!strncmp(tmp, "min", tlen)))
      {
        memset(change, '0', vlen);
      }
      else if (tlen == strlen("median")
          && (!strncmp(tmp, "median", tlen)))
      {
        memset(change, '8', vlen);
      }
      else if (tlen == strlen("mean")
          && (!strncmp(tmp, "mean", tlen)))
      {
        memset(change, '8', vlen);
      }
      else if (tlen == strlen("random")
          && (!strncmp(tmp, "random", tlen)))
      {
        for (i=0; i<vlen; i++)
        {
          random = rand() % 36;
          if (random >= 0 && random < 10)
            random += 48;
          else
            random += 55;
          change[i] = random;
        }
      }
      else
      {
        memset(change, '0', vlen);
      }
      change[vlen-1] = '=';
      change_value_from_kvp_by_name(kvp, attr, strlen(attr), change, vlen);
      shm_free(change);
    }
    else if (op == OP_TYPE_DROP)
    {
      del_value_from_kvp_by_name(kvp, attr, strlen(attr));
    }
  }

  if ((query = get_query(instance))
      && is_query_name(query, "401_unauthorized")
      && (query = get_sub_query_by_name(query, "www_authenticate"))
      && (query = get_sub_query_by_name(query, "algorithm")))
  {
    key = "WWW-Authenticate";
    attr = "algorithm";
    kvp = get_kvp_from_sip_message(sip, key, strlen(key), 0);
    LM_ERR("kvp: %p\n", kvp);
    vtype = get_query_value_type(query);
    LM_ERR("vtype: %d\n", vtype);
    op = get_query_operator(query);
    LM_ERR("op: %d\n", op);

    if ((vtype == VAL_TYPE_STRING) && op == OP_TYPE_UPDATE)
    {
      tmp = get_query_value(query, &tlen);
      LM_ERR("tmp: %.*s\n", tlen, tmp);

      if (strstr(tmp, "AKAv1"))
      {
        LM_ERR("here in strstr(tmp, AKAv1)");
        if (strstr(tmp, "sess"))
        {
          val = "AKAv1-MD5-sess";
        }
        else if (strstr(tmp, "SHA-256"))
        {
          val = "AKAv1-SHA-256";
        }
        else if (strstr(tmp, "MD5"))
        {
          val = "AKAv1-MD5";
        }
        else
        {
          val = "AKAv1-MD5";
        }
      }
      else
      {
        LM_ERR("here in not strstr(tmp, AKAv1)");
        if (strstr(tmp, "SHA-256"))
        {
          val = "SHA-256";
        }
        else if (strstr(tmp, "MD5"))
        {
          val = "MD5";
        }
        else
        {
          val = "MD5";
        }
      }

      LM_ERR("[VoWiFi] before change_value_from_kvp_by_name(): kvp: %p, attr: %s, strlen(attr): %lu, val: %s, strlen(val): %lu\n", kvp, attr, strlen(attr), val, strlen(val));
      change_value_from_kvp_by_name(kvp, attr, strlen(attr), val, strlen(val));
    }
    else if (op == OP_TYPE_DROP)
    {
      del_value_from_kvp_by_name(kvp, attr, strlen(attr));
    }
    LM_ERR("[VoWiFi] after change_value_from_kvp_by_name(): attr: %s, strlen(attr): %lu, val: %s, strlen(val): %lu\n", attr, strlen(attr), val, strlen(val));
  }

  if ((query = get_query(instance))
      && is_query_name(query, "401_unauthorized")
      && (query = get_sub_query_by_name(query, "security_server"))
      && (query = get_sub_query_by_name(query, "alg")))
  {
    key = "Security-Server";
    attr = "alg";
    kvp = get_kvp_from_sip_message(sip, key, strlen(key), 0);
    vtype = get_query_value_type(query);
    op = get_query_operator(query);

    if ((vtype == VAL_TYPE_STRING) && op == OP_TYPE_UPDATE)
    {
      tmp = get_query_value(query, &tlen);

      if (strstr(tmp, "hmac-md5-96"))
      {
        val = "hmac-md5-96";
      }
      else if (strstr(tmp, "hmac-sha-1-96"))
      {
        val = "hmac-sha-1-96";
      }
      else
      {
        val = "hmac-md5-96";
      }
      change_value_from_kvp_by_name(kvp, attr, strlen(attr), val, strlen(val));
    }
    else if (op == OP_TYPE_DROP)
    {
      del_value_from_kvp_by_name(kvp, attr, strlen(attr));
    }
  }

  if ((query = get_query(instance))
      && is_query_name(query, "401_unauthorized")
      && (query = get_sub_query_by_name(query, "security_server"))
      && (query = get_sub_query_by_name(query, "ealg")))
  {
    key = "Security-Server";
    attr = "ealg";
    kvp = get_kvp_from_sip_message(sip, key, strlen(key), 0);
    vtype = get_query_value_type(query);
    op = get_query_operator(query);

    if ((vtype == VAL_TYPE_STRING) && op == OP_TYPE_UPDATE)
    {
      tmp = get_query_value(query, &tlen);

      if (strstr(tmp, "des-ede3-cbc"))
      {
        val = "des-ede3-cbc";
      }
      else if (strstr(tmp, "null"))
      {
        val = "null";
      }
      else if (strstr(tmp, "aes256-cbc"))
      {
        val = "aes256-cbc";
      }
      else
      {
        val = "null";
      }
      change_value_from_kvp_by_name(kvp, attr, strlen(attr), val, strlen(val));
    }
    else if (op == OP_TYPE_DROP)
    {
      del_value_from_kvp_by_name(kvp, attr, strlen(attr));
    }
  }

  if ((query = get_query(instance))
      && is_query_name(query, "401_unauthorized")
      && (query = get_sub_query_by_name(query, "security_server"))
      && (query = get_sub_query_by_name(query, "prot")))
  {
    key = "Security-Server";
    attr = "prot";
    kvp = get_kvp_from_sip_message(sip, key, strlen(key), 0);
    vtype = get_query_value_type(query);
    op = get_query_operator(query);

    if ((vtype == VAL_TYPE_STRING) && op == OP_TYPE_UPDATE)
    {
      tmp = get_query_value(query, &tlen);

      if (strstr(tmp, "null"))
      {
        val = "null";
      }
      else if (strstr(tmp, "esp"))
      {
        val = "esp";
      }
      else if (strstr(tmp, "ah"))
      {
        val = "ah";
      }
      else if (strstr(tmp, "trans"))
      {
        val = "trans";
      }
      else
      {
        val = "null";
      }
      change_value_from_kvp_by_name(kvp, attr, strlen(attr), val, strlen(val));
    }
    else if (op == OP_TYPE_DROP)
    {
      del_value_from_kvp_by_name(kvp, attr, strlen(attr));
    }
  }

  if ((query = get_query(instance))
      && is_query_name(query, "401_unauthorized")
      && (query = get_sub_query_by_name(query, "security_server"))
      && (query = get_sub_query_by_name(query, "mod")))
  {
    key = "Security-Server";
    attr = "mod";
    kvp = get_kvp_from_sip_message(sip, key, strlen(key), 0);
    vtype = get_query_value_type(query);
    op = get_query_operator(query);

    if ((vtype == VAL_TYPE_STRING) && op == OP_TYPE_UPDATE)
    {
      tmp = get_query_value(query, &tlen);

      if (strstr(tmp, "trans"))
      {
        val = "trans";
      }
      else if (strstr(tmp, "null"))
      {
        val = "null";
      }
      else if (strstr(tmp, "tun"))
      {
        val = "tun";
      }
      else
      {
        val = "null";
      }
      change_value_from_kvp_by_name(kvp, attr, strlen(attr), val, strlen(val));
    }
    else if (op == OP_TYPE_DROP)
    {
      del_value_from_kvp_by_name(kvp, attr, strlen(attr));
    }
  }

}

void report_message(instance_t *instance, sip_message_t *message)
{
  const uint8_t *symbol;
  uint8_t *mname;
  int mtype, mlen;
  msg_t *msg;
  query_t *query;
  symbol = NULL;
  mtype = get_message_type(message);

  if (mtype == SC_SIP_REQUEST)
  {
    mname = get_message_name(message, &mlen);
    if (!strncmp(mname, "register", strlen("register"))
        && !strncmp(instance->rprev, "ike_auth_3_request", strlen("ike_auth_3_request")))
    {
      symbol = "register_1";
      instance->rprev = "register_1";
    }
    else if (!strncmp(mname, "register", strlen("register"))
        && !strncmp(instance->rprev, "register_1", strlen("register_1")))
    {
      symbol = "register_2";
      instance->rprev = "register_2";
    }
    else
    {
      symbol = (const char *)mname;
      instance->rprev = (const char *)mname;
    }
  }
  else if (mtype == SC_SIP_RESPONSE)
  {
    if ((query = get_next_query(instance)))
    {
      mname = get_message_name(message, &mlen);
      LM_ERR("query->name: %s, mname: %s, instance->sprev: %s\n", query->name, mname, instance->sprev);
      if (is_query_name(query, "401_unauthorized")
          && (strlen(mname) >= strlen("unauthorized"))
          && (!strncmp(mname, "unauthorized", strlen("unauthorized")))
          && (!strncmp(instance->sprev, "ike_auth_3_response",
              strlen("ike_auth_3_response"))))
      {
        symbol = "401_unauthorized";
        instance->sprev = "401_unauthorized";
      }
      else if (is_query_name(query, "200_ok")
          && (strlen(mname) >= strlen("ok"))
          && (!strncmp(mname, "ok", strlen("ok")))
          && (!strncmp(instance->sprev, "401_unauthorized",
              strlen("401_unauthorized"))))
      {
        symbol = "200_ok";
        instance->sprev = "200_ok";
      }
    }
  }

  LM_ERR("symbol: %s\n", symbol);
  if (symbol)
  {
    msg = init_message(instance, MSG_TYPE_BLOCK_START,
        symbol, VAL_TYPE_NONE, NULL, VAL_LENGTH_NONE);
    add_message_to_send_queue(instance, msg);

    msg = init_message(instance, MSG_TYPE_BLOCK_END,
        NULL, VAL_TYPE_NONE, NULL, VAL_LENGTH_NONE);
    add_message_to_send_queue(instance, msg);
  }
}
