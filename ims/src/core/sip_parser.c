#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

void parse_sip_message(uint8_t *buf, size_t len);

void usage(const char *pname)
{
  printf(">> Usage: %s [options]\n", pname);
  printf("Options\n");
  printf("  -i, --input     input file name\n");
  exit(1);
}

int main(int argc, char *argv[])
{
  const char *fname, *pname;
  int c;
  uint8_t buf[4096] = {0, };
  FILE *fp;
  size_t len;

  pname = argv[0];
  fname = NULL;
  fp = NULL;

  while (1)
  {
    int option_index = 0;
    static struct option long_options[] = {
      {"input", required_argument, 0, 'i'},
      {0, 0, 0, 0}
    };

    const char *opt = "i:0";

    c = getopt_long(argc, argv, opt, long_options, &option_index);

    if (c == -1)
      break;

    switch (c)
    {
      case 'i':
        fname = optarg;
        if (access(fname, F_OK) == -1)
        {
          printf("error: file not exist: %s\n", fname);
          fname = NULL;
        }
        break;

      default:
        usage(pname);
    }
  }

  if (!fname)
  {
    printf("You should specify the input file name\n");
    usage(pname);
  }

  printf("input file name: %s\n", fname);

  fp = fopen(fname, "r");
  len = fread(buf, 1, 4096, fp);

  //printf("buf (%ld bytes): %s\n", len, buf);
  parse_sip_message(buf, len);

  return 0;
}

void parse_sip_message(uint8_t *buf, size_t len)
{
  int first_line;
  uint8_t *start, *p;
  const uint8_t *msg;
  const uint8_t *version;
  uint8_t receiver[1024] = {0, };
  uint8_t attr[1024] = {0, };
  uint8_t key[1024] = {0, };
  uint8_t mem[1024] = {0, };

  int receiver_set;
  int attr_set;
  int key_set;
  int within;

  first_line = 1;
  start = buf;
  p = buf;
  msg = version = NULL;
  receiver_set = 0;
  attr_set = 0;
  key_set = 0;
  within = 0;
  
  while (p - buf < len)
  {
    if (*p == ' ')
    {
      if (first_line)
      {
        if (!msg)
        {
          memcpy(mem, start, p - start);
          start = p + 1;

          if (!strncmp(mem, "REGISTER", strlen("REGISTER")))
          {
            msg = "register";
          }
          else
          {
            msg = "unknown";
          }
          memset(mem, 0, 1024);
        }
        else if (!receiver_set)
        {
          memcpy(receiver, start, p - start);
          start = p + 1;
          receiver_set = 1;
        }
        else if (!version)
        {
          memcpy(mem, start, p - start);
          start = p + 1;

          if (!strncmp(mem, "SIP/2.0", strlen("SIP/2.0")))
          {
            version = "sip/2.0";
          }
          memset(mem, 0, 1024);
        }
      }
    }
    else if (*p == '\n')
    {
      if (first_line)
      {
        if (!version)
        {
          memcpy(mem, start, p - start);
          start = p + 1;

          if (!strncmp(mem, "SIP/2.0", strlen("SIP/2.0")))
          {
            version = "sip/2.0";
          }
          memset(mem, 0, 1024);
        }

        first_line = 0;
        printf("%s (%s)\n", msg, version);
      }
      else
      {
        if (!key_set)
        {
          memcpy(key, "value", strlen("value"));
        }
        memcpy(mem, start, p - start);
        start = p + 1;
        printf("    %s: %s\n", key, mem);
        memset(attr, 0, sizeof(attr));
        memset(key, 0, sizeof(key));
        memset(mem, 0, sizeof(mem));
        attr_set = 0;
        key_set = 0;
      }
    }
    else if (!within && *p == ':')
    {
      if (!first_line)
      {
        if (!attr_set)
        {
          memcpy(attr, start, p - start);
          start = p + 1;
          attr_set = 1;
          printf("  %s\n", attr);
        }
      }
    }
    else if (!within && *p == '=')
    {
      if (!key_set)
      {
        memcpy(key, start, p - start);
        start = p + 1;
        key_set = 1;
      }
      else
      {
        perror("error happened in '='");
        exit(1);
      }
    }
    else if (!within && *p == ';')
    {
      if (!key_set)
      {
        memcpy(key, "value", strlen("value"));
      }
      memcpy(mem, start, p - start);
      start = p + 1;
      printf("    %s: %s\n", key, mem);
      memset(key, 0, sizeof(key));
      memset(mem, 0, sizeof(mem));
      key_set = 0;
    }
    else if (!within && *p == ',')
    {
      if (!key_set)
      {
        memcpy(key, "value", strlen("value"));
      }
      memcpy(mem, start, p - start);
      start = p + 1;
      printf("    %s: %s\n", key, mem);
      memset(key, 0, sizeof(key));
      memset(mem, 0, sizeof(mem));
      key_set = 0;
    }
    else if (*p == '<')
    {
      within = 1;
    }
    else if (*p == '>')
    {
      within = 0;
    }
    else if (*p == '[')
    {
      within = 1;
    }
    else if (*p == ']')
    {
      within = 0;
    }
    p++;
  }
}
