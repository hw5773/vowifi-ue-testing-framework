#include "sip_controller.h"
//#include "dprint.h"

#define SIP_VERSION_TOKEN "SIP/2.0"
#define SIP_VERSION_TOKEN_LENGTH 7

uint8_t *serialize_value(kvp_t *kvp, int *vlen);

int read_message(char *input, char *buf, int max)
{
  assert(input != NULL);
  assert(buf != NULL);
  assert(max > 0);

  int len;
  FILE *fp;

  fp = fopen(input, "r");
  fseek(fp, 0L, SEEK_END);
  len = (int)ftell(fp);
  fseek(fp, 0L, SEEK_SET);
  fread(buf, 1, len, fp);

  return len;
}

void parse_first_line(sip_message_t *sip, uint8_t *line)
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

        sip->mname = calloc(1, tlen);
        memcpy(sip->mname, tok, tlen);
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
      sip->additional = calloc(1, tlen);
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
    sip->mname = calloc(1, tlen);
    memcpy(sip->mname, tok, tlen);
    sip->mlen = tlen;
  }
}

kvp_t *parse_key_value_line(sip_message_t *sip, uint8_t *line)
{
  kvp_t *ret;
  uint8_t *p, *key, *value;
  int len, klen, vlen;

  len = strlen(line);
  p = line;

  while ((*p != '\n') || (p - line < len))
  {
    if (*p == ':')
      break;
    p++;
  }

  klen = p - line;
  key = (uint8_t *)calloc(1, klen);
  memcpy(key, line, klen);

  p += 1;
  while (*p == ' ')
    p++;
  vlen = len - (p - line);
  value = (uint8_t *)calloc(1, vlen);
  memcpy(value, p, vlen);

  ret = init_kvp(key, klen, value, vlen);

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
  int klen, vlen;

  ret = (sip_message_t *)calloc(1, sizeof(sip_message_t));
  line = strtok(buf, "\r\n");
  parse_first_line(ret, line);
  while (line)
  {
    line = strtok(NULL, "\r\n");
    if (!line)
      break;
    kvp = parse_key_value_line(ret, line);
    add_kvp_to_sip_message(ret, kvp, NULL, 0, 0);
  }

  return ret;
}

int get_message_type(sip_message_t *message)
{
  return message->mtype;
}

const uint8_t *get_message_name(sip_message_t *message)
{
  return message->mname;
}

uint8_t *serialize_sip_message(sip_message_t *message, int *len)
{
  assert(message != NULL);
  assert(len != NULL);

  uint8_t *ret, *p, *vtmp;
  uint8_t tmp[SC_BUF_LENGTH];
  int flen, vlen;
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
    memcpy(p, vtmp, vlen);
    p += vlen;
    *(p++) = '\r';
    *(p++) = '\n';
    curr = curr->next;
  }
  
  *(p++) = '\r';
  *(p++) = '\n';
  *len = p - tmp;
  ret = (uint8_t *)malloc(*len);
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
}

val_t *init_val(uint8_t *val, int len, uint8_t delimiter, uint8_t space)
{
  val_t *ret;
  int tlen;
  uint8_t *p, *t;
  uint8_t tmp[SC_BUF_LENGTH] = {0, };
  uint8_t inside;

  //printf("  init_val(): val: %.*s, len: %d, delimiter: %d, space: %d\n", len, val, len, delimiter, space);

  ret = (val_t *)calloc(1, sizeof(val_t));
  p = val;
  t = tmp;
  inside = 0;
  while (p - val <= len)
  {
    if (!inside && *p == '=')
    {
      tlen = t-tmp;
      ret->attr = (uint8_t *)calloc(1, tlen);
      memcpy(ret->attr, tmp, tlen);
      ret->alen = tlen;
      //printf("    attr: %.*s, alen: %d\n", ret->alen, ret->attr, ret->alen);
      t = tmp;
    }
    else if (!inside && (p - val == len))
    {
      tlen = t-tmp;
      ret->val = (uint8_t *)calloc(1, tlen);
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
      free(val->attr);
    val->alen = 0;

    if (val->val)
      free(val->val);
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
  ret = (uint8_t *)malloc(*vlen);
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
  
  ret = (vlst_t *)calloc(1, sizeof(vlst_t));

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
    free(vlst);
  }
}

kvp_t *init_kvp(uint8_t *key, int klen, uint8_t *value, int vlen)
{
  assert(key != NULL);
  assert(klen > 0);
  assert(value != NULL);
  assert(vlen > 0);

  kvp_t *ret;
  ret = (kvp_t *)calloc(1, sizeof(kvp_t));

  ret->key = (uint8_t *)calloc(1, klen);
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
      free(kvp->key);
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

int is_401_unauthorized_message(sip_message_t *message)
{
  return is_status_code_message(message, "401");
}

int is_200_ok_message(sip_message_t *message)
{
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

void del_kvp_from_sip_message(sip_message_t *message, const uint8_t *key, int klen, int idx)
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
  
  ret = (uint8_t *)malloc(val->vlen);
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
      ret = (uint8_t *)malloc(val->vlen);
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
      free(val->val);
    val->val = (uint8_t *)malloc(vlen);
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
        free(val->val);
      val->val = (uint8_t *)malloc(vlen);
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