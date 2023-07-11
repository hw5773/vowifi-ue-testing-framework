#include <openssl/evp.h>
#include <stdint.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include "debug.h"

#define KEYLEN 16

int dtype;
void assign_key(uint8_t *key, const char *optarg, int len);
int main(int argc, char *argv[])
{
  int c, i, len;
  char *pname;
  uint8_t *ki;
  uint8_t *op;
  uint8_t opc[KEYLEN];
  uint8_t tmp[KEYLEN];
  EVP_CIPHER_CTX *ctx;

  dtype = VOWIFI_DEBUG_KEYGEN;
  pname = argv[0];
  ki = op = NULL;

  while (1)
  {
    int idx = 0;
    static struct option long_options[] = {
      {"ki", required_argument, 0, 'k'},
      {"op", required_argument, 0, 'o'},
      {0, 0, 0, 0}
    };

    const char *opt = "k:o:0";

    c = getopt_long(argc, argv, opt, long_options, &idx);

    if (c == -1)
      break;

    switch (c)
    {
      case 'k':
        ki = (uint8_t *)malloc(KEYLEN);
        assign_key(ki, optarg, KEYLEN);
        break;

      case 'o':
        op = (uint8_t *)malloc(KEYLEN);
        assign_key(op, optarg, KEYLEN);
        break;

      default:
        break;
    }
  }

  if (!ki)
  {
    emsg("Ki is not set");
    exit(1);
  }

  if (!op)
  {
    emsg("OP is not set");
    exit(1);
  }

  if (!opc)
  {
    emsg("OPc is not set");
    exit(1);
  }

  iprint(VOWIFI_DEBUG_KEYGEN, "Ki", ki, 0, KEYLEN, KEYLEN);
  iprint(VOWIFI_DEBUG_KEYGEN, "OP", op, 0, KEYLEN, KEYLEN);

  ctx = EVP_CIPHER_CTX_new();
  EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, ki, NULL);
  EVP_EncryptUpdate(ctx, tmp, &len, op, KEYLEN);

  iprint(VOWIFI_DEBUG_KEYGEN, "tmp", tmp, 0, KEYLEN, KEYLEN);

  for (i=0; i<KEYLEN; i++)
    opc[i] = tmp[i] ^ op[i];

  iprint(VOWIFI_DEBUG_KEYGEN, "OPc", opc, 0, KEYLEN, KEYLEN);

  printf("OPc:\n");
  for (i=0; i<KEYLEN; i++)
    printf("%02x", opc[i]);
  printf("\n");

  free(ki);
  free(op);
  EVP_CIPHER_CTX_free(ctx);

  return 0;
}

void assign_key(uint8_t *key, const char *optarg, int len)
{
  int i, ilen, idx;
  uint8_t tmp;

  idx = 0;
  ilen = len * 2;

  for (i=0; i<ilen; i+=2)
  {
    tmp = 0;
    if (optarg[i] >= '0' && optarg[i] <= '9')
      tmp |= (optarg[i] - '0');
    else if (optarg[i] >= 'a' && optarg[i] <= 'f')
      tmp |= (optarg[i] - 'a') + 0xa;

    tmp <<= 4;
    if (optarg[i+1] >= '0' && optarg[i+1] <= '9')
      tmp |= (optarg[i+1] - '0');
    else if (optarg[i+1] >= 'a' && optarg[i+1] <= 'f')
      tmp |= (optarg[i+1] - 'a') + 0xa;

    key[idx++] = tmp;
  }
}
