#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <assert.h>
#include <stdio.h>

#define LFINFO 0
#define LDEBUG 1
#define LINFO 2
#define LERROR 3

extern int dtype;

#define VOWIFI_DEBUG_NONE             0x0
#define VOWIFI_DEBUG_MAIN             0x1
#define VOWIFI_DEBUG_INFO_EXTRACTOR   0x2
#define VOWIFI_DEBUG_INSPECTOR        0x4
#define VOWIFI_DEBUG_KEY_RECEIVER     0x8
#define VOWIFI_DEBUG_MESSAGE_LIST     0x10
#define VOWIFI_DEBUG_DISSECTOR        0x20
#define VOWIFI_DEBUG_PROTOCOL         0x40
#define VOWIFI_DEBUG_SADB             0x80
#define VOWIFI_DEBUG_ATTACKER         0x100
#define VOWIFI_DEBUG_GENERATOR        0x200
#define VOWIFI_DEBUG_KEYMAT           0x400
#define VOWIFI_DEBUG_PROXY            0x800
#define VOWIFI_DEBUG_MITM             0x1000
#define VOWIFI_DEBUG_KEYGEN           0x2000
#define VOWIFI_DEBUG_ALL            VOWIFI_DEBUG_MAIN | VOWIFI_DEBUG_INFO_EXTRACTOR | VOWIFI_DEBUG_INSPECTOR | VOWIFI_DEBUG_KEY_RECEIVER | VOWIFI_DEBUG_MESSAGE_LIST | VOWIFI_DEBUG_DISSECTOR | VOWIFI_DEBUG_PROTOCOL | VOWIFI_DEBUG_SADB | VOWIFI_DEBUG_ATTACKER

#if DEBUG_LEVEL <= LFINFO
  #define fstart(format, ...) printf("[VOWIFI/FINFO] Start: %s:%s: " format "\n", __FILE__, __func__, ## __VA_ARGS__)
  #define ffinish(format, ...) printf("[VOWIFI/FINFO] Finish: %s:%s: " format "\n", __FILE__, __func__, ## __VA_ARGS__)
  #define ferr(format, ...) printf("[VOWIFI/FINFO] Error: %s:%s: " format "\n", __FILE__, __func__, ## __VA_ARGS__)
#else
  #define fstart(format, ...)
  #define ffinish(format, ...)
  #define ferr(format, ...)
#endif /* LFINFO */

#if DEBUG_LEVEL <= LDEBUG
  #define dmsg(type, format, ...) \
    if (dtype & type) printf("[VOWIFI/DEBUG] %s:%s:%d: " format "\n", __FILE__, __func__, __LINE__, ## __VA_ARGS__)
  #define dprint(type, msg, buf, start, end, interval) \
    if (dtype & type) { \
      do { \
        int i; \
          printf("[VOWIFI/DEBUG] %s:%s: %s (%d bytes)\n", __FILE__, __func__, msg, end - start); \
          for (i = start; i < end; i++) \
          { \
            printf("%02X ", buf[i]); \
            if (i % interval == (interval - 1)) \
            { \
              printf("\n"); \
            } \
          } \
          printf("\n"); \
      } while (0); \
    }
#else
  #define dmsg(type, format, ...)
  #define dprint(type, msg, buf, start, end, interval)
#endif /* DEBUG */

#if DEBUG_LEVEL <= LINFO
  #define imsg(type, format, ...) if (dtype & type) printf("[VOWIFI/INFO] %s:%s: " format "\n", __FILE__, __func__, ## __VA_ARGS__)
  #define iprint(type, msg, buf, start, end, interval) \
    if (dtype & type) { \
      do { \
        int i; \
        printf("[VOWIFI/INFO] %s:%s: %s (%d bytes)\n", __FILE__, __func__, msg, end - start); \
        for (i = start; i < end; i++) \
        { \
          printf("%02X ", buf[i]); \
          if (i % interval == (interval - 1)) \
          { \
            printf("\n"); \
          } \
        } \
        printf("\n"); \
      } while (0); \
    }
  #define ikprint(type, msg, buf, start, end, interval) \
    if (dtype & type) { \
      do { \
        int i; \
        printf("[VOWIFI/INFO] %s:%s: %s (%d bytes)\n", __FILE__, __func__, msg, end - start); \
        for (i = start; i < end; i++) \
        { \
          printf("%02x", buf[i]); \
          if (i % interval == (interval - 1)) \
          { \
            printf("\n"); \
          } \
        } \
        printf("\n"); \
      } while (0); \
    }
#else
  #define imsg(type, format, ...)
  #define iprint(type, msg, buf, start, end, interval)
  #define ikprint(type, msg, buf, start, end, interval)
#endif /* INFO */

#if DEBUG_LEVEL <= LERROR
  #define emsg(format, ...) printf("[VOWIFI/ERROR] " format "\n", ## __VA_ARGS__)
#else
  #define emsg(format, ...)
#endif /* ERROR */

#endif /* __DEBUG_H__ */
