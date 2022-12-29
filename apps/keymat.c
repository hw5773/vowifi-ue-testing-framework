#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <stdint.h>

#include "debug.h"

#define BUF_SIZE 16384

int dtype;

int usage(const char *pname)
{
  emsg(">> Usage: %s [options]", pname);
  emsg("Options");
  emsg("  -a, --ispi        Initiator's SPI");
  emsg("  -b, --rspi        Responder's SPI");
  emsg("  -c, --inonce      Initiator's Nonce");
  emsg("  -d, --rnonce      Responder's Nonce");
  emsg("  -e, --idh         Initiator's DH Value");
  emsg("  -f, --rdh         Responder's DH Value");
  emsg("  -g, --group       EC Group");
  exit(1);
}

int main(int argc, char *argv[])
{
  uint8_t buf[BUF_SIZE] = {0, };
  uint8_t *ispi, *rspi, *inonce, *rnonce, *idh, *rdh, *rdhp, *group;
  size_t ispi_len, rspi_len, inonce_len, rnonce_len, idh_len, rdh_len, rdhp_len, group_len;
  uint8_t eflag;
  const char *pname, *gname;
  int c;

  ispi = NULL;
  rspi = NULL;
  inonce = NULL;
  rnonce = NULL;
  idh = NULL;
  rdh = NULL;
  rdhp = NULL;
  group = NULL;
  eflag = 0;
  pname = argv[0];

  while (1)
  {
    int option_index = 0;
    static struct option long_options[] = {
      {"ispi", required_argument, 0, 'a'},
      {"rspi", required_argument, 0, 'b'},
      {"inonce", required_argument, 0, 'c'},
      {"rnonce", required_argument, 0, 'd'},
      {"idh", required_argument, 0, 'e'},
      {"rdh", required_argument, 0, 'x'},
      {"rdhp", required_argument, 0, 'y'},
      {"group", required_argument, 0, 'g'},
      {0, 0, 0, 0}
    };

    const char *opt = "a:b:c:d:e:x:y:g:0";

    c = getopt_long(argc, argv, opt, long_options, &option_index);

    if (c == -1)
      break;
    
    switch (c)
    {
      case 'a':
        ispi_len = strlen(optarg);
        ispi = (uint8_t *)malloc(ispi_len);
        memcpy(ispi, optarg, ispi_len);
        break;

      case 'b':
        rspi_len = strlen(optarg);
        rspi = (uint8_t *)malloc(rspi_len);
        memcpy(rspi, optarg, rspi_len);
        break;

      case 'c':
        inonce_len = strlen(optarg);
        inonce = (uint8_t *)malloc(inonce_len);
        memcpy(inonce, optarg, inonce_len);
        break;

      case 'd':
        rnonce_len = strlen(optarg);
        rnonce = (uint8_t *)malloc(rnonce_len);
        memcpy(rnonce, optarg, rnonce_len);
        break;

      case 'e':
        idh_len = strlen(optarg);
        idh = (uint8_t *)malloc(idh_len);
        memcpy(idh, optarg, idh_len);
        break;

      case 'x':
        rdh_len = strlen(optarg);
        rdh = (uint8_t *)malloc(rdh_len);
        memcpy(rdh, optarg, rdh_len);
        break;

      case 'y':
        rdhp_len = strlen(optarg);
        rdhp = (uint8_t *)malloc(rdhp_len);
        memcpy(rdhp, optarg, rdhp_len);
        break;

      case 'g':
        group_len = strlen(optarg);
        group = (uint8_t *)malloc(group_len);
        memcpy(group, optarg, group_len);
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

  if (!idh)
  {
    emsg("Please specify the initiator's DH value (idh)");
    eflag = 1;
  }

  if (!rdh)
  {
    emsg("Please specify the responder's DH value (rdh)");
    eflag = 1;
  }

  if (!rdhp)
  {
    emsg("Please specify the responder's DH value (rdhp)");
    eflag = 1;
  }

  if (!group)
  {
    emsg("Please specify the EC group name");
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
  printf("Initiator's DH Value: %.*s\n", (int)idh_len, idh);
  printf("Responder's DH Value (public): %.*s\n", (int)rdh_len, rdh);
  printf("Responder's DH Value (private): %.*s\n", (int)rdhp_len, rdhp);
  printf("DH Group Filename: %s\n", gname);

  return 0;
}
