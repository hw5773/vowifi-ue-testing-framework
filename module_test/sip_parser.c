#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "sip_controller.h"

int main(int argc, char *argv[])
{
  FILE *fp;
  uint8_t buf[16384];
  uint8_t *serialized, *value, *change;
  int len, slen, vlen;
  sip_message_t *reg;
  sip_message_t *unauth;
  kvp_t *kvp;
  const uint8_t *key, *val, *attr;

  memset(buf, 0, sizeof(buf));
  fp = fopen("register.txt", "r");
  len = fread(buf, 1, sizeof(buf), fp);
  printf("register (%d bytes): %.*s\n", len, len, buf);
  fclose(fp);
  reg = init_sip_message(buf, len);
  if (reg->mtype == SC_SIP_REQUEST)
  {
    printf("message name (%d bytes): %.*s\n", reg->mlen, reg->mlen, reg->mname);
    printf("address: %.*s\n", reg->alen, reg->additional);
    printf("version: %s\n", reg->version);
    printf("is_401_unauthorized_message? %d\n", is_401_unauthorized_message(reg));
    printf("is_200_ok_message? %d\n", is_200_ok_message(reg));
    
    key = "Security-Client";
    attr = "alg";
    val = "aes256-gcm";
    kvp = get_kvp_from_sip_message(reg, key, strlen(key), 0);
    del_value_from_kvp_by_name(kvp, attr, strlen(attr));

    serialized = serialize_sip_message(reg, &slen);
    printf("serialized:\n%.*s\n", slen, serialized);
  }
  printf("\n");

  /*
  memset(buf, 0, sizeof(buf));
  fp = fopen("401-2.txt", "r");
  len = fread(buf, 1, sizeof(buf), fp);
  printf("401 unauthorized (%d bytes): %.*s\n", len, len, buf);
  fclose(fp);
  unauth = init_sip_message(buf, len);
  if (unauth->mtype == SC_SIP_RESPONSE)
  {
    printf("message name (%d bytes): %.*s\n", unauth->mlen, unauth->mlen, unauth->mname);
    printf("code: %.*s\n", unauth->alen, unauth->additional);
    printf("version: %s\n", unauth->version);
    printf("is_401_unauthorized_message? %d\n", is_401_unauthorized_message(unauth));
    printf("is_200_ok_message? %d\n", is_200_ok_message(unauth));

    key = "Security-Server";
    attr = "algorithm";
    val = "AKAv1-SHA-256";
    kvp = get_kvp_from_sip_message(unauth, key, strlen(key), 0);
    change_value_from_kvp_by_name(kvp, attr, strlen(attr), val, strlen(val));

    attr = "ealg";
    val = "aes256-cbc";
    change_value_from_kvp_by_name(kvp, attr, strlen(attr), val, strlen(val));

    key = "Content-Length";
    del_kvp_from_sip_message(unauth, key, strlen(key), 0);

    serialized = serialize_sip_message(unauth, &slen);
    printf("serialized:\n%.*s\n", slen, serialized);
  }
  */

  return 0;
}
