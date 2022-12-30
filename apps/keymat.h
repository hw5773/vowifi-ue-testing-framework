#ifndef __KEYMAT_H__
#define __KEYMAT_H__

#define BUF_SIZE 16384
#define MAX_HMAC_LENGTH 128

uint8_t *make_array(uint8_t *str, size_t *len);
uint8_t *generate_dh_shared_secret(uint8_t *idh, size_t idh_len, uint8_t *rdh, size_t rdh_len, 
    uint8_t *rdhp, size_t rdhp_len, const uint8_t *gname, size_t *dh_len);
void generate_ike_shared_secrets(uint8_t *dh, size_t dh_len, uint8_t *ispi, size_t ispi_len, uint8_t *rspi, size_t rspi_len,
    uint8_t *inonce, size_t inonce_len, uint8_t *rnonce, size_t rnonce_len, int ak_len, int ek_len, int ph_len);

#endif /* __KEYMAT_H__ */
