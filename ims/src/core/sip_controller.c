#include "sip_controller.h"
//#include "dprint.h"

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

sip_message_t *init_sip_message(char *buf, int len)
{
  assert(buf != NULL);
  assert(len > 0);

  sip_message_t *ret;
  kvp_t *kvp;
  uint8_t key[SC_KEY_LENGTH];
  uint8_t value[SC_VALUE_LENGTH];
  uint8_t *c, *k, *v;
  uint8_t *p[2];
  uint8_t flag, kset;
  int klen, vlen;

  ret = (sip_message_t *)calloc(1, sizeof(sip_message_t));
  c = (uint8_t *)buf;
  p[SC_KEY_IDX] = key;
  p[SC_VALUE_IDX] = value;

  memcpy(key, "header", 6);
  p[SC_KEY_IDX] += 6;
  flag = 1;
  kset = 0;

  while (c - (uint8_t *)buf < len)
  {
    if (*c == ':' && !kset)
    {
      flag = (flag + 1) % 2;
      kset = 1;
    }
    else if (*c == '\n')
    {
      flag = (flag + 1) % 2;
      klen = p[SC_KEY_IDX] - key;
      vlen = p[SC_VALUE_IDX] - value;

      k = key;
      while (*k == ' ')
      {
        k++;
        klen--;
      }

      while (*(k + klen - 1) == ' ')
      {
        klen--;
      }

      v = value;
      while (*v == ' ')
      {
        v++;
        vlen--;
      }

      while (*(v + vlen - 1) == ' ' || *(v + vlen - 1) == '\r')
      {
        vlen--;
      }

      if (klen > 0 && vlen > 0)
      {
        kvp = init_kvp(k, klen, v, vlen);
        add_kvp_to_sip_message(ret, kvp, NULL, 0, 0);
        printf("kvp: key (%d bytes): %.*s\n", kvp->klen, kvp->klen, kvp->key);
      }

      p[SC_KEY_IDX] = key;
      p[SC_VALUE_IDX] = value;
      kset = 0;
    }
    else
    {
      *(p[flag]++) = *c;
    }
    c++;
  }

  return ret;
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
  curr = message->head;

  while (curr)
  {
    if (strncmp(curr->key, "header", curr->klen))
    {
      memcpy(p, curr->key, curr->klen);
      p += curr->klen;

      memcpy(p, ": ", 2);
      p += 2;
    }
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

  printf("  init_val(): val: %.*s, len: %d, delimiter: %d, space: %d\n", len, val, len, delimiter, space);

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
      printf("    attr: %.*s, alen: %d\n", ret->alen, ret->attr, ret->alen);
      t = tmp;
    }
    else if (!inside && (p - val == len))
    {
      tlen = t-tmp;
      ret->val = (uint8_t *)calloc(1, tlen);
      memcpy(ret->val, tmp, tlen);
      ret->vlen = tlen;
      printf("    val: %.*s, vlen: %d\n", ret->vlen, ret->val, ret->vlen);
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

  int ret, vlen;
  uint8_t *val;
  kvp_t *kvp;

  ret = SC_FALSE;
  kvp = get_kvp_from_sip_message(message, "header", 6, 0);
  if (!kvp) goto out;

  val = get_value_from_kvp_by_idx(kvp, 1, &vlen);
  if (!val) goto out;
  if (strstr((const char *)val, code))
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

kvp_t *get_kvp_from_sip_message(sip_message_t *message, uint8_t *key, int klen, int idx)
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

void change_value_from_kvp_by_name(kvp_t *kvp, uint8_t *attr, int alen, uint8_t *value, int vlen)
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
