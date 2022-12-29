#include <string.h>
#include <openssl/evp.h>
#include <openssl/dh.h>
#include <openssl/bn.h>
#include <openssl/hmac.h>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <openssl/pem.h>

#include "debug.h"

void generate_dh_shared_secret(uint8_t *ispi, size_t ispi_len, uint8_t *rspi, size_t rspi_len, 
    uint8_t *inonce, size_t inonce_len, uint8_t *rnonce, size_t rnonce_len,
    uint8_t *idh, size_t idh_len, uint8_t *rdh, size_t rdh_len, const uint8_t *gname)
{
  fstart("ispi: %p, ispi_len: %lu, rspi: %p, rspi_len: %lu, inonce: %p, inonce_len: %lu, rnonce: %p, rnonce_len: %lu, idh: %p, idh_len: %lu, rdh: %p, rdh_len: %lu, gname: %s", ispi, ispi_len, rspi, rspi_len, inonce, inonce_len, rnonce, rnonce_len, idh, idh_len, rdh, rdh_len, gname);

  DH *my_dh, *peer_dh;
  FILE *fp;
  EVP_PKEY *my_dhkey, *peer_dhkey;
  EVP_PKEY_CTX *ctx;
  BIGNUM *my_pubkey, *peer_pubkey;
  uint8_t *key, *fname;
  int ret;
  size_t klen;

  my_dhkey = EVP_PKEY_new();
  fp = fopen(gname, "r");
  my_dh = PEM_read_DHparams(fp, NULL, NULL, NULL);
  fclose(fp);
  my_pubkey = BN_bin2bn(rdh, rdh_len, NULL);
  ret = DH_set0_key(my_dh, my_pubkey, NULL);
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

  dprint(VOWIFI_DEBUG_INITIATOR, "DH Shared Secret", key, 0, (int)klen, 16);

  EVP_PKEY_CTX_free(ctx);
  EVP_PKEY_free(my_dhkey);
  EVP_PKEY_free(peer_dhkey);

  ffinish();
}
