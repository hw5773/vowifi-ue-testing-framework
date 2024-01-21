#include <string.h>
#include <openssl/evp.h>
#include <openssl/dh.h>
#include <openssl/bn.h>
#include <openssl/hmac.h>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <openssl/pem.h>

#include "debug.h"
#include "keymat.h"

int char_to_hex(uint8_t c);
int str_to_hex(uint8_t *str);

uint8_t *generate_dh_shared_secret(uint8_t *idh, size_t idh_len, uint8_t *rdh, size_t rdh_len, 
    uint8_t *rdhp, size_t rdhp_len, const uint8_t *gname, size_t *dh_len)
{
  fstart("idh: %p, idh_len: %lu, rdh: %p, rdh_len: %lu, rdhp: %p, rdhp_len: %lu, gname: %s, dh_len: %p", idh, idh_len, rdh, rdh_len, rdhp, rdhp_len, gname, dh_len);

  DH *my_dh, *peer_dh;
  FILE *fp;
  EVP_PKEY *my_dhkey, *peer_dhkey;
  EVP_PKEY_CTX *ctx;
  BIGNUM *my_pubkey, *my_privkey, *peer_pubkey;
  uint8_t *key, *fname;
  int ret;
  size_t klen;

  my_dhkey = EVP_PKEY_new();
  fp = fopen(gname, "r");
  my_dh = PEM_read_DHparams(fp, NULL, NULL, NULL);
  fclose(fp);

  my_pubkey = BN_bin2bn(rdh, rdh_len, NULL);
  my_privkey = BN_bin2bn(rdhp, rdhp_len, NULL);
  ret = DH_set0_key(my_dh, my_pubkey, my_privkey);
  ret = EVP_PKEY_assign(my_dhkey, EVP_PKEY_DHX, my_dh);

  peer_dhkey = EVP_PKEY_new();
  fp = fopen(gname, "r");
  peer_dh = PEM_read_DHparams(fp, NULL, NULL, NULL);
  fclose(fp);
  peer_pubkey = BN_bin2bn(idh, idh_len, NULL);
  ret = DH_set0_key(peer_dh, peer_pubkey, NULL);
  ret = EVP_PKEY_assign(peer_dhkey, EVP_PKEY_DHX, peer_dh);

  ctx = EVP_PKEY_CTX_new(my_dhkey, NULL);
  if (!ctx)
  {
    emsg("Out of memory: EVP_PKEY_CTX_new()");
    abort();
  }

  if (EVP_PKEY_derive_init(ctx) <= 0)
  {
    emsg("EVP_PKEY_derive_init()");
    abort();
  }

  if (EVP_PKEY_derive_set_peer(ctx, peer_dhkey) <= 0)
  {
    emsg("EVP_PKEY_derive_set_peer(): %s", ERR_error_string(ERR_get_error(), NULL));
    abort();
  }

  if (EVP_PKEY_derive(ctx, NULL, &klen) <= 0)
  {
    emsg("EVP_PKEY_derive(): %s", ERR_error_string(ERR_get_error(), NULL));
    abort();
  }

  key = (uint8_t *)malloc(klen);

  if (EVP_PKEY_derive(ctx, key, &klen) <= 0)
  {
    emsg("EVP_PKEY_derive(): %s", ERR_error_string(ERR_get_error(), NULL));
    abort();
  }

  ikprint(VOWIFI_DEBUG_KEYMAT, "DH Shared Secret", key, 0, (int)klen, 16);

  EVP_PKEY_CTX_free(ctx);
  EVP_PKEY_free(my_dhkey);
  EVP_PKEY_free(peer_dhkey);

  *dh_len = klen;

  ffinish();
  return key;
}

void generate_ike_shared_secrets(uint8_t *dh, size_t dh_len, uint8_t *ispi, size_t ispi_len, uint8_t *rspi, size_t rspi_len,
    uint8_t *inonce, size_t inonce_len, uint8_t *rnonce, size_t rnonce_len, int ak_len, int ek_len, int ph_len)
{
  fstart("dh: %p, dh_len: %lu, ispi: %p, ispi_len: %lu, rspi: %p, rspi_len: %lu, inonce: %p, inonce_len: %lu, rnonce: %p, rnonce_len: %lu, ak_len: %d, ek_len: %d, ph_len: %d", dh, dh_len, ispi, ispi_len, rspi, rspi_len, inonce, inonce_len, rnonce, rnonce_len, ak_len, ek_len, ph_len);

  uint8_t skeyseed[MAX_HMAC_LENGTH] = {0, };
  uint8_t keymat[BUF_SIZE] = {0, };
  uint8_t *key, *ts, *te, *p, *s;
  size_t key_len;
  uint8_t cnt;
  int i, tmp, offset, seed_len;

  HMAC_CTX *sctx, *pctx;
  const EVP_MD *evp;

  sctx = HMAC_CTX_new();
  if (!sctx)
  {
    emsg("Out of memory: HMAC_CTX_new()");
    abort();
  }

  switch (ph_len)
  {
    case 20:
      evp = EVP_sha1();
      break;

    case 32:
      evp = EVP_sha256();
      break;

    default:
      evp = NULL;
  }

  key_len = inonce_len + rnonce_len;
  key = (uint8_t *)malloc(key_len);
  memcpy(key, inonce, inonce_len);
  memcpy(key + inonce_len, rnonce, rnonce_len);

  HMAC_Init_ex(sctx, key, key_len, evp, NULL);
  HMAC_Update(sctx, dh, dh_len);
  HMAC_Final(sctx, skeyseed, &seed_len);
  HMAC_CTX_free(sctx);

  ikprint(VOWIFI_DEBUG_KEYMAT, "SKEYSEED", skeyseed, 0, (int)seed_len, (int)seed_len);
  ikprint(VOWIFI_DEBUG_KEYMAT, "inonce", inonce, 0, (int)inonce_len, (int)inonce_len);
  ikprint(VOWIFI_DEBUG_KEYMAT, "rnonce", rnonce, 0, (int)rnonce_len, (int)rnonce_len);
  ikprint(VOWIFI_DEBUG_KEYMAT, "ispi", ispi, 0, (int)ispi_len, (int)ispi_len);
  ikprint(VOWIFI_DEBUG_KEYMAT, "rspi", rspi, 0, (int)rspi_len, (int)rspi_len);

  tmp = inonce_len + rnonce_len + ispi_len + rspi_len;
  s = (uint8_t *)malloc(tmp);
  p = s;
  memcpy(p, inonce, inonce_len);
  p += inonce_len;
  memcpy(p, rnonce, rnonce_len);
  p += rnonce_len;
  memcpy(p, ispi, ispi_len);
  p += ispi_len;
  memcpy(p, rspi, rspi_len);
  p += rspi_len;

  ts = te = keymat;
  offset = 0;
  cnt = 0;

  for (i=0; i<10; i++)
  {
    pctx = HMAC_CTX_new();
    if (!pctx)
    {
      emsg("Out of memory: HMAC_CTX_new()");
      abort();
    }

    cnt++;
    HMAC_Init_ex(pctx, skeyseed, seed_len, evp, NULL);
    if (ts != te)
    {
      HMAC_Update(pctx, ts, te - ts);
    }
    HMAC_Update(pctx, s, tmp);
    HMAC_Update(pctx, &cnt, 1);
    ts = te;
    HMAC_Final(pctx, ts, &offset);
    te += offset;
    ikprint(VOWIFI_DEBUG_KEYMAT, "T", ts, 0, offset, offset);
    HMAC_CTX_free(pctx);
  }

  p = keymat;
  ikprint(VOWIFI_DEBUG_KEYMAT, "SK_d", p, 0, ak_len, ak_len);
  p += ak_len;

  ikprint(VOWIFI_DEBUG_KEYMAT, "SK_ai", p, 0, ak_len, ak_len);
  p += ak_len;

  ikprint(VOWIFI_DEBUG_KEYMAT, "SK_ar", p, 0, ak_len, ak_len);
  p += ak_len;

  ikprint(VOWIFI_DEBUG_KEYMAT, "SK_ei", p, 0, ek_len, ek_len);
  p += ek_len;

  ikprint(VOWIFI_DEBUG_KEYMAT, "SK_er", p, 0, ek_len, ek_len);
  p += ek_len;

  ikprint(VOWIFI_DEBUG_KEYMAT, "SK_pi", p, 0, ak_len, ak_len);
  p += ak_len;

  ikprint(VOWIFI_DEBUG_KEYMAT, "SK_pr", p, 0, ak_len, ak_len);
  p += ak_len;

  ffinish();
}

int char_to_hex(uint8_t c)
{
  int ret;

  switch (c)
  {
    case '0':
      ret = 0x0;
      break;

    case '1':
      ret = 0x1;
      break;

    case '2':
      ret = 0x2;
      break;

    case '3':
      ret = 0x3;
      break;

    case '4':
      ret = 0x4;
      break;

    case '5':
      ret = 0x5;
      break;

    case '6':
      ret = 0x6;
      break;

    case '7':
      ret = 0x7;
      break;

    case '8':
      ret = 0x8;
      break;

    case '9':
      ret = 0x9;
      break;

    case 'a':
      ret = 0xa;
      break;

    case 'b':
      ret = 0xb;
      break;

    case 'c':
      ret = 0xc;
      break;

    case 'd':
      ret = 0xd;
      break;

    case 'e':
      ret = 0xe;
      break;

    case 'f':
      ret = 0xf;
      break;

    case 'A':
      ret = 0xa;
      break;

    case 'B':
      ret = 0xb;
      break;

    case 'C':
      ret = 0xc;
      break;

    case 'D':
      ret = 0xd;
      break;

    case 'E':
      ret = 0xe;
      break;

    case 'F':
      ret = 0xf;
      break;

    default:
      ret = 0x0;
      break;
  }

  return ret;
}

int str_to_hex(uint8_t *str)
{
  int ret;
  int i, len;
  len = strlen(str);
  assert(len >= 2);

  ret = 0;
  for (i=0; i<2; i++)
  {
    ret *= 16;
    ret += char_to_hex(str[i]);
  }

  return ret;
}

uint8_t *make_array(uint8_t *str, size_t *len)
{
  int i, slen;
  uint8_t *ret, *p;
  *len = strlen(str)/2;
  ret = (uint8_t *)malloc(*len);

  p = str;
  for (i=0; i<*len; i++)
  {
    ret[i] = str_to_hex(p);
    p += 2;
  }

  return ret;
}

