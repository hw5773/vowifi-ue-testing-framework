//
// Created by rafiul on 9/20/19.
//

#ifndef SRSLTE_FUZZING_H
#define SRSLTE_FUZZING_H

// Message types for Fuzzing
#define FUZZING_MSG_TYPE_ATTACH_REQUEST 0x01
#define FUZZING_MSG_TYPE_IDENTITY_REQUEST 0x02
#define FUZZING_MSG_TYPE_AUTH_REQUEST 0x03
#define FUZZING_MSG_TYPE_SEC_MOD_CMD 0x04
#define FUZZING_MSG_TYPE_ATTACH_ACCEPT 0x05
#define FUZZING_MSG_TYPE_ATTACH_REJECT 0x06
#define FUZZING_MSG_TYPE_AUTH_REJECT 0x07
#define FUZZING_MSG_TYPE_DETACH_REQUEST 0x08
#define FUZZING_MSG_TYPE_ESM_INFO_REQUEST 0x09
#define FUZZING_MSG_TYPE_EMM_INFO 0xA0
#define FUZZING_MSG_TYPE_PAGING_TMSI 0xA1
#define FUZZING_MSG_TYPE_PAGING_IMSI 0xA2

#define FUZZING_MSG_TYPE_RRC_RELEASE 0xD0

#define FUZZING_MSG_TYPE_IDENTITY_REQUEST_MAC 0xA2
#define FUZZING_MSG_TYPE_IDENTITY_REQUEST_WMAC 0xA3
#define FUZZING_MSG_TYPE_IDENTITY_REQUEST_ENCRYPT 0xA4
#define FUZZING_MSG_TYPE_IDENTITY_REQUEST_REPLAY 0xA5
#define FUZZING_MSG_TYPE_IDENTITY_REQUEST_ENCRYPT_MAC 0xA6
#define FUZZING_MSG_TYPE_AUTH_REQUEST_MAC 0xA7
#define FUZZING_MSG_TYPE_AUTH_REQUEST_ENCRYPT_MAC 0xA8
#define FUZZING_MSG_TYPE_AUTH_REQUEST_WMAC 0xA9
#define FUZZING_MSG_TYPE_AUTH_REQUEST_ENCRYPT 0xB0
#define FUZZING_MSG_TYPE_AUTH_REQUEST_REPLAY 0xB1
#define FUZZING_MSG_TYPE_ATTACH_ACCEPT_MAC 0xB2
#define FUZZING_MSG_TYPE_ATTACH_ACCEPT_WMAC 0xB3
#define FUZZING_MSG_TYPE_ATTACH_ACCEPT_REPLAY 0xB4
#define FUZZING_MSG_TYPE_TAU_ACCEPT 0xB5
#define FUZZING_MSG_TYPE_GUTI_REALLOCATION 0xB6
#define FUZZING_MSG_TYPE_DL_NAS_TRANSPORT 0xB7
#define FUZZING_MSG_TYPE_RESET_UL_NAS_COUNT 0xB8
#define FUZZING_MSG_TYPE_SECURITY_MODE_COMMAND_REPLAY 0xB9
#define FUZZING_MSG_TYPE_GUTI_REALLOCATION_PLAIN 0xC0
#define FUZZING_MSG_TYPE_DL_NAS_TRANSPORT_PLAIN 0xC1
#define FUZZING_MSG_TYPE_TAU_ACCEPT_PLAIN 0xC2
#define FUZZING_MSG_TYPE_GUTI_REALLOCATION_REPLAY 0xC3
#define FUZZING_MSG_TYPE_DL_NAS_TRANSPORT_REPLAY 0xC4
#define FUZZING_MSG_TYPE_EOL 0xFF
#endif //SRSLTE_FUZZING_H
