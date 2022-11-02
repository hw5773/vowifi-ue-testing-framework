/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_CORE_CFG_TAB_H_INCLUDED
# define YY_YY_CORE_CFG_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    FORWARD = 258,
    FORWARD_TCP = 259,
    FORWARD_TLS = 260,
    FORWARD_SCTP = 261,
    FORWARD_UDP = 262,
    EXIT = 263,
    DROP = 264,
    RETURN = 265,
    BREAK = 266,
    LOG_TOK = 267,
    ERROR = 268,
    ROUTE = 269,
    ROUTE_REQUEST = 270,
    ROUTE_FAILURE = 271,
    ROUTE_ONREPLY = 272,
    ROUTE_REPLY = 273,
    ROUTE_BRANCH = 274,
    ROUTE_SEND = 275,
    ROUTE_EVENT = 276,
    EXEC = 277,
    SET_HOST = 278,
    SET_HOSTPORT = 279,
    SET_HOSTPORTTRANS = 280,
    PREFIX = 281,
    STRIP = 282,
    STRIP_TAIL = 283,
    SET_USERPHONE = 284,
    APPEND_BRANCH = 285,
    REMOVE_BRANCH = 286,
    CLEAR_BRANCHES = 287,
    SET_USER = 288,
    SET_USERPASS = 289,
    SET_PORT = 290,
    SET_URI = 291,
    REVERT_URI = 292,
    FORCE_RPORT = 293,
    ADD_LOCAL_RPORT = 294,
    FORCE_TCP_ALIAS = 295,
    UDP_MTU = 296,
    UDP_MTU_TRY_PROTO = 297,
    UDP4_RAW = 298,
    UDP4_RAW_MTU = 299,
    UDP4_RAW_TTL = 300,
    IF = 301,
    ELSE = 302,
    SET_ADV_ADDRESS = 303,
    SET_ADV_PORT = 304,
    FORCE_SEND_SOCKET = 305,
    SET_FWD_NO_CONNECT = 306,
    SET_RPL_NO_CONNECT = 307,
    SET_FWD_CLOSE = 308,
    SET_RPL_CLOSE = 309,
    SWITCH = 310,
    CASE = 311,
    DEFAULT = 312,
    WHILE = 313,
    CFG_SELECT = 314,
    CFG_RESET = 315,
    URIHOST = 316,
    URIPORT = 317,
    MAX_LEN = 318,
    SETFLAG = 319,
    RESETFLAG = 320,
    ISFLAGSET = 321,
    SETAVPFLAG = 322,
    RESETAVPFLAG = 323,
    ISAVPFLAGSET = 324,
    METHOD = 325,
    URI = 326,
    FROM_URI = 327,
    TO_URI = 328,
    SRCIP = 329,
    SRCPORT = 330,
    DSTIP = 331,
    DSTPORT = 332,
    TOIP = 333,
    TOPORT = 334,
    SNDIP = 335,
    SNDPORT = 336,
    SNDPROTO = 337,
    SNDAF = 338,
    PROTO = 339,
    AF = 340,
    MYSELF = 341,
    MSGLEN = 342,
    UDP = 343,
    TCP = 344,
    TLS = 345,
    SCTP = 346,
    WS = 347,
    WSS = 348,
    DEBUG_V = 349,
    FORK = 350,
    FORK_DELAY = 351,
    MODINIT_DELAY = 352,
    LOGSTDERROR = 353,
    LOGFACILITY = 354,
    LOGNAME = 355,
    LOGCOLOR = 356,
    LOGPREFIX = 357,
    LOGPREFIXMODE = 358,
    LOGENGINETYPE = 359,
    LOGENGINEDATA = 360,
    XAVPVIAPARAMS = 361,
    XAVPVIAFIELDS = 362,
    LISTEN = 363,
    ADVERTISE = 364,
    ALIAS = 365,
    SR_AUTO_ALIASES = 366,
    DNS = 367,
    REV_DNS = 368,
    DNS_TRY_IPV6 = 369,
    DNS_TRY_NAPTR = 370,
    DNS_SRV_LB = 371,
    DNS_UDP_PREF = 372,
    DNS_TCP_PREF = 373,
    DNS_TLS_PREF = 374,
    DNS_SCTP_PREF = 375,
    DNS_RETR_TIME = 376,
    DNS_SLOW_QUERY_MS = 377,
    DNS_RETR_NO = 378,
    DNS_SERVERS_NO = 379,
    DNS_USE_SEARCH = 380,
    DNS_SEARCH_FMATCH = 381,
    DNS_NAPTR_IGNORE_RFC = 382,
    DNS_CACHE_INIT = 383,
    DNS_USE_CACHE = 384,
    DNS_USE_FAILOVER = 385,
    DNS_CACHE_FLAGS = 386,
    DNS_CACHE_NEG_TTL = 387,
    DNS_CACHE_MIN_TTL = 388,
    DNS_CACHE_MAX_TTL = 389,
    DNS_CACHE_MEM = 390,
    DNS_CACHE_GC_INT = 391,
    DNS_CACHE_DEL_NONEXP = 392,
    DNS_CACHE_REC_PREF = 393,
    AUTO_BIND_IPV6 = 394,
    BIND_IPV6_LINK_LOCAL = 395,
    DST_BLST_INIT = 396,
    USE_DST_BLST = 397,
    DST_BLST_MEM = 398,
    DST_BLST_TTL = 399,
    DST_BLST_GC_INT = 400,
    DST_BLST_UDP_IMASK = 401,
    DST_BLST_TCP_IMASK = 402,
    DST_BLST_TLS_IMASK = 403,
    DST_BLST_SCTP_IMASK = 404,
    IP_FREE_BIND = 405,
    PORT = 406,
    STAT = 407,
    CHILDREN = 408,
    SOCKET_WORKERS = 409,
    ASYNC_WORKERS = 410,
    ASYNC_USLEEP = 411,
    ASYNC_NONBLOCK = 412,
    CHECK_VIA = 413,
    PHONE2TEL = 414,
    MEMLOG = 415,
    MEMDBG = 416,
    MEMSUM = 417,
    MEMSAFETY = 418,
    MEMJOIN = 419,
    MEMSTATUSMODE = 420,
    CORELOG = 421,
    SIP_WARNING = 422,
    SERVER_SIGNATURE = 423,
    SERVER_HEADER = 424,
    USER_AGENT_HEADER = 425,
    REPLY_TO_VIA = 426,
    LOADMODULE = 427,
    LOADPATH = 428,
    MODPARAM = 429,
    CFGENGINE = 430,
    MAXBUFFER = 431,
    SQL_BUFFER_SIZE = 432,
    USER = 433,
    GROUP = 434,
    CHROOT = 435,
    WDIR = 436,
    RUNDIR = 437,
    MHOMED = 438,
    DISABLE_TCP = 439,
    TCP_ACCEPT_ALIASES = 440,
    TCP_ACCEPT_UNIQUE = 441,
    TCP_CONNECTION_MATCH = 442,
    TCP_CHILDREN = 443,
    TCP_CONNECT_TIMEOUT = 444,
    TCP_SEND_TIMEOUT = 445,
    TCP_CON_LIFETIME = 446,
    TCP_POLL_METHOD = 447,
    TCP_MAX_CONNECTIONS = 448,
    TLS_MAX_CONNECTIONS = 449,
    TCP_NO_CONNECT = 450,
    TCP_SOURCE_IPV4 = 451,
    TCP_SOURCE_IPV6 = 452,
    TCP_OPT_FD_CACHE = 453,
    TCP_OPT_BUF_WRITE = 454,
    TCP_OPT_CONN_WQ_MAX = 455,
    TCP_OPT_WQ_MAX = 456,
    TCP_OPT_RD_BUF = 457,
    TCP_OPT_WQ_BLK = 458,
    TCP_OPT_DEFER_ACCEPT = 459,
    TCP_OPT_DELAYED_ACK = 460,
    TCP_OPT_SYNCNT = 461,
    TCP_OPT_LINGER2 = 462,
    TCP_OPT_KEEPALIVE = 463,
    TCP_OPT_KEEPIDLE = 464,
    TCP_OPT_KEEPINTVL = 465,
    TCP_OPT_KEEPCNT = 466,
    TCP_OPT_CRLF_PING = 467,
    TCP_OPT_ACCEPT_NO_CL = 468,
    TCP_OPT_ACCEPT_HEP3 = 469,
    TCP_OPT_ACCEPT_HAPROXY = 470,
    TCP_CLONE_RCVBUF = 471,
    TCP_REUSE_PORT = 472,
    DISABLE_TLS = 473,
    ENABLE_TLS = 474,
    TLSLOG = 475,
    TLS_PORT_NO = 476,
    TLS_METHOD = 477,
    TLS_HANDSHAKE_TIMEOUT = 478,
    TLS_SEND_TIMEOUT = 479,
    SSLv23 = 480,
    SSLv2 = 481,
    SSLv3 = 482,
    TLSv1 = 483,
    TLS_VERIFY = 484,
    TLS_REQUIRE_CERTIFICATE = 485,
    TLS_CERTIFICATE = 486,
    TLS_PRIVATE_KEY = 487,
    TLS_CA_LIST = 488,
    DISABLE_SCTP = 489,
    ENABLE_SCTP = 490,
    SCTP_CHILDREN = 491,
    ADVERTISED_ADDRESS = 492,
    ADVERTISED_PORT = 493,
    DISABLE_CORE = 494,
    OPEN_FD_LIMIT = 495,
    SHM_MEM_SZ = 496,
    SHM_FORCE_ALLOC = 497,
    MLOCK_PAGES = 498,
    REAL_TIME = 499,
    RT_PRIO = 500,
    RT_POLICY = 501,
    RT_TIMER1_PRIO = 502,
    RT_TIMER1_POLICY = 503,
    RT_TIMER2_PRIO = 504,
    RT_TIMER2_POLICY = 505,
    MCAST_LOOPBACK = 506,
    MCAST_TTL = 507,
    MCAST = 508,
    TOS = 509,
    PMTU_DISCOVERY = 510,
    KILL_TIMEOUT = 511,
    MAX_WLOOPS = 512,
    PVBUFSIZE = 513,
    PVBUFSLOTS = 514,
    PVCACHELIMIT = 515,
    PVCACHEACTION = 516,
    HTTP_REPLY_PARSE = 517,
    VERSION_TABLE_CFG = 518,
    VERBOSE_STARTUP = 519,
    ROUTE_LOCKS_SIZE = 520,
    CFG_DESCRIPTION = 521,
    SERVER_ID = 522,
    KEMI = 523,
    ONSEND_ROUTE_CALLBACK = 524,
    REPLY_ROUTE_CALLBACK = 525,
    EVENT_ROUTE_CALLBACK = 526,
    RECEIVED_ROUTE_CALLBACK = 527,
    RECEIVED_ROUTE_MODE = 528,
    MAX_RECURSIVE_LEVEL = 529,
    MAX_BRANCHES_PARAM = 530,
    LATENCY_CFG_LOG = 531,
    LATENCY_LOG = 532,
    LATENCY_LIMIT_DB = 533,
    LATENCY_LIMIT_ACTION = 534,
    LATENCY_LIMIT_CFG = 535,
    MSG_TIME = 536,
    ONSEND_RT_REPLY = 537,
    FLAGS_DECL = 538,
    AVPFLAGS_DECL = 539,
    ATTR_MARK = 540,
    SELECT_MARK = 541,
    ATTR_FROM = 542,
    ATTR_TO = 543,
    ATTR_FROMURI = 544,
    ATTR_TOURI = 545,
    ATTR_FROMUSER = 546,
    ATTR_TOUSER = 547,
    ATTR_FROMDOMAIN = 548,
    ATTR_TODOMAIN = 549,
    ATTR_GLOBAL = 550,
    ADDEQ = 551,
    SUBST = 552,
    SUBSTDEF = 553,
    SUBSTDEFS = 554,
    EQUAL = 555,
    LOG_OR = 556,
    LOG_AND = 557,
    BIN_OR = 558,
    BIN_AND = 559,
    BIN_XOR = 560,
    BIN_LSHIFT = 561,
    BIN_RSHIFT = 562,
    EQUAL_T = 563,
    DIFF = 564,
    MATCH = 565,
    INTEQ = 566,
    INTDIFF = 567,
    STREQ = 568,
    STRDIFF = 569,
    GT = 570,
    LT = 571,
    GTE = 572,
    LTE = 573,
    PLUS = 574,
    MINUS = 575,
    STAR = 576,
    SLASH = 577,
    MODULO = 578,
    NOT = 579,
    UNARY = 580,
    BIN_NOT = 581,
    DEFINED = 582,
    INTCAST = 583,
    STRCAST = 584,
    DOT = 585,
    STRLEN = 586,
    STREMPTY = 587,
    NUMBER = 588,
    ID = 589,
    NUM_ID = 590,
    STRING = 591,
    IPV6ADDR = 592,
    PVAR = 593,
    AVP_OR_PVAR = 594,
    EVENT_RT_NAME = 595,
    COMMA = 596,
    SEMICOLON = 597,
    RPAREN = 598,
    LPAREN = 599,
    LBRACE = 600,
    RBRACE = 601,
    LBRACK = 602,
    RBRACK = 603,
    CR = 604,
    COLON = 605
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 196 "core/cfg.y"

	long intval;
	unsigned long uval;
	char* strval;
	struct expr* expr;
	struct action* action;
	struct case_stms* case_stms;
	struct net* ipnet;
	struct ip_addr* ipaddr;
	struct socket_id* sockid;
	struct name_lst* name_l;
	struct avp_spec* attr;
	struct _pv_spec* pvar;
	struct lvalue* lval;
	struct rvalue* rval;
	struct rval_expr* rv_expr;
	select_t* select;

#line 427 "core/cfg.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_CORE_CFG_TAB_H_INCLUDED  */
