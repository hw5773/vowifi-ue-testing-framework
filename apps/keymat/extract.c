#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <stdint.h>

#include "debug.h"
#include "keymat.h"

int dtype;

int usage(const char *pname)
{
  emsg(">> Usage: %s [options]", pname);
  emsg("Options");
  emsg("  -a, --ispi        Initiator's SPI");
  emsg("  -b, --rspi        Responder's SPI");
  emsg("  -c, --inonce      Initiator's Nonce");
  emsg("  -d, --rnonce      Responder's Nonce");
  emsg("  -e, --share       DH Shared Secret");
  emsg("  -i, --aklen       Authentication Key Length");
  emsg("  -j, --eklen       Encryption Key Length");
  emsg("  -k, --phlen       Hash Length");
  emsg("  -z, --zero        Set DH Share to Zero");
  exit(1);
}

int main(int argc, char *argv[])
{
  uint8_t buf[BUF_SIZE] = {0, };
  uint8_t *ispi, *rspi, *inonce, *rnonce, *dh;
  size_t ispi_len, rspi_len, inonce_len, rnonce_len, dh_len;
  int ak_len, ek_len, ph_len;
  uint8_t eflag;
  const char *pname;
  int c, i, zero;

  dtype = VOWIFI_DEBUG_KEYMAT;
  ispi = NULL;
  rspi = NULL;
  inonce = NULL;
  rnonce = NULL;
  dh = NULL;
  eflag = 0;
  pname = argv[0];
  ak_len = ek_len = ph_len = 0;
  zero = 0;

  while (1)
  {
    int option_index = 0;
    static struct option long_options[] = {
      {"ispi", required_argument, 0, 'a'},
      {"rspi", required_argument, 0, 'b'},
      {"inonce", required_argument, 0, 'c'},
      {"rnonce", required_argument, 0, 'd'},
      {"share", required_argument, 0, 'e'},
      {"aklen", required_argument, 0, 'i'},
      {"eklen", required_argument, 0, 'j'},
      {"phlen", required_argument, 0, 'k'},
      {"zero", required_argument, 0, 'z'},
      {0, 0, 0, 0}
    };

    const char *opt = "a:b:c:d:e:i:j:k:z0";

    c = getopt_long(argc, argv, opt, long_options, &option_index);

    if (c == -1)
      break;
    
    switch (c)
    {
      case 'a':
        ispi = make_array(optarg, &ispi_len);
        break;

      case 'b':
        rspi = make_array(optarg, &rspi_len);
        break;

      case 'c':
        inonce = make_array(optarg, &inonce_len);
        break;

      case 'd':
        rnonce = make_array(optarg, &rnonce_len);
        break;

      case 'e':
        dh = make_array(optarg, &dh_len);
        break;

      case 'i':
        ak_len = atoi(optarg);
        break;

      case 'j':
        ek_len = atoi(optarg);
        break;

      case 'k':
        ph_len = atoi(optarg);
        break;

      case 'z':
        zero = 1;
        break;

      default:
        usage(pname);
    }
  }

  if (!ispi)
  {
    emsg("Please specify the initiator's SPI (ispi)");
    eflag = 1;
  }

  if (!rspi)
  {
    emsg("Please specify the responder's SPI (rspi)");
    eflag = 1;
  }

  if (!inonce)
  {
    emsg("Please specify the initiator's nonce (inonce)");
    eflag = 1;
  }

  if (!rnonce)
  {
    emsg("Please specify the responder's nonce (rnonce)");
    eflag = 1;
  }

  if (!dh)
  {
    emsg("Please specify the initiator's DH value (idh)");
    eflag = 1;
  }

  if (!ak_len)
  {
    emsg("Please specify the authentication key length");
    eflag = 1;
  }

  if (!ek_len)
  {
    emsg("Please specify the encryption key length");
    eflag = 1;
  }

  if (!ph_len)
  {
    emsg("Please specify the hash length");
    eflag = 1;
  }

  if (eflag)
  {
    exit(1);
  }

  printf("Initiator's SPI: %.*s\n", (int)ispi_len, ispi);
  printf("Responder's SPI: %.*s\n", (int)rspi_len, rspi);
  printf("Initiator's Nonce: %.*s\n", (int)inonce_len, inonce);
  printf("Responder's Nonce: %.*s\n", (int)rnonce_len, rnonce);
  printf("DH Shared Secret: %.*s\n", (int)dh_len, dh);
  printf("Authentication Key Length: %d\n", ak_len);
  printf("Encryption Key Length: %d\n", ek_len);
  printf("Hash Length: %d\n", ph_len);

  if (zero)
  {
    printf("The zero flag is enabled\n");
    for (i=0; i<dh_len; i++)
      dh[i] = 0;
    printf("DH Shared Secret is set to 0\n");
    printf("Revised DH Shared Secret: %.*s\n", (int)dh_len, dh);
  }

  generate_ike_shared_secrets(dh, dh_len, ispi, ispi_len, rspi, rspi_len, inonce, inonce_len, rnonce, rnonce_len, ak_len, ek_len, ph_len);

  return 0;
}
