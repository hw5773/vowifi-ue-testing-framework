/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 30 "core/cfg.y" /* yacc.c:339  */


#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include "route_struct.h"
#include "globals.h"
#include "shm_init.h"
#include "route.h"
#include "switch.h"
#include "dprint.h"
#include "sr_module.h"
#include "modparam.h"
#include "ip_addr.h"
#include "resolve.h"
#include "socket_info.h"
#include "name_alias.h"
#include "ut.h"
#include "dset.h"
#include "select.h"
#include "flags.h"
#include "tcp_init.h"
#include "tcp_options.h"
#include "sctp_core.h"
#include "pvar.h"
#include "lvalue.h"
#include "rvalue.h"
#include "sr_compat.h"
#include "msg_translator.h"
#include "async_task.h"

#include "kemi.h"
#include "ppcfg.h"
#include "pvapi.h"
#include "config.h"
#include "cfg_core.h"
#include "cfg/cfg.h"
#ifdef CORE_TLS
#include "tls/tls_config.h"
#endif
#include "timer_ticks.h"

#ifdef DEBUG_DMALLOC
#include <dmalloc.h>
#endif

/* hack to avoid alloca usage in the generated C file (needed for compiler
 with no built in alloca, like icc*/
#undef _ALLOCA_H

#define onsend_check(s) \
	do{\
		if (rt!=ONSEND_ROUTE) yyerror( s " allowed only in onsend_routes");\
	}while(0)

	#define IF_AUTO_BIND_IPV6(x) x

#ifdef USE_DNS_CACHE
	#define IF_DNS_CACHE(x) x
#else
	#define IF_DNS_CACHE(x) warn("dns cache support not compiled in")
#endif

#ifdef USE_DNS_FAILOVER
	#define IF_DNS_FAILOVER(x) x
#else
	#define IF_DNS_FAILOVER(x) warn("dns failover support not compiled in")
#endif

#ifdef USE_NAPTR
	#define IF_NAPTR(x) x
#else
	#define IF_NAPTR(x) warn("dns naptr support not compiled in")
#endif

#ifdef USE_DST_BLACKLIST
	#define IF_DST_BLACKLIST(x) x
#else
	#define IF_DST_BLACKLIST(x) warn("dst blacklist support not compiled in")
#endif

#ifdef USE_SCTP
	#define IF_SCTP(x) x
#else
	#define IF_SCTP(x) warn("sctp support not compiled in")
#endif

#ifdef USE_RAW_SOCKS
	#define IF_RAW_SOCKS(x) x
#else
	#define IF_RAW_SOCKS(x) warn("raw socket support not compiled in")
#endif


extern int yylex();
/* safer then using yytext which can be array or pointer */
extern char* yy_number_str;

static void yyerror(char* s, ...);
static void yyerror_at(struct cfg_pos* pos, char* s, ...);
static char* tmp;
static int i_tmp;
static struct socket_id* lst_tmp;
static struct name_lst*  nl_tmp;
static int rt;  /* Type of route block for find_export */
static str* str_tmp;
static str s_tmp;
static struct ip_addr* ip_tmp;
static struct avp_spec* s_attr;
static select_t sel;
static select_t* sel_ptr;
static pv_spec_t* pv_spec;
static struct action *mod_func_action;
static struct lvalue* lval_tmp;
static struct rvalue* rval_tmp;

static void warn(char* s, ...);
static void warn_at(struct cfg_pos* pos, char* s, ...);
static void get_cpos(struct cfg_pos* pos);
static struct rval_expr* mk_rve_rval(enum rval_type, void* v);
static struct rval_expr* mk_rve1(enum rval_expr_op op, struct rval_expr* rve1);
static struct rval_expr* mk_rve2(enum rval_expr_op op, struct rval_expr* rve1,
									struct rval_expr* rve2);
static int rval_expr_int_check(struct rval_expr *rve);
static int warn_ct_rve(struct rval_expr *rve, char* name);
static struct socket_id* mk_listen_id(char*, int, int);
static struct name_lst* mk_name_lst(char* name, int flags);
static struct socket_id* mk_listen_id2(struct name_lst*, int, int);
static void free_name_lst(struct name_lst* lst);
static void free_socket_id_lst(struct socket_id* i);

static struct case_stms* mk_case_stm(struct rval_expr* ct, int is_re, 
									struct action* a, int* err);
static int case_check_type(struct case_stms* stms);
static int case_check_default(struct case_stms* stms);
static int mod_f_params_pre_fixup(struct action* a);
static void free_mod_func_action(struct action* a);


extern int line;
extern int column;
extern int startcolumn;
extern int startline;
extern char *finame;
extern char *routename;
extern char *default_routename;

#define set_cfg_pos(x) \
	do{\
		if(x) {\
		(x)->cline = line;\
		(x)->cfile = (finame!=0)?finame:((cfg_file!=0)?cfg_file:"default");\
		(x)->rname = (routename!=0)?routename:((default_routename!=0)?default_routename:"DEFAULT");\
		}\
	}while(0)



#line 232 "core/cfg.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "cfg.tab.h".  */
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
#line 196 "core/cfg.y" /* yacc.c:355  */

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

#line 642 "core/cfg.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_CORE_CFG_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 659 "core/cfg.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  465
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   10575

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  351
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  104
/* YYNRULES -- Number of rules.  */
#define YYNRULES  905
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1689

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   605

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   625,   625,   628,   629,   630,   633,   634,   635,   636,
     637,   638,   638,   639,   639,   640,   641,   641,   642,   642,
     643,   643,   644,   645,   648,   664,   672,   686,   687,   693,
     694,   698,   699,   700,   701,   702,   705,   706,   707,   708,
     709,   710,   711,   714,   715,   718,   719,   720,   721,   722,
     726,   727,   728,   729,   730,   734,   735,   738,   739,   742,
     743,   745,   746,   749,   752,   758,   759,   763,   764,   767,
     768,   771,   777,   778,   779,   780,   781,   782,   783,   784,
     785,   788,   789,   795,   796,   797,   798,   799,   800,   801,
     802,   803,   804,   805,   806,   807,   808,   811,   812,   815,
     816,   817,   818,   819,   820,   821,   822,   823,   824,   825,
     826,   827,   828,   829,   830,   831,   832,   834,   835,   836,
     837,   838,   839,   840,   841,   842,   843,   844,   845,   846,
     847,   848,   849,   850,   851,   852,   853,   854,   855,   856,
     857,   858,   859,   860,   861,   862,   863,   864,   865,   866,
     867,   868,   869,   870,   871,   872,   873,   874,   875,   876,
     877,   880,   881,   884,   885,   888,   889,   890,   891,   894,
     895,   898,   899,   902,   903,   906,   907,   908,   909,   910,
     911,   912,   913,   914,   915,   916,   917,   918,   919,   920,
     921,   922,   923,   924,   925,   926,   927,   928,   929,   930,
     931,   932,   933,   934,   935,   936,   937,   938,   939,   940,
     941,   942,   943,   944,   945,   948,   949,   956,   963,   964,
     965,   966,   967,   968,   969,   970,   971,   972,   973,   974,
     975,   976,   977,   978,   985,   986,   993,   994,  1001,  1002,
    1009,  1010,  1017,  1018,  1025,  1026,  1033,  1034,  1044,  1045,
    1059,  1073,  1074,  1081,  1082,  1089,  1090,  1097,  1098,  1107,
    1108,  1117,  1118,  1125,  1126,  1133,  1134,  1141,  1142,  1149,
    1150,  1157,  1158,  1165,  1166,  1173,  1174,  1181,  1182,  1189,
    1190,  1197,  1198,  1205,  1206,  1213,  1214,  1221,  1222,  1229,
    1230,  1237,  1238,  1245,  1246,  1253,  1254,  1261,  1263,  1270,
    1271,  1282,  1283,  1290,  1291,  1298,  1299,  1306,  1307,  1314,
    1315,  1322,  1329,  1336,  1343,  1350,  1357,  1358,  1365,  1366,
    1373,  1374,  1381,  1382,  1389,  1390,  1397,  1398,  1405,  1406,
    1413,  1414,  1421,  1422,  1429,  1430,  1431,  1432,  1435,  1436,
    1439,  1440,  1441,  1442,  1454,  1467,  1480,  1482,  1493,  1494,
    1495,  1496,  1502,  1503,  1513,  1514,  1515,  1516,  1517,  1518,
    1525,  1526,  1533,  1534,  1535,  1536,  1537,  1538,  1539,  1540,
    1541,  1542,  1543,  1544,  1545,  1546,  1547,  1548,  1549,  1550,
    1557,  1558,  1565,  1566,  1573,  1580,  1581,  1582,  1609,  1610,
    1611,  1612,  1613,  1614,  1615,  1616,  1617,  1618,  1619,  1620,
    1621,  1622,  1623,  1624,  1625,  1626,  1627,  1628,  1629,  1630,
    1631,  1632,  1641,  1642,  1651,  1652,  1661,  1662,  1671,  1672,
    1673,  1674,  1675,  1676,  1677,  1678,  1679,  1680,  1681,  1682,
    1683,  1684,  1685,  1686,  1687,  1688,  1689,  1690,  1691,  1692,
    1694,  1695,  1697,  1699,  1700,  1701,  1704,  1705,  1708,  1709,
    1710,  1713,  1714,  1717,  1718,  1719,  1727,  1732,  1737,  1742,
    1747,  1750,  1755,  1763,  1769,  1770,  1780,  1781,  1791,  1792,
    1801,  1810,  1811,  1817,  1818,  1824,  1828,  1829,  1832,  1861,
    1876,  1877,  1881,  1892,  1893,  1897,  1898,  1902,  1909,  1925,
    1926,  1929,  1932,  1939,  1955,  1959,  1960,  1965,  1965,  1972,
    1973,  1975,  1974,  1997,  2002,  2005,  2012,  2028,  2031,  2034,
    2041,  2057,  2060,  2063,  2080,  2083,  2084,  2085,  2086,  2087,
    2088,  2110,  2111,  2112,  2113,  2116,  2117,  2118,  2119,  2122,
    2123,  2129,  2130,  2131,  2132,  2133,  2134,  2135,  2138,  2139,
    2140,  2141,  2148,  2149,  2150,  2157,  2158,  2159,  2163,  2164,
    2165,  2166,  2167,  2172,  2173,  2176,  2177,  2178,  2184,  2186,
    2188,  2189,  2191,  2193,  2195,  2197,  2199,  2200,  2202,  2203,
    2204,  2205,  2207,  2209,  2211,  2213,  2215,  2217,  2218,  2254,
    2256,  2258,  2260,  2263,  2265,  2267,  2269,  2273,  2274,  2285,
    2286,  2290,  2291,  2306,  2321,  2322,  2325,  2326,  2327,  2339,
    2340,  2355,  2370,  2372,  2379,  2421,  2422,  2425,  2426,  2427,
    2430,  2431,  2432,  2433,  2434,  2435,  2436,  2437,  2440,  2448,
    2458,  2475,  2482,  2489,  2496,  2503,  2508,  2513,  2514,  2515,
    2516,  2517,  2520,  2529,  2536,  2561,  2577,  2578,  2583,  2602,
    2611,  2623,  2638,  2639,  2642,  2642,  2652,  2653,  2654,  2655,
    2656,  2657,  2658,  2659,  2660,  2663,  2666,  2667,  2670,  2677,
    2680,  2687,  2693,  2694,  2703,  2704,  2705,  2708,  2709,  2721,
    2745,  2756,  2787,  2791,  2801,  2815,  2827,  2828,  2830,  2831,
    2832,  2846,  2847,  2848,  2849,  2850,  2851,  2852,  2856,  2857,
    2858,  2871,  2877,  2878,  2879,  2880,  2881,  2882,  2883,  2884,
    2885,  2886,  2887,  2888,  2889,  2890,  2891,  2909,  2910,  2911,
    2912,  2913,  2914,  2915,  2916,  2917,  2918,  2919,  2920,  2921,
    2922,  2923,  2924,  2925,  2927,  2929,  2930,  2931,  2932,  2933,
    2936,  2954,  2955,  2956,  2959,  2960,  2961,  2962,  2963,  2964,
    2965,  2966,  2967,  2968,  2969,  2970,  2971,  2972,  2973,  2974,
    2975,  2976,  2977,  2978,  2979,  2980,  2981,  2982,  2983,  2984,
    2985,  2986,  2987,  2988,  2989,  2990,  2991,  2992,  2993,  3001,
    3009,  3017,  3025,  3033,  3041,  3049,  3057,  3065,  3066,  3068,
    3076,  3084,  3092,  3101,  3110,  3119,  3127,  3136,  3144,  3145,
    3147,  3150,  3151,  3152,  3153,  3160,  3167,  3168,  3174,  3181,
    3182,  3188,  3195,  3196,  3202,  3205,  3208,  3209,  3210,  3213,
    3214,  3215,  3224,  3233,  3234,  3235,  3236,  3237,  3238,  3239,
    3240,  3241,  3242,  3243,  3244,  3245,  3246,  3247,  3248,  3249,
    3250,  3254,  3258,  3259,  3260,  3261,  3262,  3263,  3264,  3265,
    3266,  3267,  3268,  3269,  3270,  3271,  3272,  3273,  3274,  3275,
    3276,  3277,  3278,  3279,  3280,  3281,  3282,  3283,  3284,  3285,
    3293,  3301,  3309,  3310,  3312,  3314,  3325,  3326,  3327,  3343,
    3344,  3345,  3349,  3352,  3353,  3356,  3359,  3362,  3365,  3368,
    3371,  3374,  3377,  3380,  3383,  3384,  3385,  3388,  3389,  3390,
    3390,  3416,  3418,  3420,  3421,  3424,  3441,  3445,  3449,  3453,
    3458,  3462,  3467,  3471,  3475,  3479
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "FORWARD", "FORWARD_TCP", "FORWARD_TLS",
  "FORWARD_SCTP", "FORWARD_UDP", "EXIT", "DROP", "RETURN", "BREAK",
  "LOG_TOK", "ERROR", "ROUTE", "ROUTE_REQUEST", "ROUTE_FAILURE",
  "ROUTE_ONREPLY", "ROUTE_REPLY", "ROUTE_BRANCH", "ROUTE_SEND",
  "ROUTE_EVENT", "EXEC", "SET_HOST", "SET_HOSTPORT", "SET_HOSTPORTTRANS",
  "PREFIX", "STRIP", "STRIP_TAIL", "SET_USERPHONE", "APPEND_BRANCH",
  "REMOVE_BRANCH", "CLEAR_BRANCHES", "SET_USER", "SET_USERPASS",
  "SET_PORT", "SET_URI", "REVERT_URI", "FORCE_RPORT", "ADD_LOCAL_RPORT",
  "FORCE_TCP_ALIAS", "UDP_MTU", "UDP_MTU_TRY_PROTO", "UDP4_RAW",
  "UDP4_RAW_MTU", "UDP4_RAW_TTL", "IF", "ELSE", "SET_ADV_ADDRESS",
  "SET_ADV_PORT", "FORCE_SEND_SOCKET", "SET_FWD_NO_CONNECT",
  "SET_RPL_NO_CONNECT", "SET_FWD_CLOSE", "SET_RPL_CLOSE", "SWITCH", "CASE",
  "DEFAULT", "WHILE", "CFG_SELECT", "CFG_RESET", "URIHOST", "URIPORT",
  "MAX_LEN", "SETFLAG", "RESETFLAG", "ISFLAGSET", "SETAVPFLAG",
  "RESETAVPFLAG", "ISAVPFLAGSET", "METHOD", "URI", "FROM_URI", "TO_URI",
  "SRCIP", "SRCPORT", "DSTIP", "DSTPORT", "TOIP", "TOPORT", "SNDIP",
  "SNDPORT", "SNDPROTO", "SNDAF", "PROTO", "AF", "MYSELF", "MSGLEN", "UDP",
  "TCP", "TLS", "SCTP", "WS", "WSS", "DEBUG_V", "FORK", "FORK_DELAY",
  "MODINIT_DELAY", "LOGSTDERROR", "LOGFACILITY", "LOGNAME", "LOGCOLOR",
  "LOGPREFIX", "LOGPREFIXMODE", "LOGENGINETYPE", "LOGENGINEDATA",
  "XAVPVIAPARAMS", "XAVPVIAFIELDS", "LISTEN", "ADVERTISE", "ALIAS",
  "SR_AUTO_ALIASES", "DNS", "REV_DNS", "DNS_TRY_IPV6", "DNS_TRY_NAPTR",
  "DNS_SRV_LB", "DNS_UDP_PREF", "DNS_TCP_PREF", "DNS_TLS_PREF",
  "DNS_SCTP_PREF", "DNS_RETR_TIME", "DNS_SLOW_QUERY_MS", "DNS_RETR_NO",
  "DNS_SERVERS_NO", "DNS_USE_SEARCH", "DNS_SEARCH_FMATCH",
  "DNS_NAPTR_IGNORE_RFC", "DNS_CACHE_INIT", "DNS_USE_CACHE",
  "DNS_USE_FAILOVER", "DNS_CACHE_FLAGS", "DNS_CACHE_NEG_TTL",
  "DNS_CACHE_MIN_TTL", "DNS_CACHE_MAX_TTL", "DNS_CACHE_MEM",
  "DNS_CACHE_GC_INT", "DNS_CACHE_DEL_NONEXP", "DNS_CACHE_REC_PREF",
  "AUTO_BIND_IPV6", "BIND_IPV6_LINK_LOCAL", "DST_BLST_INIT",
  "USE_DST_BLST", "DST_BLST_MEM", "DST_BLST_TTL", "DST_BLST_GC_INT",
  "DST_BLST_UDP_IMASK", "DST_BLST_TCP_IMASK", "DST_BLST_TLS_IMASK",
  "DST_BLST_SCTP_IMASK", "IP_FREE_BIND", "PORT", "STAT", "CHILDREN",
  "SOCKET_WORKERS", "ASYNC_WORKERS", "ASYNC_USLEEP", "ASYNC_NONBLOCK",
  "CHECK_VIA", "PHONE2TEL", "MEMLOG", "MEMDBG", "MEMSUM", "MEMSAFETY",
  "MEMJOIN", "MEMSTATUSMODE", "CORELOG", "SIP_WARNING", "SERVER_SIGNATURE",
  "SERVER_HEADER", "USER_AGENT_HEADER", "REPLY_TO_VIA", "LOADMODULE",
  "LOADPATH", "MODPARAM", "CFGENGINE", "MAXBUFFER", "SQL_BUFFER_SIZE",
  "USER", "GROUP", "CHROOT", "WDIR", "RUNDIR", "MHOMED", "DISABLE_TCP",
  "TCP_ACCEPT_ALIASES", "TCP_ACCEPT_UNIQUE", "TCP_CONNECTION_MATCH",
  "TCP_CHILDREN", "TCP_CONNECT_TIMEOUT", "TCP_SEND_TIMEOUT",
  "TCP_CON_LIFETIME", "TCP_POLL_METHOD", "TCP_MAX_CONNECTIONS",
  "TLS_MAX_CONNECTIONS", "TCP_NO_CONNECT", "TCP_SOURCE_IPV4",
  "TCP_SOURCE_IPV6", "TCP_OPT_FD_CACHE", "TCP_OPT_BUF_WRITE",
  "TCP_OPT_CONN_WQ_MAX", "TCP_OPT_WQ_MAX", "TCP_OPT_RD_BUF",
  "TCP_OPT_WQ_BLK", "TCP_OPT_DEFER_ACCEPT", "TCP_OPT_DELAYED_ACK",
  "TCP_OPT_SYNCNT", "TCP_OPT_LINGER2", "TCP_OPT_KEEPALIVE",
  "TCP_OPT_KEEPIDLE", "TCP_OPT_KEEPINTVL", "TCP_OPT_KEEPCNT",
  "TCP_OPT_CRLF_PING", "TCP_OPT_ACCEPT_NO_CL", "TCP_OPT_ACCEPT_HEP3",
  "TCP_OPT_ACCEPT_HAPROXY", "TCP_CLONE_RCVBUF", "TCP_REUSE_PORT",
  "DISABLE_TLS", "ENABLE_TLS", "TLSLOG", "TLS_PORT_NO", "TLS_METHOD",
  "TLS_HANDSHAKE_TIMEOUT", "TLS_SEND_TIMEOUT", "SSLv23", "SSLv2", "SSLv3",
  "TLSv1", "TLS_VERIFY", "TLS_REQUIRE_CERTIFICATE", "TLS_CERTIFICATE",
  "TLS_PRIVATE_KEY", "TLS_CA_LIST", "DISABLE_SCTP", "ENABLE_SCTP",
  "SCTP_CHILDREN", "ADVERTISED_ADDRESS", "ADVERTISED_PORT", "DISABLE_CORE",
  "OPEN_FD_LIMIT", "SHM_MEM_SZ", "SHM_FORCE_ALLOC", "MLOCK_PAGES",
  "REAL_TIME", "RT_PRIO", "RT_POLICY", "RT_TIMER1_PRIO",
  "RT_TIMER1_POLICY", "RT_TIMER2_PRIO", "RT_TIMER2_POLICY",
  "MCAST_LOOPBACK", "MCAST_TTL", "MCAST", "TOS", "PMTU_DISCOVERY",
  "KILL_TIMEOUT", "MAX_WLOOPS", "PVBUFSIZE", "PVBUFSLOTS", "PVCACHELIMIT",
  "PVCACHEACTION", "HTTP_REPLY_PARSE", "VERSION_TABLE_CFG",
  "VERBOSE_STARTUP", "ROUTE_LOCKS_SIZE", "CFG_DESCRIPTION", "SERVER_ID",
  "KEMI", "ONSEND_ROUTE_CALLBACK", "REPLY_ROUTE_CALLBACK",
  "EVENT_ROUTE_CALLBACK", "RECEIVED_ROUTE_CALLBACK", "RECEIVED_ROUTE_MODE",
  "MAX_RECURSIVE_LEVEL", "MAX_BRANCHES_PARAM", "LATENCY_CFG_LOG",
  "LATENCY_LOG", "LATENCY_LIMIT_DB", "LATENCY_LIMIT_ACTION",
  "LATENCY_LIMIT_CFG", "MSG_TIME", "ONSEND_RT_REPLY", "FLAGS_DECL",
  "AVPFLAGS_DECL", "ATTR_MARK", "SELECT_MARK", "ATTR_FROM", "ATTR_TO",
  "ATTR_FROMURI", "ATTR_TOURI", "ATTR_FROMUSER", "ATTR_TOUSER",
  "ATTR_FROMDOMAIN", "ATTR_TODOMAIN", "ATTR_GLOBAL", "ADDEQ", "SUBST",
  "SUBSTDEF", "SUBSTDEFS", "EQUAL", "LOG_OR", "LOG_AND", "BIN_OR",
  "BIN_AND", "BIN_XOR", "BIN_LSHIFT", "BIN_RSHIFT", "EQUAL_T", "DIFF",
  "MATCH", "INTEQ", "INTDIFF", "STREQ", "STRDIFF", "GT", "LT", "GTE",
  "LTE", "PLUS", "MINUS", "STAR", "SLASH", "MODULO", "NOT", "UNARY",
  "BIN_NOT", "DEFINED", "INTCAST", "STRCAST", "DOT", "STRLEN", "STREMPTY",
  "NUMBER", "ID", "NUM_ID", "STRING", "IPV6ADDR", "PVAR", "AVP_OR_PVAR",
  "EVENT_RT_NAME", "COMMA", "SEMICOLON", "RPAREN", "LPAREN", "LBRACE",
  "RBRACE", "LBRACK", "RBRACK", "CR", "COLON", "$accept", "cfg",
  "statements", "statement", "$@1", "$@2", "$@3", "$@4", "$@5",
  "listen_id", "listen_id_lst", "listen_id2", "proto", "eqproto", "port",
  "phostport", "listen_phostport", "id_lst", "intno", "flags_decl",
  "flag_list", "flag_spec", "flag_name", "avpflags_decl", "avpflag_list",
  "avpflag_spec", "assign_stm", "cfg_var_id", "cfg_var_idn", "cfg_var",
  "module_stm", "ip", "ipv4", "ipv6addr", "ipv6", "route_name",
  "route_main", "route_stm", "failure_route_main", "failure_route_stm",
  "route_reply_main", "onreply_route_stm", "$@6", "$@7",
  "branch_route_main", "branch_route_stm", "send_route_main",
  "send_route_stm", "event_route_main", "event_route_stm",
  "preprocess_stm", "equalop", "cmpop", "strop", "rve_equalop",
  "rve_cmpop", "uri_type", "eint_op_onsend", "eint_op", "eip_op_onsend",
  "eip_op", "exp_elem", "ipnet", "host", "host_if_id", "host_or_if",
  "fcmd", "stm", "actions", "action", "if_cmd", "ct_rval", "single_case",
  "case_stms", "switch_cmd", "while_cmd", "select_param", "select_params",
  "select_id", "$@8", "attr_class_spec", "attr_name_spec", "attr_spec",
  "attr_mark", "attr_id", "attr_id_num_idx", "attr_id_no_idx",
  "attr_id_ass", "attr_id_any", "attr_id_any_str", "pvar", "avp_pvar",
  "assign_op", "lval", "rval", "rve_un_op", "rval_expr", "assign_action",
  "avpflag_oper", "cmd", "$@9", "func_params", "func_param", "ret_cmd", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,   539,   540,   541,   542,   543,   544,
     545,   546,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   566,   567,   568,   569,   570,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,   581,   582,   583,   584,
     585,   586,   587,   588,   589,   590,   591,   592,   593,   594,
     595,   596,   597,   598,   599,   600,   601,   602,   603,   604,
     605
};
# endif

#define YYPACT_NINF -1166

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-1166)))

#define YYTABLE_NINF -890

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    2414,  -276,    20,    14,  -253,  -208,  -185,  -178,  -138,  -119,
   -1166,  -116,  -102,   -94,   -91,   -65,   -62,   -56,   -53,   -37,
     -33,   -32,   -15,    21,    22,    25,   425,   474,   478,   490,
      51,    54,   686,   696,   697,   732,   740,   837,   864,   865,
     870,   889,   890,   896,   898,   900,   908,   909,   912,   922,
     924,   925,   929,   930,   931,   937,   947,   948,   949,   951,
     952,   964,   968,   969,   975,   976,   491,   511,   536,   541,
     545,   549,   550,   553,   554,   576,   607,   611,   615,   619,
     622,   627,   645,   659,   664,   710,   722,    78,    41,    44,
     199,   734,   736,   743,   747,   748,   787,   798,   839,   845,
     863,   888,   894,   919,   927,   928,   950,   965,   966,   967,
     983,   987,   992,   993,   999,   977,   978,   979,   980,  1010,
    1013,  1015,  1020,  1059,  1064,  1066,  1074,  1076,  1077,  1079,
    1105,  1116,  1118,  1121,  1126,  1127,  1128,  1129,  1130,  1131,
    1132,  1136,  1183,  1184,  1185,  1196,  1197,  1203,  1204,  1205,
    1206,  1207,  1208,  1210,  1211,  1213,  1214,  1215,  1216,  1217,
    1218,  1219,  1220,  1223,  1225,  1226,   982,  1227,  1228,  1229,
    1231,  1232,  1233,  1234,  1260,  1261,  1267,  1268,  -231,  1269,
    1270,  1271,  1272,  1273,  1274,  1275,  1276,  1277,  1302,   159,
     182,   101,   165,   185, -1166, -1166, -1166,   284,  1521, -1166,
     137,   300,   848,   306,   882, -1166, -1166, -1166,  -247, -1166,
   -1166,   -39, -1166, -1166, -1166, -1166,   600, -1166,    75,   120,
     792,   209,   167,   207,   318,   280,   319,   327,   337,    26,
     218,   344,   219,   466,   240,   248,   251,   252,    35,    65,
     522,   531,   559, -1166,   642, -1166,   660, -1166,   665, -1166,
    -162, -1166,  -162, -1166,  -162, -1166,  -162, -1166,   671, -1166,
     676, -1166,   683, -1166,   695, -1166,   706, -1166,   711, -1166,
     718, -1166,   719, -1166,   726, -1166,   768, -1166,   774, -1166,
     775, -1166,   778, -1166,   786, -1166,   823, -1166,   838, -1166,
     850, -1166,  1045, -1166,  1048, -1166,  1279, -1166,  1280, -1166,
    1297, -1166,  1340, -1166,  1413, -1166,  1414, -1166,  1415, -1166,
    1416, -1166,  1454, -1166,  1457,   330,   567,   579,   588,   629,
     630,   633,   635,   667,   333,   338,   339,   341,   342,   345,
     349,   672,   675,   256,   259,   687, -1166, -1166, -1166,   261,
   -1166, -1166,   745, -1166,   269, -1166,   688,   690,   188,   223,
     227,   230,   231,   691,   692,   693,   694,   700,   702,   402,
     446,   468,   235,   704,   705,   708,   709,    38,   712,   713,
   -1166,  1458, -1166,  1459, -1166,  1460, -1166,  1473,   725,   727,
     728,   729,   730,   731,   741,   742,   744,   750,   751,   752,
     753,   755,   756,   757,   758,   759,  1069,   760,   761,   762,
     763,   270,   271,   273,   764,   766,   767,   149,   769,   776,
     779,   781,   782,   788,   794,   795,   796,   797,   799,   801,
     802,   803,   805,   239,   310, -1166,  1474,   807,   808,   809,
     811,   813,   814,   815,   274,   816,   817,   818,   -58,   472,
    1475,  1476,   493,   537,   819,   820,   821,   822,   828, -1166,
   -1166, -1166, -1166,   714,  1253, -1166, -1166, -1166,  1263, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166,  -276, -1166,    36,    39,
    1262, -1166,    42,  1264, -1166,    48,  1265, -1166,    59,  1266,
   -1166,    31,  1258, -1166,   716,  1477, -1166, -1166, -1166, -1166,
    1463, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166,  1479, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,  1483,
   -1166, -1166, -1166, -1166,  1078,  1478, -1166,  1464,  1466,  1165,
    1499, -1166, -1166, -1166, -1166, -1166,  -187, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166,  1480, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166,  1483, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,  1517,
    1546,  1547,  1548, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166,  -278,  1489,  -278, -1166,   600, -1166, 10233, -1166,   600,
   10233, -1166,   600, 10233, -1166,   600, 10233, -1166,  1509, -1166,
   -1166, -1166,  1550,  1503, 10233,    52, -1166,  1519,  1512,  1511,
    1508,   346,  1073, -1166,  1078,   307,   314,  1523,   279,   282,
     285,   286, -1166, -1166, -1166,  1510,    63,    69,    71,    79,
      80,  9324,  9547,  9667, -1166,    81,    85,    86,  1513,    88,
      90,    94,    95,    96,    97,   100,   111,  1516,   113,   116,
     117,   119,  1518,  1520,  1524,  1539,  1540,  9770,   126,   128,
     130,  1541,  1542,  1543,  1544,  5693,  9770,   133,   134,   136,
     143,   144, -1166, -1166, -1166, -1166,   147, -1166, -1166, -1166,
      24,  3441, -1166, -1166, -1166, -1166,  1039, -1166, -1166, -1166,
   -1166, -1166,  1561,  1525,   148, -1166,  1556,  1563,  3515,  1564,
    3599,  1565,  3679,  1567,   118,  1536,  3863, -1166,  1579,  1559,
    1078, -1166, -1166, -1166, -1166, -1166, -1166,  1575,  1580, -1166,
   -1166, -1166, -1166, -1166,  1568, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166,  1584, -1166,   109, -1166,   194, -1166,   200,
   -1166,   249, -1166,   257,   871, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166,   878, -1166,   878, -1166,
     698, -1166, -1166,  1479, -1166, -1166,  5794,  5916,  6139,  1587,
    1595, -1166,  5125, 10119, -1166,   897, -1166,   860, -1166,   907,
   -1166, -1166, -1166,  1039, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166,  6261,  1278,  5226,  1278,  5348,  1278, -1166,   215, -1166,
     287, -1166,  6362,  1616, -1166,   288, -1166,   290, -1166,   291,
   -1166,   292, -1166,   829, -1166,   833, -1166,  1610, -1166,   206,
    1611, -1166,   293, -1166,   296, -1166,   297, -1166,   299,  1612,
    1613,  1614,   164,   883,  6484,  9186, -1166,   265, -1166,   834,
   -1166,    84,  1615,  1624,  1635,  1637, -1166,  1035,  9186, -1166,
     301, -1166,   303, -1166,   651, -1166,   784, -1166,  1027, -1166,
    1638, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166,  1651, -1166,  1646, -1166,
    9770, -1166, -1166,   181, -1166,  1639, -1166,  1649, -1166,  1653,
   -1166,  1654, -1166,  1734,  1742,   716, -1166, 10233,  1688, -1166,
    -106,  1689,  -243, 10233,  1680,  -104, -1166,   393, -1166,   517,
    -151,  1681,   561,   573,   603,    -6,  1682,   617,   648,   735,
     852,  1683,   739,   783,   790,   961,  1684,   856,   893,   957,
     981, -1166, -1166, -1166, -1166, -1166, -1166, -1166,  6707,  4780,
    5003, -1166,  1245,  1702, -1166, -1166, -1166, -1166, -1166, -1166,
    6829,  6930,  1704, -1166,  1749,  1750,  1751,  1753,  1081,  1719,
    1717,  3943, -1166,  7052, -1166, -1166, -1166, -1166, -1166,  7275,
    7397, -1166,  2746,  1720, -1166, -1166,  7498,  7620,  7843,  7965,
    9770,  9770,  9770, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166,  8066,  8188,  8411,  8533,  8634,  8756,
    8979, -1166, -1166,  1725,  1728,  1747,  1748,  1736,  1752,    67,
    1139,  1762,  1773,  1775,  1776,  1777,  1779,  1788,  1789,  1793,
    1794,  1795,  1802,  1803, -1166,  1804, -1166,  1805, -1166,  1806,
    1807,  1808,  1809,  1811,  1812,  1813,  1814, -1166, -1166, -1166,
    1815,  1816, -1166,  1817,  1818, 10233,  2045, -1166,  1819,  1820,
    1821,  1822,  1823,  1743,  1744,  1824, -1166, -1166, -1166, -1166,
      17, -1166,  1825,  1755,  1849,  1850,  1851,  1852,  1853,  1854,
    1856,  1857,  9770,  1867,  1855,  1278,   962, -1166,  1039, -1166,
    1863, -1166, 10233, 10233, 10233, 10233,  1869,  1872,  1914,  4023,
    1885, -1166, -1166,  1873,  1874,  4092, -1166,   -21, -1166,  1886,
   -1166,  1887, -1166,    58,   145,  1888, -1166, -1166,   -16, -1166,
    1890, -1166,  1891, -1166,  1892, -1166, -1166,    -1, -1166,  1893,
   -1166,  1894, -1166,  1895, -1166, -1166,    16, -1166,  1896, -1166,
    1897, -1166,  1898, -1166, -1166,   662, -1166,  1899, -1166,  1900,
   -1166,  1901, -1166, -1166,  1937,  1172, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166,  1172, -1166, -1166,  1172, -1166,
   -1166, -1166,  1889, -1166,  1905,  1918,  1522,  1919,  2180, -1166,
   -1166, -1166, -1166, -1166, -1166,  2006,  1172, -1166,  1172, -1166,
    1049, -1166,  2075,  1483,  1868,  1915, -1166,  -173,  1172,   -43,
   -1166,  2886, -1166,  2907, -1166,  3101, -1166,  1158,  3443,  3494,
    1235, -1166,  -145, -1166,  -145, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166,  1172, -1166,  1049, -1166,  1927, -1166, -1166,  1928,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
    4207, 10011, -1166, -1166, -1166, -1166, -1166,   504,  1078, -1166,
    1920,  5571,  1917, -1166, -1166,     6, -1166,  9890, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166,  1278,   971, -1166, -1166,
   -1166,  -278, -1166, -1166,   245,  4287,  4436,  4551,  4631, -1166,
   -1166,  -220, -1166,  1932, -1166, -1166, -1166,  1925,  1926,  1929,
    1930, -1166, -1166, -1166, -1166,  1931,  1934,  1940,  1941,  1942,
    1943,  1945,  1946,  1947,  1949,  1950,  1951,  1952,  1953,  1954,
    1955,  1956,  1957,  1958,  1959,  1960,  -140,  1702, -1166, -1166,
   -1166, -1166,    43,  1922,  1961,  1962, -1166, -1166, -1166, -1166,
    1980, -1166,  1981,  9101,  1982,  1278, 10233, -1166, -1166,  1963,
    2418,  9770, -1166,  1990,  1991,  1992, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,  1988,  1989,
   -1166, -1166,  1483, -1166, -1166, -1166, -1166,  -106, 10233,  1993,
    1996,  2833,  2902, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1166,  2987, 10233, 10233, -1166,  3097,  3251,  3335
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     452,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   451,    22,    23,     0,     0,     4,
       0,     0,     0,     0,     0,     8,     9,     6,     0,   449,
      10,     0,    15,     7,   450,   499,     0,   500,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   105,     0,   107,     0,   109,     0,   111,
       0,   113,     0,   115,     0,   117,     0,   119,     0,   121,
       0,   123,     0,   125,     0,   127,     0,   129,     0,   131,
       0,   133,     0,   135,     0,   137,     0,   139,     0,   141,
       0,   145,     0,   143,     0,   147,     0,   149,     0,   151,
       0,   153,     0,   155,     0,   157,     0,   159,     0,   161,
       0,   163,     0,   165,     0,   167,     0,   169,     0,   171,
       0,   173,     0,   175,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   464,   463,   466,     0,
     465,   471,     0,   473,     0,   472,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     267,     0,   269,     0,   271,     0,   273,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   390,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
      66,    65,    59,    61,    63,    68,    71,    67,    69,   516,
     515,   518,   517,   520,   519,     1,     5,     3,     0,     0,
       0,    12,     0,     0,    14,     0,     0,    17,     0,     0,
      19,     0,     0,    21,     0,     0,   497,   482,   483,   484,
       0,   440,   439,   438,   437,   442,    31,    32,    33,    34,
      35,   441,   444,     0,    57,   443,   446,   445,   448,   447,
      73,    72,    75,    74,    77,    76,    79,    78,    81,    80,
      83,    82,    85,    84,    87,    86,    89,    88,    91,    90,
      93,    92,    95,    94,    97,    96,    99,    98,   346,   598,
     596,   597,    25,   479,     0,     0,    30,    50,     0,    55,
     343,    24,   476,   480,   477,   599,    26,   348,   347,   350,
     349,   101,   100,   103,   102,   104,   106,   108,   110,   112,
     114,   116,   118,   120,   122,   124,   126,   128,   130,   132,
     134,   136,   138,   140,   144,   142,   146,   148,   150,   152,
     154,   156,   158,   160,   162,   164,   166,   168,   170,   172,
     174,   177,   176,   183,   178,   185,   184,   187,   186,   189,
     188,   191,   190,   193,   192,   195,   194,   197,   196,   199,
     198,   201,   200,   203,   202,   205,   204,   207,   206,   209,
     208,   211,   210,   213,   212,   336,   335,   338,   337,   340,
     339,   342,   341,   468,   467,     0,   475,   474,   180,   179,
     182,   181,   218,   217,   216,   221,   220,   219,   224,   223,
     222,   227,   226,   225,   230,   229,   228,   232,   231,   234,
     233,   236,   235,   238,   237,   240,   239,   242,   241,   244,
     243,   246,   245,   248,   247,   251,   249,   250,   253,   252,
     255,   254,   257,   256,   259,     0,   258,   261,   260,   263,
     262,   265,   264,   266,   268,   270,   272,   275,   274,   277,
     276,   279,   278,   281,   280,   283,   282,   285,   284,   287,
     286,   289,   288,   291,   290,   293,   292,   295,   294,   297,
     296,   299,   298,   301,   300,   303,   302,   305,   304,   307,
     306,   309,   308,   314,   310,   311,   312,   313,   326,   325,
     328,   327,   316,   315,   318,   317,   320,   319,   322,   321,
     324,   323,   330,   329,   332,   331,   334,   333,   352,   351,
     354,   353,   356,   355,   358,   357,   360,   359,   362,   361,
     364,   363,   366,   365,   368,   367,   370,   369,   372,   371,
     374,   373,   376,   375,   378,   377,   380,   379,   382,   381,
     385,   383,   384,   388,   386,   387,   389,   392,   391,   394,
     393,   396,   395,   398,   397,   400,   399,   402,   401,   404,
     403,   215,   214,   406,   405,   408,   407,   410,   409,     0,
       0,     0,     0,   420,   419,   421,   422,   426,   425,   424,
     423,   428,   427,   430,   429,   432,   431,   434,   433,   436,
     435,     0,     0,     0,   489,     0,   490,     0,   494,     0,
       0,   507,     0,     0,   511,     0,     0,   514,     0,   454,
     455,   453,     0,     0,     0,     0,    58,     0,    27,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    62,    64,    70,     0,     0,     0,     0,     0,
       0,   901,   898,   902,   905,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   854,   856,   858,   861,     0,     0,     0,     0,
       0,   875,   877,   879,   881,     0,     0,     0,     0,     0,
       0,     0,   731,   732,   733,   658,     0,   670,   671,   616,
       0,     0,   608,   611,   612,   613,     0,   662,   663,   673,
     674,   675,     0,     0,     0,   604,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   503,     0,     0,
       0,    29,   481,    54,    44,    43,    51,    52,   345,   603,
     598,   601,   602,   600,     0,   412,   411,   414,   413,   416,
     415,   418,   417,     0,   744,     0,   766,     0,   777,     0,
     788,     0,   755,     0,     0,   542,   543,   544,   555,   548,
     556,   549,   554,   546,   553,   545,     0,   547,     0,   550,
       0,   551,   644,   690,   688,   689,     0,     0,     0,     0,
       0,   677,     0,     0,   676,     0,   552,     0,   557,     0,
     683,   682,   681,     0,   664,   666,   665,   678,   679,   680,
     691,     0,   900,     0,   897,     0,   904,   792,     0,   809,
       0,   813,     0,     0,   817,     0,   836,     0,   839,     0,
     820,     0,   826,     0,   823,     0,   829,     0,   832,     0,
       0,   845,     0,   848,     0,   842,     0,   851,     0,     0,
       0,     0,     0,     0,     0,     0,   867,     0,   870,     0,
     873,     0,     0,     0,     0,     0,   636,     0,     0,   884,
       0,   887,     0,   796,     0,   799,     0,   802,     0,   891,
       0,   617,   610,   609,   487,   607,   646,   647,   648,   649,
     650,   651,   652,   653,   654,   655,     0,   656,   659,   672,
       0,   615,   807,     0,   614,     0,   492,     0,   505,     0,
     509,     0,   460,   456,   457,     0,   498,     0,     0,    28,
       0,     0,     0,     0,     0,     0,   591,     0,   734,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   561,   521,   522,   530,   523,   524,   529,     0,     0,
       0,   586,     0,     0,   729,   712,   714,   693,   715,   694,
       0,     0,     0,   899,   664,   665,   679,   680,     0,     0,
       0,     0,   565,     0,   570,   525,   526,   527,   528,     0,
       0,   582,     0,   659,   713,   692,     0,     0,     0,     0,
       0,     0,     0,   531,   532,   537,   533,   534,   535,   536,
     538,   539,   540,   541,     0,     0,     0,     0,     0,     0,
       0,   896,   903,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   828,     0,   831,     0,   834,     0,
       0,     0,     0,     0,     0,     0,     0,   853,   855,   857,
       0,     0,   860,     0,     0,     0,   618,   605,     0,     0,
       0,     0,     0,    45,     0,     0,   874,   876,   878,   880,
       0,   638,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   892,     0,     0,   730,     0,   669,     0,   667,
       0,   668,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,   344,     0,     0,     0,   745,     0,   743,     0,
     736,     0,   737,     0,     0,     0,   735,   767,     0,   765,
       0,   758,     0,   759,     0,   757,   778,     0,   776,     0,
     769,     0,   770,     0,   768,   789,     0,   787,     0,   780,
       0,   781,     0,   779,   756,     0,   754,     0,   747,     0,
     748,     0,   746,   560,     0,   558,   576,    36,    37,    38,
      39,    40,    41,    42,   574,   575,   573,   571,   572,   585,
     583,   584,   639,   643,   645,     0,     0,     0,     0,   687,
     709,   686,   685,   684,   564,     0,   562,   569,   567,   568,
     566,   581,     0,    57,     0,   589,   577,   579,   578,     0,
     726,   708,   725,   707,   721,   700,   722,   701,   702,   703,
     704,   716,   695,   717,   696,   718,   697,   719,   698,   720,
     699,   724,   706,   723,   705,   793,     0,   790,   810,     0,
     814,   812,   811,   815,   818,   816,   837,   835,   840,   838,
     821,   819,   827,   825,   824,   822,   833,   830,   846,   844,
     849,   847,   843,   841,   852,   850,   862,   859,   864,   863,
       0,     0,   866,   865,   869,   868,   872,     0,     0,   871,
       0,     0,     0,   635,   633,     0,   885,     0,   888,   886,
     794,   795,   797,   798,   800,   801,   895,     0,   894,   657,
     661,     0,   806,   659,     0,     0,     0,     0,     0,   458,
     459,     0,   502,     0,   469,   470,   488,     0,     0,     0,
       0,   595,   593,   594,   592,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   727,   710,
     728,   711,     0,     0,     0,     0,   606,   619,    49,    46,
      47,   637,     0,     0,     0,   620,   626,   634,   632,    57,
       0,     0,   890,     0,     0,     0,   493,   506,   510,   513,
     461,   462,   478,   741,   742,   739,   740,   738,   763,   764,
     761,   762,   760,   774,   775,   772,   773,   771,   785,   786,
     783,   784,   782,   752,   753,   750,   751,   749,     0,     0,
     642,   590,   588,   587,   660,   791,   808,     0,   629,     0,
       0,     0,     0,   882,   883,   893,   805,   804,   803,   641,
     640,    48,     0,   630,   624,   631,     0,     0,     0
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1166, -1166, -1166,  2140, -1166, -1166, -1166, -1166, -1166,  -403,
    1359,  1468,  -218,  1142, -1165, -1166, -1166,  -201,  -221, -1166,
    1490, -1166,  -189, -1166,  1500, -1166, -1166, -1166,  1177, -1166,
   -1166,  -999,  1995,  1810,  1987,  -586, -1166, -1166, -1166, -1166,
   -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166, -1166,
   -1166, -1019, -1166,  -961, -1166, -1166,  1145, -1166, -1166, -1166,
    1146, -1166, -1166,   -19,  -656, -1166,  -249, -1105,  -844,  -901,
   -1166,   746,   827, -1166, -1166, -1166,   770, -1166, -1166, -1166,
   -1166,  1021,  -269,  -398,  -840, -1166,  -669, -1166, -1166, -1166,
    -615,  -809, -1166, -1166, -1166, -1166,    -7,  -878, -1166, -1166,
   -1166, -1166,   749, -1166
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,   197,   198,   199,   200,   201,   202,   203,   204,   546,
     879,   547,   548,  1424,   986,  1325,   549,   550,  1044,   205,
     452,   453,   454,   206,   457,   458,   207,   208,   872,   209,
     210,   551,   552,   553,   554,   490,   470,   471,   473,   474,
     211,   212,   874,   978,   476,   477,   479,   480,   482,   483,
     213,  1207,  1240,  1208,  1269,  1270,  1045,  1046,  1047,  1048,
    1049,  1050,  1456,  1180,   555,   556,  1051,  1316,   951,   952,
     953,  1614,  1534,  1535,   954,   955,  1433,  1434,  1052,  1213,
    1146,  1147,  1148,  1053,  1054,  1055,  1056,   959,  1057,  1350,
    1058,  1059,  1150,   962,  1060,  1061,  1228,   963,   964,   965,
    1130,  1547,  1548,   966
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     505,   456,   501,   511,   769,  1361,  1179,  1209,  1184,  1210,
    1189,  1212,  1194,  1331,  1199,   217,   968,   957,  1530,   970,
     957,   215,   972,   957,   214,  1131,   957,   520,  1239,   568,
     976,   569,   867,   570,   957,   571,   538,   854,   558,   697,
     856,  1567,   338,   858,  1661,   341,  1576,   218,   961,   861,
    1135,   961,   243,   977,   961,   245,   450,   961,   451,  1571,
     864,  1581,  1531,  1532,  1004,   961,   557,  1135,  1129,  1135,
    1006,  1135,  1008,  1531,  1532,  1135,   491,   503,  1586,   336,
    1010,  1012,  1067,   484,  1233,  1322,  1069,  1071,  1242,  1074,
     504,  1076,   219,  1363,   602,  1078,  1080,  1082,  1084,   438,
     485,  1086,   459,   620,   622,   624,   626,   628,   630,   632,
    1174,   957,  1088,  1630,  1091,   220,  1631,  1093,  1095,  1162,
    1097,   493,   221,   496,   497,   498,   499,  1106,   957,  1108,
     957,  1110,   957,   885,  1119,  1121,   957,  1123,   680,   682,
     684,   878,   961,   886,  1125,  1127,  1573,  1373,  1129,  1152,
     768,   468,   469,   496,   497,   498,   499,  1374,   503,   961,
     449,   961,   222,   961,  1229,  1310,   461,   961,   506,  1373,
    1175,   504,   496,   497,   498,   499,  1266,  1267,  1268,  1374,
     503,   223,  1346,   455,   224,  1229,   463,  1229,   958,   652,
    1375,   958,  1376,   504,   958,  1181,  1658,   958,   225,  1231,
     343,  1186,  1224,   957,  1317,   958,   226,  1295,   508,   227,
     502,   829,   830,   831,   832,   984,  1273,  1317,   834,   522,
     526,   838,   840,  1224,   655,  1224,  1229,   985,   658,   991,
     993,   661,   664,  1227,   961,   228,   685,  1367,   229,  1368,
     800,   530,   960,  1455,   230,   960,  1624,   231,   960,   532,
    1191,   960,   534,   536,  1227,  1182,  1227,   637,  1196,   960,
     639,  1187,   643,   232,  1224,   957,  1318,   233,   234,   895,
     646,   756,   758,   967,   760,   821,   969,   503,   957,   971,
     995,   512,   958,   997,   465,   235,   999,  1001,  1276,  1282,
     504,  1284,  1286,  1288,  1299,  1227,   961,  1301,  1303,   958,
    1305,   958,  1332,   958,  1334,  1550,   486,   958,   989,   961,
    1192,   803,  1568,  1349,  1373,   992,   472,  1577,  1197,   510,
     514,   236,   237,  1359,  1374,   238,   478,   957,   516,  1365,
    1135,   601,  1582,   957,   619,  1384,   960,  1385,   518,   621,
     623,   339,   625,   627,  1351,   524,   629,   983,   883,  1587,
     631,   244,  1617,   960,   246,   960,   500,   960,   961,  -496,
     521,   960,  1609,  1533,   961,  -495,  1132,   216,   539,   540,
     541,   542,   543,  1225,   958,   543,  1662,   340,  -512,   544,
     543,  -485,   545,   855,  -486,   545,   500,  -491,   342,   859,
     545,   957,  1572,  -504,  1225,   862,  1225,  -501,   539,   540,
     541,   542,   543,   679,  -508,   500,   865,  1005,   492,   544,
    1491,  -889,   545,  1007,   337,  1009,  1607,   539,   540,   541,
     542,   543,   961,  1011,  1013,  1068,  1212,  1226,   960,  1070,
    1072,   545,  1075,  1212,  1077,  1225,   958,   460,  1079,  1081,
    1083,  1085,   695,  1176,  1087,  1177,   543,   681,  1226,   958,
    1226,  1163,  1178,   494,  1164,  1089,   545,  1092,  1135,   956,
    1094,  1096,   956,  1098,  1135,   956,   945,   528,   956,   683,
    1107,  1520,  1109,   833,  1111,   957,   956,  1120,  1122,  1574,
    1124,   988,   539,   540,   541,   542,   543,  1126,  1128,  1226,
     960,  -889,  1153,   450,   837,   451,   545,  1311,   958,   344,
     507,   462,  1681,   960,   958,  1608,   961,  1312,  1555,  1556,
    1557,  1558,   957,   957,   957,   957,   450,  1347,   451,   957,
     948,   464,   653,   559,   654,   957,   503,   695,  1176,   503,
    1183,   543,   561,   695,  1176,   345,  1188,   543,   839,   504,
     509,   545,   504,   961,   961,   961,   961,   545,  1274,  1296,
     961,  1275,   960,   956,   523,   527,   961,   656,   960,   657,
     563,   659,   958,   660,   662,   665,   663,   666,   603,   686,
     956,   687,   956,   801,   956,   802,   531,   878,   956,   450,
     605,   451,   695,  1176,   533,  1193,   543,   535,   537,   607,
     695,  1176,   638,  1198,   543,   640,   545,   644,   539,   540,
     541,   542,   543,  1663,   545,   647,   757,   759,   950,   761,
     822,   950,   545,   513,   950,   996,   960,   950,   998,  1135,
    1317,  1000,  1002,  1277,  1283,   950,  1285,  1287,  1289,  1300,
     609,   611,  1302,  1304,   613,  1306,   615,  1333,   503,  1335,
     990,   540,   541,   804,   805,   956,   958,   990,   540,   541,
     503,   504,   515,   503,  1135,  1135,  1135,  1135,   503,   503,
     517,   503,   503,   504,   456,   503,   504,   984,   617,   503,
     519,   504,   504,   633,   504,   504,   635,   525,   504,   985,
     957,   957,   504,   958,   958,   958,   958,   247,   641,   648,
     958,   650,   667,   669,   671,   673,   958,   249,   251,  1211,
     960,   675,   950,   677,  1319,   688,   690,   956,  1323,   692,
     694,   961,   961,   699,   701,   957,   957,   957,   957,   950,
     956,   950,   503,   950,  1591,   239,   707,   950,   709,   711,
     713,   715,   717,   253,  1369,   504,  1370,   960,   960,   960,
     960,   255,   719,   721,   960,   723,   961,   961,   961,   961,
     960,   725,   727,   729,   731,  1348,   733,   735,   737,   739,
     741,   748,   750,   752,   754,   762,   503,   764,   766,   956,
     770,  1135,  1672,   869,   240,   956,   957,   772,   241,   504,
     774,  1135,   776,   778,  1243,  1135,  1135,  1135,   503,   780,
     242,   315,   503,   495,   950,   782,   784,   786,   788,   529,
     790,   504,   792,   794,   796,   504,   798,   961,   807,   809,
     811,   316,   813,   503,   815,   817,   819,   823,   825,   827,
     841,   843,   845,   847,  1682,   984,   504,  1686,   957,   849,
    1290,   957,   957,   956,  1292,  1320,   317,   985,   257,  1687,
    1688,   318,   957,   957,   957,   319,   957,   957,   957,   320,
     321,   958,   958,   322,   323,   560,   950,   503,  1371,   961,
    1372,  1234,   961,   961,   562,   259,   261,   475,  1297,   950,
     504,   263,  1201,   961,   961,   961,   324,   961,   961,   961,
     496,   497,   498,   499,  1313,  1314,   958,   958,   958,   958,
     265,   267,   564,  1324,  1062,  1064,  1066,   269,  1232,   271,
     604,   273,  1378,   481,  1379,   960,   960,   325,  1241,   275,
     277,   326,   606,   279,  1380,   327,  1381,   956,   950,   328,
    1105,   608,   329,   281,   950,   283,   285,   330,  1117,  1118,
     287,   289,   291,   487,   488,  1337,   489,  1339,   293,  1341,
     960,   960,   960,   960,  1382,   331,  1383,   958,   295,   297,
     299,  1364,   301,   303,   956,   956,   956,   956,  1387,   332,
    1388,   956,   610,   612,   333,   305,   614,   956,   616,   307,
     309,   496,   497,   498,   499,   565,   311,   313,   370,   372,
     374,   376,   950,   425,  1336,   450,   248,   451,  1185,  1389,
    1190,  1390,  1195,   566,  1200,  1592,   250,   252,   567,   958,
     618,   960,   958,   958,   572,   634,  1202,  1203,   636,   573,
     334,  1205,  1206,   958,   958,   958,   574,   958,   958,   958,
     642,   649,   335,   651,   668,   670,   672,   674,   575,  1215,
    1217,  1219,   254,   676,   346,   678,   347,   689,   691,   576,
     256,   693,   695,   348,   577,   700,   702,   349,   350,   870,
     871,   578,   579,   960,  1245,   851,   960,   960,   708,   580,
     710,   712,   714,   716,   718,  1280,   950,   960,   960,   960,
     743,   960,   960,   960,   720,   722,  1391,   724,  1392,  1553,
    1396,   645,  1397,   726,   728,   730,   732,   351,   734,   736,
     738,   740,   742,   749,   751,   753,   755,   763,   352,   765,
     767,   581,   771,   950,   950,   950,   950,   582,   583,   773,
     950,   584,   775,   500,   777,   779,   950,  1338,   450,   585,
     451,   781,   956,   956,  1398,  1610,  1399,   783,   785,   787,
     789,  1400,   791,  1401,   793,   795,   797,   258,   799,   353,
     808,   810,   812,  1345,   814,   354,   816,   818,   820,   824,
     826,   828,   842,   844,   846,   848,   586,   956,   956,   956,
     956,   850,  1291,   355,   260,   262,  1293,  1321,  1202,  1203,
     264,   587,  1373,  1205,  1206,  1235,  1236,  1237,  1238,  1202,
    1203,  1204,  1374,   588,  1205,  1206,  1202,  1203,   356,   266,
     268,  1205,  1206,  1393,   357,  1394,   270,  1405,   272,  1406,
     274,  1415,  1425,  1428,   500,  1202,  1203,  1204,   276,   278,
    1205,  1206,   280,  1436,  1438,  1202,  1203,  1204,   956,   358,
    1205,  1206,   282,  1457,   284,   286,  1446,   359,   360,   288,
     290,   292,  1448,  1450,  1407,  1458,  1408,   294,  1603,  1461,
    1463,  1465,  1467,  1468,  1469,  1470,  1429,   296,   298,   300,
     361,   302,   304,   496,   497,   498,   499,  1472,  1474,  1476,
    1478,  1480,  1482,  1484,   306,   362,   363,   364,   308,   310,
     956,   950,   950,   956,   956,   312,   314,   371,   373,   375,
     377,  1373,   426,   365,   956,   956,   956,   366,   956,   956,
     956,  1374,   367,   368,   744,   745,   746,   747,  1409,   369,
    1410,  1373,  1402,  1551,  1403,  1552,   950,   950,   950,   950,
     378,  1374,  1621,   379,  1622,   380,  1015,  1016,  1017,  1018,
     381,  1020,  1411,  1022,  1412,  1024,  1136,  1137,  1138,  1139,
    1140,  1141,  1142,  1143,  1144,  1546,  1246,  1247,  1248,  1249,
    1250,  1251,  1252,  1253,  1254,  1255,  1256,  1257,  1258,  1259,
    1260,  1261,  1262,  1263,  1264,  1265,  1266,  1267,  1268,   382,
    1340,   450,  1623,   451,   383,  1625,   384,   950,  1264,  1265,
    1266,  1267,  1268,  1145,   385,  1659,   386,   387,   589,   388,
    1330,   590,  1246,  1247,  1248,  1249,  1250,  1251,  1252,  1253,
    1254,  1255,  1256,  1257,  1258,  1259,  1260,  1261,  1262,  1263,
    1264,  1265,  1266,  1267,  1268,   389,   539,   540,   541,   542,
     543,   539,   540,   541,   542,   543,   390,   544,   391,   950,
     545,   392,   950,   950,  1440,   545,   393,   394,   395,   396,
     397,   398,   399,   950,   950,   950,   400,   950,   950,   950,
    1246,  1247,  1248,  1249,  1250,  1251,  1252,  1253,  1254,  1255,
    1256,  1257,  1258,  1259,  1260,  1261,  1262,  1263,  1264,  1265,
    1266,  1267,  1268,  1250,  1251,  1252,  1253,  1254,  1255,  1256,
    1257,  1258,  1259,  1260,  1261,  1262,  1263,  1264,  1265,  1266,
    1267,  1268,  1492,   401,   402,   403,   500,  1260,  1261,  1262,
    1263,  1264,  1265,  1266,  1267,  1268,   404,   405,   539,   540,
     541,   542,   543,   406,   407,   408,   409,   410,   411,   544,
     412,   413,   545,   414,   415,   416,   417,   418,   419,   420,
     421,    -2,   466,   422,  1615,   423,   424,   427,   428,   429,
    1620,   430,   431,   432,   433,   -11,   -11,   -13,     2,     3,
     -16,   -18,   -20,  1253,  1254,  1255,  1256,  1257,  1258,  1259,
    1260,  1261,  1262,  1263,  1264,  1265,  1266,  1267,  1268,     4,
     434,   435,     5,     6,     7,     8,     9,   436,   437,   439,
     440,   441,   442,   443,   444,   445,   446,   447,    10,  1246,
    1247,  1248,  1249,  1250,  1251,  1252,  1253,  1254,  1255,  1256,
    1257,  1258,  1259,  1260,  1261,  1262,  1263,  1264,  1265,  1266,
    1267,  1268,   448,   852,   853,   868,  1615,   857,   884,   860,
     863,   866,   591,   592,  1546,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
     593,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,   594,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   595,   596,   597,   598,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   599,   177,   178,
     600,   703,   704,   705,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   706,   806,   835,   836,
     873,   875,   876,   877,   881,   543,   882,   888,   191,   192,
     193,   887,   893,  1246,  1247,  1248,  1249,  1250,  1251,  1252,
    1253,  1254,  1255,  1256,  1257,  1258,  1259,  1260,  1261,  1262,
    1263,  1264,  1265,  1266,  1267,  1268,   889,   890,   891,   973,
     974,   975,   979,   980,   981,   194,   982,  1073,  1003,   994,
    1090,  1149,  1099,   195,  1100,  1599,  1165,  1151,  1101,  1129,
     196,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  1102,  1103,  1112,  1113,  1114,  1115,  1168,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  1154,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  1172,
    -591,  1155,  1157,  1159,  -591,  1161,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  1167,  1170,  -591,  -591,  -591,  1173,
    1171,  1220,  -591,  -591,  -591,  -591,  -591,  -591,  1129,  1221,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,
    -559,  -559,  1281,  1294,  1298,  1307,  1308,  1309,  1326,  -559,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,  1327,  -559,  -559,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  1328,  -559,
    1329,  1343,  1342,  -559,  1352,  -559,  -559,  -559,  -559,  -559,
    -559,  -559,  -559,  1344,  1353,  -559,  -559,  -559,  1354,  1355,
    1356,  -559,  -559,  -559,  -559,  -559,  -559,  1211,  1357,  -563,
    -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,
    -563,  1360,  1362,  1366,  1377,  1386,  1395,  1404,  -563,  -563,
    -563,  -563,  -563,  -563,  -563,  -563,  1432,  -563,  -563,  -563,
    -563,  -563,  -563,  -563,  -563,  -563,  -563,  1439,  -563,  -662,
    -663,  -674,  -563,  -675,  -563,  -563,  -563,  -563,  -563,  -563,
    -563,  -563,  1441,  1442,  -563,  -563,  -563,  1459,  1485,  1486,
    -563,  -563,  -563,  -563,  -563,  -563,  1211,  1489,  -580,  -580,
    -580,  -580,  -580,  -580,  -580,  -580,  -580,  -580,  -580,  -580,
    1487,  1488,  1521,  1527,  1528,  1490,  1537,  -580,  -580,  -580,
    -580,  -580,  -580,  -580,  -580,  1493,  -580,  -580,  -580,  -580,
    -580,  -580,  -580,  -580,  -580,  -580,  1494,  -580,  1495,  1496,
    1497,  -580,  1498,  -580,  -580,  -580,  -580,  -580,  -580,  -580,
    -580,  1499,  1500,  -580,  -580,  -580,  1501,  1502,  1503,  -580,
    -580,  -580,  -580,  -580,  -580,  1504,  1505,  1506,  1507,  1508,
    1509,  1510,  1511,  -591,  1512,  1513,  1514,  1515,  1516,  1517,
    1518,  1519,  1522,  1523,  1524,  1525,  1526,  1529,  1536,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  1538,  1539,  1540,  1541,  1542,  1543,  -591,  1544,
    1545,  1145,  -591,  1550,  1554,  1559,  -591,  -591,  1560,  -591,
    -591,  -591,  -889,  -591,  1561,  1563,  1564,  1565,  -591,  1569,
    1570,  1575,  -559,  1578,  1579,  1580,  1583,  1584,  1585,  1588,
    1589,  1590,  1593,  1594,  1595,  1597,  1596,  1602,  -559,  -559,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,
    -559,  1598,  1600,  1604,  1605,  1632,  1611,  1616,  1633,  1634,
    1664,  -559,  1635,  1636,  1637,  -559,  -559,  1638,  -559,  -559,
    -559,  -889,  -559,  1639,  1640,  1641,  1642,  -559,  1643,  1644,
    1645,  -563,  1646,  1647,  1648,  1649,  1650,  1651,  1652,  1653,
    1654,  1655,  1656,  1657,  1665,  1666,  1673,  -563,  -563,  -563,
    -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,
    -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,
    1667,  1668,  1671,  1676,  1677,  1678,  1679,  1680,   467,  1169,
    -563,   892,  1358,  1683,  -563,  -563,  1684,  -563,  -563,  -563,
     987,  -563,  1427,   894,   698,   880,  -563,  1430,  1431,  1670,
    -580,   696,  1618,     0,  1549,     0,     0,  1660,     0,     0,
    1675,     0,     0,     0,     0,     0,  -580,  -580,  -580,  -580,
    -580,  -580,  -580,  -580,  -580,  -580,  -580,  -580,  -580,  -580,
    -580,  -580,  -580,  -580,  -580,  -580,  -580,  -580,  -580,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -580,
       0,     0,     0,  -580,  -580,     1,  -580,  -580,  -580,     0,
    -580,     0,     0,     0,     0,  -580,     0,     0,   -11,   -11,
     -13,     2,     3,   -16,   -18,   -20,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     0,     0,     5,     6,     7,     8,     9,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    10,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1246,  1247,  1248,  1249,  1250,  1251,  1252,  1253,  1254,
    1255,  1256,  1257,  1258,  1259,  1260,  1261,  1262,  1263,  1264,
    1265,  1266,  1267,  1268,     0,     0,     0,     0,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,  1601,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,     0,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,     0,
       0,     0,     0,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
       0,   177,   178,     0,     0,     0,     0,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   191,   192,   193,     0,     0,     0,     0,     0,  1246,
    1247,  1248,  1249,  1250,  1251,  1252,  1253,  1254,  1255,  1256,
    1257,  1258,  1259,  1260,  1261,  1262,  1263,  1264,  1265,  1266,
    1267,  1268,     0,     0,     0,     0,     0,  1451,   194,   896,
     897,   898,   899,   900,     0,     0,   195,     0,   905,   906,
     907,  1674,     0,   196,     0,     0,     0,     0,   908,   909,
     910,   911,   912,   913,   914,   915,     0,   916,   917,   918,
     919,   920,   921,   922,   923,   924,   925,     0,   926,     0,
       0,     0,     0,     0,   928,   929,   930,   931,   932,   933,
     934,     0,     0,     0,     0,   937,   938,     0,     0,     0,
     939,   940,   941,   942,   943,   944,  1014,  1015,  1016,  1017,
    1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,  1026,  1027,
    1028,  1029,  1452,  1031,  1685,     0,   896,   897,   898,   899,
     900,   901,   902,   903,   904,   905,   906,   907,     0,     0,
       0,     0,     0,     0,     0,   908,   909,   910,   911,   912,
     913,   914,   915,     0,   916,   917,   918,   919,   920,   921,
     922,   923,   924,   925,     0,   926,     0,     0,     0,   927,
       0,   928,   929,   930,   931,   932,   933,   934,   935,  -623,
    -623,   936,   937,   938,     0,     0,     0,   939,   940,   941,
     942,   943,   944,  1133,     0,   896,   897,   898,   899,   900,
     901,   902,   903,   904,   905,   906,   907,     0,     0,     0,
       0,     0,     0,     0,   908,   909,   910,   911,   912,   913,
     914,   915,     0,   916,   917,   918,   919,   920,   921,   922,
     923,   924,   925,     0,   926,     0,     0,     0,   927,     0,
     928,   929,   930,   931,   932,   933,   934,   935,  -625,  -625,
     936,   937,   938,     0,     0,     0,   939,   940,   941,   942,
     943,   944,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1133,     0,
     896,   897,   898,   899,   900,   901,   902,   903,   904,   905,
     906,   907,     0,     0,     0,     0,     0,     0,     0,   908,
     909,   910,   911,   912,   913,   914,   915,     0,   916,   917,
     918,   919,   920,   921,   922,   923,   924,   925,     0,   926,
       0,   945,  1032,   927,     0,   928,   929,   930,   931,   932,
     933,   934,   935,  -627,  -627,   936,   937,   938,     0,     0,
       0,   939,   940,   941,   942,   943,   944,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1033,     0,     0,     0,
    1034,     0,  1035,  1036,  1037,  1038,     0,  1039,  1040,  1453,
    1454,     0,  1041,   543,   947,   948,     0,     0,     0,     0,
    1104,  1043,     0,   545,     0,     0,     0,     0,  1133,     0,
     896,   897,   898,   899,   900,   901,   902,   903,   904,   905,
     906,   907,     0,     0,     0,     0,     0,     0,   945,   908,
     909,   910,   911,   912,   913,   914,   915,     0,   916,   917,
     918,   919,   920,   921,   922,   923,   924,   925,     0,   926,
       0,     0,     0,   927,     0,   928,   929,   930,   931,   932,
     933,   934,   935,  -621,  -621,   936,   937,   938,     0,     0,
       0,   939,   940,   941,   942,   943,   944,   946,     0,     0,
       0,   947,   948,     0,     0,   949,     0,     0,     0,  -623,
       0,     0,     0,     0,     0,     0,     0,   945,  1247,  1248,
    1249,  1250,  1251,  1252,  1253,  1254,  1255,  1256,  1257,  1258,
    1259,  1260,  1261,  1262,  1263,  1264,  1265,  1266,  1267,  1268,
    1248,  1249,  1250,  1251,  1252,  1253,  1254,  1255,  1256,  1257,
    1258,  1259,  1260,  1261,  1262,  1263,  1264,  1265,  1266,  1267,
    1268,     0,     0,     0,     0,     0,   946,     0,     0,     0,
     947,   948,     0,     0,   949,     0,     0,     0,  -625,     0,
       0,     0,  1133,     0,   896,   897,   898,   899,   900,   901,
     902,   903,   904,   905,   906,   907,     0,     0,     0,     0,
       0,     0,   945,   908,   909,   910,   911,   912,   913,   914,
     915,     0,   916,   917,   918,   919,   920,   921,   922,   923,
     924,   925,     0,   926,     0,     0,     0,   927,     0,   928,
     929,   930,   931,   932,   933,   934,   935,  -628,  -628,   936,
     937,   938,     0,     0,     0,   939,   940,   941,   942,   943,
     944,   946,     0,     0,     0,   947,   948,     0,     0,   949,
       0,     0,     0,  -627,     0,     0,  1133,     0,   896,   897,
     898,   899,   900,   901,   902,   903,   904,   905,   906,   907,
       0,     0,     0,     0,     0,     0,     0,   908,   909,   910,
     911,   912,   913,   914,   915,     0,   916,   917,   918,   919,
     920,   921,   922,   923,   924,   925,     0,   926,     0,     0,
       0,   927,   945,   928,   929,   930,   931,   932,   933,   934,
     935,  -622,  -622,   936,   937,   938,     0,     0,     0,   939,
     940,   941,   942,   943,   944,  1249,  1250,  1251,  1252,  1253,
    1254,  1255,  1256,  1257,  1258,  1259,  1260,  1261,  1262,  1263,
    1264,  1265,  1266,  1267,  1268,     0,     0,     0,     0,     0,
       0,   946,     0,     0,     0,   947,   948,     0,     0,   949,
       0,     0,  1133,  -621,   896,   897,   898,   899,   900,   901,
     902,   903,   904,   905,   906,   907,     0,     0,     0,     0,
       0,     0,     0,   908,   909,   910,   911,   912,   913,   914,
     915,     0,   916,   917,   918,   919,   920,   921,   922,   923,
     924,   925,     0,   926,     0,     0,     0,   927,     0,   928,
     929,   930,   931,   932,   933,   934,   935,     0,     0,   936,
     937,   938,     0,     0,     0,   939,   940,   941,   942,   943,
     944,     0,     0,     0,     0,     0,  1133,     0,   896,   897,
     898,   899,   900,   901,   902,   903,   904,   905,   906,   907,
       0,     0,     0,     0,     0,     0,   945,   908,   909,   910,
     911,   912,   913,   914,   915,     0,   916,   917,   918,   919,
     920,   921,   922,   923,   924,   925,     0,   926,     0,     0,
       0,   927,     0,   928,   929,   930,   931,   932,   933,   934,
     935,     0,     0,   936,   937,   938,     0,     0,     0,   939,
     940,   941,   942,   943,   944,   946,     0,     0,     0,   947,
     948,     0,     0,   949,     0,     0,     0,  -628,     0,     0,
    1133,     0,   896,   897,   898,   899,   900,   901,   902,   903,
     904,   905,   906,   907,     0,     0,     0,     0,     0,     0,
     945,   908,   909,   910,   911,   912,   913,   914,   915,     0,
     916,   917,   918,   919,   920,   921,   922,   923,   924,   925,
       0,   926,     0,     0,     0,   927,     0,   928,   929,   930,
     931,   932,   933,   934,   935,     0,     0,   936,   937,   938,
       0,     0,     0,   939,   940,   941,   942,   943,   944,   946,
       0,     0,     0,   947,   948,     0,     0,   949,     0,     0,
    1133,  -622,   896,   897,   898,   899,   900,   901,   902,   903,
     904,   905,   906,   907,     0,     0,     0,     0,     0,     0,
       0,   908,   909,   910,   911,   912,   913,   914,   915,     0,
     916,   917,   918,   919,   920,   921,   922,   923,   924,   925,
       0,   926,     0,     0,     0,   927,   945,   928,   929,   930,
     931,   932,   933,   934,   935,     0,     0,   936,   937,   938,
       0,     0,     0,   939,   940,   941,   942,   943,   944,  1251,
    1252,  1253,  1254,  1255,  1256,  1257,  1258,  1259,  1260,  1261,
    1262,  1263,  1264,  1265,  1266,  1267,  1268,     0,     0,     0,
       0,     0,     0,     0,     0,   946,     0,     0,     0,   947,
     948,     0,     0,   949,     0,     0,     0,  1134,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     945,  1252,  1253,  1254,  1255,  1256,  1257,  1258,  1259,  1260,
    1261,  1262,  1263,  1264,  1265,  1266,  1267,  1268,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   946,
       0,     0,     0,   947,   948,     0,     0,   949,     0,     0,
       0,  1156,     0,     0,  1133,     0,   896,   897,   898,   899,
     900,   901,   902,   903,   904,   905,   906,   907,     0,     0,
       0,     0,     0,     0,   945,   908,   909,   910,   911,   912,
     913,   914,   915,     0,   916,   917,   918,   919,   920,   921,
     922,   923,   924,   925,     0,   926,     0,     0,     0,   927,
       0,   928,   929,   930,   931,   932,   933,   934,   935,     0,
       0,   936,   937,   938,     0,     0,     0,   939,   940,   941,
     942,   943,   944,   946,     0,     0,     0,   947,   948,     0,
       0,   949,     0,     0,  1133,  1158,   896,   897,   898,   899,
     900,   901,   902,   903,   904,   905,   906,   907,     0,     0,
       0,     0,     0,     0,   945,   908,   909,   910,   911,   912,
     913,   914,   915,     0,   916,   917,   918,   919,   920,   921,
     922,   923,   924,   925,     0,   926,     0,     0,     0,   927,
       0,   928,   929,   930,   931,   932,   933,   934,   935,     0,
       0,   936,   937,   938,     0,     0,     0,   939,   940,   941,
     942,   943,   944,   946,     0,     0,     0,   947,   948,     0,
       0,   949,     0,     0,  1133,  1160,   896,   897,   898,   899,
     900,   901,   902,   903,   904,   905,   906,   907,     0,     0,
       0,     0,     0,     0,     0,   908,   909,   910,   911,   912,
     913,   914,   915,     0,   916,   917,   918,   919,   920,   921,
     922,   923,   924,   925,     0,   926,     0,     0,     0,   927,
       0,   928,   929,   930,   931,   932,   933,   934,   935,     0,
       0,   936,   937,   938,     0,     0,     0,   939,   940,   941,
     942,   943,   944,  1133,     0,   896,   897,   898,   899,   900,
     901,   902,   903,   904,   905,   906,   907,     0,     0,     0,
       0,     0,     0,     0,   908,   909,   910,   911,   912,   913,
     914,   915,     0,   916,   917,   918,   919,   920,   921,   922,
     923,   924,   925,     0,   926,     0,     0,     0,   927,     0,
     928,   929,   930,   931,   932,   933,   934,   935,   945,     0,
     936,   937,   938,     0,     0,     0,   939,   940,   941,   942,
     943,   944,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   946,     0,     0,
       0,   947,   948,     0,     0,   949,     0,     0,  1133,  1166,
     896,   897,   898,   899,   900,   901,   902,   903,   904,   905,
     906,   907,     0,     0,     0,     0,     0,     0,   945,   908,
     909,   910,   911,   912,   913,   914,   915,     0,   916,   917,
     918,   919,   920,   921,   922,   923,   924,   925,     0,   926,
       0,     0,     0,   927,     0,   928,   929,   930,   931,   932,
     933,   934,   935,     0,     0,   936,   937,   938,     0,     0,
       0,   939,   940,   941,   942,   943,   944,   946,     0,     0,
       0,   947,   948,     0,     0,   949,     0,     0,  1133,  1443,
     896,   897,   898,   899,   900,   901,   902,   903,   904,   905,
     906,   907,     0,     0,     0,     0,     0,     0,   945,   908,
     909,   910,   911,   912,   913,   914,   915,     0,   916,   917,
     918,   919,   920,   921,   922,   923,   924,   925,     0,   926,
       0,     0,     0,   927,     0,   928,   929,   930,   931,   932,
     933,   934,   935,     0,     0,   936,   937,   938,     0,     0,
       0,   939,   940,   941,   942,   943,   944,   946,     0,     0,
       0,   947,   948,     0,     0,   949,     0,     0,     0,  1562,
       0,     0,     0,     0,     0,     0,     0,   945,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   946,     0,     0,     0,
     947,   948,     0,     0,   949,     0,     0,  1133,  1566,   896,
     897,   898,   899,   900,   901,   902,   903,   904,   905,   906,
     907,     0,     0,     0,     0,     0,     0,     0,   908,   909,
     910,   911,   912,   913,   914,   915,     0,   916,   917,   918,
     919,   920,   921,   922,   923,   924,   925,     0,   926,     0,
       0,     0,   927,     0,   928,   929,   930,   931,   932,   933,
     934,   935,   945,     0,   936,   937,   938,     0,     0,     0,
     939,   940,   941,   942,   943,   944,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   946,     0,     0,     0,   947,   948,     0,     0,   949,
       0,     0,  1133,  1606,   896,   897,   898,   899,   900,   901,
     902,   903,   904,   905,   906,   907,     0,     0,     0,     0,
       0,     0,   945,   908,   909,   910,   911,   912,   913,   914,
     915,     0,   916,   917,   918,   919,   920,   921,   922,   923,
     924,   925,     0,   926,     0,     0,     0,   927,     0,   928,
     929,   930,   931,   932,   933,   934,   935,     0,     0,   936,
     937,   938,     0,     0,     0,   939,   940,   941,   942,   943,
     944,   946,     0,     0,     0,   947,   948,     0,     0,   949,
       0,     0,  1133,  1626,   896,   897,   898,   899,   900,   901,
     902,   903,   904,   905,   906,   907,     0,     0,     0,     0,
       0,     0,     0,   908,   909,   910,   911,   912,   913,   914,
     915,     0,   916,   917,   918,   919,   920,   921,   922,   923,
     924,   925,     0,   926,     0,     0,     0,   927,     0,   928,
     929,   930,   931,   932,   933,   934,   935,     0,     0,   936,
     937,   938,     0,     0,     0,   939,   940,   941,   942,   943,
     944,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   945,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     946,     0,     0,     0,   947,   948,     0,     0,   949,     0,
       0,  1416,  1627,   896,   897,   898,   899,   900,     0,     0,
       0,     0,   905,   906,   907,     0,     0,     0,     0,     0,
       0,     0,   908,   909,   910,   911,   912,   913,   914,   915,
       0,   916,   917,   918,   919,   920,   921,   922,   923,   924,
     925,     0,   926,     0,     0,     0,     0,     0,   928,   929,
     930,   931,   932,   933,   934,     0,   945,     0,     0,   937,
     938,     0,     0,     0,   939,   940,   941,   942,   943,   944,
    1014,  1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,
    1024,  1025,  1026,  1027,  1028,  1029,  1030,  1031,  1417,  1418,
    1419,  1420,  1421,  1422,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   946,     0,     0,     0,   947,
     948,     0,     0,   949,     0,     0,     0,  1628,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   945,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   946,     0,     0,     0,   947,
     948,     0,     0,   949,     0,     0,     0,  1629,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1426,     0,   896,   897,   898,   899,
     900,     0,     0,     0,     0,   905,   906,   907,     0,     0,
       0,     0,     0,     0,     0,   908,   909,   910,   911,   912,
     913,   914,   915,     0,   916,   917,   918,   919,   920,   921,
     922,   923,   924,   925,     0,   926,     0,     0,     0,     0,
       0,   928,   929,   930,   931,   932,   933,   934,     0,     0,
       0,     0,   937,   938,     0,   945,  1032,   939,   940,   941,
     942,   943,   944,  1014,  1015,  1016,  1017,  1018,  1019,  1020,
    1021,  1022,  1023,  1024,  1025,  1026,  1027,  1028,  1029,  1030,
    1031,  1417,  1418,  1419,  1420,  1421,  1422,     0,     0,     0,
    1033,  1423,     0,     0,  1034,     0,  1035,  1036,  1037,  1038,
       0,  1039,  1040,   504,   946,     0,  1041,     0,   947,   948,
       0,     0,     0,     0,  1104,  1043,  1222,     0,   896,   897,
     898,   899,   900,     0,     0,     0,     0,   905,   906,   907,
       0,     0,     0,     0,     0,     0,     0,   908,   909,   910,
     911,   912,   913,   914,   915,     0,   916,   917,   918,   919,
     920,   921,   922,   923,   924,   925,     0,   926,     0,     0,
       0,     0,     0,   928,   929,   930,   931,   932,   933,   934,
       0,     0,     0,     0,   937,   938,     0,     0,     0,   939,
     940,   941,   942,   943,   944,  1014,  1015,  1016,  1017,  1018,
    1019,  1020,  1021,  1022,  1023,  1024,  1025,  1026,  1027,  1028,
    1029,  1030,  1031,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1222,     0,   896,
     897,   898,   899,   900,     0,     0,     0,     0,   905,   906,
     907,     0,     0,     0,     0,     0,     0,     0,   908,   909,
     910,   911,   912,   913,   914,   915,     0,   916,   917,   918,
     919,   920,   921,   922,   923,   924,   925,     0,   926,     0,
       0,     0,     0,     0,   928,   929,   930,   931,   932,   933,
     934,     0,     0,     0,     0,   937,   938,     0,   945,  1032,
     939,   940,   941,   942,   943,   944,  1014,  1015,  1016,  1017,
    1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,  1026,  1027,
    1028,  1029,  1030,  1031,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1033,  1423,     0,     0,  1034,     0,  1035,
    1036,  1037,  1038,     0,  1039,  1040,   504,   946,     0,  1041,
       0,   947,   948,     0,     0,     0,     0,  1104,  1043,  1222,
       0,   896,   897,   898,   899,   900,     0,     0,     0,     0,
     905,   906,   907,     0,     0,     0,     0,     0,     0,     0,
     908,   909,   910,   911,   912,   913,   914,   915,     0,   916,
     917,   918,   919,   920,   921,   922,   923,   924,   925,     0,
     926,     0,     0,     0,     0,     0,   928,   929,   930,   931,
     932,   933,   934,     0,     0,     0,     0,   937,   938,     0,
     945,  1032,   939,   940,   941,   942,   943,   944,  1014,  1015,
    1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,
    1026,  1027,  1028,  1029,  1030,  1031,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1033,     0,     0,     0,  1034,
       0,  1035,  1036,  1037,  1038,     0,  1039,  1040,   504,   946,
       0,  1041,     0,   947,   948,     0,     0,     0,  1223,  1104,
    1043,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   945,  1032,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1033,     0,     0,     0,
    1034,     0,  1035,  1036,  1037,  1038,     0,  1039,  1040,   504,
     946,     0,  1041,     0,   947,   948,     0,     0,     0,  1271,
    1104,  1043,  1612,     0,   896,   897,   898,   899,   900,     0,
       0,     0,     0,   905,   906,   907,     0,     0,     0,     0,
       0,     0,     0,   908,   909,   910,   911,   912,   913,   914,
     915,     0,   916,   917,   918,   919,   920,   921,   922,   923,
     924,   925,     0,   926,     0,     0,     0,     0,     0,   928,
     929,   930,   931,   932,   933,   934,     0,     0,     0,     0,
     937,   938,     0,   945,  1032,   939,   940,   941,   942,   943,
     944,  1014,  1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,
    1023,  1024,  1025,  1026,  1027,  1028,  1029,  1030,  1031,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1033,     0,
       0,     0,  1034,     0,  1035,  1036,  1037,  1038,     0,  1039,
    1040,   504,   946,     0,  1041,     0,   947,   948,     0,     0,
       0,  1272,  1104,  1043,  1116,     0,   896,   897,   898,   899,
     900,     0,     0,     0,     0,   905,   906,   907,     0,     0,
       0,     0,     0,     0,     0,   908,   909,   910,   911,   912,
     913,   914,   915,     0,   916,   917,   918,   919,   920,   921,
     922,   923,   924,   925,     0,   926,     0,     0,     0,     0,
       0,   928,   929,   930,   931,   932,   933,   934,     0,     0,
       0,     0,   937,   938,     0,     0,     0,   939,   940,   941,
     942,   943,   944,  1014,  1015,  1016,  1017,  1018,  1019,  1020,
    1021,  1022,  1023,  1024,  1025,  1026,  1027,  1028,  1029,  1030,
    1031,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1214,     0,   896,   897,   898,
     899,   900,     0,     0,     0,     0,   905,   906,   907,     0,
       0,     0,     0,     0,     0,     0,   908,   909,   910,   911,
     912,   913,   914,   915,     0,   916,   917,   918,   919,   920,
     921,   922,   923,   924,   925,     0,   926,     0,     0,     0,
       0,     0,   928,   929,   930,   931,   932,   933,   934,     0,
       0,     0,     0,   937,   938,     0,   945,  1032,   939,   940,
     941,   942,   943,   944,  1014,  1015,  1016,  1017,  1018,  1019,
    1020,  1021,  1022,  1023,  1024,  1025,  1026,  1027,  1028,  1029,
    1030,  1031,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1033,     0,  1613,     0,  1034,     0,  1035,  1036,  1037,
    1038,     0,  1039,  1040,   504,   946,     0,  1041,     0,   947,
     948,     0,     0,     0,     0,  1104,  1043,  1216,     0,   896,
     897,   898,   899,   900,     0,     0,     0,     0,   905,   906,
     907,     0,     0,     0,     0,     0,     0,     0,   908,   909,
     910,   911,   912,   913,   914,   915,     0,   916,   917,   918,
     919,   920,   921,   922,   923,   924,   925,     0,   926,     0,
       0,     0,     0,     0,   928,   929,   930,   931,   932,   933,
     934,     0,     0,     0,     0,   937,   938,     0,   945,  1032,
     939,   940,   941,   942,   943,   944,  1014,  1015,  1016,  1017,
    1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,  1026,  1027,
    1028,  1029,  1030,  1031,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1033,     0,     0,     0,  1034,     0,  1035,
    1036,  1037,  1038,     0,  1039,  1040,   504,   946,     0,  1041,
       0,   947,   948,     0,     0,     0,     0,  1104,  1043,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   945,
    1032,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1033,     0,     0,     0,  1034,     0,
    1035,  1036,  1037,  1038,     0,  1039,  1040,   504,   946,     0,
    1041,     0,   947,   948,     0,     0,     0,     0,  1104,  1043,
    1218,     0,   896,   897,   898,   899,   900,     0,     0,     0,
       0,   905,   906,   907,     0,     0,     0,     0,     0,     0,
       0,   908,   909,   910,   911,   912,   913,   914,   915,     0,
     916,   917,   918,   919,   920,   921,   922,   923,   924,   925,
       0,   926,     0,     0,     0,     0,     0,   928,   929,   930,
     931,   932,   933,   934,     0,     0,     0,     0,   937,   938,
       0,   945,  1032,   939,   940,   941,   942,   943,   944,  1014,
    1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,  1024,
    1025,  1026,  1027,  1028,  1029,  1030,  1031,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1033,     0,     0,     0,
    1034,     0,  1035,  1036,  1037,  1038,     0,  1039,  1040,   504,
     946,     0,  1041,     0,   947,   948,     0,     0,     0,     0,
    1104,  1043,  1244,     0,   896,   897,   898,   899,   900,     0,
       0,     0,     0,   905,   906,   907,     0,     0,     0,     0,
       0,     0,     0,   908,   909,   910,   911,   912,   913,   914,
     915,     0,   916,   917,   918,   919,   920,   921,   922,   923,
     924,   925,     0,   926,     0,     0,     0,     0,     0,   928,
     929,   930,   931,   932,   933,   934,     0,     0,     0,     0,
     937,   938,     0,     0,     0,   939,   940,   941,   942,   943,
     944,  1014,  1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,
    1023,  1024,  1025,  1026,  1027,  1028,  1029,  1030,  1031,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1278,     0,   896,   897,   898,   899,   900,
       0,     0,     0,     0,   905,   906,   907,     0,     0,     0,
       0,     0,     0,     0,   908,   909,   910,   911,   912,   913,
     914,   915,     0,   916,   917,   918,   919,   920,   921,   922,
     923,   924,   925,     0,   926,     0,     0,     0,     0,     0,
     928,   929,   930,   931,   932,   933,   934,     0,     0,     0,
       0,   937,   938,     0,   945,  1032,   939,   940,   941,   942,
     943,   944,  1014,  1015,  1016,  1017,  1018,  1019,  1020,  1021,
    1022,  1023,  1024,  1025,  1026,  1027,  1028,  1029,  1030,  1031,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1033,
       0,     0,     0,  1034,     0,  1035,  1036,  1037,  1038,     0,
    1039,  1040,   504,   946,     0,  1041,     0,   947,   948,     0,
       0,     0,     0,  1104,  1043,  1222,     0,   896,   897,   898,
     899,   900,     0,     0,     0,     0,   905,   906,   907,     0,
       0,     0,     0,     0,     0,     0,   908,   909,   910,   911,
     912,   913,   914,   915,     0,   916,   917,   918,   919,   920,
     921,   922,   923,   924,   925,     0,   926,     0,     0,     0,
       0,     0,   928,   929,   930,   931,   932,   933,   934,     0,
       0,     0,     0,   937,   938,     0,   945,  1032,   939,   940,
     941,   942,   943,   944,  1014,  1015,  1016,  1017,  1018,  1019,
    1020,  1021,  1022,  1023,  1024,  1025,  1026,  1027,  1028,  1029,
    1030,  1031,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1033,     0,     0,     0,  1034,     0,  1035,  1036,  1037,
    1038,     0,  1039,  1040,   504,   946,     0,  1041,     0,   947,
     948,     0,     0,     0,     0,  1104,  1043,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   945,  1032,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1033,     0,     0,     0,  1034,     0,  1035,  1036,
    1037,  1038,     0,  1039,  1040,   504,  1279,     0,  1041,     0,
     947,   948,     0,     0,     0,     0,  1104,  1043,  1413,     0,
     896,   897,   898,   899,   900,     0,     0,     0,     0,   905,
     906,   907,     0,     0,     0,     0,     0,     0,     0,   908,
     909,   910,   911,   912,   913,   914,   915,     0,   916,   917,
     918,   919,   920,   921,   922,   923,   924,   925,     0,   926,
       0,     0,     0,     0,     0,   928,   929,   930,   931,   932,
     933,   934,     0,     0,     0,     0,   937,   938,     0,   945,
    1032,   939,   940,   941,   942,   943,   944,  1014,  1015,  1016,
    1017,  1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,  1026,
    1027,  1028,  1029,  1030,  1031,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1033,     0,     0,     0,  1034,     0,
    1035,  1036,  1037,  1038,     0,  1039,  1040,   504,   946,     0,
    1041,     0,   947,   948,     0,     0,     0,     0,  1104,  1043,
    1435,     0,   896,   897,   898,   899,   900,     0,     0,     0,
       0,   905,   906,   907,     0,     0,     0,     0,     0,     0,
       0,   908,   909,   910,   911,   912,   913,   914,   915,     0,
     916,   917,   918,   919,   920,   921,   922,   923,   924,   925,
       0,   926,     0,     0,     0,     0,     0,   928,   929,   930,
     931,   932,   933,   934,     0,     0,     0,     0,   937,   938,
       0,     0,     0,   939,   940,   941,   942,   943,   944,  1014,
    1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,  1024,
    1025,  1026,  1027,  1028,  1029,  1030,  1031,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1437,     0,   896,   897,   898,   899,   900,     0,     0,
       0,     0,   905,   906,   907,     0,     0,     0,     0,     0,
       0,     0,   908,   909,   910,   911,   912,   913,   914,   915,
       0,   916,   917,   918,   919,   920,   921,   922,   923,   924,
     925,     0,   926,     0,     0,     0,     0,     0,   928,   929,
     930,   931,   932,   933,   934,     0,     0,     0,     0,   937,
     938,     0,   945,  1032,   939,   940,   941,   942,   943,   944,
    1014,  1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,
    1024,  1025,  1026,  1027,  1028,  1029,  1030,  1031,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1033,     0,     0,
       0,  1034,     0,  1035,  1036,  1037,  1038,     0,  1039,  1040,
     504,  1414,     0,  1041,     0,   947,   948,     0,     0,     0,
       0,  1104,  1043,  1444,     0,   896,   897,   898,   899,   900,
       0,     0,     0,     0,   905,   906,   907,     0,     0,     0,
       0,     0,     0,     0,   908,   909,   910,   911,   912,   913,
     914,   915,     0,   916,   917,   918,   919,   920,   921,   922,
     923,   924,   925,     0,   926,     0,     0,     0,     0,     0,
     928,   929,   930,   931,   932,   933,   934,     0,     0,     0,
       0,   937,   938,     0,   945,  1032,   939,   940,   941,   942,
     943,   944,  1014,  1015,  1016,  1017,  1018,  1019,  1020,  1021,
    1022,  1023,  1024,  1025,  1026,  1027,  1028,  1029,  1445,  1031,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1033,
       0,     0,     0,  1034,     0,  1035,  1036,  1037,  1038,     0,
    1039,  1040,   504,   946,     0,  1041,     0,   947,   948,     0,
       0,     0,     0,  1104,  1043,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   945,  1032,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1033,     0,     0,     0,  1034,     0,  1035,  1036,  1037,  1038,
       0,  1039,  1040,   504,   946,     0,  1041,     0,   947,   948,
       0,     0,     0,     0,  1104,  1043,  1447,     0,   896,   897,
     898,   899,   900,     0,     0,     0,     0,   905,   906,   907,
       0,     0,     0,     0,     0,     0,     0,   908,   909,   910,
     911,   912,   913,   914,   915,     0,   916,   917,   918,   919,
     920,   921,   922,   923,   924,   925,     0,   926,     0,     0,
       0,     0,     0,   928,   929,   930,   931,   932,   933,   934,
       0,     0,     0,     0,   937,   938,     0,   945,  1032,   939,
     940,   941,   942,   943,   944,  1014,  1015,  1016,  1017,  1018,
    1019,  1020,  1021,  1022,  1023,  1024,  1025,  1026,  1027,  1028,
    1029,  1030,  1031,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1033,     0,     0,     0,  1034,     0,  1035,  1036,
    1037,  1038,     0,  1039,  1040,   504,   946,     0,  1041,     0,
     947,   948,     0,     0,     0,     0,  1104,  1043,  1449,     0,
     896,   897,   898,   899,   900,     0,     0,     0,     0,   905,
     906,   907,     0,     0,     0,     0,     0,     0,     0,   908,
     909,   910,   911,   912,   913,   914,   915,     0,   916,   917,
     918,   919,   920,   921,   922,   923,   924,   925,     0,   926,
       0,     0,     0,     0,     0,   928,   929,   930,   931,   932,
     933,   934,     0,     0,     0,     0,   937,   938,     0,     0,
       0,   939,   940,   941,   942,   943,   944,  1014,  1015,  1016,
    1017,  1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,  1026,
    1027,  1028,  1029,  1030,  1031,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1460,
       0,   896,   897,   898,   899,   900,     0,     0,     0,     0,
     905,   906,   907,     0,     0,     0,     0,     0,     0,     0,
     908,   909,   910,   911,   912,   913,   914,   915,     0,   916,
     917,   918,   919,   920,   921,   922,   923,   924,   925,     0,
     926,     0,     0,     0,     0,     0,   928,   929,   930,   931,
     932,   933,   934,     0,     0,     0,     0,   937,   938,     0,
     945,  1032,   939,   940,   941,   942,   943,   944,  1014,  1015,
    1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,
    1026,  1027,  1028,  1029,  1030,  1031,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1033,     0,     0,     0,  1034,
       0,  1035,  1036,  1037,  1038,     0,  1039,  1040,   504,   946,
       0,  1041,     0,   947,   948,     0,     0,     0,     0,  1104,
    1043,  1462,     0,   896,   897,   898,   899,   900,     0,     0,
       0,     0,   905,   906,   907,     0,     0,     0,     0,     0,
       0,     0,   908,   909,   910,   911,   912,   913,   914,   915,
       0,   916,   917,   918,   919,   920,   921,   922,   923,   924,
     925,     0,   926,     0,     0,     0,     0,     0,   928,   929,
     930,   931,   932,   933,   934,     0,     0,     0,     0,   937,
     938,     0,   945,  1032,   939,   940,   941,   942,   943,   944,
    1014,  1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,
    1024,  1025,  1026,  1027,  1028,  1029,  1030,  1031,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1033,     0,     0,
       0,  1034,     0,  1035,  1036,  1037,  1038,     0,  1039,  1040,
     504,   946,     0,  1041,     0,   947,   948,     0,     0,     0,
       0,  1104,  1043,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   945,  1032,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1033,     0,
       0,     0,  1034,     0,  1035,  1036,  1037,  1038,     0,  1039,
    1040,   504,   946,     0,  1041,     0,   947,   948,     0,     0,
       0,     0,  1104,  1043,  1464,     0,   896,   897,   898,   899,
     900,     0,     0,     0,     0,   905,   906,   907,     0,     0,
       0,     0,     0,     0,     0,   908,   909,   910,   911,   912,
     913,   914,   915,     0,   916,   917,   918,   919,   920,   921,
     922,   923,   924,   925,     0,   926,     0,     0,     0,     0,
       0,   928,   929,   930,   931,   932,   933,   934,     0,     0,
       0,     0,   937,   938,     0,   945,  1032,   939,   940,   941,
     942,   943,   944,  1014,  1015,  1016,  1017,  1018,  1019,  1020,
    1021,  1022,  1023,  1024,  1025,  1026,  1027,  1028,  1029,  1030,
    1031,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1033,     0,     0,     0,  1034,     0,  1035,  1036,  1037,  1038,
       0,  1039,  1040,   504,   946,     0,  1041,     0,   947,   948,
       0,     0,     0,     0,  1104,  1043,  1466,     0,   896,   897,
     898,   899,   900,     0,     0,     0,     0,   905,   906,   907,
       0,     0,     0,     0,     0,     0,     0,   908,   909,   910,
     911,   912,   913,   914,   915,     0,   916,   917,   918,   919,
     920,   921,   922,   923,   924,   925,     0,   926,     0,     0,
       0,     0,     0,   928,   929,   930,   931,   932,   933,   934,
       0,     0,     0,     0,   937,   938,     0,     0,     0,   939,
     940,   941,   942,   943,   944,  1014,  1015,  1016,  1017,  1018,
    1019,  1020,  1021,  1022,  1023,  1024,  1025,  1026,  1027,  1028,
    1029,  1030,  1031,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1471,     0,   896,
     897,   898,   899,   900,     0,     0,     0,     0,   905,   906,
     907,     0,     0,     0,     0,     0,     0,     0,   908,   909,
     910,   911,   912,   913,   914,   915,     0,   916,   917,   918,
     919,   920,   921,   922,   923,   924,   925,     0,   926,     0,
       0,     0,     0,     0,   928,   929,   930,   931,   932,   933,
     934,     0,     0,     0,     0,   937,   938,     0,   945,  1032,
     939,   940,   941,   942,   943,   944,  1014,  1015,  1016,  1017,
    1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,  1026,  1027,
    1028,  1029,  1030,  1031,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1033,     0,     0,     0,  1034,     0,  1035,
    1036,  1037,  1038,     0,  1039,  1040,   504,   946,     0,  1041,
       0,   947,   948,     0,     0,     0,     0,  1104,  1043,  1473,
       0,   896,   897,   898,   899,   900,     0,     0,     0,     0,
     905,   906,   907,     0,     0,     0,     0,     0,     0,     0,
     908,   909,   910,   911,   912,   913,   914,   915,     0,   916,
     917,   918,   919,   920,   921,   922,   923,   924,   925,     0,
     926,     0,     0,     0,     0,     0,   928,   929,   930,   931,
     932,   933,   934,     0,     0,     0,     0,   937,   938,     0,
     945,  1032,   939,   940,   941,   942,   943,   944,  1014,  1015,
    1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,
    1026,  1027,  1028,  1029,  1030,  1031,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1033,     0,     0,     0,  1034,
       0,  1035,  1036,  1037,  1038,     0,  1039,  1040,   504,   946,
       0,  1041,     0,   947,   948,     0,     0,     0,     0,  1104,
    1043,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   945,  1032,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1033,     0,     0,     0,
    1034,     0,  1035,  1036,  1037,  1038,     0,  1039,  1040,   504,
     946,     0,  1041,     0,   947,   948,     0,     0,     0,     0,
    1104,  1043,  1475,     0,   896,   897,   898,   899,   900,     0,
       0,     0,     0,   905,   906,   907,     0,     0,     0,     0,
       0,     0,     0,   908,   909,   910,   911,   912,   913,   914,
     915,     0,   916,   917,   918,   919,   920,   921,   922,   923,
     924,   925,     0,   926,     0,     0,     0,     0,     0,   928,
     929,   930,   931,   932,   933,   934,     0,     0,     0,     0,
     937,   938,     0,   945,  1032,   939,   940,   941,   942,   943,
     944,  1014,  1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,
    1023,  1024,  1025,  1026,  1027,  1028,  1029,  1030,  1031,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1033,     0,
       0,     0,  1034,     0,  1035,  1036,  1037,  1038,     0,  1039,
    1040,   504,   946,     0,  1041,     0,   947,   948,     0,     0,
       0,     0,  1104,  1043,  1477,     0,   896,   897,   898,   899,
     900,     0,     0,     0,     0,   905,   906,   907,     0,     0,
       0,     0,     0,     0,     0,   908,   909,   910,   911,   912,
     913,   914,   915,     0,   916,   917,   918,   919,   920,   921,
     922,   923,   924,   925,     0,   926,     0,     0,     0,     0,
       0,   928,   929,   930,   931,   932,   933,   934,     0,     0,
       0,     0,   937,   938,     0,     0,     0,   939,   940,   941,
     942,   943,   944,  1014,  1015,  1016,  1017,  1018,  1019,  1020,
    1021,  1022,  1023,  1024,  1025,  1026,  1027,  1028,  1029,  1030,
    1031,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1479,     0,   896,   897,   898,
     899,   900,     0,     0,     0,     0,   905,   906,   907,     0,
       0,     0,     0,     0,     0,     0,   908,   909,   910,   911,
     912,   913,   914,   915,     0,   916,   917,   918,   919,   920,
     921,   922,   923,   924,   925,     0,   926,     0,     0,     0,
       0,     0,   928,   929,   930,   931,   932,   933,   934,     0,
       0,     0,     0,   937,   938,     0,   945,  1032,   939,   940,
     941,   942,   943,   944,  1014,  1015,  1016,  1017,  1018,  1019,
    1020,  1021,  1022,  1023,  1024,  1025,  1026,  1027,  1028,  1029,
    1030,  1031,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1033,     0,     0,     0,  1034,     0,  1035,  1036,  1037,
    1038,     0,  1039,  1040,   504,   946,     0,  1041,     0,   947,
     948,     0,     0,     0,     0,  1104,  1043,  1481,     0,   896,
     897,   898,   899,   900,     0,     0,     0,     0,   905,   906,
     907,     0,     0,     0,     0,     0,     0,     0,   908,   909,
     910,   911,   912,   913,   914,   915,     0,   916,   917,   918,
     919,   920,   921,   922,   923,   924,   925,     0,   926,     0,
       0,     0,     0,     0,   928,   929,   930,   931,   932,   933,
     934,     0,     0,     0,     0,   937,   938,     0,   945,  1032,
     939,   940,   941,   942,   943,   944,  1014,  1015,  1016,  1017,
    1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,  1026,  1027,
    1028,  1029,  1030,  1031,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1033,     0,     0,     0,  1034,     0,  1035,
    1036,  1037,  1038,     0,  1039,  1040,   504,   946,     0,  1041,
       0,   947,   948,     0,     0,     0,     0,  1104,  1043,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   945,
    1032,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1033,     0,     0,     0,  1034,     0,
    1035,  1036,  1037,  1038,     0,  1039,  1040,   504,   946,     0,
    1041,     0,   947,   948,     0,     0,     0,     0,  1104,  1043,
    1483,     0,   896,   897,   898,   899,   900,     0,     0,     0,
       0,   905,   906,   907,     0,     0,     0,     0,     0,     0,
       0,   908,   909,   910,   911,   912,   913,   914,   915,     0,
     916,   917,   918,   919,   920,   921,   922,   923,   924,   925,
       0,   926,     0,     0,     0,     0,     0,   928,   929,   930,
     931,   932,   933,   934,     0,     0,     0,     0,   937,   938,
       0,   945,  1032,   939,   940,   941,   942,   943,   944,  1014,
    1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,  1024,
    1025,  1026,  1027,  1028,  1029,  1030,  1031,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1033,     0,     0,     0,
    1034,     0,  1035,  1036,  1037,  1038,     0,  1039,  1040,   504,
     946,     0,  1041,     0,   947,   948,     0,     0,     0,     0,
    1104,  1043,  1669,     0,   896,   897,   898,   899,   900,     0,
       0,     0,     0,   905,   906,   907,     0,     0,     0,     0,
       0,     0,     0,   908,   909,   910,   911,   912,   913,   914,
     915,     0,   916,   917,   918,   919,   920,   921,   922,   923,
     924,   925,     0,   926,     0,     0,     0,     0,     0,   928,
     929,   930,   931,   932,   933,   934,     0,     0,     0,     0,
     937,   938,     0,     0,     0,   939,   940,   941,   942,   943,
     944,  1014,  1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,
    1023,  1024,  1025,  1026,  1027,  1028,  1029,  1030,  1031,   896,
     897,   898,   899,   900,   901,   902,   903,   904,   905,   906,
     907,     0,     0,     0,     0,     0,     0,     0,   908,   909,
     910,   911,   912,   913,   914,   915,     0,   916,   917,   918,
     919,   920,   921,   922,   923,   924,   925,     0,   926,     0,
       0,     0,   927,     0,   928,   929,   930,   931,   932,   933,
     934,   935,     0,     0,   936,   937,   938,     0,     0,     0,
     939,   940,   941,   942,   943,   944,     0,     0,     0,     0,
       0,     0,     0,     0,   945,  1032,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1033,
       0,     0,     0,  1034,     0,  1035,  1036,  1037,  1038,     0,
    1039,  1040,   504,   946,     0,  1041,     0,   947,   948,     0,
       0,     0,     0,  1104,  1043,     0,     0,   896,   897,   898,
     899,   900,     0,     0,     0,     0,   905,   906,   907,     0,
       0,     0,     0,     0,     0,     0,   908,   909,   910,   911,
     912,   913,   914,   915,     0,   916,   917,   918,   919,   920,
     921,   922,   923,   924,   925,     0,   926,     0,     0,     0,
       0,     0,   928,   929,   930,   931,   932,   933,   934,     0,
       0,     0,     0,   937,   938,     0,   945,  1032,   939,   940,
     941,   942,   943,   944,  1014,  1015,  1016,  1017,  1018,  1019,
    1020,  1021,  1022,  1023,  1024,  1025,  1026,  1027,  1028,  1029,
    1030,  1031,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1033,     0,     0,     0,  1034,     0,  1035,  1036,  1037,
    1038,     0,  1039,  1040,   504,   946,     0,  1041,     0,   947,
     948,     0,     0,     0,     0,  1104,  1043,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   945,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1246,  1247,  1248,
    1249,  1250,  1251,  1252,  1253,  1254,  1255,  1256,  1257,  1258,
    1259,  1260,  1261,  1262,  1263,  1264,  1265,  1266,  1267,  1268,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     946,     0,     0,     0,   947,   948,     0,     0,   949,     0,
       0,  1315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     896,   897,   898,   899,   900,     0,     0,     0,     0,   905,
     906,   907,     0,     0,     0,     0,     0,     0,     0,   908,
     909,   910,   911,   912,   913,   914,   915,     0,   916,   917,
     918,   919,   920,   921,   922,   923,   924,   925,     0,   926,
       0,     0,     0,     0,     0,   928,   929,   930,   931,   932,
     933,   934,     0,     0,     0,     0,   937,   938,     0,   945,
    1032,   939,   940,   941,   942,   943,   944,  1014,  1015,  1016,
    1017,  1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,  1026,
    1027,  1028,  1029,  1030,  1031,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1033,     0,     0,     0,  1034,     0,
    1035,  1036,  1037,  1038,     0,  1039,  1040,   504,   946,     0,
    1041,     0,   947,   948,     0,     0,     0,     0,  1042,  1043,
     896,   897,   898,   899,   900,     0,     0,     0,     0,   905,
     906,   907,     0,     0,     0,     0,     0,     0,     0,   908,
     909,   910,   911,   912,   913,   914,   915,     0,   916,   917,
     918,   919,   920,   921,   922,   923,   924,   925,     0,   926,
       0,     0,     0,     0,     0,   928,   929,   930,   931,   932,
     933,   934,     0,     0,     0,     0,   937,   938,     0,     0,
       0,   939,   940,   941,   942,   943,   944,  1014,  1015,  1016,
    1017,  1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,  1026,
    1027,  1028,  1029,  1030,  1031,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   896,   897,   898,   899,   900,     0,     0,
       0,     0,   905,   906,   907,     0,     0,     0,     0,     0,
       0,     0,   908,   909,   910,   911,   912,   913,   914,   915,
       0,   916,   917,   918,   919,   920,   921,   922,   923,   924,
     925,     0,   926,     0,     0,     0,     0,     0,   928,   929,
     930,   931,   932,   933,   934,     0,     0,     0,     0,   937,
     938,     0,   945,  1032,   939,   940,   941,   942,   943,   944,
    1014,  1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,
    1024,  1025,  1026,  1027,  1028,  1029,  1030,  1031,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1033,     0,     0,
       0,  1034,     0,  1035,  1036,  1037,  1038,     0,  1039,  1040,
     504,   946,     0,  1041,     0,   947,   948,     0,     0,     0,
       0,  1063,  1043,   896,   897,   898,   899,   900,     0,     0,
       0,     0,   905,   906,   907,     0,     0,     0,     0,     0,
       0,     0,   908,   909,   910,   911,   912,   913,   914,   915,
       0,   916,   917,   918,   919,   920,   921,   922,   923,   924,
     925,     0,   926,     0,     0,     0,     0,     0,   928,   929,
     930,   931,   932,   933,   934,     0,     0,     0,     0,   937,
     938,     0,   945,  1032,   939,   940,   941,   942,   943,   944,
    1014,  1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,
    1024,  1025,  1026,  1027,  1028,  1029,  1030,  1031,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1033,     0,     0,
       0,  1034,     0,  1035,  1036,  1037,  1038,     0,  1039,  1040,
     504,   946,     0,  1041,     0,   947,   948,     0,     0,     0,
       0,  1065,  1043,     0,   896,   897,   898,   899,   900,   901,
     902,   903,   904,   905,   906,   907,     0,     0,     0,     0,
       0,     0,     0,   908,   909,   910,   911,   912,   913,   914,
     915,     0,   916,   917,   918,   919,   920,   921,   922,   923,
     924,   925,     0,   926,     0,   945,  1032,   927,     0,   928,
     929,   930,   931,   932,   933,   934,   935,     0,     0,   936,
     937,   938,     0,     0,     0,   939,   940,   941,   942,   943,
     944,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1033,     0,     0,     0,  1034,     0,  1035,  1036,  1037,  1038,
       0,  1039,  1040,   504,   946,     0,  1041,     0,   947,   948,
       0,     0,     0,     0,  1104,  1043,     0,     0,     0,     0,
    1230,     0,   896,   897,   898,   899,   900,   901,   902,   903,
     904,   905,   906,   907,     0,     0,     0,     0,     0,     0,
       0,   908,   909,   910,   911,   912,   913,   914,   915,     0,
     916,   917,   918,   919,   920,   921,   922,   923,   924,   925,
       0,   926,     0,     0,     0,   927,     0,   928,   929,   930,
     931,   932,   933,   934,   935,   945,  1032,   936,   937,   938,
       0,     0,     0,   939,   940,   941,   942,   943,   944,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1033,     0,     0,     0,  1034,     0,  1035,  1036,  1037,  1038,
       0,  1039,  1040,  1619,   946,     0,  1041,     0,   947,   948,
       0,     0,     0,     0,  1104,  1043,   896,   897,   898,   899,
     900,   901,   902,   903,   904,   905,   906,   907,     0,     0,
       0,     0,     0,     0,     0,   908,   909,   910,   911,   912,
     913,   914,   915,     0,   916,   917,   918,   919,   920,   921,
     922,   923,   924,   925,     0,   926,     0,     0,     0,   927,
       0,   928,   929,   930,   931,   932,   933,   934,   935,     0,
       0,   936,   937,   938,     0,     0,   945,   939,   940,   941,
     942,   943,   944,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   946,     0,     0,     0,   947,
     948,     0,     0,   949,     0,     0,  1315,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   945,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   946,     0,     0,     0,   947,   948,     0,
       0,   949,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   945,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   946,     0,     0,
       0,   947,   948,     0,     0,   949
};

static const yytype_int16 yycheck[] =
{
     221,   190,   220,   224,   407,  1170,  1005,  1026,  1007,  1028,
    1009,  1030,  1011,  1118,  1013,     1,   860,   857,     1,   863,
     860,     1,   866,   863,   300,     1,   866,     1,  1047,   250,
     874,   252,     1,   254,   874,   256,     1,     1,   239,     1,
       1,    62,     1,     1,     1,     1,    62,   300,   857,     1,
     951,   860,     1,     1,   863,     1,   334,   866,   336,     1,
       1,    62,    56,    57,     1,   874,     1,   968,     1,   970,
       1,   972,     1,    56,    57,   976,     1,   320,    62,     1,
       1,     1,     1,   330,  1045,     1,     1,     1,  1049,     1,
     333,     1,   300,   336,   315,     1,     1,     1,     1,   330,
     347,     1,     1,   324,   325,   326,   327,   328,   329,   330,
       1,   951,     1,   333,     1,   300,   336,     1,     1,     1,
       1,     1,   300,    88,    89,    90,    91,     1,   968,     1,
     970,     1,   972,   320,     1,     1,   976,     1,   359,   360,
     361,   544,   951,   330,     1,     1,     1,   320,     1,     1,
       1,    14,    15,    88,    89,    90,    91,   330,   320,   968,
       1,   970,   300,   972,  1042,     1,     1,   976,     1,   320,
      61,   333,    88,    89,    90,    91,   321,   322,   323,   330,
     320,   300,     1,     1,   300,  1063,     1,  1065,   857,     1,
     341,   860,   343,   333,   863,     1,   336,   866,   300,  1043,
       1,     1,  1042,  1043,  1105,   874,   300,     1,     1,   300,
       1,   269,   270,   271,   272,   321,     1,  1118,   439,     1,
       1,   442,   443,  1063,     1,  1065,  1104,   333,     1,   885,
     886,     1,     1,  1042,  1043,   300,     1,   341,   300,   343,
       1,     1,   857,  1242,   300,   860,     1,   300,   863,     1,
       1,   866,     1,     1,  1063,    61,  1065,     1,     1,   874,
       1,    61,     1,   300,  1104,  1105,     1,   300,   300,   855,
       1,     1,     1,   859,     1,     1,   862,   320,  1118,   865,
       1,     1,   951,     1,     0,   300,     1,     1,     1,     1,
     333,     1,     1,     1,     1,  1104,  1105,     1,     1,   968,
       1,   970,     1,   972,     1,   348,   345,   976,     1,  1118,
      61,     1,   333,  1153,   320,     1,    16,   333,    61,     1,
       1,   300,   300,  1167,   330,   300,    20,  1167,     1,  1173,
    1231,     1,   333,  1173,     1,   341,   951,   343,     1,     1,
       1,   300,     1,     1,  1153,     1,     1,     1,   549,   333,
       1,   300,   346,   968,   300,   970,   321,   972,  1167,   345,
     334,   976,  1527,   346,  1173,   345,   342,   347,   333,   334,
     335,   336,   337,  1042,  1043,   337,   333,   336,   347,   344,
     337,   345,   347,   347,   345,   347,   321,   345,   344,   347,
     347,  1231,   334,   345,  1063,   347,  1065,   345,   333,   334,
     335,   336,   337,     1,   345,   321,   347,   344,   333,   344,
     343,   344,   347,   344,   336,   344,  1521,   333,   334,   335,
     336,   337,  1231,   344,   344,   344,  1445,  1042,  1043,   344,
     344,   347,   344,  1452,   344,  1104,  1105,   336,   344,   344,
     344,   344,   333,   334,   344,   336,   337,     1,  1063,  1118,
    1065,   333,   343,   333,   336,   344,   347,   344,  1359,   857,
     344,   344,   860,   344,  1365,   863,   285,     1,   866,     1,
     344,  1315,   344,     1,   344,  1315,   874,   344,   344,   334,
     344,   884,   333,   334,   335,   336,   337,   344,   344,  1104,
    1105,   344,   344,   334,     1,   336,   347,   333,  1167,   300,
     333,   336,  1667,  1118,  1173,     1,  1315,   343,  1352,  1353,
    1354,  1355,  1352,  1353,  1354,  1355,   334,   336,   336,  1359,
     339,   336,   334,     1,   336,  1365,   320,   333,   334,   320,
     336,   337,     1,   333,   334,   336,   336,   337,     1,   333,
     333,   347,   333,  1352,  1353,  1354,  1355,   347,   333,   343,
    1359,   336,  1167,   951,   336,   336,  1365,   334,  1173,   336,
       1,   334,  1231,   336,   334,   334,   336,   336,     1,   334,
     968,   336,   970,   334,   972,   336,   336,   980,   976,   334,
       1,   336,   333,   334,   336,   336,   337,   336,   336,     1,
     333,   334,   336,   336,   337,   336,   347,   336,   333,   334,
     335,   336,   337,  1602,   347,   336,   336,   336,   857,   336,
     336,   860,   347,   333,   863,   336,  1231,   866,   336,  1520,
    1521,   336,   336,   336,   336,   874,   336,   336,   336,   336,
       1,     1,   336,   336,     1,   336,     1,   336,   320,   336,
     333,   334,   335,   333,   334,  1043,  1315,   333,   334,   335,
     320,   333,   333,   320,  1555,  1556,  1557,  1558,   320,   320,
     333,   320,   320,   333,   853,   320,   333,   321,     1,   320,
     333,   333,   333,     1,   333,   333,     1,   333,   333,   333,
    1520,  1521,   333,  1352,  1353,  1354,  1355,     1,     1,     1,
    1359,     1,     1,     1,     1,     1,  1365,     1,     1,     1,
    1315,     1,   951,     1,  1107,     1,     1,  1105,  1111,     1,
       1,  1520,  1521,     1,     1,  1555,  1556,  1557,  1558,   968,
    1118,   970,   320,   972,    62,   300,     1,   976,     1,     1,
       1,     1,     1,     1,   341,   333,   343,  1352,  1353,  1354,
    1355,     1,     1,     1,  1359,     1,  1555,  1556,  1557,  1558,
    1365,     1,     1,     1,     1,  1153,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,   320,     1,     1,  1167,
       1,  1672,  1616,    57,   300,  1173,  1616,     1,   300,   333,
       1,  1682,     1,     1,  1053,  1686,  1687,  1688,   320,     1,
     300,   300,   320,     1,  1043,     1,     1,     1,     1,   333,
       1,   333,     1,     1,     1,   333,     1,  1616,     1,     1,
       1,   300,     1,   320,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,  1668,   321,   333,  1671,  1668,     1,
       1,  1671,  1672,  1231,     1,     1,   300,   333,     1,  1683,
    1684,   300,  1682,  1683,  1684,   300,  1686,  1687,  1688,   300,
     300,  1520,  1521,   300,   300,   333,  1105,   320,   341,  1668,
     343,     1,  1671,  1672,   333,     1,     1,    19,  1089,  1118,
     333,     1,     1,  1682,  1683,  1684,   300,  1686,  1687,  1688,
      88,    89,    90,    91,     1,  1103,  1555,  1556,  1557,  1558,
       1,     1,   333,  1111,   901,   902,   903,     1,     1,     1,
     333,     1,   341,    21,   343,  1520,  1521,   300,     1,     1,
       1,   300,   333,     1,   341,   300,   343,  1315,  1167,   300,
     927,   333,   300,     1,  1173,     1,     1,   300,   935,   936,
       1,     1,     1,   333,   334,  1124,   336,  1126,     1,  1128,
    1555,  1556,  1557,  1558,   341,   300,   343,  1616,     1,     1,
       1,  1172,     1,     1,  1352,  1353,  1354,  1355,   341,   300,
     343,  1359,   333,   333,   300,     1,   333,  1365,   333,     1,
       1,    88,    89,    90,    91,   333,     1,     1,     1,     1,
       1,     1,  1231,     1,   333,   334,   300,   336,  1007,   341,
    1009,   343,  1011,   333,  1013,   333,   300,   300,   333,  1668,
     333,  1616,  1671,  1672,   333,   333,   308,   309,   333,   333,
     300,   313,   314,  1682,  1683,  1684,   333,  1686,  1687,  1688,
     333,   333,   300,   333,   333,   333,   333,   333,   333,  1036,
    1037,  1038,   300,   333,   300,   333,   300,   333,   333,   333,
     300,   333,   333,   300,   333,   333,   333,   300,   300,   333,
     334,   333,   333,  1668,  1061,   341,  1671,  1672,   333,   333,
     333,   333,   333,   333,   333,  1072,  1315,  1682,  1683,  1684,
       1,  1686,  1687,  1688,   333,   333,   341,   333,   343,  1348,
     341,   336,   343,   333,   333,   333,   333,   300,   333,   333,
     333,   333,   333,   333,   333,   333,   333,   333,   300,   333,
     333,   333,   333,  1352,  1353,  1354,  1355,   333,   333,   333,
    1359,   333,   333,   321,   333,   333,  1365,   333,   334,   333,
     336,   333,  1520,  1521,   341,  1528,   343,   333,   333,   333,
     333,   341,   333,   343,   333,   333,   333,   300,   333,   300,
     333,   333,   333,  1150,   333,   300,   333,   333,   333,   333,
     333,   333,   333,   333,   333,   333,   333,  1555,  1556,  1557,
    1558,   333,   333,   300,   300,   300,   333,   333,   308,   309,
     300,   333,   320,   313,   314,   315,   316,   317,   318,   308,
     309,   310,   330,   333,   313,   314,   308,   309,   300,   300,
     300,   313,   314,   341,   300,   343,   300,   341,   300,   343,
     300,  1208,  1209,  1210,   321,   308,   309,   310,   300,   300,
     313,   314,   300,  1220,  1221,   308,   309,   310,  1616,   300,
     313,   314,   300,  1242,   300,   300,  1233,   300,   300,   300,
     300,   300,  1239,  1240,   341,  1242,   343,   300,  1459,  1246,
    1247,  1248,  1249,  1250,  1251,  1252,     1,   300,   300,   300,
     300,   300,   300,    88,    89,    90,    91,  1264,  1265,  1266,
    1267,  1268,  1269,  1270,   300,   300,   300,   300,   300,   300,
    1668,  1520,  1521,  1671,  1672,   300,   300,   300,   300,   300,
     300,   320,   300,   300,  1682,  1683,  1684,   300,  1686,  1687,
    1688,   330,   300,   300,   225,   226,   227,   228,   341,   300,
     343,   320,   341,   341,   343,   343,  1555,  1556,  1557,  1558,
     300,   330,   341,   300,   343,   300,    71,    72,    73,    74,
     300,    76,   341,    78,   343,    80,   287,   288,   289,   290,
     291,   292,   293,   294,   295,  1342,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   300,
     333,   334,  1551,   336,   300,  1554,   300,  1616,   319,   320,
     321,   322,   323,   334,   300,  1596,   300,   300,   333,   300,
     345,   333,   301,   302,   303,   304,   305,   306,   307,   308,
     309,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   323,   300,   333,   334,   335,   336,
     337,   333,   334,   335,   336,   337,   300,   344,   300,  1668,
     347,   300,  1671,  1672,   343,   347,   300,   300,   300,   300,
     300,   300,   300,  1682,  1683,  1684,   300,  1686,  1687,  1688,
     301,   302,   303,   304,   305,   306,   307,   308,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   305,   306,   307,   308,   309,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   323,   343,   300,   300,   300,   321,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   300,   300,   333,   334,
     335,   336,   337,   300,   300,   300,   300,   300,   300,   344,
     300,   300,   347,   300,   300,   300,   300,   300,   300,   300,
     300,     0,     1,   300,  1531,   300,   300,   300,   300,   300,
    1537,   300,   300,   300,   300,    14,    15,    16,    17,    18,
      19,    20,    21,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,    38,
     300,   300,    41,    42,    43,    44,    45,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,    57,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   323,   300,   350,   341,   347,  1613,   345,   109,   345,
     345,   345,   333,   333,  1621,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     333,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   333,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   333,   333,   333,   333,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   333,   267,   268,
     333,   333,   333,   333,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   333,   333,   333,   333,
     333,   348,   333,   330,   350,   337,   350,   300,   297,   298,
     299,   341,   333,   301,   302,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   300,   300,   300,   340,
     300,   348,   333,   341,   343,   334,   348,   344,   348,   336,
     344,   300,   344,   342,   344,   343,   330,   342,   344,     1,
     349,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,   344,   344,   344,   344,   344,   344,   330,
      22,    23,    24,    25,    26,    27,    28,    29,   342,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,   341,
      42,   348,   348,   348,    46,   348,    48,    49,    50,    51,
      52,    53,    54,    55,   345,   350,    58,    59,    60,   345,
     350,   344,    64,    65,    66,    67,    68,    69,     1,   344,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,   336,   343,   343,   343,   343,   343,   343,    22,
      23,    24,    25,    26,    27,    28,    29,   343,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,   343,    42,
     343,   330,   344,    46,   345,    48,    49,    50,    51,    52,
      53,    54,    55,   347,   345,    58,    59,    60,   345,   345,
     266,    64,    65,    66,    67,    68,    69,     1,   266,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,   333,   333,   343,   343,   343,   343,   343,    22,    23,
      24,    25,    26,    27,    28,    29,   334,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,   343,    42,   300,
     300,   300,    46,   300,    48,    49,    50,    51,    52,    53,
      54,    55,   343,   346,    58,    59,    60,   347,   343,   341,
      64,    65,    66,    67,    68,    69,     1,   341,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
     343,   343,    47,   350,   350,   343,   341,    22,    23,    24,
      25,    26,    27,    28,    29,   343,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,   343,    42,   343,   343,
     343,    46,   343,    48,    49,    50,    51,    52,    53,    54,
      55,   343,   343,    58,    59,    60,   343,   343,   343,    64,
      65,    66,    67,    68,    69,   343,   343,   343,   343,   343,
     343,   343,   343,   285,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   323,   343,   343,   343,   343,   343,   343,   330,   343,
     343,   334,   334,   348,   341,   336,   338,   339,   336,   341,
     342,   343,   344,   345,   300,   330,   343,   343,   350,   333,
     333,   333,   285,   333,   333,   333,   333,   333,   333,   333,
     333,   333,   333,   333,   333,   330,   347,   322,   301,   302,
     303,   304,   305,   306,   307,   308,   309,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   322,
     323,   343,   343,   336,   336,   333,   346,   350,   343,   343,
     348,   334,   343,   343,   343,   338,   339,   343,   341,   342,
     343,   344,   345,   343,   343,   343,   343,   350,   343,   343,
     343,   285,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   322,   323,
     350,   350,   350,   343,   343,   343,   348,   348,   198,   980,
     334,   851,  1165,   350,   338,   339,   350,   341,   342,   343,
     882,   345,  1210,   853,   367,   545,   350,  1212,  1212,  1613,
     285,   366,  1535,    -1,  1343,    -1,    -1,  1597,    -1,    -1,
    1621,    -1,    -1,    -1,    -1,    -1,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   334,
      -1,    -1,    -1,   338,   339,     1,   341,   342,   343,    -1,
     345,    -1,    -1,    -1,    -1,   350,    -1,    -1,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    -1,    -1,    41,    42,    43,    44,    45,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321,   322,   323,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   343,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,    -1,
      -1,    -1,    -1,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
      -1,   267,   268,    -1,    -1,    -1,    -1,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   297,   298,   299,    -1,    -1,    -1,    -1,    -1,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   323,    -1,    -1,    -1,    -1,    -1,     1,   334,     3,
       4,     5,     6,     7,    -1,    -1,   342,    -1,    12,    13,
      14,   343,    -1,   349,    -1,    -1,    -1,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    -1,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    42,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,
      54,    -1,    -1,    -1,    -1,    59,    60,    -1,    -1,    -1,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,     1,    -1,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    42,    -1,    -1,    -1,    46,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    -1,    -1,    -1,    64,    65,    66,
      67,    68,    69,     1,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    -1,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    -1,    42,    -1,    -1,    -1,    46,    -1,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    67,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    -1,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    42,
      -1,   285,   286,    46,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    67,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   320,    -1,    -1,    -1,
     324,    -1,   326,   327,   328,   329,    -1,   331,   332,   333,
     334,    -1,   336,   337,   338,   339,    -1,    -1,    -1,    -1,
     344,   345,    -1,   347,    -1,    -1,    -1,    -1,     1,    -1,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    -1,    -1,    -1,    -1,    -1,    -1,   285,    22,
      23,    24,    25,    26,    27,    28,    29,    -1,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    42,
      -1,    -1,    -1,    46,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    67,    68,    69,   334,    -1,    -1,
      -1,   338,   339,    -1,    -1,   342,    -1,    -1,    -1,   346,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   285,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   322,   323,
     303,   304,   305,   306,   307,   308,   309,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   322,
     323,    -1,    -1,    -1,    -1,    -1,   334,    -1,    -1,    -1,
     338,   339,    -1,    -1,   342,    -1,    -1,    -1,   346,    -1,
      -1,    -1,     1,    -1,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    -1,    -1,    -1,    -1,
      -1,    -1,   285,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    42,    -1,    -1,    -1,    46,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    -1,    -1,    -1,    64,    65,    66,    67,    68,
      69,   334,    -1,    -1,    -1,   338,   339,    -1,    -1,   342,
      -1,    -1,    -1,   346,    -1,    -1,     1,    -1,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    -1,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    42,    -1,    -1,
      -1,    46,   285,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    -1,    -1,    -1,    64,
      65,    66,    67,    68,    69,   304,   305,   306,   307,   308,
     309,   310,   311,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   323,    -1,    -1,    -1,    -1,    -1,
      -1,   334,    -1,    -1,    -1,   338,   339,    -1,    -1,   342,
      -1,    -1,     1,   346,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    42,    -1,    -1,    -1,    46,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    -1,    -1,    58,
      59,    60,    -1,    -1,    -1,    64,    65,    66,    67,    68,
      69,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      -1,    -1,    -1,    -1,    -1,    -1,   285,    22,    23,    24,
      25,    26,    27,    28,    29,    -1,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    42,    -1,    -1,
      -1,    46,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    -1,    -1,    58,    59,    60,    -1,    -1,    -1,    64,
      65,    66,    67,    68,    69,   334,    -1,    -1,    -1,   338,
     339,    -1,    -1,   342,    -1,    -1,    -1,   346,    -1,    -1,
       1,    -1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,
     285,    22,    23,    24,    25,    26,    27,    28,    29,    -1,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    42,    -1,    -1,    -1,    46,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    -1,    -1,    58,    59,    60,
      -1,    -1,    -1,    64,    65,    66,    67,    68,    69,   334,
      -1,    -1,    -1,   338,   339,    -1,    -1,   342,    -1,    -1,
       1,   346,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    -1,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    42,    -1,    -1,    -1,    46,   285,    48,    49,    50,
      51,    52,    53,    54,    55,    -1,    -1,    58,    59,    60,
      -1,    -1,    -1,    64,    65,    66,    67,    68,    69,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   334,    -1,    -1,    -1,   338,
     339,    -1,    -1,   342,    -1,    -1,    -1,   346,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     285,   307,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   321,   322,   323,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   334,
      -1,    -1,    -1,   338,   339,    -1,    -1,   342,    -1,    -1,
      -1,   346,    -1,    -1,     1,    -1,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    -1,    -1,
      -1,    -1,    -1,    -1,   285,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    42,    -1,    -1,    -1,    46,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    -1,
      -1,    58,    59,    60,    -1,    -1,    -1,    64,    65,    66,
      67,    68,    69,   334,    -1,    -1,    -1,   338,   339,    -1,
      -1,   342,    -1,    -1,     1,   346,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    -1,    -1,
      -1,    -1,    -1,    -1,   285,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    42,    -1,    -1,    -1,    46,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    -1,
      -1,    58,    59,    60,    -1,    -1,    -1,    64,    65,    66,
      67,    68,    69,   334,    -1,    -1,    -1,   338,   339,    -1,
      -1,   342,    -1,    -1,     1,   346,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    42,    -1,    -1,    -1,    46,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    -1,
      -1,    58,    59,    60,    -1,    -1,    -1,    64,    65,    66,
      67,    68,    69,     1,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    -1,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    -1,    42,    -1,    -1,    -1,    46,    -1,
      48,    49,    50,    51,    52,    53,    54,    55,   285,    -1,
      58,    59,    60,    -1,    -1,    -1,    64,    65,    66,    67,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   334,    -1,    -1,
      -1,   338,   339,    -1,    -1,   342,    -1,    -1,     1,   346,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    -1,    -1,    -1,    -1,    -1,    -1,   285,    22,
      23,    24,    25,    26,    27,    28,    29,    -1,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    42,
      -1,    -1,    -1,    46,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    -1,    -1,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    67,    68,    69,   334,    -1,    -1,
      -1,   338,   339,    -1,    -1,   342,    -1,    -1,     1,   346,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    -1,    -1,    -1,    -1,    -1,    -1,   285,    22,
      23,    24,    25,    26,    27,    28,    29,    -1,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    42,
      -1,    -1,    -1,    46,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    -1,    -1,    58,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    67,    68,    69,   334,    -1,    -1,
      -1,   338,   339,    -1,    -1,   342,    -1,    -1,    -1,   346,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   285,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   334,    -1,    -1,    -1,
     338,   339,    -1,    -1,   342,    -1,    -1,     1,   346,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    -1,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    42,    -1,
      -1,    -1,    46,    -1,    48,    49,    50,    51,    52,    53,
      54,    55,   285,    -1,    58,    59,    60,    -1,    -1,    -1,
      64,    65,    66,    67,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   334,    -1,    -1,    -1,   338,   339,    -1,    -1,   342,
      -1,    -1,     1,   346,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    -1,    -1,    -1,    -1,
      -1,    -1,   285,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    42,    -1,    -1,    -1,    46,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    -1,    -1,    58,
      59,    60,    -1,    -1,    -1,    64,    65,    66,    67,    68,
      69,   334,    -1,    -1,    -1,   338,   339,    -1,    -1,   342,
      -1,    -1,     1,   346,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    42,    -1,    -1,    -1,    46,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    -1,    -1,    58,
      59,    60,    -1,    -1,    -1,    64,    65,    66,    67,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   285,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     334,    -1,    -1,    -1,   338,   339,    -1,    -1,   342,    -1,
      -1,     1,   346,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      -1,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    -1,    42,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    -1,   285,    -1,    -1,    59,
      60,    -1,    -1,    -1,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   334,    -1,    -1,    -1,   338,
     339,    -1,    -1,   342,    -1,    -1,    -1,   346,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   285,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   334,    -1,    -1,    -1,   338,
     339,    -1,    -1,   342,    -1,    -1,    -1,   346,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    -1,    12,    13,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    42,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    -1,    -1,
      -1,    -1,    59,    60,    -1,   285,   286,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    -1,    -1,    -1,
     320,   321,    -1,    -1,   324,    -1,   326,   327,   328,   329,
      -1,   331,   332,   333,   334,    -1,   336,    -1,   338,   339,
      -1,    -1,    -1,    -1,   344,   345,     1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    -1,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    42,    -1,    -1,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      -1,    -1,    -1,    -1,    59,    60,    -1,    -1,    -1,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    -1,    12,    13,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    -1,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    42,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,
      54,    -1,    -1,    -1,    -1,    59,    60,    -1,   285,   286,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   320,   321,    -1,    -1,   324,    -1,   326,
     327,   328,   329,    -1,   331,   332,   333,   334,    -1,   336,
      -1,   338,   339,    -1,    -1,    -1,    -1,   344,   345,     1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    -1,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      42,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,
      52,    53,    54,    -1,    -1,    -1,    -1,    59,    60,    -1,
     285,   286,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   320,    -1,    -1,    -1,   324,
      -1,   326,   327,   328,   329,    -1,   331,   332,   333,   334,
      -1,   336,    -1,   338,   339,    -1,    -1,    -1,   343,   344,
     345,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   285,   286,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   320,    -1,    -1,    -1,
     324,    -1,   326,   327,   328,   329,    -1,   331,   332,   333,
     334,    -1,   336,    -1,   338,   339,    -1,    -1,    -1,   343,
     344,   345,     1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    42,    -1,    -1,    -1,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    -1,    -1,    -1,    -1,
      59,    60,    -1,   285,   286,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   320,    -1,
      -1,    -1,   324,    -1,   326,   327,   328,   329,    -1,   331,
     332,   333,   334,    -1,   336,    -1,   338,   339,    -1,    -1,
      -1,   343,   344,   345,     1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    -1,    12,    13,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    42,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    -1,    -1,
      -1,    -1,    59,    60,    -1,    -1,    -1,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    -1,    12,    13,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    -1,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    42,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    51,    52,    53,    54,    -1,
      -1,    -1,    -1,    59,    60,    -1,   285,   286,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   320,    -1,   322,    -1,   324,    -1,   326,   327,   328,
     329,    -1,   331,   332,   333,   334,    -1,   336,    -1,   338,
     339,    -1,    -1,    -1,    -1,   344,   345,     1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    -1,    12,    13,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    -1,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    42,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,
      54,    -1,    -1,    -1,    -1,    59,    60,    -1,   285,   286,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   320,    -1,    -1,    -1,   324,    -1,   326,
     327,   328,   329,    -1,   331,   332,   333,   334,    -1,   336,
      -1,   338,   339,    -1,    -1,    -1,    -1,   344,   345,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   285,
     286,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   320,    -1,    -1,    -1,   324,    -1,
     326,   327,   328,   329,    -1,   331,   332,   333,   334,    -1,
     336,    -1,   338,   339,    -1,    -1,    -1,    -1,   344,   345,
       1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    -1,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    42,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    -1,    -1,    -1,    -1,    59,    60,
      -1,   285,   286,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   320,    -1,    -1,    -1,
     324,    -1,   326,   327,   328,   329,    -1,   331,   332,   333,
     334,    -1,   336,    -1,   338,   339,    -1,    -1,    -1,    -1,
     344,   345,     1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    42,    -1,    -1,    -1,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    -1,    -1,    -1,    -1,
      59,    60,    -1,    -1,    -1,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    -1,    12,    13,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    -1,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    -1,    42,    -1,    -1,    -1,    -1,    -1,
      48,    49,    50,    51,    52,    53,    54,    -1,    -1,    -1,
      -1,    59,    60,    -1,   285,   286,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   320,
      -1,    -1,    -1,   324,    -1,   326,   327,   328,   329,    -1,
     331,   332,   333,   334,    -1,   336,    -1,   338,   339,    -1,
      -1,    -1,    -1,   344,   345,     1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    -1,    12,    13,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    -1,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    42,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    51,    52,    53,    54,    -1,
      -1,    -1,    -1,    59,    60,    -1,   285,   286,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   320,    -1,    -1,    -1,   324,    -1,   326,   327,   328,
     329,    -1,   331,   332,   333,   334,    -1,   336,    -1,   338,
     339,    -1,    -1,    -1,    -1,   344,   345,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   285,   286,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   320,    -1,    -1,    -1,   324,    -1,   326,   327,
     328,   329,    -1,   331,   332,   333,   334,    -1,   336,    -1,
     338,   339,    -1,    -1,    -1,    -1,   344,   345,     1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    -1,    12,
      13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    -1,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    42,
      -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,
      53,    54,    -1,    -1,    -1,    -1,    59,    60,    -1,   285,
     286,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   320,    -1,    -1,    -1,   324,    -1,
     326,   327,   328,   329,    -1,   331,   332,   333,   334,    -1,
     336,    -1,   338,   339,    -1,    -1,    -1,    -1,   344,   345,
       1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    -1,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    42,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    -1,    -1,    -1,    -1,    59,    60,
      -1,    -1,    -1,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      -1,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    -1,    42,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    -1,    -1,    -1,    -1,    59,
      60,    -1,   285,   286,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   320,    -1,    -1,
      -1,   324,    -1,   326,   327,   328,   329,    -1,   331,   332,
     333,   334,    -1,   336,    -1,   338,   339,    -1,    -1,    -1,
      -1,   344,   345,     1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    -1,    12,    13,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    -1,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    -1,    42,    -1,    -1,    -1,    -1,    -1,
      48,    49,    50,    51,    52,    53,    54,    -1,    -1,    -1,
      -1,    59,    60,    -1,   285,   286,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   320,
      -1,    -1,    -1,   324,    -1,   326,   327,   328,   329,    -1,
     331,   332,   333,   334,    -1,   336,    -1,   338,   339,    -1,
      -1,    -1,    -1,   344,   345,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   285,   286,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     320,    -1,    -1,    -1,   324,    -1,   326,   327,   328,   329,
      -1,   331,   332,   333,   334,    -1,   336,    -1,   338,   339,
      -1,    -1,    -1,    -1,   344,   345,     1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    -1,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    42,    -1,    -1,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      -1,    -1,    -1,    -1,    59,    60,    -1,   285,   286,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   320,    -1,    -1,    -1,   324,    -1,   326,   327,
     328,   329,    -1,   331,   332,   333,   334,    -1,   336,    -1,
     338,   339,    -1,    -1,    -1,    -1,   344,   345,     1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    -1,    12,
      13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    -1,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    42,
      -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,
      53,    54,    -1,    -1,    -1,    -1,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    -1,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      42,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,
      52,    53,    54,    -1,    -1,    -1,    -1,    59,    60,    -1,
     285,   286,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   320,    -1,    -1,    -1,   324,
      -1,   326,   327,   328,   329,    -1,   331,   332,   333,   334,
      -1,   336,    -1,   338,   339,    -1,    -1,    -1,    -1,   344,
     345,     1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      -1,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    -1,    42,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    -1,    -1,    -1,    -1,    59,
      60,    -1,   285,   286,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   320,    -1,    -1,
      -1,   324,    -1,   326,   327,   328,   329,    -1,   331,   332,
     333,   334,    -1,   336,    -1,   338,   339,    -1,    -1,    -1,
      -1,   344,   345,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   285,   286,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   320,    -1,
      -1,    -1,   324,    -1,   326,   327,   328,   329,    -1,   331,
     332,   333,   334,    -1,   336,    -1,   338,   339,    -1,    -1,
      -1,    -1,   344,   345,     1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    -1,    12,    13,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    42,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    -1,    -1,
      -1,    -1,    59,    60,    -1,   285,   286,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     320,    -1,    -1,    -1,   324,    -1,   326,   327,   328,   329,
      -1,   331,   332,   333,   334,    -1,   336,    -1,   338,   339,
      -1,    -1,    -1,    -1,   344,   345,     1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    -1,    12,    13,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    -1,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    42,    -1,    -1,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      -1,    -1,    -1,    -1,    59,    60,    -1,    -1,    -1,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    -1,    12,    13,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    -1,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    42,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,
      54,    -1,    -1,    -1,    -1,    59,    60,    -1,   285,   286,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   320,    -1,    -1,    -1,   324,    -1,   326,
     327,   328,   329,    -1,   331,   332,   333,   334,    -1,   336,
      -1,   338,   339,    -1,    -1,    -1,    -1,   344,   345,     1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,    -1,
      12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    -1,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    -1,
      42,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,
      52,    53,    54,    -1,    -1,    -1,    -1,    59,    60,    -1,
     285,   286,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   320,    -1,    -1,    -1,   324,
      -1,   326,   327,   328,   329,    -1,   331,   332,   333,   334,
      -1,   336,    -1,   338,   339,    -1,    -1,    -1,    -1,   344,
     345,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   285,   286,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   320,    -1,    -1,    -1,
     324,    -1,   326,   327,   328,   329,    -1,   331,   332,   333,
     334,    -1,   336,    -1,   338,   339,    -1,    -1,    -1,    -1,
     344,   345,     1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    42,    -1,    -1,    -1,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    -1,    -1,    -1,    -1,
      59,    60,    -1,   285,   286,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   320,    -1,
      -1,    -1,   324,    -1,   326,   327,   328,   329,    -1,   331,
     332,   333,   334,    -1,   336,    -1,   338,   339,    -1,    -1,
      -1,    -1,   344,   345,     1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    -1,    12,    13,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    42,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    -1,    -1,
      -1,    -1,    59,    60,    -1,    -1,    -1,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    -1,    12,    13,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    -1,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    42,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    51,    52,    53,    54,    -1,
      -1,    -1,    -1,    59,    60,    -1,   285,   286,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   320,    -1,    -1,    -1,   324,    -1,   326,   327,   328,
     329,    -1,   331,   332,   333,   334,    -1,   336,    -1,   338,
     339,    -1,    -1,    -1,    -1,   344,   345,     1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    -1,    12,    13,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    -1,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    42,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,
      54,    -1,    -1,    -1,    -1,    59,    60,    -1,   285,   286,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   320,    -1,    -1,    -1,   324,    -1,   326,
     327,   328,   329,    -1,   331,   332,   333,   334,    -1,   336,
      -1,   338,   339,    -1,    -1,    -1,    -1,   344,   345,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   285,
     286,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   320,    -1,    -1,    -1,   324,    -1,
     326,   327,   328,   329,    -1,   331,   332,   333,   334,    -1,
     336,    -1,   338,   339,    -1,    -1,    -1,    -1,   344,   345,
       1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      -1,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    -1,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    42,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    -1,    -1,    -1,    -1,    59,    60,
      -1,   285,   286,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   320,    -1,    -1,    -1,
     324,    -1,   326,   327,   328,   329,    -1,   331,   332,   333,
     334,    -1,   336,    -1,   338,   339,    -1,    -1,    -1,    -1,
     344,   345,     1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    -1,    12,    13,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    42,    -1,    -1,    -1,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    -1,    -1,    -1,    -1,
      59,    60,    -1,    -1,    -1,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    -1,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    42,    -1,
      -1,    -1,    46,    -1,    48,    49,    50,    51,    52,    53,
      54,    55,    -1,    -1,    58,    59,    60,    -1,    -1,    -1,
      64,    65,    66,    67,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   285,   286,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   320,
      -1,    -1,    -1,   324,    -1,   326,   327,   328,   329,    -1,
     331,   332,   333,   334,    -1,   336,    -1,   338,   339,    -1,
      -1,    -1,    -1,   344,   345,    -1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    -1,    12,    13,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    -1,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    -1,    42,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    51,    52,    53,    54,    -1,
      -1,    -1,    -1,    59,    60,    -1,   285,   286,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   320,    -1,    -1,    -1,   324,    -1,   326,   327,   328,
     329,    -1,   331,   332,   333,   334,    -1,   336,    -1,   338,
     339,    -1,    -1,    -1,    -1,   344,   345,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   285,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   322,   323,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     334,    -1,    -1,    -1,   338,   339,    -1,    -1,   342,    -1,
      -1,   345,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    -1,    12,
      13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    -1,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    42,
      -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,
      53,    54,    -1,    -1,    -1,    -1,    59,    60,    -1,   285,
     286,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   320,    -1,    -1,    -1,   324,    -1,
     326,   327,   328,   329,    -1,   331,   332,   333,   334,    -1,
     336,    -1,   338,   339,    -1,    -1,    -1,    -1,   344,   345,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    -1,    12,
      13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    -1,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    42,
      -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,
      53,    54,    -1,    -1,    -1,    -1,    59,    60,    -1,    -1,
      -1,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      -1,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    -1,    42,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    -1,    -1,    -1,    -1,    59,
      60,    -1,   285,   286,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   320,    -1,    -1,
      -1,   324,    -1,   326,   327,   328,   329,    -1,   331,   332,
     333,   334,    -1,   336,    -1,   338,   339,    -1,    -1,    -1,
      -1,   344,   345,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    -1,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      -1,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    -1,    42,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    -1,    -1,    -1,    -1,    59,
      60,    -1,   285,   286,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   320,    -1,    -1,
      -1,   324,    -1,   326,   327,   328,   329,    -1,   331,   332,
     333,   334,    -1,   336,    -1,   338,   339,    -1,    -1,    -1,
      -1,   344,   345,    -1,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    42,    -1,   285,   286,    46,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    -1,    -1,    58,
      59,    60,    -1,    -1,    -1,    64,    65,    66,    67,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     320,    -1,    -1,    -1,   324,    -1,   326,   327,   328,   329,
      -1,   331,   332,   333,   334,    -1,   336,    -1,   338,   339,
      -1,    -1,    -1,    -1,   344,   345,    -1,    -1,    -1,    -1,
       1,    -1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    -1,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    42,    -1,    -1,    -1,    46,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,   285,   286,    58,    59,    60,
      -1,    -1,    -1,    64,    65,    66,    67,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     320,    -1,    -1,    -1,   324,    -1,   326,   327,   328,   329,
      -1,   331,   332,   333,   334,    -1,   336,    -1,   338,   339,
      -1,    -1,    -1,    -1,   344,   345,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    42,    -1,    -1,    -1,    46,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    -1,
      -1,    58,    59,    60,    -1,    -1,   285,    64,    65,    66,
      67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   334,    -1,    -1,    -1,   338,
     339,    -1,    -1,   342,    -1,    -1,   345,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   285,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   334,    -1,    -1,    -1,   338,   339,    -1,
      -1,   342,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   285,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   334,    -1,    -1,
      -1,   338,   339,    -1,    -1,   342
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     1,    17,    18,    38,    41,    42,    43,    44,    45,
      57,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   267,   268,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   297,   298,   299,   334,   342,   349,   352,   353,   354,
     355,   356,   357,   358,   359,   370,   374,   377,   378,   380,
     381,   391,   392,   401,   300,     1,   347,     1,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,     1,   300,     1,   300,     1,   300,     1,
     300,     1,   300,     1,   300,     1,   300,     1,   300,     1,
     300,     1,   300,     1,   300,     1,   300,     1,   300,     1,
     300,     1,   300,     1,   300,     1,   300,     1,   300,     1,
     300,     1,   300,     1,   300,     1,   300,     1,   300,     1,
     300,     1,   300,     1,   300,     1,   300,     1,   300,     1,
     300,     1,   300,     1,   300,     1,   300,     1,   300,     1,
     300,     1,   300,     1,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,     1,   336,     1,   300,
     336,     1,   344,     1,   300,   336,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
       1,   300,     1,   300,     1,   300,     1,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,     1,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   330,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,     1,
     334,   336,   371,   372,   373,     1,   373,   375,   376,     1,
     336,     1,   336,     1,   336,     0,     1,   354,    14,    15,
     387,   388,    16,   389,   390,    19,   395,   396,    20,   397,
     398,    21,   399,   400,   330,   347,   345,   333,   334,   336,
     386,     1,   333,     1,   333,     1,    88,    89,    90,    91,
     321,   363,     1,   320,   333,   369,     1,   333,     1,   333,
       1,   369,     1,   333,     1,   333,     1,   333,     1,   333,
       1,   334,     1,   336,     1,   333,     1,   336,     1,   333,
       1,   336,     1,   336,     1,   336,     1,   336,     1,   333,
     334,   335,   336,   337,   344,   347,   360,   362,   363,   367,
     368,   382,   383,   384,   385,   415,   416,     1,   368,     1,
     333,     1,   333,     1,   333,   333,   333,   333,   369,   369,
     369,   369,   333,   333,   333,   333,   333,   333,   333,   333,
     333,   333,   333,   333,   333,   333,   333,   333,   333,   333,
     333,   333,   333,   333,   333,   333,   333,   333,   333,   333,
     333,     1,   369,     1,   333,     1,   333,     1,   333,     1,
     333,     1,   333,     1,   333,     1,   333,     1,   333,     1,
     369,     1,   369,     1,   369,     1,   369,     1,   369,     1,
     369,     1,   369,     1,   333,     1,   333,     1,   336,     1,
     336,     1,   333,     1,   336,   336,     1,   336,     1,   333,
       1,   333,     1,   334,   336,     1,   334,   336,     1,   334,
     336,     1,   334,   336,     1,   334,   336,     1,   333,     1,
     333,     1,   333,     1,   333,     1,   333,     1,   333,     1,
     369,     1,   369,     1,   369,     1,   334,   336,     1,   333,
       1,   333,     1,   333,     1,   333,   383,     1,   385,     1,
     333,     1,   333,   333,   333,   333,   333,     1,   333,     1,
     333,     1,   333,     1,   333,     1,   333,     1,   333,     1,
     333,     1,   333,     1,   333,     1,   333,     1,   333,     1,
     333,     1,   333,     1,   333,     1,   333,     1,   333,     1,
     333,     1,   333,     1,   225,   226,   227,   228,     1,   333,
       1,   333,     1,   333,     1,   333,     1,   336,     1,   336,
       1,   336,     1,   333,     1,   333,     1,   333,     1,   360,
       1,   333,     1,   333,     1,   333,     1,   333,     1,   333,
       1,   333,     1,   333,     1,   333,     1,   333,     1,   333,
       1,   333,     1,   333,     1,   333,     1,   333,     1,   333,
       1,   334,   336,     1,   333,   334,   333,     1,   333,     1,
     333,     1,   333,     1,   333,     1,   333,     1,   333,     1,
     333,     1,   336,     1,   333,     1,   333,     1,   333,   269,
     270,   271,   272,     1,   369,   333,   333,     1,   369,     1,
     369,     1,   333,     1,   333,     1,   333,     1,   333,     1,
     333,   341,   350,   341,     1,   347,     1,   345,     1,   347,
     345,     1,   347,   345,     1,   347,   345,     1,   347,    57,
     333,   334,   379,   333,   393,   348,   333,   330,   360,   361,
     384,   350,   350,   368,   109,   320,   330,   341,   300,   300,
     300,   300,   371,   333,   375,   386,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    22,    23,
      24,    25,    26,    27,    28,    29,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    42,    46,    48,    49,
      50,    51,    52,    53,    54,    55,    58,    59,    60,    64,
      65,    66,    67,    68,    69,   285,   334,   338,   339,   342,
     417,   419,   420,   421,   425,   426,   434,   435,   437,   438,
     441,   442,   444,   448,   449,   450,   454,   386,   419,   386,
     419,   386,   419,   340,   300,   348,   419,     1,   394,   333,
     341,   343,   348,     1,   321,   333,   365,   362,   360,     1,
     333,   415,     1,   415,   336,     1,   336,     1,   336,     1,
     336,     1,   336,   348,     1,   344,     1,   344,     1,   344,
       1,   344,     1,   344,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,   286,   320,   324,   326,   327,   328,   329,   331,
     332,   336,   344,   345,   369,   407,   408,   409,   410,   411,
     412,   417,   429,   434,   435,   436,   437,   439,   441,   442,
     445,   446,   447,   344,   447,   344,   447,     1,   344,     1,
     344,     1,   344,   344,     1,   344,     1,   344,     1,   344,
       1,   344,     1,   344,     1,   344,     1,   344,     1,   344,
     344,     1,   344,     1,   344,     1,   344,     1,   344,   344,
     344,   344,   344,   344,   344,   447,     1,   344,     1,   344,
       1,   344,   344,   344,   344,   344,     1,   447,   447,     1,
     344,     1,   344,     1,   344,     1,   344,     1,   344,     1,
     451,     1,   342,     1,   346,   420,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   334,   431,   432,   433,   300,
     443,   342,     1,   344,   342,   348,   346,   348,   346,   348,
     346,   348,     1,   333,   336,   330,   346,   345,   330,   361,
     350,   350,   341,   345,     1,    61,   334,   336,   343,   382,
     414,     1,    61,   336,   382,   414,     1,    61,   336,   382,
     414,     1,    61,   336,   382,   414,     1,    61,   336,   382,
     414,     1,   308,   309,   310,   313,   314,   402,   404,   402,
     402,     1,   402,   430,     1,   447,     1,   447,     1,   447,
     344,   344,     1,   343,   435,   437,   441,   442,   447,   448,
       1,   419,     1,   404,     1,   315,   316,   317,   318,   402,
     403,     1,   404,   433,     1,   447,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   405,
     406,   343,   343,     1,   333,   336,     1,   336,     1,   334,
     447,   336,     1,   336,     1,   336,     1,   336,     1,   336,
       1,   333,     1,   333,   343,     1,   343,   369,   343,     1,
     336,     1,   336,     1,   336,     1,   336,   343,   343,   343,
       1,   333,   343,     1,   363,   345,   418,   420,     1,   360,
       1,   333,     1,   360,   363,   366,   343,   343,   343,   343,
     345,   418,     1,   336,     1,   336,   333,   373,   333,   373,
     333,   373,   344,   330,   347,   447,     1,   336,   434,   435,
     440,   442,   345,   345,   345,   345,   266,   266,   379,   419,
     333,   365,   333,   336,   369,   419,   343,   341,   343,   341,
     343,   341,   343,   320,   330,   341,   343,   343,   341,   343,
     341,   343,   341,   343,   341,   343,   343,   341,   343,   341,
     343,   341,   343,   341,   343,   343,   341,   343,   341,   343,
     341,   343,   341,   343,   343,   341,   343,   341,   343,   341,
     343,   341,   343,     1,   334,   447,     1,    88,    89,    90,
      91,    92,    93,   321,   364,   447,     1,   364,   447,     1,
     407,   411,   334,   427,   428,     1,   447,     1,   447,   343,
     343,   343,   346,   346,     1,    86,   447,     1,   447,     1,
     447,     1,    86,   333,   334,   382,   413,   414,   447,   347,
       1,   447,     1,   447,     1,   447,     1,   447,   447,   447,
     447,     1,   447,     1,   447,     1,   447,     1,   447,     1,
     447,     1,   447,     1,   447,   343,   341,   343,   343,   341,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     419,    47,   343,   343,   343,   343,   343,   350,   350,   343,
       1,    56,    57,   346,   423,   424,   343,   341,   343,   343,
     343,   343,   343,   343,   343,   343,   447,   452,   453,   432,
     348,   341,   343,   433,   341,   419,   419,   419,   419,   336,
     336,   300,   346,   330,   343,   343,   346,    62,   333,   333,
     333,     1,   334,     1,   334,   333,    62,   333,   333,   333,
     333,    62,   333,   333,   333,   333,    62,   333,   333,   333,
     333,    62,   333,   333,   333,   333,   347,   330,   343,   343,
     343,   343,   322,   369,   336,   336,   346,   418,     1,   365,
     360,   346,     1,   322,   422,   447,   350,   346,   423,   333,
     447,   341,   343,   373,     1,   373,   346,   346,   346,   346,
     333,   336,   333,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   336,   369,
     427,     1,   333,   382,   348,   343,   343,   350,   350,     1,
     422,   350,   419,   343,   343,   453,   343,   343,   343,   348,
     348,   365,   419,   350,   350,     1,   419,   419,   419
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   351,   352,   353,   353,   353,   354,   354,   354,   354,
     354,   355,   354,   356,   354,   354,   357,   354,   358,   354,
     359,   354,   354,   354,   360,   360,   360,   361,   361,   362,
     362,   363,   363,   363,   363,   363,   364,   364,   364,   364,
     364,   364,   364,   365,   365,   366,   366,   366,   366,   366,
     367,   367,   367,   367,   367,   368,   368,   369,   369,   370,
     370,   371,   371,   372,   372,   373,   373,   374,   374,   375,
     375,   376,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   377,   377,   377,   377,   377,   377,   377,   377,   377,
     377,   378,   378,   379,   379,   379,   380,   380,   380,   380,
     380,   380,   380,   381,   381,   381,   381,   381,   381,   381,
     381,   381,   381,   381,   381,   381,   382,   382,   383,   384,
     385,   385,   386,   386,   386,   387,   387,   388,   388,   388,
     388,   389,   390,   390,   390,   391,   391,   393,   392,   392,
     392,   394,   392,   392,   395,   396,   396,   396,   397,   398,
     398,   398,   399,   400,   400,   401,   401,   401,   401,   401,
     401,   402,   402,   402,   402,   403,   403,   403,   403,   404,
     404,   405,   405,   405,   405,   405,   405,   405,   406,   406,
     406,   406,   407,   407,   407,   408,   408,   408,   409,   409,
     409,   409,   409,   410,   410,   411,   411,   411,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   413,   413,   413,
     413,   414,   414,   414,   414,   414,   415,   415,   415,   416,
     416,   416,   416,   416,   417,   418,   418,   419,   419,   419,
     420,   420,   420,   420,   420,   420,   420,   420,   421,   421,
     422,   423,   423,   423,   423,   423,   423,   423,   423,   423,
     423,   423,   424,   424,   425,   425,   425,   425,   426,   427,
     427,   427,   428,   428,   430,   429,   431,   431,   431,   431,
     431,   431,   431,   431,   431,   432,   433,   433,   434,   435,
     436,   437,   438,   438,   439,   439,   439,   440,   440,   440,
     441,   442,   443,   444,   444,   444,   445,   445,   445,   445,
     445,   445,   445,   445,   445,   445,   445,   445,   446,   446,
     446,   447,   447,   447,   447,   447,   447,   447,   447,   447,
     447,   447,   447,   447,   447,   447,   447,   447,   447,   447,
     447,   447,   447,   447,   447,   447,   447,   447,   447,   447,
     447,   447,   447,   447,   447,   447,   447,   447,   447,   447,
     448,   449,   449,   449,   450,   450,   450,   450,   450,   450,
     450,   450,   450,   450,   450,   450,   450,   450,   450,   450,
     450,   450,   450,   450,   450,   450,   450,   450,   450,   450,
     450,   450,   450,   450,   450,   450,   450,   450,   450,   450,
     450,   450,   450,   450,   450,   450,   450,   450,   450,   450,
     450,   450,   450,   450,   450,   450,   450,   450,   450,   450,
     450,   450,   450,   450,   450,   450,   450,   450,   450,   450,
     450,   450,   450,   450,   450,   450,   450,   450,   450,   450,
     450,   450,   450,   450,   450,   450,   450,   450,   450,   450,
     450,   450,   450,   450,   450,   450,   450,   450,   450,   450,
     450,   450,   450,   450,   450,   450,   450,   450,   450,   450,
     450,   450,   450,   450,   450,   450,   450,   450,   450,   450,
     450,   450,   450,   450,   450,   450,   450,   450,   450,   450,
     450,   450,   450,   450,   450,   450,   450,   450,   450,   450,
     450,   450,   450,   450,   450,   450,   450,   450,   450,   450,
     450,   450,   450,   450,   450,   450,   450,   450,   450,   451,
     450,   450,   452,   452,   452,   453,   454,   454,   454,   454,
     454,   454,   454,   454,   454,   454
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     1,     1,     1,     1,
       1,     0,     2,     0,     2,     1,     0,     2,     0,     2,
       0,     2,     1,     1,     1,     1,     1,     1,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     5,     3,
       1,     3,     3,     5,     3,     1,     2,     1,     2,     2,
       2,     1,     3,     1,     3,     1,     1,     2,     2,     1,
       3,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     3,     2,     3,     2,
       3,     2,     3,     2,     3,     2,     3,     2,     3,     2,
       3,     2,     3,     2,     3,     2,     3,     2,     3,     2,
       3,     2,     3,     2,     3,     2,     3,     2,     3,     2,
       3,     2,     3,     2,     3,     2,     3,     2,     3,     2,
       3,     2,     3,     2,     3,     2,     3,     2,     3,     2,
       3,     2,     3,     2,     3,     2,     3,     2,     3,     2,
       3,     2,     3,     2,     3,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     3,     2,
       3,     2,     3,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     7,     5,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     5,     5,     5,     5,     5,     5,     5,     5,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       2,     1,     1,     1,     1,     1,     5,     5,     7,     7,
       5,     8,     8,     2,     2,     2,     2,     3,     3,     8,
       8,     2,     2,     2,     3,     3,     1,     1,     7,     1,
       1,     3,     1,     1,     1,     1,     1,     4,     7,     2,
       2,     1,     4,     7,     2,     1,     1,     0,     5,     2,
       2,     0,     8,     5,     1,     4,     7,     2,     1,     4,
       7,     2,     1,     7,     2,     2,     2,     2,     2,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     2,     3,     3,     3,     2,     3,     3,     3,     3,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     3,     3,     3,     2,     3,     3,     1,
       3,     1,     3,     3,     3,     3,     1,     1,     1,     1,
       3,     3,     3,     3,     1,     1,     3,     2,     1,     2,
       2,     1,     1,     1,     2,     2,     1,     2,     3,     5,
       1,     4,     5,     3,     4,     3,     2,     4,     5,     3,
       4,     4,     2,     1,     5,     4,     2,     5,     3,     1,
       4,     4,     3,     1,     0,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     2,
       5,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     1,     1,
       1,     1,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       4,     4,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     4,     4,     2,
       3,     1,     1,     1,     3,     4,     4,     4,     6,     6,
       6,     6,     6,     4,     2,     4,     4,     4,     4,     6,
       6,     6,     6,     6,     4,     2,     4,     4,     4,     4,
       6,     6,     6,     6,     6,     4,     2,     4,     4,     4,
       4,     6,     6,     6,     6,     6,     4,     2,     4,     4,
       4,     4,     6,     6,     6,     6,     6,     4,     2,     4,
       4,     6,     2,     4,     4,     4,     2,     4,     4,     2,
       4,     4,     2,     6,     6,     6,     4,     2,     6,     2,
       4,     4,     4,     2,     4,     4,     4,     2,     4,     4,
       2,     4,     4,     2,     4,     4,     2,     4,     3,     2,
       4,     3,     2,     4,     3,     4,     2,     4,     4,     2,
       4,     4,     2,     4,     4,     2,     4,     4,     2,     4,
       4,     2,     4,     3,     1,     3,     1,     3,     1,     4,
       3,     1,     4,     4,     4,     4,     4,     2,     4,     4,
       2,     4,     4,     2,     3,     1,     3,     1,     3,     1,
       3,     1,     6,     6,     2,     4,     4,     2,     4,     0,
       5,     2,     0,     3,     1,     1,     3,     2,     1,     3,
       2,     1,     1,     3,     2,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 628 "core/cfg.y" /* yacc.c:1646  */
    {}
#line 4798 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 629 "core/cfg.y" /* yacc.c:1646  */
    {}
#line 4804 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 630 "core/cfg.y" /* yacc.c:1646  */
    { yyerror(""); YYABORT;}
#line 4810 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 638 "core/cfg.y" /* yacc.c:1646  */
    {rt=REQUEST_ROUTE;}
#line 4816 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 639 "core/cfg.y" /* yacc.c:1646  */
    {rt=FAILURE_ROUTE;}
#line 4822 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 641 "core/cfg.y" /* yacc.c:1646  */
    {rt=BRANCH_ROUTE;}
#line 4828 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 642 "core/cfg.y" /* yacc.c:1646  */
    {rt=ONSEND_ROUTE;}
#line 4834 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 643 "core/cfg.y" /* yacc.c:1646  */
    {rt=EVENT_ROUTE;}
#line 4840 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 648 "core/cfg.y" /* yacc.c:1646  */
    {
		if ((yyvsp[0].ipaddr)){
			tmp=ip_addr2a((yyvsp[0].ipaddr));
			if (tmp==0) {
				LM_CRIT("cfg. parser: bad ip address.\n");
				(yyval.strval)=0;
			} else {
				(yyval.strval)=pkg_malloc(strlen(tmp)+1);
				if ((yyval.strval)==0) {
					PKG_MEM_CRITICAL;
				} else {
					strncpy((yyval.strval), tmp, strlen(tmp)+1);
				}
			}
		}
	}
#line 4861 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 664 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.strval)=pkg_malloc(strlen((yyvsp[0].strval))+1);
		if ((yyval.strval)==0) {
				PKG_MEM_CRITICAL;
		} else {
				strncpy((yyval.strval), (yyvsp[0].strval), strlen((yyvsp[0].strval))+1);
		}
	}
#line 4874 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 672 "core/cfg.y" /* yacc.c:1646  */
    {
		if ((yyvsp[0].strval)){
			(yyval.strval)=pkg_malloc(strlen((yyvsp[0].strval))+1);
			if ((yyval.strval)==0) {
					PKG_MEM_CRITICAL;
			} else {
					strncpy((yyval.strval), (yyvsp[0].strval), strlen((yyvsp[0].strval))+1);
			}
		}
	}
#line 4889 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 686 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.name_l)=mk_name_lst((yyvsp[0].strval), SI_IS_MHOMED); }
#line 4895 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 687 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.name_l)=mk_name_lst((yyvsp[-2].strval), SI_IS_MHOMED);
										if ((yyval.name_l)) (yyval.name_l)->next=(yyvsp[0].name_l);
									}
#line 4903 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 693 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.name_l)=(yyvsp[-1].name_l); }
#line 4909 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 694 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.name_l)=mk_name_lst((yyvsp[0].strval), 0); }
#line 4915 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 698 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=PROTO_UDP; }
#line 4921 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 699 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=PROTO_TCP; }
#line 4927 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 700 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=PROTO_TLS; }
#line 4933 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 701 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=PROTO_SCTP; }
#line 4939 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 702 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=0; }
#line 4945 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 705 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=PROTO_UDP; }
#line 4951 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 706 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=PROTO_TCP; }
#line 4957 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 707 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=PROTO_TLS; }
#line 4963 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 708 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=PROTO_SCTP; }
#line 4969 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 709 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=PROTO_WS; }
#line 4975 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 710 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=PROTO_WSS; }
#line 4981 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 711 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=0; }
#line 4987 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 714 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=(yyvsp[0].intval); }
#line 4993 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 715 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=0; }
#line 4999 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 718 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.sockid)=mk_listen_id((yyvsp[0].strval), 0, 0); }
#line 5005 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 719 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.sockid)=mk_listen_id((yyvsp[-2].strval), 0, (yyvsp[0].intval)); }
#line 5011 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 720 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.sockid)=mk_listen_id((yyvsp[0].strval), (yyvsp[-2].intval), 0); }
#line 5017 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 721 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.sockid)=mk_listen_id((yyvsp[-2].strval), (yyvsp[-4].intval), (yyvsp[0].intval));}
#line 5023 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 722 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.sockid)=0; yyerror("port number expected"); }
#line 5029 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 726 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.sockid)=mk_listen_id2((yyvsp[0].name_l), 0, 0); }
#line 5035 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 727 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.sockid)=mk_listen_id2((yyvsp[-2].name_l), 0, (yyvsp[0].intval)); }
#line 5041 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 728 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.sockid)=mk_listen_id2((yyvsp[0].name_l), (yyvsp[-2].intval), 0); }
#line 5047 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 729 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.sockid)=mk_listen_id2((yyvsp[-2].name_l), (yyvsp[-4].intval), (yyvsp[0].intval));}
#line 5053 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 730 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.sockid)=0; yyerror("port number expected"); }
#line 5059 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 734 "core/cfg.y" /* yacc.c:1646  */
    {  (yyval.sockid)=(yyvsp[0].sockid) ; }
#line 5065 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 735 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.sockid)=(yyvsp[-1].sockid);  if ((yyval.sockid)) (yyval.sockid)->next=(yyvsp[0].sockid); }
#line 5071 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 739 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=-(yyvsp[0].intval); }
#line 5077 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 743 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("flag list expected\n"); }
#line 5083 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 749 "core/cfg.y" /* yacc.c:1646  */
    { if (register_flag((yyvsp[0].strval),-1)<0)
								yyerror("register flag failed");
						}
#line 5091 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 752 "core/cfg.y" /* yacc.c:1646  */
    {
						if (register_flag((yyvsp[-2].strval), (yyvsp[0].intval))<0)
								yyerror("register flag failed");
										}
#line 5100 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 758 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.strval)=(yyvsp[0].strval); }
#line 5106 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 759 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.strval)=(yyvsp[0].strval); }
#line 5112 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 764 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("avpflag list expected\n"); }
#line 5118 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 771 "core/cfg.y" /* yacc.c:1646  */
    {
		if (register_avpflag((yyvsp[0].strval))==0)
			yyerror("cannot declare avpflag");
	}
#line 5127 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 777 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.debug=(yyvsp[0].intval); }
#line 5133 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 778 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number  expected"); }
#line 5139 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 779 "core/cfg.y" /* yacc.c:1646  */
    { dont_fork= ! (yyvsp[0].intval); }
#line 5145 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 780 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5151 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 781 "core/cfg.y" /* yacc.c:1646  */
    { set_fork_delay((yyvsp[0].intval)); }
#line 5157 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 782 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 5163 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 783 "core/cfg.y" /* yacc.c:1646  */
    { set_modinit_delay((yyvsp[0].intval)); }
#line 5169 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 784 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 5175 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 785 "core/cfg.y" /* yacc.c:1646  */
    { if (!config_check)  /* if set from cmd line, don't overwrite from yyparse()*/
					if(log_stderr == 0) log_stderr=(yyvsp[0].intval);
				   }
#line 5183 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 788 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5189 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 789 "core/cfg.y" /* yacc.c:1646  */
    {
		if ( (i_tmp=str2facility((yyvsp[0].strval)))==-1)
			yyerror("bad facility (see syslog(3) man page)");
		if (!config_check)
			default_core_cfg.log_facility=i_tmp;
	}
#line 5200 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 795 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("ID expected"); }
#line 5206 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 796 "core/cfg.y" /* yacc.c:1646  */
    { log_name=(yyvsp[0].strval); }
#line 5212 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 797 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 5218 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 798 "core/cfg.y" /* yacc.c:1646  */
    { log_color=(yyvsp[0].intval); }
#line 5224 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 799 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5230 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 800 "core/cfg.y" /* yacc.c:1646  */
    { log_prefix_fmt=(yyvsp[0].strval); }
#line 5236 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 801 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 5242 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 802 "core/cfg.y" /* yacc.c:1646  */
    { log_prefix_mode=(yyvsp[0].intval); }
#line 5248 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 803 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 5254 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 804 "core/cfg.y" /* yacc.c:1646  */
    { _km_log_engine_type=(yyvsp[0].strval); }
#line 5260 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 805 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 5266 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 806 "core/cfg.y" /* yacc.c:1646  */
    { _km_log_engine_data=(yyvsp[0].strval); }
#line 5272 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 807 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 5278 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 808 "core/cfg.y" /* yacc.c:1646  */
    { _ksr_xavp_via_params.s=(yyvsp[0].strval);
			_ksr_xavp_via_params.len=strlen((yyvsp[0].strval));
		}
#line 5286 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 811 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 5292 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 812 "core/cfg.y" /* yacc.c:1646  */
    { _ksr_xavp_via_params.s=(yyvsp[0].strval);
			_ksr_xavp_via_fields.len=strlen((yyvsp[0].strval));
		}
#line 5300 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 815 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 5306 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 816 "core/cfg.y" /* yacc.c:1646  */
    { received_dns|= ((yyvsp[0].intval))?DO_DNS:0; }
#line 5312 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 817 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5318 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 818 "core/cfg.y" /* yacc.c:1646  */
    { received_dns|= ((yyvsp[0].intval))?DO_REV_DNS:0; }
#line 5324 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 819 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5330 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 820 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.dns_try_ipv6=(yyvsp[0].intval); }
#line 5336 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 821 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5342 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 822 "core/cfg.y" /* yacc.c:1646  */
    { IF_NAPTR(default_core_cfg.dns_try_naptr=(yyvsp[0].intval)); }
#line 5348 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 823 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5354 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 824 "core/cfg.y" /* yacc.c:1646  */
    { IF_DNS_FAILOVER(default_core_cfg.dns_srv_lb=(yyvsp[0].intval)); }
#line 5360 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 825 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5366 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 826 "core/cfg.y" /* yacc.c:1646  */
    { IF_NAPTR(default_core_cfg.dns_udp_pref=(yyvsp[0].intval));}
#line 5372 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 827 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 5378 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 828 "core/cfg.y" /* yacc.c:1646  */
    { IF_NAPTR(default_core_cfg.dns_tcp_pref=(yyvsp[0].intval));}
#line 5384 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 829 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 5390 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 830 "core/cfg.y" /* yacc.c:1646  */
    { IF_NAPTR(default_core_cfg.dns_tls_pref=(yyvsp[0].intval));}
#line 5396 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 831 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 5402 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 832 "core/cfg.y" /* yacc.c:1646  */
    {
								IF_NAPTR(default_core_cfg.dns_sctp_pref=(yyvsp[0].intval)); }
#line 5409 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 834 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 5415 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 835 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.dns_retr_time=(yyvsp[0].intval); }
#line 5421 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 836 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 5427 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 837 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.dns_slow_query_ms=(yyvsp[0].intval); }
#line 5433 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 838 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 5439 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 839 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.dns_retr_no=(yyvsp[0].intval); }
#line 5445 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 840 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 5451 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 841 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.dns_servers_no=(yyvsp[0].intval); }
#line 5457 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 842 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 5463 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 843 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.dns_search_list=(yyvsp[0].intval); }
#line 5469 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 844 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5475 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 845 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.dns_search_fmatch=(yyvsp[0].intval); }
#line 5481 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 846 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5487 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 847 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.dns_naptr_ignore_rfc=(yyvsp[0].intval); }
#line 5493 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 848 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5499 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 849 "core/cfg.y" /* yacc.c:1646  */
    { IF_DNS_CACHE(dns_cache_init=(yyvsp[0].intval)); }
#line 5505 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 850 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5511 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 851 "core/cfg.y" /* yacc.c:1646  */
    { IF_DNS_CACHE(default_core_cfg.use_dns_cache=(yyvsp[0].intval)); }
#line 5517 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 852 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5523 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 853 "core/cfg.y" /* yacc.c:1646  */
    { IF_DNS_FAILOVER(default_core_cfg.use_dns_failover=(yyvsp[0].intval));}
#line 5529 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 854 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5535 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 855 "core/cfg.y" /* yacc.c:1646  */
    { IF_DNS_CACHE(default_core_cfg.dns_cache_flags=(yyvsp[0].intval)); }
#line 5541 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 856 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5547 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 857 "core/cfg.y" /* yacc.c:1646  */
    { IF_DNS_CACHE(default_core_cfg.dns_neg_cache_ttl=(yyvsp[0].intval)); }
#line 5553 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 141:
#line 858 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5559 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 142:
#line 859 "core/cfg.y" /* yacc.c:1646  */
    { IF_DNS_CACHE(default_core_cfg.dns_cache_max_ttl=(yyvsp[0].intval)); }
#line 5565 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 143:
#line 860 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5571 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 144:
#line 861 "core/cfg.y" /* yacc.c:1646  */
    { IF_DNS_CACHE(default_core_cfg.dns_cache_min_ttl=(yyvsp[0].intval)); }
#line 5577 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 862 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5583 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 146:
#line 863 "core/cfg.y" /* yacc.c:1646  */
    { IF_DNS_CACHE(default_core_cfg.dns_cache_max_mem=(yyvsp[0].intval)); }
#line 5589 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 147:
#line 864 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5595 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 148:
#line 865 "core/cfg.y" /* yacc.c:1646  */
    { IF_DNS_CACHE(dns_timer_interval=(yyvsp[0].intval)); }
#line 5601 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 149:
#line 866 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5607 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 150:
#line 867 "core/cfg.y" /* yacc.c:1646  */
    { IF_DNS_CACHE(default_core_cfg.dns_cache_del_nonexp=(yyvsp[0].intval)); }
#line 5613 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 151:
#line 868 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5619 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 152:
#line 869 "core/cfg.y" /* yacc.c:1646  */
    { IF_DNS_CACHE(default_core_cfg.dns_cache_rec_pref=(yyvsp[0].intval)); }
#line 5625 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 153:
#line 870 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5631 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 154:
#line 871 "core/cfg.y" /* yacc.c:1646  */
    {IF_AUTO_BIND_IPV6(auto_bind_ipv6 = (yyvsp[0].intval));}
#line 5637 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 155:
#line 872 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5643 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 156:
#line 873 "core/cfg.y" /* yacc.c:1646  */
    {sr_bind_ipv6_link_local = (yyvsp[0].intval);}
#line 5649 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 157:
#line 874 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5655 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 158:
#line 875 "core/cfg.y" /* yacc.c:1646  */
    { IF_DST_BLACKLIST(dst_blacklist_init=(yyvsp[0].intval)); }
#line 5661 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 159:
#line 876 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5667 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 160:
#line 877 "core/cfg.y" /* yacc.c:1646  */
    {
		IF_DST_BLACKLIST(default_core_cfg.use_dst_blacklist=(yyvsp[0].intval));
	}
#line 5675 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 161:
#line 880 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5681 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 162:
#line 881 "core/cfg.y" /* yacc.c:1646  */
    {
		IF_DST_BLACKLIST(default_core_cfg.blst_max_mem=(yyvsp[0].intval));
	}
#line 5689 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 163:
#line 884 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5695 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 164:
#line 885 "core/cfg.y" /* yacc.c:1646  */
    {
		IF_DST_BLACKLIST(default_core_cfg.blst_timeout=(yyvsp[0].intval));
	}
#line 5703 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 165:
#line 888 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5709 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 166:
#line 889 "core/cfg.y" /* yacc.c:1646  */
    { IF_DST_BLACKLIST(blst_timer_interval=(yyvsp[0].intval));}
#line 5715 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 167:
#line 890 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5721 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 168:
#line 891 "core/cfg.y" /* yacc.c:1646  */
    {
		IF_DST_BLACKLIST(default_core_cfg.blst_udp_imask=(yyvsp[0].intval));
	}
#line 5729 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 169:
#line 894 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number(flags) expected"); }
#line 5735 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 170:
#line 895 "core/cfg.y" /* yacc.c:1646  */
    {
		IF_DST_BLACKLIST(default_core_cfg.blst_tcp_imask=(yyvsp[0].intval));
	}
#line 5743 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 171:
#line 898 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number(flags) expected"); }
#line 5749 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 172:
#line 899 "core/cfg.y" /* yacc.c:1646  */
    {
		IF_DST_BLACKLIST(default_core_cfg.blst_tls_imask=(yyvsp[0].intval));
	}
#line 5757 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 173:
#line 902 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number(flags) expected"); }
#line 5763 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 174:
#line 903 "core/cfg.y" /* yacc.c:1646  */
    {
		IF_DST_BLACKLIST(default_core_cfg.blst_sctp_imask=(yyvsp[0].intval));
	}
#line 5771 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 175:
#line 906 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number(flags) expected"); }
#line 5777 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 176:
#line 907 "core/cfg.y" /* yacc.c:1646  */
    { _sr_ip_free_bind=(yyvsp[0].intval); }
#line 5783 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 177:
#line 908 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("int value expected"); }
#line 5789 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 178:
#line 909 "core/cfg.y" /* yacc.c:1646  */
    { port_no=(yyvsp[0].intval); }
#line 5795 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 179:
#line 910 "core/cfg.y" /* yacc.c:1646  */
    { maxbuffer=(yyvsp[0].intval); }
#line 5801 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 180:
#line 911 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 5807 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 181:
#line 912 "core/cfg.y" /* yacc.c:1646  */
    { sql_buffer_size=(yyvsp[0].intval); }
#line 5813 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 182:
#line 913 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 5819 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 183:
#line 914 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 5825 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 184:
#line 915 "core/cfg.y" /* yacc.c:1646  */
    { children_no=(yyvsp[0].intval); }
#line 5831 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 185:
#line 916 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 5837 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 186:
#line 917 "core/cfg.y" /* yacc.c:1646  */
    { socket_workers=(yyvsp[0].intval); }
#line 5843 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 187:
#line 918 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 5849 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 188:
#line 919 "core/cfg.y" /* yacc.c:1646  */
    { async_task_set_workers((yyvsp[0].intval)); }
#line 5855 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 189:
#line 920 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 5861 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 190:
#line 921 "core/cfg.y" /* yacc.c:1646  */
    { async_task_set_usleep((yyvsp[0].intval)); }
#line 5867 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 191:
#line 922 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 5873 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 192:
#line 923 "core/cfg.y" /* yacc.c:1646  */
    { async_task_set_nonblock((yyvsp[0].intval)); }
#line 5879 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 193:
#line 924 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 5885 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 194:
#line 925 "core/cfg.y" /* yacc.c:1646  */
    { check_via=(yyvsp[0].intval); }
#line 5891 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 195:
#line 926 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5897 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 196:
#line 927 "core/cfg.y" /* yacc.c:1646  */
    { phone2tel=(yyvsp[0].intval); }
#line 5903 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 197:
#line 928 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 5909 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 198:
#line 929 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.memlog=(yyvsp[0].intval); }
#line 5915 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 199:
#line 930 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("int value expected"); }
#line 5921 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 200:
#line 931 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.memdbg=(yyvsp[0].intval); }
#line 5927 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 201:
#line 932 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("int value expected"); }
#line 5933 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 202:
#line 933 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.mem_summary=(yyvsp[0].intval); }
#line 5939 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 203:
#line 934 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("int value expected"); }
#line 5945 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 204:
#line 935 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.mem_safety=(yyvsp[0].intval); }
#line 5951 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 205:
#line 936 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("int value expected"); }
#line 5957 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 206:
#line 937 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.mem_join=(yyvsp[0].intval); }
#line 5963 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 207:
#line 938 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("int value expected"); }
#line 5969 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 208:
#line 939 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.mem_status_mode=(yyvsp[0].intval); }
#line 5975 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 209:
#line 940 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("int value expected"); }
#line 5981 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 210:
#line 941 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.corelog=(yyvsp[0].intval); }
#line 5987 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 211:
#line 942 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("int value expected"); }
#line 5993 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 212:
#line 943 "core/cfg.y" /* yacc.c:1646  */
    { sip_warning=(yyvsp[0].intval); }
#line 5999 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 213:
#line 944 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 6005 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 214:
#line 945 "core/cfg.y" /* yacc.c:1646  */
    { version_table.s=(yyvsp[0].strval);
			version_table.len=strlen(version_table.s);
	}
#line 6013 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 215:
#line 948 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 6019 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 216:
#line 949 "core/cfg.y" /* yacc.c:1646  */
    {
		if (shm_initialized())
			yyerror("user must be before any modparam or the"
					" route blocks");
		else if (user==0)
			user=(yyvsp[0].strval);
	}
#line 6031 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 217:
#line 956 "core/cfg.y" /* yacc.c:1646  */
    {
		if (shm_initialized())
			yyerror("user must be before any modparam or the"
					" route blocks");
		else if (user==0)
			user=(yyvsp[0].strval);
	}
#line 6043 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 218:
#line 963 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 6049 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 219:
#line 964 "core/cfg.y" /* yacc.c:1646  */
    { group=(yyvsp[0].strval); }
#line 6055 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 220:
#line 965 "core/cfg.y" /* yacc.c:1646  */
    { group=(yyvsp[0].strval); }
#line 6061 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 221:
#line 966 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 6067 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 222:
#line 967 "core/cfg.y" /* yacc.c:1646  */
    { chroot_dir=(yyvsp[0].strval); }
#line 6073 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 223:
#line 968 "core/cfg.y" /* yacc.c:1646  */
    { chroot_dir=(yyvsp[0].strval); }
#line 6079 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 224:
#line 969 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 6085 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 225:
#line 970 "core/cfg.y" /* yacc.c:1646  */
    { working_dir=(yyvsp[0].strval); }
#line 6091 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 226:
#line 971 "core/cfg.y" /* yacc.c:1646  */
    { working_dir=(yyvsp[0].strval); }
#line 6097 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 227:
#line 972 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 6103 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 228:
#line 973 "core/cfg.y" /* yacc.c:1646  */
    { runtime_dir=(yyvsp[0].strval); }
#line 6109 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 229:
#line 974 "core/cfg.y" /* yacc.c:1646  */
    { runtime_dir=(yyvsp[0].strval); }
#line 6115 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 230:
#line 975 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 6121 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 231:
#line 976 "core/cfg.y" /* yacc.c:1646  */
    { mhomed=(yyvsp[0].intval); }
#line 6127 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 232:
#line 977 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 6133 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 233:
#line 978 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_disable=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6145 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 234:
#line 985 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 6151 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 235:
#line 986 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_default_cfg.accept_aliases=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6163 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 236:
#line 993 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 6169 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 237:
#line 994 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_accept_unique=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6181 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 238:
#line 1001 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 6187 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 239:
#line 1002 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_connection_match=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6199 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 240:
#line 1009 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 6205 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 241:
#line 1010 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_cfg_children_no=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6217 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 242:
#line 1017 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 6223 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 243:
#line 1018 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_default_cfg.connect_timeout_s=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6235 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 244:
#line 1025 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 6241 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 245:
#line 1026 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_default_cfg.send_timeout=S_TO_TICKS((yyvsp[0].intval));
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6253 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 246:
#line 1033 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 6259 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 247:
#line 1034 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			if ((yyvsp[0].intval)<0)
				tcp_default_cfg.con_lifetime=-1;
			else
				tcp_default_cfg.con_lifetime=S_TO_TICKS((yyvsp[0].intval));
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6274 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 248:
#line 1044 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 6280 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 249:
#line 1045 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_poll_method=get_poll_type((yyvsp[0].strval));
			if (tcp_poll_method==POLL_NONE) {
				LOG(L_CRIT, "bad poll method name:"
						" %s\n, try one of %s.\n",
						(yyvsp[0].strval), poll_support);
				yyerror("bad tcp_poll_method "
						"value");
			}
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6299 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 250:
#line 1059 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_poll_method=get_poll_type((yyvsp[0].strval));
			if (tcp_poll_method==POLL_NONE) {
				LOG(L_CRIT, "bad poll method name:"
						" %s\n, try one of %s.\n",
						(yyvsp[0].strval), poll_support);
				yyerror("bad tcp_poll_method "
						"value");
			}
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6318 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 251:
#line 1073 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("poll method name expected"); }
#line 6324 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 252:
#line 1074 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_max_connections=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6336 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 253:
#line 1081 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 6342 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 254:
#line 1082 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			tls_max_connections=(yyvsp[0].intval);
		#else
			warn("tls support not compiled in");
		#endif
	}
#line 6354 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 255:
#line 1089 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 6360 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 256:
#line 1090 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_default_cfg.no_connect=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6372 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 257:
#line 1097 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 6378 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 258:
#line 1098 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			if (tcp_set_src_addr((yyvsp[0].ipaddr))<0)
				warn("tcp_source_ipv4 failed");
		#else
			warn("tcp support not compiled in");
		#endif
		pkg_free((yyvsp[0].ipaddr));
	}
#line 6392 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 259:
#line 1107 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("IPv4 address expected"); }
#line 6398 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 260:
#line 1108 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
				if (tcp_set_src_addr((yyvsp[0].ipaddr))<0)
					warn("tcp_source_ipv6 failed");
		#else
			warn("tcp support not compiled in");
		#endif
		pkg_free((yyvsp[0].ipaddr));
	}
#line 6412 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 261:
#line 1117 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("IPv6 address expected"); }
#line 6418 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 262:
#line 1118 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_default_cfg.fd_cache=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6430 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 263:
#line 1125 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 6436 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 264:
#line 1126 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_default_cfg.async=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6448 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 265:
#line 1133 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 6454 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 266:
#line 1134 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_default_cfg.tcpconn_wq_max=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6466 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 267:
#line 1141 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 6472 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 268:
#line 1142 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_default_cfg.tcp_wq_max=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6484 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 269:
#line 1149 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 6490 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 270:
#line 1150 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_default_cfg.rd_buf_size=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6502 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 271:
#line 1157 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 6508 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 272:
#line 1158 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_default_cfg.wq_blk_size=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6520 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 273:
#line 1165 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 6526 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 274:
#line 1166 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_default_cfg.defer_accept=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6538 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 275:
#line 1173 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 6544 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 276:
#line 1174 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_default_cfg.delayed_ack=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6556 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 277:
#line 1181 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 6562 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 278:
#line 1182 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_default_cfg.syncnt=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6574 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 279:
#line 1189 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 6580 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 280:
#line 1190 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_default_cfg.linger2=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6592 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 281:
#line 1197 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 6598 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 282:
#line 1198 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_default_cfg.keepalive=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6610 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 283:
#line 1205 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected");}
#line 6616 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 284:
#line 1206 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_default_cfg.keepidle=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6628 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 285:
#line 1213 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 6634 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 286:
#line 1214 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_default_cfg.keepintvl=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6646 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 287:
#line 1221 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 6652 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 288:
#line 1222 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_default_cfg.keepcnt=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6664 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 289:
#line 1229 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 6670 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 290:
#line 1230 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_default_cfg.crlf_ping=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6682 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 291:
#line 1237 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 6688 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 292:
#line 1238 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_default_cfg.accept_no_cl=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6700 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 293:
#line 1245 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 6706 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 294:
#line 1246 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			ksr_tcp_accept_hep3=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6718 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 295:
#line 1253 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 6724 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 296:
#line 1254 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			ksr_tcp_accept_haproxy=(yyvsp[0].intval);
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6736 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 297:
#line 1261 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 6742 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 298:
#line 1263 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			tcp_set_clone_rcvbuf((yyvsp[0].intval));
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6754 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 299:
#line 1270 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 6760 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 300:
#line 1271 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
		#ifdef SO_REUSEPORT
			tcp_default_cfg.reuse_port=(yyvsp[0].intval);
		#else
			warn("support for SO_REUSEPORT not compiled in");
		#endif
		#else
			warn("tcp support not compiled in");
		#endif
	}
#line 6776 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 301:
#line 1282 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 6782 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 302:
#line 1283 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			tls_disable=(yyvsp[0].intval);
		#else
			warn("tls support not compiled in");
		#endif
	}
#line 6794 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 303:
#line 1290 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 6800 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 304:
#line 1291 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			tls_disable=!((yyvsp[0].intval));
		#else
			warn("tls support not compiled in");
		#endif
	}
#line 6812 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 305:
#line 1298 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 6818 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 306:
#line 1299 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_log=(yyvsp[0].intval);
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 6830 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 307:
#line 1306 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("int value expected"); }
#line 6836 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 308:
#line 1307 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			tls_port_no=(yyvsp[0].intval);
		#else
			warn("tls support not compiled in");
		#endif
	}
#line 6848 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 309:
#line 1314 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 6854 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 310:
#line 1315 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_method=TLS_USE_SSLv23;
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 6866 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 311:
#line 1322 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_method=TLS_USE_SSLv2;
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 6878 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 312:
#line 1329 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_method=TLS_USE_SSLv3;
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 6890 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 313:
#line 1336 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_method=TLS_USE_TLSv1;
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 6902 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 314:
#line 1343 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			yyerror("SSLv23, SSLv2, SSLv3 or TLSv1 expected");
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 6914 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 315:
#line 1350 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_verify_cert=(yyvsp[0].intval);
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 6926 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 316:
#line 1357 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 6932 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 317:
#line 1358 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_require_cert=(yyvsp[0].intval);
		#else
			warn( "tls-in-core support not compiled in");
		#endif
	}
#line 6944 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 318:
#line 1365 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 6950 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 319:
#line 1366 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_cert_file=(yyvsp[0].strval);
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 6962 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 320:
#line 1373 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 6968 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 321:
#line 1374 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_pkey_file=(yyvsp[0].strval);
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 6980 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 322:
#line 1381 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 6986 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 323:
#line 1382 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_ca_file=(yyvsp[0].strval);
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 6998 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 324:
#line 1389 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 7004 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 325:
#line 1390 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_handshake_timeout=(yyvsp[0].intval);
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 7016 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 326:
#line 1397 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 7022 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 327:
#line 1398 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef CORE_TLS
			tls_send_timeout=(yyvsp[0].intval);
		#else
			warn("tls-in-core support not compiled in");
		#endif
	}
#line 7034 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 328:
#line 1405 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 7040 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 329:
#line 1406 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_SCTP
			sctp_disable=(yyvsp[0].intval);
		#else
			warn("sctp support not compiled in");
		#endif
	}
#line 7052 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 330:
#line 1413 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 7058 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 331:
#line 1414 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_SCTP
			sctp_disable=((yyvsp[0].intval)<=1)?!(yyvsp[0].intval):(yyvsp[0].intval);
		#else
			warn("sctp support not compiled in");
		#endif
	}
#line 7070 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 332:
#line 1421 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean or number expected"); }
#line 7076 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 333:
#line 1422 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_SCTP
			sctp_children_no=(yyvsp[0].intval);
		#else
			warn("sctp support not compiled in");
		#endif
	}
#line 7088 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 334:
#line 1429 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 7094 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 335:
#line 1430 "core/cfg.y" /* yacc.c:1646  */
    { server_signature=(yyvsp[0].intval); }
#line 7100 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 336:
#line 1431 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 7106 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 337:
#line 1432 "core/cfg.y" /* yacc.c:1646  */
    { server_hdr.s=(yyvsp[0].strval);
			server_hdr.len=strlen(server_hdr.s);
	}
#line 7114 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 338:
#line 1435 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 7120 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 339:
#line 1436 "core/cfg.y" /* yacc.c:1646  */
    { user_agent_hdr.s=(yyvsp[0].strval);
			user_agent_hdr.len=strlen(user_agent_hdr.s);
	}
#line 7128 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 340:
#line 1439 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string value expected"); }
#line 7134 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 341:
#line 1440 "core/cfg.y" /* yacc.c:1646  */
    { reply_to_via=(yyvsp[0].intval); }
#line 7140 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 342:
#line 1441 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 7146 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 343:
#line 1442 "core/cfg.y" /* yacc.c:1646  */
    {
		for(lst_tmp=(yyvsp[0].sockid); lst_tmp; lst_tmp=lst_tmp->next) {
			if (add_listen_iface(	lst_tmp->addr_lst->name,
									lst_tmp->addr_lst->next,
									lst_tmp->port, lst_tmp->proto,
									lst_tmp->flags)!=0) {
				LM_CRIT("cfg. parser: failed to add listen address\n");
				break;
			}
		}
		free_socket_id_lst((yyvsp[0].sockid));
	}
#line 7163 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 344:
#line 1454 "core/cfg.y" /* yacc.c:1646  */
    {
		for(lst_tmp=(yyvsp[-4].sockid); lst_tmp; lst_tmp=lst_tmp->next) {
			if (add_listen_advertise_iface(	lst_tmp->addr_lst->name,
									lst_tmp->addr_lst->next,
									lst_tmp->port, lst_tmp->proto,
									(yyvsp[-2].strval), (yyvsp[0].intval),
									lst_tmp->flags)!=0) {
				LM_CRIT("cfg. parser: failed to add listen address\n");
				break;
			}
		}
		free_socket_id_lst((yyvsp[-4].sockid));
	}
#line 7181 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 345:
#line 1467 "core/cfg.y" /* yacc.c:1646  */
    {
		for(lst_tmp=(yyvsp[-2].sockid); lst_tmp; lst_tmp=lst_tmp->next) {
			if (add_listen_advertise_iface(	lst_tmp->addr_lst->name,
									lst_tmp->addr_lst->next,
									lst_tmp->port, lst_tmp->proto,
									(yyvsp[0].strval), 0,
									lst_tmp->flags)!=0) {
				LM_CRIT("cfg. parser: failed to add listen address\n");
				break;
			}
		}
		free_socket_id_lst((yyvsp[-2].sockid));
	}
#line 7199 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 346:
#line 1480 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("ip address, interface name or"
									" hostname expected"); }
#line 7206 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 347:
#line 1482 "core/cfg.y" /* yacc.c:1646  */
    {
		for(lst_tmp=(yyvsp[0].sockid); lst_tmp; lst_tmp=lst_tmp->next){
			add_alias(	lst_tmp->addr_lst->name,
						strlen(lst_tmp->addr_lst->name),
						lst_tmp->port, lst_tmp->proto);
			for (nl_tmp=lst_tmp->addr_lst->next; nl_tmp; nl_tmp=nl_tmp->next)
				add_alias(nl_tmp->name, strlen(nl_tmp->name),
							lst_tmp->port, lst_tmp->proto);
		}
		free_socket_id_lst((yyvsp[0].sockid));
	}
#line 7222 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 348:
#line 1493 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("hostname expected"); }
#line 7228 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 349:
#line 1494 "core/cfg.y" /* yacc.c:1646  */
    { sr_auto_aliases=(yyvsp[0].intval); }
#line 7234 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 350:
#line 1495 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 7240 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 351:
#line 1496 "core/cfg.y" /* yacc.c:1646  */
    {
		if ((yyvsp[0].strval)){
			default_global_address.s=(yyvsp[0].strval);
			default_global_address.len=strlen((yyvsp[0].strval));
		}
	}
#line 7251 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 352:
#line 1502 "core/cfg.y" /* yacc.c:1646  */
    {yyerror("ip address or hostname expected"); }
#line 7257 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 353:
#line 1503 "core/cfg.y" /* yacc.c:1646  */
    {
		tmp=int2str((yyvsp[0].intval), &i_tmp);
		if ((default_global_port.s=pkg_malloc(i_tmp))==0) {
			PKG_MEM_CRITICAL;
			default_global_port.len=0;
		} else {
			default_global_port.len=i_tmp;
			memcpy(default_global_port.s, tmp, default_global_port.len);
		};
	}
#line 7272 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 354:
#line 1513 "core/cfg.y" /* yacc.c:1646  */
    {yyerror("ip address or hostname expected"); }
#line 7278 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 355:
#line 1514 "core/cfg.y" /* yacc.c:1646  */
    { disable_core_dump=(yyvsp[0].intval); }
#line 7284 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 356:
#line 1515 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 7290 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 357:
#line 1516 "core/cfg.y" /* yacc.c:1646  */
    { open_files_limit=(yyvsp[0].intval); }
#line 7296 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 358:
#line 1517 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 7302 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 359:
#line 1518 "core/cfg.y" /* yacc.c:1646  */
    {
		if (shm_initialized())
			yyerror("shm/shm_mem_size must be before any modparam or the"
					" route blocks");
		else if (shm_mem_size == 0 || shm_mem_size == SHM_MEM_POOL_SIZE)
			shm_mem_size=(yyvsp[0].intval) * 1024 * 1024;
	}
#line 7314 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 360:
#line 1525 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 7320 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 361:
#line 1526 "core/cfg.y" /* yacc.c:1646  */
    {
		if (shm_initialized())
			yyerror("shm_force_alloc must be before any modparam or the"
					" route blocks");
		else
			shm_force_alloc=(yyvsp[0].intval);
	}
#line 7332 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 362:
#line 1533 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 7338 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 363:
#line 1534 "core/cfg.y" /* yacc.c:1646  */
    { mlock_pages=(yyvsp[0].intval); }
#line 7344 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 364:
#line 1535 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 7350 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 365:
#line 1536 "core/cfg.y" /* yacc.c:1646  */
    { real_time=(yyvsp[0].intval); }
#line 7356 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 366:
#line 1537 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 7362 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 367:
#line 1538 "core/cfg.y" /* yacc.c:1646  */
    { rt_prio=(yyvsp[0].intval); }
#line 7368 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 368:
#line 1539 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 7374 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 369:
#line 1540 "core/cfg.y" /* yacc.c:1646  */
    { rt_policy=(yyvsp[0].intval); }
#line 7380 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 370:
#line 1541 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 7386 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 371:
#line 1542 "core/cfg.y" /* yacc.c:1646  */
    { rt_timer1_prio=(yyvsp[0].intval); }
#line 7392 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 372:
#line 1543 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 7398 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 373:
#line 1544 "core/cfg.y" /* yacc.c:1646  */
    { rt_timer1_policy=(yyvsp[0].intval); }
#line 7404 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 374:
#line 1545 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 7410 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 375:
#line 1546 "core/cfg.y" /* yacc.c:1646  */
    { rt_timer2_prio=(yyvsp[0].intval); }
#line 7416 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 376:
#line 1547 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 7422 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 377:
#line 1548 "core/cfg.y" /* yacc.c:1646  */
    { rt_timer2_policy=(yyvsp[0].intval); }
#line 7428 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 378:
#line 1549 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 7434 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 379:
#line 1550 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_MCAST
			mcast_loopback=(yyvsp[0].intval);
		#else
			warn("no multicast support compiled in");
		#endif
	}
#line 7446 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 380:
#line 1557 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 7452 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 381:
#line 1558 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_MCAST
			mcast_ttl=(yyvsp[0].intval);
		#else
			warn("no multicast support compiled in");
		#endif
	}
#line 7464 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 382:
#line 1565 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 7470 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 383:
#line 1566 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_MCAST
			mcast=(yyvsp[0].strval);
		#else
			warn("no multicast support compiled in");
		#endif
	}
#line 7482 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 384:
#line 1573 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_MCAST
			mcast=(yyvsp[0].strval);
		#else
			warn("no multicast support compiled in");
		#endif
	}
#line 7494 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 385:
#line 1580 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string expected"); }
#line 7500 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 386:
#line 1581 "core/cfg.y" /* yacc.c:1646  */
    { tos=(yyvsp[0].intval); }
#line 7506 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 387:
#line 1582 "core/cfg.y" /* yacc.c:1646  */
    { if (strcasecmp((yyvsp[0].strval),"IPTOS_LOWDELAY")) {
			tos=IPTOS_LOWDELAY;
		} else if (strcasecmp((yyvsp[0].strval),"IPTOS_THROUGHPUT")) {
			tos=IPTOS_THROUGHPUT;
		} else if (strcasecmp((yyvsp[0].strval),"IPTOS_RELIABILITY")) {
			tos=IPTOS_RELIABILITY;
#if defined(IPTOS_MINCOST)
		} else if (strcasecmp((yyvsp[0].strval),"IPTOS_MINCOST")) {
			tos=IPTOS_MINCOST;
#endif
#if defined(IPTOS_LOWCOST)
		} else if (strcasecmp((yyvsp[0].strval),"IPTOS_LOWCOST")) {
			tos=IPTOS_LOWCOST;
#endif
		} else {
			yyerror("invalid tos value - allowed: "
				"IPTOS_LOWDELAY,IPTOS_THROUGHPUT,"
				"IPTOS_RELIABILITY"
#if defined(IPTOS_LOWCOST)
				",IPTOS_LOWCOST"
#endif
#if !defined(IPTOS_MINCOST)
				",IPTOS_MINCOST"
#endif
				"\n");
		}
	}
#line 7538 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 388:
#line 1609 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 7544 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 389:
#line 1610 "core/cfg.y" /* yacc.c:1646  */
    { pmtu_discovery=(yyvsp[0].intval); }
#line 7550 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 390:
#line 1611 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 7556 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 391:
#line 1612 "core/cfg.y" /* yacc.c:1646  */
    { ser_kill_timeout=(yyvsp[0].intval); }
#line 7562 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 392:
#line 1613 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 7568 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 393:
#line 1614 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.max_while_loops=(yyvsp[0].intval); }
#line 7574 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 394:
#line 1615 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 7580 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 395:
#line 1616 "core/cfg.y" /* yacc.c:1646  */
    { pv_set_buffer_size((yyvsp[0].intval)); }
#line 7586 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 396:
#line 1617 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 7592 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 397:
#line 1618 "core/cfg.y" /* yacc.c:1646  */
    { pv_set_buffer_slots((yyvsp[0].intval)); }
#line 7598 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 398:
#line 1619 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 7604 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 399:
#line 1620 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.pv_cache_limit=(yyvsp[0].intval); }
#line 7610 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 400:
#line 1621 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 7616 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 401:
#line 1622 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.pv_cache_action=(yyvsp[0].intval); }
#line 7622 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 402:
#line 1623 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 7628 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 403:
#line 1624 "core/cfg.y" /* yacc.c:1646  */
    { http_reply_parse=(yyvsp[0].intval); }
#line 7634 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 404:
#line 1625 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 7640 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 405:
#line 1626 "core/cfg.y" /* yacc.c:1646  */
    { ksr_verbose_startup=(yyvsp[0].intval); }
#line 7646 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 406:
#line 1627 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 7652 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 407:
#line 1628 "core/cfg.y" /* yacc.c:1646  */
    { ksr_route_locks_size=(yyvsp[0].intval); }
#line 7658 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 408:
#line 1629 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 7664 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 409:
#line 1630 "core/cfg.y" /* yacc.c:1646  */
    { server_id=(yyvsp[0].intval); }
#line 7670 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 410:
#line 1631 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 7676 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 411:
#line 1632 "core/cfg.y" /* yacc.c:1646  */
    {
			kemi_onsend_route_callback.s = (yyvsp[0].strval);
			kemi_onsend_route_callback.len = strlen((yyvsp[0].strval));
			if(kemi_onsend_route_callback.len==4
					&& strcasecmp(kemi_onsend_route_callback.s, "none")==0) {
				kemi_onsend_route_callback.s = "";
				kemi_onsend_route_callback.len = 0;
			}
		}
#line 7690 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 412:
#line 1641 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string expected"); }
#line 7696 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 413:
#line 1642 "core/cfg.y" /* yacc.c:1646  */
    {
			kemi_reply_route_callback.s = (yyvsp[0].strval);
			kemi_reply_route_callback.len = strlen((yyvsp[0].strval));
			if(kemi_reply_route_callback.len==4
					&& strcasecmp(kemi_reply_route_callback.s, "none")==0) {
				kemi_reply_route_callback.s = "";
				kemi_reply_route_callback.len = 0;
			}
		}
#line 7710 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 414:
#line 1651 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string expected"); }
#line 7716 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 415:
#line 1652 "core/cfg.y" /* yacc.c:1646  */
    {
			kemi_event_route_callback.s = (yyvsp[0].strval);
			kemi_event_route_callback.len = strlen((yyvsp[0].strval));
			if(kemi_event_route_callback.len==4
					&& strcasecmp(kemi_event_route_callback.s, "none")==0) {
				kemi_event_route_callback.s = "";
				kemi_event_route_callback.len = 0;
			}
		}
#line 7730 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 416:
#line 1661 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string expected"); }
#line 7736 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 417:
#line 1662 "core/cfg.y" /* yacc.c:1646  */
    {
			kemi_received_route_callback.s = (yyvsp[0].strval);
			kemi_received_route_callback.len = strlen((yyvsp[0].strval));
			if(kemi_received_route_callback.len==4
					&& strcasecmp(kemi_received_route_callback.s, "none")==0) {
				kemi_received_route_callback.s = "";
				kemi_received_route_callback.len = 0;
			}
		}
#line 7750 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 418:
#line 1671 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string expected"); }
#line 7756 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 419:
#line 1672 "core/cfg.y" /* yacc.c:1646  */
    { ksr_evrt_received_mode=(yyvsp[0].intval); }
#line 7762 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 420:
#line 1673 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number  expected"); }
#line 7768 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 421:
#line 1674 "core/cfg.y" /* yacc.c:1646  */
    { set_max_recursive_level((yyvsp[0].intval)); }
#line 7774 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 422:
#line 1675 "core/cfg.y" /* yacc.c:1646  */
    { sr_dst_max_branches = (yyvsp[0].intval); }
#line 7780 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 423:
#line 1676 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.latency_log=(yyvsp[0].intval); }
#line 7786 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 424:
#line 1677 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number  expected"); }
#line 7792 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 425:
#line 1678 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.latency_cfg_log=(yyvsp[0].intval); }
#line 7798 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 426:
#line 1679 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number  expected"); }
#line 7804 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 427:
#line 1680 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.latency_limit_db=(yyvsp[0].intval); }
#line 7810 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 428:
#line 1681 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number  expected"); }
#line 7816 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 429:
#line 1682 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.latency_limit_action=(yyvsp[0].intval); }
#line 7822 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 430:
#line 1683 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number  expected"); }
#line 7828 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 431:
#line 1684 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.latency_limit_cfg=(yyvsp[0].intval); }
#line 7834 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 432:
#line 1685 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number  expected"); }
#line 7840 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 433:
#line 1686 "core/cfg.y" /* yacc.c:1646  */
    { sr_msg_time=(yyvsp[0].intval); }
#line 7846 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 434:
#line 1687 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number  expected"); }
#line 7852 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 435:
#line 1688 "core/cfg.y" /* yacc.c:1646  */
    { onsend_route_reply=(yyvsp[0].intval); }
#line 7858 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 436:
#line 1689 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("int value expected"); }
#line 7864 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 437:
#line 1690 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.udp_mtu=(yyvsp[0].intval); }
#line 7870 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 438:
#line 1691 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 7876 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 439:
#line 1693 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.force_rport=(yyvsp[0].intval); fix_global_req_flags(0, 0); }
#line 7882 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 440:
#line 1694 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("boolean value expected"); }
#line 7888 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 441:
#line 1696 "core/cfg.y" /* yacc.c:1646  */
    { default_core_cfg.udp_mtu_try_proto=(yyvsp[0].intval); fix_global_req_flags(0, 0); }
#line 7894 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 442:
#line 1698 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("TCP, TLS, SCTP or UDP expected"); }
#line 7900 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 443:
#line 1699 "core/cfg.y" /* yacc.c:1646  */
    { IF_RAW_SOCKS(default_core_cfg.udp4_raw=(yyvsp[0].intval)); }
#line 7906 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 444:
#line 1700 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 7912 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 445:
#line 1701 "core/cfg.y" /* yacc.c:1646  */
    {
		IF_RAW_SOCKS(default_core_cfg.udp4_raw_mtu=(yyvsp[0].intval));
	}
#line 7920 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 446:
#line 1704 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 7926 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 447:
#line 1705 "core/cfg.y" /* yacc.c:1646  */
    {
		IF_RAW_SOCKS(default_core_cfg.udp4_raw_ttl=(yyvsp[0].intval));
	}
#line 7934 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 448:
#line 1708 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("number expected"); }
#line 7940 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 450:
#line 1710 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("unknown config variable"); }
#line 7946 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 452:
#line 1714 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.strval)="default" ; }
#line 7952 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 454:
#line 1718 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.strval)="default" ; }
#line 7958 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 455:
#line 1719 "core/cfg.y" /* yacc.c:1646  */
    {
		yyerror("cfg var field name - use of number or reserved token not allowed: %s",
				yy_number_str);
		YYERROR;
	}
#line 7968 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 456:
#line 1727 "core/cfg.y" /* yacc.c:1646  */
    {
		if (cfg_declare_int((yyvsp[-4].strval), (yyvsp[-2].strval), (yyvsp[0].intval), 0, 0, NULL)) {
			yyerror("variable cannot be declared");
		}
	}
#line 7978 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 457:
#line 1732 "core/cfg.y" /* yacc.c:1646  */
    {
		if (cfg_declare_str((yyvsp[-4].strval), (yyvsp[-2].strval), (yyvsp[0].strval), NULL)) {
			yyerror("variable cannot be declared");
		}
	}
#line 7988 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 458:
#line 1737 "core/cfg.y" /* yacc.c:1646  */
    {
		if (cfg_declare_int((yyvsp[-6].strval), (yyvsp[-4].strval), (yyvsp[-2].intval), 0, 0, (yyvsp[0].strval))) {
			yyerror("variable cannot be declared");
		}
	}
#line 7998 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 459:
#line 1742 "core/cfg.y" /* yacc.c:1646  */
    {
		if (cfg_declare_str((yyvsp[-6].strval), (yyvsp[-4].strval), (yyvsp[-2].strval), (yyvsp[0].strval))) {
			yyerror("variable cannot be declared");
		}
	}
#line 8008 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 460:
#line 1747 "core/cfg.y" /* yacc.c:1646  */
    {
		yyerror("number or string expected");
	}
#line 8016 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 461:
#line 1750 "core/cfg.y" /* yacc.c:1646  */
    {
		if (cfg_ginst_var_int((yyvsp[-7].strval), (yyvsp[-5].intval), (yyvsp[-2].strval), (yyvsp[0].intval))) {
			yyerror("variable cannot be added to the group instance");
		}
	}
#line 8026 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 462:
#line 1755 "core/cfg.y" /* yacc.c:1646  */
    {
		if (cfg_ginst_var_string((yyvsp[-7].strval), (yyvsp[-5].intval), (yyvsp[-2].strval), (yyvsp[0].strval))) {
			yyerror("variable cannot be added to the group instance");
		}
	}
#line 8036 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 463:
#line 1763 "core/cfg.y" /* yacc.c:1646  */
    {
		LM_DBG("loading module %s\n", (yyvsp[0].strval));
			if (load_module((yyvsp[0].strval))!=0) {
				yyerror("failed to load module");
			}
	}
#line 8047 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 464:
#line 1769 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string expected"); }
#line 8053 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 465:
#line 1770 "core/cfg.y" /* yacc.c:1646  */
    {
		if(mods_dir_cmd==0) {
			LM_DBG("loading modules under %s\n", (yyvsp[0].strval));
			printf("loading modules under config path: %s\n", (yyvsp[0].strval));
			mods_dir = (yyvsp[0].strval);
		} else {
			LM_DBG("ignoring mod path given in config: %s\n", (yyvsp[0].strval));
			printf("loading modules under command line path: %s\n", mods_dir);
		}
	}
#line 8068 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 466:
#line 1780 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string expected"); }
#line 8074 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 467:
#line 1781 "core/cfg.y" /* yacc.c:1646  */
    {
		if(mods_dir_cmd==0) {
			LM_DBG("loading modules under %s\n", (yyvsp[0].strval));
			printf("loading modules under config path: %s\n", (yyvsp[0].strval));
			mods_dir = (yyvsp[0].strval);
		} else {
			LM_DBG("ignoring mod path given in config: %s\n", (yyvsp[0].strval));
			printf("loading modules under command line path: %s\n", mods_dir);
		}
	}
#line 8089 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 468:
#line 1791 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string expected"); }
#line 8095 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 469:
#line 1792 "core/cfg.y" /* yacc.c:1646  */
    {
		if (!shm_initialized() && init_shm()<0) {
			yyerror("Can't initialize shared memory");
			YYABORT;
		}
		if (set_mod_param_regex((yyvsp[-5].strval), (yyvsp[-3].strval), PARAM_STRING, (yyvsp[-1].strval)) != 0) {
			 yyerror("Can't set module parameter");
		}
	}
#line 8109 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 470:
#line 1801 "core/cfg.y" /* yacc.c:1646  */
    {
		if (!shm_initialized() && init_shm()<0) {
			yyerror("Can't initialize shared memory");
			YYABORT;
		}
		if (set_mod_param_regex((yyvsp[-5].strval), (yyvsp[-3].strval), PARAM_INT, (void*)(yyvsp[-1].intval)) != 0) {
			 yyerror("Can't set module parameter");
		}
	}
#line 8123 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 471:
#line 1810 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("Invalid arguments"); }
#line 8129 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 472:
#line 1811 "core/cfg.y" /* yacc.c:1646  */
    {
		if(sr_kemi_eng_setz((yyvsp[0].strval), NULL)) {
			yyerror("Can't set config routing engine");
			YYABORT;
		}
	}
#line 8140 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 473:
#line 1817 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string expected"); }
#line 8146 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 474:
#line 1818 "core/cfg.y" /* yacc.c:1646  */
    {
		if(sr_kemi_eng_setz((yyvsp[0].strval), NULL)) {
			yyerror("Can't set config routing engine");
			YYABORT;
		}
	}
#line 8157 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 475:
#line 1824 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("string expected"); }
#line 8163 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 476:
#line 1828 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.ipaddr)=(yyvsp[0].ipaddr); }
#line 8169 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 477:
#line 1829 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.ipaddr)=(yyvsp[0].ipaddr); }
#line 8175 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 478:
#line 1832 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.ipaddr)=pkg_malloc(sizeof(struct ip_addr));
		if ((yyval.ipaddr)==0) {
			PKG_MEM_CRITICAL;
		} else {
			memset((yyval.ipaddr), 0, sizeof(struct ip_addr));
			(yyval.ipaddr)->af=AF_INET;
			(yyval.ipaddr)->len=4;
			if (((yyvsp[-6].intval)>255) || ((yyvsp[-6].intval)<0) ||
				((yyvsp[-4].intval)>255) || ((yyvsp[-4].intval)<0) ||
				((yyvsp[-2].intval)>255) || ((yyvsp[-2].intval)<0) ||
				((yyvsp[0].intval)>255) || ((yyvsp[0].intval)<0)) {
				yyerror("invalid ipv4 address");
				(yyval.ipaddr)->u.addr32[0]=0;
				/* $$=0; */
			} else {
				(yyval.ipaddr)->u.addr[0]=(yyvsp[-6].intval);
				(yyval.ipaddr)->u.addr[1]=(yyvsp[-4].intval);
				(yyval.ipaddr)->u.addr[2]=(yyvsp[-2].intval);
				(yyval.ipaddr)->u.addr[3]=(yyvsp[0].intval);
				/*
				$$=htonl( ($1<<24)|
				($3<<16)| ($5<<8)|$7 );
				*/
			}
		}
	}
#line 8207 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 479:
#line 1861 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.ipaddr)=pkg_malloc(sizeof(struct ip_addr));
		if ((yyval.ipaddr)==0) {
			PKG_MEM_CRITICAL;
		} else {
			memset((yyval.ipaddr), 0, sizeof(struct ip_addr));
			(yyval.ipaddr)->af=AF_INET6;
			(yyval.ipaddr)->len=16;
			if (inet_pton(AF_INET6, (yyvsp[0].strval), (yyval.ipaddr)->u.addr)<=0) {
				yyerror("bad ipv6 address");
			}
		}
	}
#line 8225 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 480:
#line 1876 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.ipaddr)=(yyvsp[0].ipaddr); }
#line 8231 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 481:
#line 1877 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.ipaddr)=(yyvsp[-1].ipaddr); }
#line 8237 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 482:
#line 1881 "core/cfg.y" /* yacc.c:1646  */
    {
					tmp=int2str((yyvsp[0].intval), &i_tmp);
					if (((yyval.strval)=pkg_malloc(i_tmp+1))==0) {
						yyerror("out of  memory");
						YYABORT;
					} else {
						memcpy((yyval.strval), tmp, i_tmp);
						(yyval.strval)[i_tmp]=0;
					}
					routename = tmp;
						}
#line 8253 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 483:
#line 1892 "core/cfg.y" /* yacc.c:1646  */
    { routename = (yyvsp[0].strval); (yyval.strval)=(yyvsp[0].strval); }
#line 8259 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 484:
#line 1893 "core/cfg.y" /* yacc.c:1646  */
    { routename = (yyvsp[0].strval); (yyval.strval)=(yyvsp[0].strval); }
#line 8265 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 485:
#line 1897 "core/cfg.y" /* yacc.c:1646  */
    { routename=NULL; }
#line 8271 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 486:
#line 1898 "core/cfg.y" /* yacc.c:1646  */
    { routename=NULL; }
#line 8277 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 487:
#line 1902 "core/cfg.y" /* yacc.c:1646  */
    {
		if (!shm_initialized() && init_shm()<0) {
			yyerror("Can't initialize shared memory");
			YYABORT;
		}
		push((yyvsp[-1].action), &main_rt.rlist[DEFAULT_RT]);
	}
#line 8289 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 488:
#line 1909 "core/cfg.y" /* yacc.c:1646  */
    {
		if (!shm_initialized() && init_shm()<0) {
			yyerror("Can't initialize shared memory");
			YYABORT;
		}
		i_tmp=route_get(&main_rt, (yyvsp[-4].strval));
		if (i_tmp==-1){
			yyerror("internal error");
			YYABORT;
		}
		if (main_rt.rlist[i_tmp]){
			yyerror("duplicate route");
			YYABORT;
		}
		push((yyvsp[-1].action), &main_rt.rlist[i_tmp]);
	}
#line 8310 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 489:
#line 1925 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("invalid  route  statement"); }
#line 8316 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 490:
#line 1926 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("invalid  request_route  statement"); }
#line 8322 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 491:
#line 1929 "core/cfg.y" /* yacc.c:1646  */
    { routename=NULL; }
#line 8328 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 492:
#line 1932 "core/cfg.y" /* yacc.c:1646  */
    {
		if (!shm_initialized() && init_shm()<0) {
			yyerror("Can't initialize shared memory");
			YYABORT;
		}
		push((yyvsp[-1].action), &failure_rt.rlist[DEFAULT_RT]);
	}
#line 8340 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 493:
#line 1939 "core/cfg.y" /* yacc.c:1646  */
    {
		if (!shm_initialized() && init_shm()<0) {
			yyerror("Can't initialize shared memory");
			YYABORT;
		}
		i_tmp=route_get(&failure_rt, (yyvsp[-4].strval));
		if (i_tmp==-1){
			yyerror("internal error");
			YYABORT;
		}
		if (failure_rt.rlist[i_tmp]){
			yyerror("duplicate route");
			YYABORT;
		}
		push((yyvsp[-1].action), &failure_rt.rlist[i_tmp]);
	}
#line 8361 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 494:
#line 1955 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("invalid failure_route statement"); }
#line 8367 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 495:
#line 1959 "core/cfg.y" /* yacc.c:1646  */
    { routename=NULL; }
#line 8373 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 496:
#line 1960 "core/cfg.y" /* yacc.c:1646  */
    { routename=NULL; }
#line 8379 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 497:
#line 1965 "core/cfg.y" /* yacc.c:1646  */
    {rt=CORE_ONREPLY_ROUTE;}
#line 8385 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 498:
#line 1965 "core/cfg.y" /* yacc.c:1646  */
    {
		if (!shm_initialized() && init_shm()<0) {
			yyerror("Can't initialize shared memory");
			YYABORT;
		}
		push((yyvsp[-1].action), &onreply_rt.rlist[DEFAULT_RT]);
	}
#line 8397 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 499:
#line 1972 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("invalid onreply_route statement"); }
#line 8403 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 500:
#line 1973 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("invalid onreply_route statement"); }
#line 8409 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 501:
#line 1975 "core/cfg.y" /* yacc.c:1646  */
    {rt=(*(yyvsp[-1].strval)=='0' && (yyvsp[-1].strval)[1]==0)?CORE_ONREPLY_ROUTE:TM_ONREPLY_ROUTE;}
#line 8415 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 502:
#line 1976 "core/cfg.y" /* yacc.c:1646  */
    {
		if (!shm_initialized() && init_shm()<0) {
			yyerror("Can't initialize shared memory");
			YYABORT;
		}
		if (*(yyvsp[-5].strval)=='0' && (yyvsp[-5].strval)[1]==0){
			/* onreply_route[0] {} is equivalent with onreply_route {}*/
			push((yyvsp[-1].action), &onreply_rt.rlist[DEFAULT_RT]);
		}else{
			i_tmp=route_get(&onreply_rt, (yyvsp[-5].strval));
			if (i_tmp==-1){
				yyerror("internal error");
				YYABORT;
			}
			if (onreply_rt.rlist[i_tmp]){
				yyerror("duplicate route");
				YYABORT;
			}
			push((yyvsp[-1].action), &onreply_rt.rlist[i_tmp]);
		}
	}
#line 8441 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 503:
#line 1997 "core/cfg.y" /* yacc.c:1646  */
    {
		yyerror("invalid onreply_route statement");
	}
#line 8449 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 504:
#line 2002 "core/cfg.y" /* yacc.c:1646  */
    { routename=NULL; }
#line 8455 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 505:
#line 2005 "core/cfg.y" /* yacc.c:1646  */
    {
		if (!shm_initialized() && init_shm()<0) {
			yyerror("Can't initialize shared memory");
			YYABORT;
		}
		push((yyvsp[-1].action), &branch_rt.rlist[DEFAULT_RT]);
	}
#line 8467 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 506:
#line 2012 "core/cfg.y" /* yacc.c:1646  */
    {
		if (!shm_initialized() && init_shm()<0) {
			yyerror("Can't initialize shared memory");
			YYABORT;
		}
		i_tmp=route_get(&branch_rt, (yyvsp[-4].strval));
		if (i_tmp==-1){
			yyerror("internal error");
			YYABORT;
		}
		if (branch_rt.rlist[i_tmp]){
			yyerror("duplicate route");
			YYABORT;
		}
		push((yyvsp[-1].action), &branch_rt.rlist[i_tmp]);
	}
#line 8488 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 507:
#line 2028 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("invalid branch_route statement"); }
#line 8494 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 508:
#line 2031 "core/cfg.y" /* yacc.c:1646  */
    { routename=NULL; }
#line 8500 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 509:
#line 2034 "core/cfg.y" /* yacc.c:1646  */
    {
		if (!shm_initialized() && init_shm()<0) {
			yyerror("Can't initialize shared memory");
			YYABORT;
		}
		push((yyvsp[-1].action), &onsend_rt.rlist[DEFAULT_RT]);
	}
#line 8512 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 510:
#line 2041 "core/cfg.y" /* yacc.c:1646  */
    {
		if (!shm_initialized() && init_shm()<0) {
			yyerror("Can't initialize shared memory");
			YYABORT;
		}
		i_tmp=route_get(&onsend_rt, (yyvsp[-4].strval));
		if (i_tmp==-1){
			yyerror("internal error");
			YYABORT;
		}
		if (onsend_rt.rlist[i_tmp]){
			yyerror("duplicate route");
			YYABORT;
		}
		push((yyvsp[-1].action), &onsend_rt.rlist[i_tmp]);
	}
#line 8533 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 511:
#line 2057 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("invalid onsend_route statement"); }
#line 8539 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 512:
#line 2060 "core/cfg.y" /* yacc.c:1646  */
    { routename=NULL; }
#line 8545 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 513:
#line 2063 "core/cfg.y" /* yacc.c:1646  */
    {
		if (!shm_initialized() && init_shm()<0) {
			yyerror("Can't initialize shared memory");
			YYABORT;
		}
		i_tmp=route_get(&event_rt, (yyvsp[-4].strval));
		if (i_tmp==-1){
			yyerror("internal error");
			YYABORT;
		}
		if (event_rt.rlist[i_tmp]){
			yyerror("duplicate route");
			YYABORT;
		}
		push((yyvsp[-1].action), &event_rt.rlist[i_tmp]);
	}
#line 8566 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 514:
#line 2080 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("invalid event_route statement"); }
#line 8572 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 515:
#line 2083 "core/cfg.y" /* yacc.c:1646  */
    { if(pp_subst_add((yyvsp[0].strval))<0) YYERROR; }
#line 8578 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 516:
#line 2084 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("invalid subst preprocess statement"); }
#line 8584 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 517:
#line 2085 "core/cfg.y" /* yacc.c:1646  */
    { if(pp_substdef_add((yyvsp[0].strval), 0)<0) YYERROR; }
#line 8590 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 518:
#line 2086 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("invalid substdef preprocess statement"); }
#line 8596 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 519:
#line 2087 "core/cfg.y" /* yacc.c:1646  */
    { if(pp_substdef_add((yyvsp[0].strval), 1)<0) YYERROR; }
#line 8602 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 520:
#line 2088 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("invalid substdefs preprocess statement"); }
#line 8608 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 521:
#line 2110 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=EQUAL_OP; }
#line 8614 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 522:
#line 2111 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=DIFF_OP; }
#line 8620 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 523:
#line 2112 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=EQUAL_OP; }
#line 8626 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 524:
#line 2113 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=DIFF_OP; }
#line 8632 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 525:
#line 2116 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=GT_OP; }
#line 8638 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 526:
#line 2117 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=LT_OP; }
#line 8644 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 527:
#line 2118 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=GTE_OP; }
#line 8650 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 528:
#line 2119 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=LTE_OP; }
#line 8656 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 529:
#line 2122 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=(yyvsp[0].intval); }
#line 8662 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 530:
#line 2123 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=MATCH_OP; }
#line 8668 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 531:
#line 2129 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=RVE_EQ_OP; }
#line 8674 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 532:
#line 2130 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=RVE_DIFF_OP; }
#line 8680 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 533:
#line 2131 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=RVE_IEQ_OP; }
#line 8686 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 534:
#line 2132 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=RVE_IDIFF_OP; }
#line 8692 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 535:
#line 2133 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=RVE_STREQ_OP; }
#line 8698 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 536:
#line 2134 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=RVE_STRDIFF_OP; }
#line 8704 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 537:
#line 2135 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=RVE_MATCH_OP; }
#line 8710 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 538:
#line 2138 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=RVE_GT_OP; }
#line 8716 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 539:
#line 2139 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=RVE_LT_OP; }
#line 8722 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 540:
#line 2140 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=RVE_GTE_OP; }
#line 8728 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 541:
#line 2141 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=RVE_LTE_OP; }
#line 8734 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 542:
#line 2148 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=URI_O;}
#line 8740 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 543:
#line 2149 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=FROM_URI_O;}
#line 8746 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 544:
#line 2150 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.intval)=TO_URI_O;}
#line 8752 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 545:
#line 2157 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=SNDPORT_O; }
#line 8758 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 546:
#line 2158 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=TOPORT_O; }
#line 8764 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 547:
#line 2159 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=SNDAF_O; }
#line 8770 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 548:
#line 2163 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=SRCPORT_O; }
#line 8776 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 549:
#line 2164 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=DSTPORT_O; }
#line 8782 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 550:
#line 2165 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=AF_O; }
#line 8788 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 551:
#line 2166 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=MSGLEN_O; }
#line 8794 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 553:
#line 2172 "core/cfg.y" /* yacc.c:1646  */
    { onsend_check("snd_ip"); (yyval.intval)=SNDIP_O; }
#line 8800 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 554:
#line 2173 "core/cfg.y" /* yacc.c:1646  */
    { onsend_check("to_ip");  (yyval.intval)=TOIP_O; }
#line 8806 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 555:
#line 2176 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=SRCIP_O; }
#line 8812 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 556:
#line 2177 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=DSTIP_O; }
#line 8818 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 558:
#line 2185 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.expr)= mk_elem((yyvsp[-1].intval), METHOD_O, 0, RVE_ST, (yyvsp[0].rv_expr));}
#line 8824 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 559:
#line 2187 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.expr) = mk_elem((yyvsp[-1].intval), METHOD_O, 0, STRING_ST,(yyvsp[0].strval)); }
#line 8830 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 560:
#line 2188 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("string expected"); }
#line 8836 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 561:
#line 2190 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("invalid operator,== , !=, or =~ expected"); }
#line 8842 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 562:
#line 2192 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.expr) = mk_elem((yyvsp[-1].intval), (yyvsp[-2].intval), 0, RVE_ST, (yyvsp[0].rv_expr)); }
#line 8848 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 563:
#line 2194 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.expr)=mk_elem((yyvsp[-1].intval), (yyvsp[-2].intval), 0, MYSELF_ST, 0); }
#line 8854 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 564:
#line 2196 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("string or MYSELF expected"); }
#line 8860 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 565:
#line 2198 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("invalid operator, == , != or =~ expected"); }
#line 8866 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 566:
#line 2199 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), (yyvsp[-2].intval), 0, RVE_ST, (yyvsp[0].rv_expr) ); }
#line 8872 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 567:
#line 2201 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), (yyvsp[-2].intval), 0, RVE_ST, (yyvsp[0].rv_expr) ); }
#line 8878 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 568:
#line 2202 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("number expected"); }
#line 8884 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 569:
#line 2203 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("number expected"); }
#line 8890 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 570:
#line 2204 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("==, !=, <,>, >= or <=  expected"); }
#line 8896 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 571:
#line 2206 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), PROTO_O, 0, NUMBER_ST, (void*)(yyvsp[0].intval) ); }
#line 8902 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 572:
#line 2208 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), PROTO_O, 0, RVE_ST, (yyvsp[0].rv_expr) ); }
#line 8908 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 573:
#line 2210 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("protocol expected (udp, tcp, tls, sctp, ws, or wss)"); }
#line 8914 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 574:
#line 2212 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), SNDPROTO_O, 0, NUMBER_ST, (void*)(yyvsp[0].intval) ); }
#line 8920 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 575:
#line 2214 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), SNDPROTO_O, 0, RVE_ST, (yyvsp[0].rv_expr) ); }
#line 8926 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 576:
#line 2216 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("protocol expected (udp, tcp, tls, sctp, ws, or wss)"); }
#line 8932 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 577:
#line 2217 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), (yyvsp[-2].intval), 0, NET_ST, (yyvsp[0].ipnet)); }
#line 8938 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 578:
#line 2218 "core/cfg.y" /* yacc.c:1646  */
    {
			s_tmp.s=0;
			(yyval.expr)=0;
			if (rve_is_constant((yyvsp[0].rv_expr))){
				i_tmp=rve_guess_type((yyvsp[0].rv_expr));
				if (i_tmp==RV_INT)
					yyerror("string expected");
				else if (i_tmp==RV_STR){
					if (((rval_tmp=rval_expr_eval(0, 0, (yyvsp[0].rv_expr)))==0) ||
								(rval_get_str(0, 0, &s_tmp, rval_tmp, 0)<0)){
						rval_destroy(rval_tmp);
						yyerror("bad rvalue expression");
					}else{
						rval_destroy(rval_tmp);
					}
				}else{
					yyerror("BUG: unexpected dynamic type");
				}
			}else{
					/* warn("non constant rvalue in ip comparison") */;
			}
			if (s_tmp.s){
				ip_tmp=str2ip(&s_tmp);
				if (ip_tmp==0)
					ip_tmp=str2ip6(&s_tmp);
				pkg_free(s_tmp.s);
				if (ip_tmp) {
					(yyval.expr)=mk_elem((yyvsp[-1].intval), (yyvsp[-2].intval), 0, NET_ST,
								mk_new_net_bitlen(ip_tmp, ip_tmp->len*8) );
				} else {
					(yyval.expr)=mk_elem((yyvsp[-1].intval), (yyvsp[-2].intval), 0, RVE_ST, (yyvsp[0].rv_expr));
				}
			}else{
				(yyval.expr)=mk_elem((yyvsp[-1].intval), (yyvsp[-2].intval), 0, RVE_ST, (yyvsp[0].rv_expr));
			}
		}
#line 8979 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 579:
#line 2255 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), (yyvsp[-2].intval), 0, STRING_ST, (yyvsp[0].strval)); }
#line 8985 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 580:
#line 2257 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), (yyvsp[-2].intval), 0, MYSELF_ST, 0); }
#line 8991 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 581:
#line 2259 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror( "ip address or hostname expected" ); }
#line 8997 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 582:
#line 2261 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("invalid operator, ==, != or =~ expected");}
#line 9003 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 583:
#line 2264 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), (yyvsp[0].intval), 0, MYSELF_ST, 0); }
#line 9009 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 584:
#line 2266 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=mk_elem((yyvsp[-1].intval), (yyvsp[0].intval), 0, MYSELF_ST, 0); }
#line 9015 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 585:
#line 2268 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror("URI, SRCIP or DSTIP expected"); }
#line 9021 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 586:
#line 2269 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.expr)=0; yyerror ("invalid operator, == or != expected"); }
#line 9027 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 587:
#line 2273 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.ipnet)=mk_new_net((yyvsp[-2].ipaddr), (yyvsp[0].ipaddr)); }
#line 9033 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 588:
#line 2274 "core/cfg.y" /* yacc.c:1646  */
    {
		if (((yyvsp[0].intval)<0) || ((yyvsp[0].intval)>(yyvsp[-2].ipaddr)->len*8)) {
			yyerror("invalid bit number in netmask");
			(yyval.ipnet)=0;
		} else {
			(yyval.ipnet)=mk_new_net_bitlen((yyvsp[-2].ipaddr), (yyvsp[0].intval));
		/*
			$$=mk_new_net($1, htonl( ($3)?~( (1<<(32-$3))-1 ):0 ) );
		*/
		}
	}
#line 9049 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 589:
#line 2285 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.ipnet)=mk_new_net_bitlen((yyvsp[0].ipaddr), (yyvsp[0].ipaddr)->len*8); }
#line 9055 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 590:
#line 2286 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.ipnet)=0; yyerror("netmask (eg:255.0.0.0 or 8) expected"); }
#line 9061 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 591:
#line 2290 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.strval)=(yyvsp[0].strval); }
#line 9067 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 592:
#line 2291 "core/cfg.y" /* yacc.c:1646  */
    {
		if ((yyvsp[-2].strval)){
			(yyval.strval)=(char*)pkg_malloc(strlen((yyvsp[-2].strval))+1+strlen((yyvsp[0].strval))+1);
			if ((yyval.strval)==0) {
				PKG_MEM_CRITICAL;
			} else {
				memcpy((yyval.strval), (yyvsp[-2].strval), strlen((yyvsp[-2].strval)));
				(yyval.strval)[strlen((yyvsp[-2].strval))]='.';
				memcpy((yyval.strval)+strlen((yyvsp[-2].strval))+1, (yyvsp[0].strval), strlen((yyvsp[0].strval)));
				(yyval.strval)[strlen((yyvsp[-2].strval))+1+strlen((yyvsp[0].strval))]=0;
			}
			pkg_free((yyvsp[-2].strval));
		}
		if ((yyvsp[0].strval)) pkg_free((yyvsp[0].strval));
	}
#line 9087 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 593:
#line 2306 "core/cfg.y" /* yacc.c:1646  */
    {
		if ((yyvsp[-2].strval)){
			(yyval.strval)=(char*)pkg_malloc(strlen((yyvsp[-2].strval))+1+strlen((yyvsp[0].strval))+1);
			if ((yyval.strval)==0) {
				PKG_MEM_CRITICAL;
			} else {
				memcpy((yyval.strval), (yyvsp[-2].strval), strlen((yyvsp[-2].strval)));
				(yyval.strval)[strlen((yyvsp[-2].strval))]='-';
				memcpy((yyval.strval)+strlen((yyvsp[-2].strval))+1, (yyvsp[0].strval), strlen((yyvsp[0].strval)));
				(yyval.strval)[strlen((yyvsp[-2].strval))+1+strlen((yyvsp[0].strval))]=0;
			}
			pkg_free((yyvsp[-2].strval));
		}
		if ((yyvsp[0].strval)) pkg_free((yyvsp[0].strval));
	}
#line 9107 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 594:
#line 2321 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.strval)=0; pkg_free((yyvsp[-2].strval)); yyerror("invalid hostname"); }
#line 9113 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 595:
#line 2322 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.strval)=0; pkg_free((yyvsp[-2].strval)); yyerror("invalid hostname"); }
#line 9119 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 598:
#line 2327 "core/cfg.y" /* yacc.c:1646  */
    {
			/* get string version */
			(yyval.strval)=pkg_malloc(strlen(yy_number_str)+1);
			if ((yyval.strval)==0) {
				PKG_MEM_ERROR;
			} else {
				strcpy((yyval.strval), yy_number_str);
			}
		}
#line 9133 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 599:
#line 2339 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.strval)=(yyvsp[0].strval); }
#line 9139 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 600:
#line 2340 "core/cfg.y" /* yacc.c:1646  */
    {
		if ((yyvsp[-2].strval)){
			(yyval.strval)=(char*)pkg_malloc(strlen((yyvsp[-2].strval))+1+strlen((yyvsp[0].strval))+1);
			if ((yyval.strval)==0) {
				PKG_MEM_CRITICAL;
			} else {
				memcpy((yyval.strval), (yyvsp[-2].strval), strlen((yyvsp[-2].strval)));
				(yyval.strval)[strlen((yyvsp[-2].strval))]='.';
				memcpy((yyval.strval)+strlen((yyvsp[-2].strval))+1, (yyvsp[0].strval), strlen((yyvsp[0].strval)));
				(yyval.strval)[strlen((yyvsp[-2].strval))+1+strlen((yyvsp[0].strval))]=0;
			}
			pkg_free((yyvsp[-2].strval));
		}
		if ((yyvsp[0].strval)) pkg_free((yyvsp[0].strval));
	}
#line 9159 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 601:
#line 2355 "core/cfg.y" /* yacc.c:1646  */
    {
		if ((yyvsp[-2].strval)){
			(yyval.strval)=(char*)pkg_malloc(strlen((yyvsp[-2].strval))+1+strlen((yyvsp[0].strval))+1);
			if ((yyval.strval)==0) {
				PKG_MEM_CRITICAL;
			} else {
				memcpy((yyval.strval), (yyvsp[-2].strval), strlen((yyvsp[-2].strval)));
				(yyval.strval)[strlen((yyvsp[-2].strval))]='-';
				memcpy((yyval.strval)+strlen((yyvsp[-2].strval))+1, (yyvsp[0].strval), strlen((yyvsp[0].strval)));
				(yyval.strval)[strlen((yyvsp[-2].strval))+1+strlen((yyvsp[0].strval))]=0;
			}
			pkg_free((yyvsp[-2].strval));
		}
		if ((yyvsp[0].strval)) pkg_free((yyvsp[0].strval));
	}
#line 9179 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 602:
#line 2370 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.strval)=0; pkg_free((yyvsp[-2].strval));
								yyerror("invalid host or interface name"); }
#line 9186 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 603:
#line 2372 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.strval)=0; pkg_free((yyvsp[-2].strval));
								yyerror("invalid host or interface name"); }
#line 9193 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 604:
#line 2379 "core/cfg.y" /* yacc.c:1646  */
    {
		/* check if allowed */
		if ((yyvsp[0].action) && rt==ONSEND_ROUTE) {
			switch((yyvsp[0].action)->type) {
				case DROP_T:
				case LOG_T:
				case SETFLAG_T:
				case RESETFLAG_T:
				case ISFLAGSET_T:
				case IF_T:
				case MODULE0_T:
				case MODULE1_T:
				case MODULE2_T:
				case MODULE3_T:
				case MODULE4_T:
				case MODULE5_T:
				case MODULE6_T:
				case MODULEX_T:
				case SET_FWD_NO_CONNECT_T:
				case SET_RPL_NO_CONNECT_T:
				case SET_FWD_CLOSE_T:
				case SET_RPL_CLOSE_T:
					(yyval.action)=(yyvsp[0].action);
					break;
				default:
					(yyval.action)=0;
					yyerror("command not allowed in onsend_route\n");
			}
		} else {
			(yyval.action)=(yyvsp[0].action);
		}
	}
#line 9230 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 605:
#line 2421 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=(yyvsp[0].action); }
#line 9236 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 606:
#line 2422 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=(yyvsp[-1].action); }
#line 9242 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 607:
#line 2425 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.action)=append_action((yyvsp[-1].action), (yyvsp[0].action)); }
#line 9248 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 608:
#line 2426 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.action)=(yyvsp[0].action);}
#line 9254 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 609:
#line 2427 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad command"); }
#line 9260 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 610:
#line 2430 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.action)=(yyvsp[-1].action);}
#line 9266 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 611:
#line 2431 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.action)=(yyvsp[0].action);}
#line 9272 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 612:
#line 2432 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.action)=(yyvsp[0].action);}
#line 9278 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 613:
#line 2433 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=(yyvsp[0].action); }
#line 9284 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 614:
#line 2434 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=(yyvsp[-1].action); }
#line 9290 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 615:
#line 2435 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.action)=(yyvsp[-1].action);}
#line 9296 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 616:
#line 2436 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.action)=0;}
#line 9302 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 617:
#line 2437 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad command: missing ';'?"); }
#line 9308 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 618:
#line 2440 "core/cfg.y" /* yacc.c:1646  */
    {
		if ((yyvsp[-1].rv_expr) && rval_expr_int_check((yyvsp[-1].rv_expr))>=0){
			warn_ct_rve((yyvsp[-1].rv_expr), "if");
			(yyval.action)=mk_action( IF_T, 3, RVE_ST, (yyvsp[-1].rv_expr), ACTIONS_ST, (yyvsp[0].action), NOSUBTYPE, 0);
			set_cfg_pos((yyval.action));
		}else
			YYERROR;
	}
#line 9321 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 619:
#line 2448 "core/cfg.y" /* yacc.c:1646  */
    {
		if ((yyvsp[-3].rv_expr) && rval_expr_int_check((yyvsp[-3].rv_expr))>=0){
			warn_ct_rve((yyvsp[-3].rv_expr), "if");
			(yyval.action)=mk_action( IF_T, 3, RVE_ST, (yyvsp[-3].rv_expr), ACTIONS_ST, (yyvsp[-2].action), ACTIONS_ST, (yyvsp[0].action));
			set_cfg_pos((yyval.action));
		}else
			YYERROR;
	}
#line 9334 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 620:
#line 2458 "core/cfg.y" /* yacc.c:1646  */
    {
			(yyval.rv_expr)=0;
			if ((yyvsp[0].rv_expr) && !rve_is_constant((yyvsp[0].rv_expr))){
				yyerror("constant expected");
				YYERROR;
			/*
			} else if ($1 &&
						!rve_check_type((enum rval_type*)&i_tmp, $1, 0, 0 ,0)){
				yyerror("invalid expression (bad type)");
			}else if ($1 && i_tmp!=RV_INT){
				yyerror("invalid expression type, int expected\n");
			*/
			}else
				(yyval.rv_expr)=(yyvsp[0].rv_expr);
		}
#line 9354 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 621:
#line 2475 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.case_stms)=0;
		if ((yyvsp[-2].rv_expr)==0) { yyerror ("bad case label"); YYERROR; }
		else if ((((yyval.case_stms)=mk_case_stm((yyvsp[-2].rv_expr), 0, (yyvsp[0].action), &i_tmp))==0) && (i_tmp==-10)){
				YYABORT;
		}
	}
#line 9366 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 622:
#line 2482 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.case_stms)=0;
		if ((yyvsp[-2].rv_expr)==0) { yyerror ("bad case label"); YYERROR; }
		else if ((((yyval.case_stms)=mk_case_stm((yyvsp[-2].rv_expr), 1, (yyvsp[0].action), &i_tmp))==0) && (i_tmp==-10)){
				YYABORT;
		}
	}
#line 9378 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 623:
#line 2489 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.case_stms)=0;
		if ((yyvsp[-1].rv_expr)==0) { yyerror ("bad case label"); YYERROR; }
		else if ((((yyval.case_stms)=mk_case_stm((yyvsp[-1].rv_expr), 0, 0, &i_tmp))==0) && (i_tmp==-10)){
				YYABORT;
		}
	}
#line 9390 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 624:
#line 2496 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.case_stms)=0;
		if ((yyvsp[-1].rv_expr)==0) { yyerror ("bad regex case label"); YYERROR; }
		else if ((((yyval.case_stms)=mk_case_stm((yyvsp[-1].rv_expr), 1, 0, &i_tmp))==0) && (i_tmp==-10)){
				YYABORT;
		}
	}
#line 9402 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 625:
#line 2503 "core/cfg.y" /* yacc.c:1646  */
    {
		if ((((yyval.case_stms)=mk_case_stm(0, 0, (yyvsp[0].action), &i_tmp))==0) && (i_tmp==-10)){
				YYABORT;
		}
	}
#line 9412 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 626:
#line 2508 "core/cfg.y" /* yacc.c:1646  */
    {
		if ((((yyval.case_stms)=mk_case_stm(0, 0, 0, &i_tmp))==0) && (i_tmp==-10)){
				YYABORT;
		}
	}
#line 9422 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 627:
#line 2513 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.case_stms)=0; yyerror("bad case label"); }
#line 9428 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 628:
#line 2514 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.case_stms)=0; yyerror("bad case regex label"); }
#line 9434 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 629:
#line 2515 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.case_stms)=0; yyerror("bad case label"); }
#line 9440 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 630:
#line 2516 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.case_stms)=0; yyerror("bad case regex label"); }
#line 9446 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 631:
#line 2517 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.case_stms)=0; yyerror ("bad case body"); }
#line 9452 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 632:
#line 2520 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.case_stms)=(yyvsp[-1].case_stms);
		if ((yyvsp[0].case_stms)==0) yyerror ("bad case");
		if ((yyval.case_stms)){
			*((yyval.case_stms)->append)=(yyvsp[0].case_stms);
			if (*((yyval.case_stms)->append)!=0)
				(yyval.case_stms)->append=&((*((yyval.case_stms)->append))->next);
		}
	}
#line 9466 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 633:
#line 2529 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.case_stms)=(yyvsp[0].case_stms);
		if ((yyvsp[0].case_stms)==0) yyerror ("bad case");
		else (yyval.case_stms)->append=&((yyval.case_stms)->next);
	}
#line 9476 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 634:
#line 2536 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=0;
		if ((yyvsp[-3].rv_expr)==0){
			yyerror("bad expression in switch(...)");
			YYERROR;
		}else if ((yyvsp[-1].case_stms)==0){
			yyerror ("bad switch body");
			YYERROR;
		}else if (case_check_default((yyvsp[-1].case_stms))!=0){
			yyerror_at(&(yyvsp[-3].rv_expr)->fpos, "bad switch(): too many "
							"\"default:\" labels\n");
			YYERROR;
		}else if (case_check_type((yyvsp[-1].case_stms))!=0){
			yyerror_at(&(yyvsp[-3].rv_expr)->fpos, "bad switch(): mixed integer and"
							" string/RE cases not allowed\n");
			YYERROR;
		}else{
			(yyval.action)=mk_action(SWITCH_T, 2, RVE_ST, (yyvsp[-3].rv_expr), CASE_ST, (yyvsp[-1].case_stms));
			if ((yyval.action)==0) {
				yyerror("internal error");
				YYABORT;
			}
			set_cfg_pos((yyval.action));
		}
	}
#line 9506 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 635:
#line 2561 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=0;
		warn("empty switch()");
		if ((yyvsp[-2].rv_expr)==0){
			yyerror("bad expression in switch(...)");
			YYERROR;
		}else{
			/* it might have sideffects, so leave it for the optimizer */
			(yyval.action)=mk_action(SWITCH_T, 2, RVE_ST, (yyvsp[-2].rv_expr), CASE_ST, 0);
			if ((yyval.action)==0) {
				yyerror("internal error");
				YYABORT;
			}
			set_cfg_pos((yyval.action));
		}
	}
#line 9527 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 636:
#line 2577 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror ("bad expression in switch(...)"); }
#line 9533 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 637:
#line 2579 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.action)=0; yyerror ("bad switch body"); }
#line 9539 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 638:
#line 2583 "core/cfg.y" /* yacc.c:1646  */
    {
		if ((yyvsp[-1].rv_expr) && rval_expr_int_check((yyvsp[-1].rv_expr))>=0){
			warn_ct_rve((yyvsp[-1].rv_expr), "while");
			(yyval.action)=mk_action( WHILE_T, 2, RVE_ST, (yyvsp[-1].rv_expr), ACTIONS_ST, (yyvsp[0].action));
			set_cfg_pos((yyval.action));
		}else{
			yyerror_at(&(yyvsp[-1].rv_expr)->fpos, "bad while(...) expression");
			YYERROR;
		}
	}
#line 9554 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 639:
#line 2602 "core/cfg.y" /* yacc.c:1646  */
    {
		if (sel.n >= MAX_SELECT_PARAMS-1) {
			yyerror("Select identifier too long\n");
		}
		sel.params[sel.n].type = SEL_PARAM_STR;
		sel.params[sel.n].v.s.s = (yyvsp[0].strval);
		sel.params[sel.n].v.s.len = strlen((yyvsp[0].strval));
		sel.n++;
	}
#line 9568 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 640:
#line 2611 "core/cfg.y" /* yacc.c:1646  */
    {
		if (sel.n >= MAX_SELECT_PARAMS-2) {
			yyerror("Select identifier too long\n");
		}
		sel.params[sel.n].type = SEL_PARAM_STR;
		sel.params[sel.n].v.s.s = (yyvsp[-3].strval);
		sel.params[sel.n].v.s.len = strlen((yyvsp[-3].strval));
		sel.n++;
		sel.params[sel.n].type = SEL_PARAM_INT;
		sel.params[sel.n].v.i = (yyvsp[-1].intval);
		sel.n++;
	}
#line 9585 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 641:
#line 2623 "core/cfg.y" /* yacc.c:1646  */
    {
		if (sel.n >= MAX_SELECT_PARAMS-2) {
			yyerror("Select identifier too long\n");
		}
		sel.params[sel.n].type = SEL_PARAM_STR;
		sel.params[sel.n].v.s.s = (yyvsp[-3].strval);
		sel.params[sel.n].v.s.len = strlen((yyvsp[-3].strval));
		sel.n++;
		sel.params[sel.n].type = SEL_PARAM_STR;
		sel.params[sel.n].v.s.s = (yyvsp[-1].strval);
		sel.params[sel.n].v.s.len = strlen((yyvsp[-1].strval));
		sel.n++;
	}
#line 9603 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 644:
#line 2642 "core/cfg.y" /* yacc.c:1646  */
    { sel.n = 0; sel.f[0] = 0; }
#line 9609 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 645:
#line 2642 "core/cfg.y" /* yacc.c:1646  */
    {
		sel_ptr = (select_t*)pkg_malloc(sizeof(select_t));
		if (!sel_ptr) {
			yyerror("No memory left to allocate select structure\n");
		}
		memcpy(sel_ptr, &sel, sizeof(select_t));
		(yyval.select) = sel_ptr;
	}
#line 9622 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 646:
#line 2652 "core/cfg.y" /* yacc.c:1646  */
    { s_attr->type |= AVP_TRACK_FROM; }
#line 9628 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 647:
#line 2653 "core/cfg.y" /* yacc.c:1646  */
    { s_attr->type |= AVP_TRACK_TO; }
#line 9634 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 648:
#line 2654 "core/cfg.y" /* yacc.c:1646  */
    { s_attr->type |= AVP_TRACK_FROM | AVP_CLASS_URI; }
#line 9640 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 649:
#line 2655 "core/cfg.y" /* yacc.c:1646  */
    { s_attr->type |= AVP_TRACK_TO | AVP_CLASS_URI; }
#line 9646 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 650:
#line 2656 "core/cfg.y" /* yacc.c:1646  */
    { s_attr->type |= AVP_TRACK_FROM | AVP_CLASS_USER; }
#line 9652 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 651:
#line 2657 "core/cfg.y" /* yacc.c:1646  */
    { s_attr->type |= AVP_TRACK_TO | AVP_CLASS_USER; }
#line 9658 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 652:
#line 2658 "core/cfg.y" /* yacc.c:1646  */
    { s_attr->type |= AVP_TRACK_FROM | AVP_CLASS_DOMAIN; }
#line 9664 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 653:
#line 2659 "core/cfg.y" /* yacc.c:1646  */
    { s_attr->type |= AVP_TRACK_TO | AVP_CLASS_DOMAIN; }
#line 9670 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 654:
#line 2660 "core/cfg.y" /* yacc.c:1646  */
    { s_attr->type |= AVP_TRACK_ALL | AVP_CLASS_GLOBAL; }
#line 9676 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 655:
#line 2663 "core/cfg.y" /* yacc.c:1646  */
    { s_attr->type |= AVP_NAME_STR; s_attr->name.s.s = (yyvsp[0].strval); s_attr->name.s.len = strlen ((yyvsp[0].strval)); }
#line 9682 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 658:
#line 2670 "core/cfg.y" /* yacc.c:1646  */
    {
		s_attr = (struct avp_spec*)pkg_malloc(sizeof(struct avp_spec));
		if (!s_attr) { yyerror("No memory left"); YYABORT; }
		else s_attr->type = 0;
	}
#line 9692 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 659:
#line 2677 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.attr) = s_attr; }
#line 9698 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 660:
#line 2680 "core/cfg.y" /* yacc.c:1646  */
    {
		s_attr->type|= (AVP_NAME_STR | ((yyvsp[-1].intval)<0?AVP_INDEX_BACKWARD:AVP_INDEX_FORWARD));
		s_attr->index = ((yyvsp[-1].intval)<0?-(yyvsp[-1].intval):(yyvsp[-1].intval));
		(yyval.attr) = s_attr;
	}
#line 9708 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 661:
#line 2687 "core/cfg.y" /* yacc.c:1646  */
    {
		s_attr->type|= AVP_INDEX_ALL;
		(yyval.attr) = s_attr;
	}
#line 9717 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 668:
#line 2709 "core/cfg.y" /* yacc.c:1646  */
    {
		if ((yyvsp[0].lval)->type==LV_AVP){
			s_attr = pkg_malloc(sizeof(struct avp_spec));
			if (!s_attr) { yyerror("No memory left"); YYABORT; }
			else{
				*s_attr=(yyvsp[0].lval)->lv.avps;
			}
			(yyval.attr)=s_attr;
		}else
			(yyval.attr)=0; /* not an avp, a pvar */
		pkg_free((yyvsp[0].lval));
	}
#line 9734 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 669:
#line 2721 "core/cfg.y" /* yacc.c:1646  */
    {
		avp_spec_t *avp_spec;
		str s;
		int type, idx;
		avp_spec = pkg_malloc(sizeof(*avp_spec));
		if (!avp_spec) {
			yyerror("Not enough memory");
			YYABORT;
		}
		s.s = (yyvsp[0].strval);
		if (s.s[0] == '$')
			s.s++;
		s.len = strlen(s.s);
		if (parse_avp_name(&s, &type, &avp_spec->name, &idx)) {
			yyerror("error when parsing AVP");
			pkg_free(avp_spec);
			YYABORT;
		}
		avp_spec->type = type;
		avp_spec->index = idx;
		(yyval.attr) = avp_spec;
	}
#line 9761 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 670:
#line 2745 "core/cfg.y" /* yacc.c:1646  */
    {
			s_tmp.s=(yyvsp[0].strval); s_tmp.len=strlen((yyvsp[0].strval));
			pv_spec=pv_cache_get(&s_tmp);
			if (!pv_spec) {
				yyerror("Can't get from cache: %s", (yyvsp[0].strval));
				YYABORT;
			}
			(yyval.pvar)=pv_spec;
		}
#line 9775 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 671:
#line 2756 "core/cfg.y" /* yacc.c:1646  */
    {
				lval_tmp=pkg_malloc(sizeof(*lval_tmp));
				if (!lval_tmp) {
					yyerror("Not enough memory");
					YYABORT;
				}
				memset(lval_tmp, 0, sizeof(*lval_tmp));
				s_tmp.s=(yyvsp[0].strval); s_tmp.len=strlen(s_tmp.s);
				lval_tmp->lv.pvs = pv_cache_get(&s_tmp);
				if (lval_tmp->lv.pvs==NULL){
					lval_tmp->lv.avps.type|= AVP_NAME_STR;
					lval_tmp->lv.avps.name.s.s = s_tmp.s+1;
					lval_tmp->lv.avps.name.s.len = s_tmp.len-1;
					lval_tmp->type=LV_AVP;
				}else{
					lval_tmp->type=LV_PVAR;
				}
				(yyval.lval) = lval_tmp;
				DBG("parsed ambiguous avp/pvar \"%.*s\" to %d\n",
							s_tmp.len, s_tmp.s, lval_tmp->type);
			}
#line 9801 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 672:
#line 2787 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval) = ASSIGN_T; }
#line 9807 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 673:
#line 2791 "core/cfg.y" /* yacc.c:1646  */
    {
					lval_tmp=pkg_malloc(sizeof(*lval_tmp));
					if (!lval_tmp) {
						yyerror("Not enough memory");
						YYABORT;
					}
					lval_tmp->type=LV_AVP; lval_tmp->lv.avps=*(yyvsp[0].attr);
					pkg_free((yyvsp[0].attr)); /* free the avp spec we just copied */
					(yyval.lval)=lval_tmp;
				}
#line 9822 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 674:
#line 2801 "core/cfg.y" /* yacc.c:1646  */
    {
					if (!pv_is_w((yyvsp[0].pvar)))
						yyerror("read only pvar in assignment left side");
					if ((yyvsp[0].pvar)->trans!=0)
						yyerror("pvar with transformations in assignment"
								" left side");
					lval_tmp=pkg_malloc(sizeof(*lval_tmp));
					if (!lval_tmp) {
						yyerror("Not enough memory");
						YYABORT;
					}
					lval_tmp->type=LV_PVAR; lval_tmp->lv.pvs=(yyvsp[0].pvar);
					(yyval.lval)=lval_tmp;
				}
#line 9841 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 675:
#line 2815 "core/cfg.y" /* yacc.c:1646  */
    {
					if (((yyvsp[0].lval))->type==LV_PVAR){
						if (!pv_is_w((yyvsp[0].lval)->lv.pvs))
							yyerror("read only pvar in assignment left side");
						if ((yyvsp[0].lval)->lv.pvs->trans!=0)
							yyerror("pvar with transformations in assignment"
									" left side");
					}
					(yyval.lval)=(yyvsp[0].lval);
				}
#line 9856 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 676:
#line 2827 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.rv_expr)=mk_rve_rval(RV_INT, (void*)(yyvsp[0].intval)); }
#line 9862 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 677:
#line 2828 "core/cfg.y" /* yacc.c:1646  */
    {	s_tmp.s=(yyvsp[0].strval); s_tmp.len=strlen((yyvsp[0].strval));
							(yyval.rv_expr)=mk_rve_rval(RV_STR, &s_tmp); }
#line 9869 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 678:
#line 2830 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.rv_expr)=mk_rve_rval(RV_AVP, (yyvsp[0].attr)); pkg_free((yyvsp[0].attr)); }
#line 9875 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 679:
#line 2831 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.rv_expr)=mk_rve_rval(RV_PVAR, (yyvsp[0].pvar)); }
#line 9881 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 680:
#line 2832 "core/cfg.y" /* yacc.c:1646  */
    {
							switch((yyvsp[0].lval)->type){
								case LV_AVP:
									(yyval.rv_expr)=mk_rve_rval(RV_AVP, &(yyvsp[0].lval)->lv.avps);
									break;
								case LV_PVAR:
									(yyval.rv_expr)=mk_rve_rval(RV_PVAR, (yyvsp[0].lval)->lv.pvs);
									break;
								default:
									yyerror("BUG: invalid lvalue type ");
									YYABORT;
							}
							pkg_free((yyvsp[0].lval)); /* not needed anymore */
						}
#line 9900 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 681:
#line 2846 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.rv_expr)=mk_rve_rval(RV_SEL, (yyvsp[0].select)); pkg_free((yyvsp[0].select)); }
#line 9906 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 682:
#line 2847 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.rv_expr)=mk_rve_rval(RV_ACTION_ST, (yyvsp[0].action)); }
#line 9912 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 683:
#line 2848 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=mk_rve_rval(RV_BEXPR, (yyvsp[0].expr)); }
#line 9918 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 684:
#line 2849 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.rv_expr)=mk_rve_rval(RV_ACTION_ST, (yyvsp[-1].action)); }
#line 9924 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 685:
#line 2850 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=0; yyerror("bad command block"); }
#line 9930 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 686:
#line 2851 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.rv_expr)=mk_rve_rval(RV_ACTION_ST, (yyvsp[-1].action)); }
#line 9936 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 687:
#line 2852 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=0; yyerror("bad expression"); }
#line 9942 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 688:
#line 2856 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=RVE_LNOT_OP; }
#line 9948 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 689:
#line 2857 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=RVE_BNOT_OP; }
#line 9954 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 690:
#line 2858 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval)=RVE_UMINUS_OP; }
#line 9960 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 691:
#line 2871 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=(yyvsp[0].rv_expr);
										if ((yyval.rv_expr)==0){
											/*yyerror("out of memory\n");*/
											YYERROR;
										}
									}
#line 9971 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 692:
#line 2877 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.rv_expr)=mk_rve1((yyvsp[-1].intval), (yyvsp[0].rv_expr)); }
#line 9977 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 693:
#line 2878 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.rv_expr)=mk_rve1(RVE_INT_OP, (yyvsp[0].rv_expr)); }
#line 9983 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 694:
#line 2879 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.rv_expr)=mk_rve1(RVE_STR_OP, (yyvsp[0].rv_expr)); }
#line 9989 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 695:
#line 2880 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.rv_expr)=mk_rve2(RVE_PLUS_OP, (yyvsp[-2].rv_expr), (yyvsp[0].rv_expr)); }
#line 9995 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 696:
#line 2881 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.rv_expr)=mk_rve2(RVE_MINUS_OP, (yyvsp[-2].rv_expr), (yyvsp[0].rv_expr)); }
#line 10001 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 697:
#line 2882 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.rv_expr)=mk_rve2(RVE_MUL_OP, (yyvsp[-2].rv_expr), (yyvsp[0].rv_expr)); }
#line 10007 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 698:
#line 2883 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.rv_expr)=mk_rve2(RVE_DIV_OP, (yyvsp[-2].rv_expr), (yyvsp[0].rv_expr)); }
#line 10013 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 699:
#line 2884 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.rv_expr)=mk_rve2(RVE_MOD_OP, (yyvsp[-2].rv_expr), (yyvsp[0].rv_expr)); }
#line 10019 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 700:
#line 2885 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.rv_expr)=mk_rve2(RVE_BOR_OP, (yyvsp[-2].rv_expr),  (yyvsp[0].rv_expr)); }
#line 10025 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 701:
#line 2886 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.rv_expr)=mk_rve2(RVE_BAND_OP, (yyvsp[-2].rv_expr),  (yyvsp[0].rv_expr));}
#line 10031 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 702:
#line 2887 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.rv_expr)=mk_rve2(RVE_BXOR_OP, (yyvsp[-2].rv_expr),  (yyvsp[0].rv_expr));}
#line 10037 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 703:
#line 2888 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.rv_expr)=mk_rve2(RVE_BLSHIFT_OP, (yyvsp[-2].rv_expr),  (yyvsp[0].rv_expr));}
#line 10043 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 704:
#line 2889 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.rv_expr)=mk_rve2(RVE_BRSHIFT_OP, (yyvsp[-2].rv_expr),  (yyvsp[0].rv_expr));}
#line 10049 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 705:
#line 2890 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=mk_rve2( (yyvsp[-1].intval), (yyvsp[-2].rv_expr), (yyvsp[0].rv_expr));}
#line 10055 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 706:
#line 2891 "core/cfg.y" /* yacc.c:1646  */
    {
			/* comparing with $null => treat as defined or !defined */
			if((yyvsp[0].rv_expr)->op==RVE_RVAL_OP && (yyvsp[0].rv_expr)->left.rval.type==RV_PVAR
					&& (yyvsp[0].rv_expr)->left.rval.v.pvs.type==PVT_NULL) {
				if((yyvsp[-1].intval)==RVE_DIFF_OP || (yyvsp[-1].intval)==RVE_IDIFF_OP
						|| (yyvsp[-1].intval)==RVE_STRDIFF_OP) {
					DBG("comparison with $null switched to notdefined operator\n");
					(yyval.rv_expr)=mk_rve1(RVE_DEFINED_OP, (yyvsp[-2].rv_expr));
				} else {
					DBG("comparison with $null switched to defined operator\n");
					(yyval.rv_expr)=mk_rve1(RVE_NOTDEFINED_OP, (yyvsp[-2].rv_expr));
				}
				/* free rve struct for $null */
				rve_destroy((yyvsp[0].rv_expr));
			} else {
				(yyval.rv_expr)=mk_rve2((yyvsp[-1].intval), (yyvsp[-2].rv_expr), (yyvsp[0].rv_expr));
			}
		}
#line 10078 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 707:
#line 2909 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=mk_rve2(RVE_LAND_OP, (yyvsp[-2].rv_expr), (yyvsp[0].rv_expr));}
#line 10084 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 708:
#line 2910 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=mk_rve2(RVE_LOR_OP, (yyvsp[-2].rv_expr), (yyvsp[0].rv_expr));}
#line 10090 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 709:
#line 2911 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=(yyvsp[-1].rv_expr);}
#line 10096 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 710:
#line 2912 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=mk_rve1(RVE_STRLEN_OP, (yyvsp[-1].rv_expr));}
#line 10102 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 711:
#line 2913 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.rv_expr)=mk_rve1(RVE_STREMPTY_OP, (yyvsp[-1].rv_expr));}
#line 10108 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 712:
#line 2914 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=mk_rve1(RVE_DEFINED_OP, (yyvsp[0].rv_expr));}
#line 10114 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 713:
#line 2915 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=0; yyerror("bad expression"); }
#line 10120 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 714:
#line 2916 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=0; yyerror("bad expression"); }
#line 10126 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 715:
#line 2917 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=0; yyerror("bad expression"); }
#line 10132 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 716:
#line 2918 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=0; yyerror("bad expression"); }
#line 10138 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 717:
#line 2919 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=0; yyerror("bad expression"); }
#line 10144 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 718:
#line 2920 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=0; yyerror("bad expression"); }
#line 10150 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 719:
#line 2921 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=0; yyerror("bad expression"); }
#line 10156 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 720:
#line 2922 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=0; yyerror("bad expression"); }
#line 10162 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 721:
#line 2923 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=0; yyerror("bad expression"); }
#line 10168 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 722:
#line 2924 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=0; yyerror("bad expression"); }
#line 10174 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 723:
#line 2926 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=0; yyerror("bad expression"); }
#line 10180 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 724:
#line 2928 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=0; yyerror("bad expression"); }
#line 10186 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 725:
#line 2929 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=0; yyerror("bad expression"); }
#line 10192 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 726:
#line 2930 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=0; yyerror("bad expression"); }
#line 10198 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 727:
#line 2931 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=0; yyerror("bad expression"); }
#line 10204 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 728:
#line 2932 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=0; yyerror("bad expression"); }
#line 10210 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 729:
#line 2933 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.rv_expr)=0; yyerror("bad expression"); }
#line 10216 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 730:
#line 2936 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action((yyvsp[-1].intval), 2, LVAL_ST, (yyvsp[-2].lval),
														 	  RVE_ST, (yyvsp[0].rv_expr));
											set_cfg_pos((yyval.action));
										}
#line 10225 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 731:
#line 2954 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval) = 1; }
#line 10231 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 732:
#line 2955 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval) = 0; }
#line 10237 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 733:
#line 2956 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.intval) = -1; }
#line 10243 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 734:
#line 2959 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_T, 2, URIHOST_ST, 0, URIPORT_ST, 0); set_cfg_pos((yyval.action)); }
#line 10249 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 735:
#line 2960 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(	FORWARD_T, 2, STRING_ST, (yyvsp[-1].strval), NUMBER_ST, 0); set_cfg_pos((yyval.action)); }
#line 10255 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 736:
#line 2961 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(	FORWARD_T, 2, STRING_ST, (yyvsp[-1].strval), NUMBER_ST, 0); set_cfg_pos((yyval.action)); }
#line 10261 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 737:
#line 2962 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(	FORWARD_T, 2, IP_ST, (void*)(yyvsp[-1].ipaddr), NUMBER_ST, 0); set_cfg_pos((yyval.action)); }
#line 10267 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 738:
#line 2963 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST, (void*)(yyvsp[-1].intval)); set_cfg_pos((yyval.action)); }
#line 10273 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 739:
#line 2964 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(FORWARD_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST, (void*)(yyvsp[-1].intval)); set_cfg_pos((yyval.action)); }
#line 10279 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 740:
#line 2965 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_T, 2, IP_ST, (void*)(yyvsp[-3].ipaddr), NUMBER_ST, (void*)(yyvsp[-1].intval)); set_cfg_pos((yyval.action)); }
#line 10285 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 741:
#line 2966 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_T, 2, URIHOST_ST, 0, URIPORT_ST, 0); set_cfg_pos((yyval.action)); }
#line 10291 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 742:
#line 2967 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(FORWARD_T, 2, URIHOST_ST, 0, NUMBER_ST, (void*)(yyvsp[-1].intval)); set_cfg_pos((yyval.action)); }
#line 10297 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 743:
#line 2968 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_T, 2, URIHOST_ST, 0, NUMBER_ST, 0); set_cfg_pos((yyval.action)); }
#line 10303 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 744:
#line 2969 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 10309 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 745:
#line 2970 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad forward argument"); }
#line 10315 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 746:
#line 2971 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_UDP_T, 2, STRING_ST, (yyvsp[-1].strval), NUMBER_ST, 0); set_cfg_pos((yyval.action)); }
#line 10321 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 747:
#line 2972 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_UDP_T, 2, STRING_ST, (yyvsp[-1].strval), NUMBER_ST, 0); set_cfg_pos((yyval.action)); }
#line 10327 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 748:
#line 2973 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_UDP_T, 2, IP_ST, (void*)(yyvsp[-1].ipaddr), NUMBER_ST, 0); set_cfg_pos((yyval.action)); }
#line 10333 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 749:
#line 2974 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_UDP_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST, (void*)(yyvsp[-1].intval)); set_cfg_pos((yyval.action)); }
#line 10339 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 750:
#line 2975 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(FORWARD_UDP_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST, (void*)(yyvsp[-1].intval)); set_cfg_pos((yyval.action)); }
#line 10345 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 751:
#line 2976 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_UDP_T, 2, IP_ST, (void*)(yyvsp[-3].ipaddr), NUMBER_ST, (void*)(yyvsp[-1].intval)); set_cfg_pos((yyval.action)); }
#line 10351 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 752:
#line 2977 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(FORWARD_UDP_T, 2, URIHOST_ST, 0, URIPORT_ST, 0); set_cfg_pos((yyval.action)); }
#line 10357 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 753:
#line 2978 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_UDP_T, 2, URIHOST_ST, 0, NUMBER_ST, (void*)(yyvsp[-1].intval)); set_cfg_pos((yyval.action)); }
#line 10363 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 754:
#line 2979 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_UDP_T, 2, URIHOST_ST, 0, NUMBER_ST, 0); set_cfg_pos((yyval.action)); }
#line 10369 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 755:
#line 2980 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 10375 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 756:
#line 2981 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad forward_udp argument"); }
#line 10381 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 757:
#line 2982 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_TCP_T, 2, STRING_ST, (yyvsp[-1].strval), NUMBER_ST, 0); set_cfg_pos((yyval.action)); }
#line 10387 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 758:
#line 2983 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_TCP_T, 2, STRING_ST, (yyvsp[-1].strval), NUMBER_ST, 0); set_cfg_pos((yyval.action)); }
#line 10393 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 759:
#line 2984 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_TCP_T, 2, IP_ST, (void*)(yyvsp[-1].ipaddr), NUMBER_ST, 0); set_cfg_pos((yyval.action)); }
#line 10399 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 760:
#line 2985 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_TCP_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST, (void*)(yyvsp[-1].intval)); set_cfg_pos((yyval.action)); }
#line 10405 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 761:
#line 2986 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(FORWARD_TCP_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST, (void*)(yyvsp[-1].intval)); set_cfg_pos((yyval.action)); }
#line 10411 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 762:
#line 2987 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_TCP_T, 2, IP_ST, (void*)(yyvsp[-3].ipaddr), NUMBER_ST, (void*)(yyvsp[-1].intval)); set_cfg_pos((yyval.action)); }
#line 10417 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 763:
#line 2988 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(FORWARD_TCP_T, 2, URIHOST_ST, 0, URIPORT_ST, 0); set_cfg_pos((yyval.action)); }
#line 10423 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 764:
#line 2989 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_TCP_T, 2, URIHOST_ST, 0, NUMBER_ST, (void*)(yyvsp[-1].intval)); set_cfg_pos((yyval.action)); }
#line 10429 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 765:
#line 2990 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORWARD_TCP_T, 2, URIHOST_ST, 0, NUMBER_ST, 0); set_cfg_pos((yyval.action)); }
#line 10435 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 766:
#line 2991 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 10441 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 767:
#line 2992 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad forward_tcp argument"); }
#line 10447 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 768:
#line 2993 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			(yyval.action)=mk_action(FORWARD_TLS_T, 2, STRING_ST, (yyvsp[-1].strval), NUMBER_ST, 0); set_cfg_pos((yyval.action));
		#else
			(yyval.action)=0;
			yyerror("tls support not compiled in");
		#endif
	}
#line 10460 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 769:
#line 3001 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			(yyval.action)=mk_action(FORWARD_TLS_T, 2, STRING_ST, (yyvsp[-1].strval), NUMBER_ST, 0); set_cfg_pos((yyval.action));
		#else
			(yyval.action)=0;
			yyerror("tls support not compiled in");
		#endif
	}
#line 10473 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 770:
#line 3009 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			(yyval.action)=mk_action(FORWARD_TLS_T, 2, IP_ST, (void*)(yyvsp[-1].ipaddr), NUMBER_ST, 0); set_cfg_pos((yyval.action));
		#else
			(yyval.action)=0;
			yyerror("tls support not compiled in");
		#endif
	}
#line 10486 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 771:
#line 3017 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			(yyval.action)=mk_action(FORWARD_TLS_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST, (void*)(yyvsp[-1].intval)); set_cfg_pos((yyval.action));
		#else
			(yyval.action)=0;
			yyerror("tls support not compiled in");
		#endif
	}
#line 10499 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 772:
#line 3025 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			(yyval.action)=mk_action(FORWARD_TLS_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST, (void*)(yyvsp[-1].intval)); set_cfg_pos((yyval.action));
		#else
			(yyval.action)=0;
			yyerror("tls support not compiled in");
		#endif
	}
#line 10512 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 773:
#line 3033 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			(yyval.action)=mk_action(FORWARD_TLS_T, 2, IP_ST, (void*)(yyvsp[-3].ipaddr), NUMBER_ST, (void*)(yyvsp[-1].intval)); set_cfg_pos((yyval.action));
		#else
			(yyval.action)=0;
			yyerror("tls support not compiled in");
		#endif
					}
#line 10525 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 774:
#line 3041 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			(yyval.action)=mk_action(FORWARD_TLS_T, 2, URIHOST_ST, 0, URIPORT_ST, 0); set_cfg_pos((yyval.action));
		#else
			(yyval.action)=0;
			yyerror("tls support not compiled in");
		#endif
	}
#line 10538 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 775:
#line 3049 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			(yyval.action)=mk_action(FORWARD_TLS_T, 2, URIHOST_ST, 0, NUMBER_ST, (void*)(yyvsp[-1].intval)); set_cfg_pos((yyval.action));
		#else
			(yyval.action)=0;
			yyerror("tls support not compiled in");
		#endif
	}
#line 10551 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 776:
#line 3057 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TLS
			(yyval.action)=mk_action(FORWARD_TLS_T, 2, URIHOST_ST, 0, NUMBER_ST, 0); set_cfg_pos((yyval.action));
		#else
			(yyval.action)=0;
			yyerror("tls support not compiled in");
		#endif
	}
#line 10564 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 777:
#line 3065 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 10570 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 778:
#line 3066 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0;
									yyerror("bad forward_tls argument"); }
#line 10577 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 779:
#line 3068 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_SCTP
			(yyval.action)=mk_action(FORWARD_SCTP_T, 2, STRING_ST, (yyvsp[-1].strval), NUMBER_ST, 0); set_cfg_pos((yyval.action));
		#else
			(yyval.action)=0;
			yyerror("sctp support not compiled in");
		#endif
	}
#line 10590 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 780:
#line 3076 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_SCTP
			(yyval.action)=mk_action(FORWARD_SCTP_T, 2, STRING_ST, (yyvsp[-1].strval), NUMBER_ST, 0); set_cfg_pos((yyval.action));
		#else
			(yyval.action)=0;
			yyerror("sctp support not compiled in");
		#endif
	}
#line 10603 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 781:
#line 3084 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_SCTP
			(yyval.action)=mk_action(FORWARD_SCTP_T, 2, IP_ST, (void*)(yyvsp[-1].ipaddr), NUMBER_ST, 0); set_cfg_pos((yyval.action));
		#else
			(yyval.action)=0;
			yyerror("sctp support not compiled in");
		#endif
	}
#line 10616 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 782:
#line 3092 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_SCTP
			(yyval.action)=mk_action(FORWARD_SCTP_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST,
							(void*)(yyvsp[-1].intval)); set_cfg_pos((yyval.action));
		#else
			(yyval.action)=0;
			yyerror("sctp support not compiled in");
		#endif
	}
#line 10630 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 783:
#line 3101 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_SCTP
			(yyval.action)=mk_action(FORWARD_SCTP_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST,
							(void*)(yyvsp[-1].intval)); set_cfg_pos((yyval.action));
		#else
			(yyval.action)=0;
			yyerror("sctp support not compiled in");
		#endif
	}
#line 10644 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 784:
#line 3110 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_SCTP
			(yyval.action)=mk_action(FORWARD_SCTP_T, 2, IP_ST, (void*)(yyvsp[-3].ipaddr), NUMBER_ST,
							(void*)(yyvsp[-1].intval)); set_cfg_pos((yyval.action));
		#else
			(yyval.action)=0;
			yyerror("sctp support not compiled in");
		#endif
					}
#line 10658 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 785:
#line 3119 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_SCTP
			(yyval.action)=mk_action(FORWARD_SCTP_T, 2, URIHOST_ST, 0, URIPORT_ST, 0); set_cfg_pos((yyval.action));
		#else
			(yyval.action)=0;
			yyerror("sctp support not compiled in");
		#endif
	}
#line 10671 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 786:
#line 3127 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_SCTP
			(yyval.action)=mk_action(FORWARD_SCTP_T, 2, URIHOST_ST, 0, NUMBER_ST,
							(void*)(yyvsp[-1].intval)); set_cfg_pos((yyval.action));
		#else
			(yyval.action)=0;
			yyerror("sctp support not compiled in");
		#endif
	}
#line 10685 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 787:
#line 3136 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_SCTP
			(yyval.action)=mk_action(FORWARD_SCTP_T, 2, URIHOST_ST, 0, NUMBER_ST, 0); set_cfg_pos((yyval.action));
		#else
			(yyval.action)=0;
			yyerror("tls support not compiled in");
		#endif
	}
#line 10698 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 788:
#line 3144 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 10704 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 789:
#line 3145 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0;
									yyerror("bad forward_sctp argument"); }
#line 10711 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 790:
#line 3147 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(LOG_T, 2, NUMBER_ST,
										(void*)(L_DBG+1), STRING_ST, (yyvsp[-1].strval));
									set_cfg_pos((yyval.action)); }
#line 10719 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 791:
#line 3150 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(LOG_T, 2, NUMBER_ST, (void*)(yyvsp[-3].intval), STRING_ST, (yyvsp[-1].strval)); set_cfg_pos((yyval.action)); }
#line 10725 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 792:
#line 3151 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 10731 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 793:
#line 3152 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad log argument"); }
#line 10737 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 794:
#line 3153 "core/cfg.y" /* yacc.c:1646  */
    {
							if (check_flag((yyvsp[-1].intval))==-1)
								yyerror("bad flag value");
							(yyval.action)=mk_action(SETFLAG_T, 1, NUMBER_ST,
													(void*)(yyvsp[-1].intval));
							set_cfg_pos((yyval.action));
									}
#line 10749 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 795:
#line 3160 "core/cfg.y" /* yacc.c:1646  */
    {
							i_tmp=get_flag_no((yyvsp[-1].strval), strlen((yyvsp[-1].strval)));
							if (i_tmp<0) yyerror("flag not declared");
							(yyval.action)=mk_action(SETFLAG_T, 1, NUMBER_ST,
										(void*)(long)i_tmp);
							set_cfg_pos((yyval.action));
									}
#line 10761 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 796:
#line 3167 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')'?"); }
#line 10767 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 797:
#line 3168 "core/cfg.y" /* yacc.c:1646  */
    {
							if (check_flag((yyvsp[-1].intval))==-1)
								yyerror("bad flag value");
							(yyval.action)=mk_action(RESETFLAG_T, 1, NUMBER_ST, (void*)(yyvsp[-1].intval));
							set_cfg_pos((yyval.action));
									}
#line 10778 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 798:
#line 3174 "core/cfg.y" /* yacc.c:1646  */
    {
							i_tmp=get_flag_no((yyvsp[-1].strval), strlen((yyvsp[-1].strval)));
							if (i_tmp<0) yyerror("flag not declared");
							(yyval.action)=mk_action(RESETFLAG_T, 1, NUMBER_ST,
										(void*)(long)i_tmp);
							set_cfg_pos((yyval.action));
									}
#line 10790 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 799:
#line 3181 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')'?"); }
#line 10796 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 800:
#line 3182 "core/cfg.y" /* yacc.c:1646  */
    {
							if (check_flag((yyvsp[-1].intval))==-1)
								yyerror("bad flag value");
							(yyval.action)=mk_action(ISFLAGSET_T, 1, NUMBER_ST, (void*)(yyvsp[-1].intval));
							set_cfg_pos((yyval.action));
									}
#line 10807 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 801:
#line 3188 "core/cfg.y" /* yacc.c:1646  */
    {
							i_tmp=get_flag_no((yyvsp[-1].strval), strlen((yyvsp[-1].strval)));
							if (i_tmp<0) yyerror("flag not declared");
							(yyval.action)=mk_action(ISFLAGSET_T, 1, NUMBER_ST,
										(void*)(long)i_tmp);
							set_cfg_pos((yyval.action));
									}
#line 10819 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 802:
#line 3195 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')'?"); }
#line 10825 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 803:
#line 3196 "core/cfg.y" /* yacc.c:1646  */
    {
		i_tmp=get_avpflag_no((yyvsp[-1].strval));
		if (i_tmp==0) yyerror("avpflag not declared");
		(yyval.action)=mk_action(AVPFLAG_OPER_T, 3, AVP_ST, (yyvsp[-3].attr), NUMBER_ST, (void*)(long)i_tmp, NUMBER_ST, (void*)(yyvsp[-5].intval));
		set_cfg_pos((yyval.action));
	}
#line 10836 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 804:
#line 3202 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=0; yyerror("error parsing flag name");
	}
#line 10844 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 805:
#line 3205 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=0; yyerror("error parsing first parameter (avp or string)");
	}
#line 10852 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 806:
#line 3208 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad parameters"); }
#line 10858 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 807:
#line 3209 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')'?"); }
#line 10864 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 808:
#line 3210 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(ERROR_T, 2, STRING_ST, (yyvsp[-3].strval), STRING_ST, (yyvsp[-1].strval));
			set_cfg_pos((yyval.action));
	}
#line 10872 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 809:
#line 3213 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 10878 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 810:
#line 3214 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad error argument"); }
#line 10884 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 811:
#line 3215 "core/cfg.y" /* yacc.c:1646  */
    {
		if ((yyvsp[-1].rv_expr)) {
			(yyval.action) = mk_action(ROUTE_T, 1, RVE_ST, (void*)(yyvsp[-1].rv_expr));
			set_cfg_pos((yyval.action));
		} else {
			(yyval.action) = 0;
			YYERROR;
		}
	}
#line 10898 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 812:
#line 3224 "core/cfg.y" /* yacc.c:1646  */
    {
		if ((yyvsp[-1].strval)) {
			(yyval.action) = mk_action(ROUTE_T, 1, STRING_ST, (void*)(yyvsp[-1].strval));
			set_cfg_pos((yyval.action));
		} else {
			(yyval.action) = 0;
			YYERROR;
		}
	}
#line 10912 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 813:
#line 3233 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 10918 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 814:
#line 3234 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad route argument"); }
#line 10924 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 815:
#line 3235 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(EXEC_T, 1, STRING_ST, (yyvsp[-1].strval)); set_cfg_pos((yyval.action)); }
#line 10930 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 816:
#line 3236 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(SET_HOST_T, 1, STRING_ST, (yyvsp[-1].strval)); set_cfg_pos((yyval.action)); }
#line 10936 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 817:
#line 3237 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 10942 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 818:
#line 3238 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, string expected"); }
#line 10948 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 819:
#line 3239 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(PREFIX_T, 1, STRING_ST,  (yyvsp[-1].strval)); set_cfg_pos((yyval.action)); }
#line 10954 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 820:
#line 3240 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 10960 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 821:
#line 3241 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, string expected"); }
#line 10966 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 822:
#line 3242 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(STRIP_TAIL_T, 1, NUMBER_ST, (void*)(yyvsp[-1].intval)); set_cfg_pos((yyval.action)); }
#line 10972 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 823:
#line 3243 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 10978 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 824:
#line 3244 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, number expected"); }
#line 10984 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 825:
#line 3245 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(STRIP_T, 1, NUMBER_ST, (void*) (yyvsp[-1].intval)); set_cfg_pos((yyval.action)); }
#line 10990 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 826:
#line 3246 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 10996 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 827:
#line 3247 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, number expected"); }
#line 11002 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 828:
#line 3248 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(SET_USERPHONE_T, 0); set_cfg_pos((yyval.action)); }
#line 11008 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 829:
#line 3249 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 11014 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 830:
#line 3250 "core/cfg.y" /* yacc.c:1646  */
    {
			(yyval.action)=mk_action(REMOVE_BRANCH_T, 1, NUMBER_ST, (void*)(yyvsp[-1].intval));
			set_cfg_pos((yyval.action));
	}
#line 11023 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 831:
#line 3254 "core/cfg.y" /* yacc.c:1646  */
    {
			(yyval.action)=mk_action(REMOVE_BRANCH_T, 0);
			set_cfg_pos((yyval.action));
	}
#line 11032 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 832:
#line 3258 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 11038 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 833:
#line 3259 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, number expected"); }
#line 11044 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 834:
#line 3260 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(CLEAR_BRANCHES_T, 0); set_cfg_pos((yyval.action)); }
#line 11050 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 835:
#line 3261 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(SET_HOSTPORT_T, 1, STRING_ST, (yyvsp[-1].strval)); set_cfg_pos((yyval.action)); }
#line 11056 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 836:
#line 3262 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 11062 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 837:
#line 3263 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, string expected"); }
#line 11068 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 838:
#line 3264 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(SET_HOSTPORTTRANS_T, 1, STRING_ST, (yyvsp[-1].strval)); set_cfg_pos((yyval.action)); }
#line 11074 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 839:
#line 3265 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 11080 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 840:
#line 3266 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, string expected"); }
#line 11086 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 841:
#line 3267 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(SET_PORT_T, 1, STRING_ST, (yyvsp[-1].strval)); set_cfg_pos((yyval.action)); }
#line 11092 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 842:
#line 3268 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 11098 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 843:
#line 3269 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, string expected"); }
#line 11104 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 844:
#line 3270 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(SET_USER_T, 1, STRING_ST, (yyvsp[-1].strval)); set_cfg_pos((yyval.action)); }
#line 11110 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 845:
#line 3271 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 11116 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 846:
#line 3272 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, string expected"); }
#line 11122 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 847:
#line 3273 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(SET_USERPASS_T, 1, STRING_ST, (yyvsp[-1].strval)); set_cfg_pos((yyval.action)); }
#line 11128 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 848:
#line 3274 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 11134 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 849:
#line 3275 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, string expected"); }
#line 11140 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 850:
#line 3276 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(SET_URI_T, 1, STRING_ST,(yyvsp[-1].strval)); set_cfg_pos((yyval.action)); }
#line 11146 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 851:
#line 3277 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 11152 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 852:
#line 3278 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, string expected"); }
#line 11158 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 853:
#line 3279 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(REVERT_URI_T, 0); set_cfg_pos((yyval.action)); }
#line 11164 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 854:
#line 3280 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(REVERT_URI_T, 0); set_cfg_pos((yyval.action)); }
#line 11170 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 855:
#line 3281 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(FORCE_RPORT_T, 0); set_cfg_pos((yyval.action)); }
#line 11176 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 856:
#line 3282 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(FORCE_RPORT_T, 0); set_cfg_pos((yyval.action)); }
#line 11182 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 857:
#line 3283 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(ADD_LOCAL_RPORT_T, 0); set_cfg_pos((yyval.action)); }
#line 11188 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 858:
#line 3284 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.action)=mk_action(ADD_LOCAL_RPORT_T, 0); set_cfg_pos((yyval.action)); }
#line 11194 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 859:
#line 3285 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			(yyval.action)=mk_action(FORCE_TCP_ALIAS_T, 1, NUMBER_ST, (void*)(yyvsp[-1].intval));
			set_cfg_pos((yyval.action));
		#else
			yyerror("tcp support not compiled in");
		#endif
	}
#line 11207 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 860:
#line 3293 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			(yyval.action)=mk_action(FORCE_TCP_ALIAS_T, 0);
			set_cfg_pos((yyval.action));
		#else
			yyerror("tcp support not compiled in");
		#endif
	}
#line 11220 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 861:
#line 3301 "core/cfg.y" /* yacc.c:1646  */
    {
		#ifdef USE_TCP
			(yyval.action)=mk_action(FORCE_TCP_ALIAS_T, 0);
			set_cfg_pos((yyval.action));
		#else
			yyerror("tcp support not compiled in");
		#endif
	}
#line 11233 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 862:
#line 3309 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.action)=0; yyerror("bad argument, number expected"); }
#line 11239 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 863:
#line 3311 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=mk_action(UDP_MTU_TRY_PROTO_T, 1, NUMBER_ST, (yyvsp[-1].intval)); set_cfg_pos((yyval.action)); }
#line 11245 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 864:
#line 3313 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, UDP, TCP, TLS or SCTP expected"); }
#line 11251 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 865:
#line 3314 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=0;
		if ((str_tmp=pkg_malloc(sizeof(str)))==0) {
			PKG_MEM_CRITICAL;
		} else {
			str_tmp->s=(yyvsp[-1].strval);
			str_tmp->len=(yyvsp[-1].strval)?strlen((yyvsp[-1].strval)):0;
			(yyval.action)=mk_action(SET_ADV_ADDR_T, 1, STR_ST, str_tmp);
			set_cfg_pos((yyval.action));
		}
	}
#line 11267 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 866:
#line 3325 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, string expected"); }
#line 11273 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 867:
#line 3326 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 11279 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 868:
#line 3327 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=0;
		tmp=int2str((yyvsp[-1].intval), &i_tmp);
		if ((str_tmp=pkg_malloc(sizeof(str)))==0) {
			PKG_MEM_CRITICAL;
		} else {
			if ((str_tmp->s=pkg_malloc(i_tmp))==0) {
				PKG_MEM_CRITICAL;
			} else {
				memcpy(str_tmp->s, tmp, i_tmp);
				str_tmp->len=i_tmp;
				(yyval.action)=mk_action(SET_ADV_PORT_T, 1, STR_ST, str_tmp);
				set_cfg_pos((yyval.action));
			}
		}
	}
#line 11300 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 869:
#line 3343 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad argument, string expected"); }
#line 11306 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 870:
#line 3344 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 11312 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 871:
#line 3345 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=mk_action(FORCE_SEND_SOCKET_T, 1, SOCKID_ST, (yyvsp[-1].sockid));
		set_cfg_pos((yyval.action));
	}
#line 11321 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 872:
#line 3349 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=0; yyerror("bad argument, [proto:]host[:port] expected");
	}
#line 11329 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 873:
#line 3352 "core/cfg.y" /* yacc.c:1646  */
    {(yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 11335 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 874:
#line 3353 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=mk_action(SET_FWD_NO_CONNECT_T, 0); set_cfg_pos((yyval.action));
	}
#line 11343 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 875:
#line 3356 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=mk_action(SET_FWD_NO_CONNECT_T, 0); set_cfg_pos((yyval.action));
	}
#line 11351 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 876:
#line 3359 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=mk_action(SET_RPL_NO_CONNECT_T, 0); set_cfg_pos((yyval.action));
	}
#line 11359 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 877:
#line 3362 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=mk_action(SET_RPL_NO_CONNECT_T, 0); set_cfg_pos((yyval.action));
	}
#line 11367 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 878:
#line 3365 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=mk_action(SET_FWD_CLOSE_T, 0); set_cfg_pos((yyval.action));
	}
#line 11375 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 879:
#line 3368 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=mk_action(SET_FWD_CLOSE_T, 0); set_cfg_pos((yyval.action));
	}
#line 11383 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 880:
#line 3371 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=mk_action(SET_RPL_CLOSE_T, 0); set_cfg_pos((yyval.action));
	}
#line 11391 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 881:
#line 3374 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=mk_action(SET_RPL_CLOSE_T, 0); set_cfg_pos((yyval.action));
	}
#line 11399 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 882:
#line 3377 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=mk_action(CFG_SELECT_T, 2, STRING_ST, (yyvsp[-3].strval), NUMBER_ST, (void*)(yyvsp[-1].intval)); set_cfg_pos((yyval.action));
	}
#line 11407 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 883:
#line 3380 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=mk_action(CFG_SELECT_T, 2, STRING_ST, (yyvsp[-3].strval), RVE_ST, (yyvsp[-1].rv_expr)); set_cfg_pos((yyval.action));
	}
#line 11415 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 884:
#line 3383 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 11421 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 885:
#line 3384 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad arguments, string and number expected"); }
#line 11427 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 886:
#line 3385 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=mk_action(CFG_RESET_T, 1, STRING_ST, (yyvsp[-1].strval)); set_cfg_pos((yyval.action));
	}
#line 11435 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 887:
#line 3388 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("missing '(' or ')' ?"); }
#line 11441 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 888:
#line 3389 "core/cfg.y" /* yacc.c:1646  */
    { (yyval.action)=0; yyerror("bad arguments, string expected"); }
#line 11447 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 889:
#line 3390 "core/cfg.y" /* yacc.c:1646  */
    {mod_func_action = mk_action(MODULE0_T, 2, MODEXP_ST, NULL, NUMBER_ST,
			0); }
#line 11454 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 890:
#line 3391 "core/cfg.y" /* yacc.c:1646  */
    {
		mod_func_action->val[0].u.data =
			find_export_record((yyvsp[-4].strval), mod_func_action->val[1].u.number, rt);
		if (mod_func_action->val[0].u.data == 0) {
			if (find_export_record((yyvsp[-4].strval), mod_func_action->val[1].u.number, 0) ) {
					LM_ERR("misused command %s\n", (yyvsp[-4].strval));
					yyerror("Command cannot be used in the block\n");
			} else {
				LM_ERR("cfg. parser: failed to find command %s (params %ld)\n",
						(yyvsp[-4].strval), mod_func_action->val[1].u.number);
				yyerror("unknown command, missing loadmodule?\n");
			}
			free_mod_func_action(mod_func_action);
			mod_func_action=0;
		}else{
			if (mod_func_action && mod_f_params_pre_fixup(mod_func_action)<0) {
				/* error messages are printed inside the function */
				free_mod_func_action(mod_func_action);
				mod_func_action = 0;
				YYERROR;
			}
		}
		(yyval.action) = mod_func_action;
		set_cfg_pos((yyval.action));
	}
#line 11484 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 891:
#line 3416 "core/cfg.y" /* yacc.c:1646  */
    { yyerror("'('')' expected (function call)");}
#line 11490 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 893:
#line 3420 "core/cfg.y" /* yacc.c:1646  */
    { }
#line 11496 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 894:
#line 3421 "core/cfg.y" /* yacc.c:1646  */
    {}
#line 11502 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 895:
#line 3424 "core/cfg.y" /* yacc.c:1646  */
    {
		if ((yyvsp[0].rv_expr) && mod_func_action->val[1].u.number < MAX_ACTIONS-2) {
			mod_func_action->val[mod_func_action->val[1].u.number+2].type =
				RVE_ST;
			mod_func_action->val[mod_func_action->val[1].u.number+2].u.data =
				(yyvsp[0].rv_expr);
			mod_func_action->val[1].u.number++;
		} else if ((yyvsp[0].rv_expr)) {
			yyerror("Too many arguments\n");
			YYERROR;
		} else {
			YYERROR;
		}
	}
#line 11521 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 896:
#line 3441 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=mk_action(DROP_T, 2, NUMBER_ST, 0, NUMBER_ST,
						(void*)(DROP_R_F|EXIT_R_F)); set_cfg_pos((yyval.action));
	}
#line 11530 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 897:
#line 3445 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=mk_action(DROP_T, 2, RVE_ST, (yyvsp[0].rv_expr), NUMBER_ST,
						(void*)(DROP_R_F|EXIT_R_F)); set_cfg_pos((yyval.action));
	}
#line 11539 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 898:
#line 3449 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=mk_action(DROP_T, 2, NUMBER_ST, 0, NUMBER_ST,
						(void*)(DROP_R_F|EXIT_R_F)); set_cfg_pos((yyval.action));
	}
#line 11548 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 899:
#line 3453 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=mk_action(DROP_T, 2, NUMBER_ST, (void*)1, NUMBER_ST,
						(void*)EXIT_R_F);
		set_cfg_pos((yyval.action));
	}
#line 11558 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 900:
#line 3458 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=mk_action(DROP_T, 2, RVE_ST, (yyvsp[0].rv_expr), NUMBER_ST, (void*)EXIT_R_F);
		set_cfg_pos((yyval.action));
	}
#line 11567 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 901:
#line 3462 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=mk_action(DROP_T, 2, NUMBER_ST, (void*)1, NUMBER_ST,
						(void*)EXIT_R_F);
		set_cfg_pos((yyval.action));
	}
#line 11577 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 902:
#line 3467 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=mk_action(DROP_T, 2, NUMBER_ST, (void*)1, NUMBER_ST,
						(void*)RETURN_R_F); set_cfg_pos((yyval.action));
	}
#line 11586 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 903:
#line 3471 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=mk_action(DROP_T, 2, NUMBER_ST, (void*)1, NUMBER_ST,
						(void*)RETURN_R_F); set_cfg_pos((yyval.action));
	}
#line 11595 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 904:
#line 3475 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=mk_action(DROP_T, 2, RVE_ST, (yyvsp[0].rv_expr), NUMBER_ST, (void*)RETURN_R_F);
		set_cfg_pos((yyval.action));
	}
#line 11604 "core/cfg.tab.c" /* yacc.c:1646  */
    break;

  case 905:
#line 3479 "core/cfg.y" /* yacc.c:1646  */
    {
		(yyval.action)=mk_action(DROP_T, 2, NUMBER_ST, 0, NUMBER_ST, (void*)BREAK_R_F);
		set_cfg_pos((yyval.action));
	}
#line 11613 "core/cfg.tab.c" /* yacc.c:1646  */
    break;


#line 11617 "core/cfg.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 3485 "core/cfg.y" /* yacc.c:1906  */


static void get_cpos(struct cfg_pos* pos)
{
	pos->s_line=startline;
	pos->e_line=line;
	pos->s_col=startcolumn;
	pos->e_col=column-1;
	if(finame==0)
		finame = (cfg_file!=0)?cfg_file:"default";
	pos->fname=finame;
	pos->rname=(routename!=0)?routename:default_routename;
}


static void warn_at(struct cfg_pos* p, char* format, ...)
{
	va_list ap;
	char s[256];

	va_start(ap, format);
	vsnprintf(s, sizeof(s), format, ap);
	va_end(ap);
	if (p->e_line!=p->s_line)
		LM_WARN("warning in config file %s, from line %d, column %d to"
					" line %d, column %d: %s\n",
					p->fname, p->s_line, p->s_col, p->e_line, p->e_col, s);
	else if (p->s_col!=p->e_col)
		LM_WARN("warning in config file %s, line %d, column %d-%d: %s\n",
					p->fname, p->s_line, p->s_col, p->e_col, s);
	else
		LM_WARN("warning in config file %s, line %d, column %d: %s\n",
				p->fname, p->s_line, p->s_col, s);
	cfg_warnings++;
}



static void yyerror_at(struct cfg_pos* p, char* format, ...)
{
	va_list ap;
	char s[256];

	va_start(ap, format);
	vsnprintf(s, sizeof(s), format, ap);
	va_end(ap);
	if (p->e_line!=p->s_line)
		LM_CRIT("parse error in config file %s, from line %d, column %d"
					" to line %d, column %d: %s\n",
					p->fname, p->s_line, p->s_col, p->e_line, p->e_col, s);
	else if (p->s_col!=p->e_col)
		LM_CRIT("parse error in config file %s, line %d, column %d-%d: %s\n",
					p->fname, p->s_line, p->s_col, p->e_col, s);
	else
		LM_CRIT("parse error in config file %s, line %d, column %d: %s\n",
					p->fname, p->s_line, p->s_col, s);
	cfg_errors++;
}



static void warn(char* format, ...)
{
	va_list ap;
	char s[256];
	struct cfg_pos pos;

	get_cpos(&pos);
	va_start(ap, format);
	vsnprintf(s, sizeof(s), format, ap);
	va_end(ap);
	warn_at(&pos, s);
}



static void yyerror(char* format, ...)
{
	va_list ap;
	char s[256];
	struct cfg_pos pos;

	get_cpos(&pos);
	va_start(ap, format);
	vsnprintf(s, sizeof(s), format, ap);
	va_end(ap);
	yyerror_at(&pos, s);
}



/** mk_rval_expr_v wrapper.
 *  checks mk_rval_expr_v return value and sets the cfg. pos
 *  (line and column numbers)
 *  @return rval_expr* on success, 0 on error (@see mk_rval_expr_v)
 */
static struct rval_expr* mk_rve_rval(enum rval_type type, void* v)
{
	struct rval_expr* ret;
	struct cfg_pos pos;

	get_cpos(&pos);
	ret=mk_rval_expr_v(type, v, &pos);
	if (ret==0){
		yyerror("internal error: failed to create rval expr");
		/* YYABORT; */
	}
	return ret;
}


/** mk_rval_expr1 wrapper.
 *  checks mk_rval_expr1 return value (!=0 and type checking)
 *  @return rval_expr* on success, 0 on error (@see mk_rval_expr1)
 */
static struct rval_expr* mk_rve1(enum rval_expr_op op, struct rval_expr* rve1)
{
	struct rval_expr* ret;
	struct rval_expr* bad_rve;
	enum rval_type type, bad_t, exp_t;

	if (rve1==0)
		return 0;
	ret=mk_rval_expr1(op, rve1, &rve1->fpos);
	if (ret && (rve_check_type(&type, ret, &bad_rve, &bad_t, &exp_t)!=1)){
		yyerror_at(&rve1->fpos, "bad expression: type mismatch"
					" (%s instead of %s)", rval_type_name(bad_t),
					rval_type_name(exp_t));
		rve_destroy(ret);
		ret=0;
	}
	return ret;
}


/** mk_rval_expr2 wrapper.
 *  checks mk_rval_expr2 return value (!=0 and type checking)
 *  @return rval_expr* on success, 0 on error (@see mk_rval_expr2)
 */
static struct rval_expr* mk_rve2(enum rval_expr_op op, struct rval_expr* rve1,
									struct rval_expr* rve2)
{
	struct rval_expr* ret;
	struct rval_expr* bad_rve;
	enum rval_type type, bad_t, exp_t;
	struct cfg_pos pos;

	if ((rve1==0) || (rve2==0))
		return 0;
	bad_rve=0;
	bad_t=0;
	exp_t=0;
	cfg_pos_join(&pos, &rve1->fpos, &rve2->fpos);
	ret=mk_rval_expr2(op, rve1, rve2, &pos);
	if (ret && (rve_check_type(&type, ret, &bad_rve, &bad_t, &exp_t)!=1)){
		if (bad_rve)
			yyerror_at(&pos, "bad expression: type mismatch:"
						" %s instead of %s at (%d,%d)",
						rval_type_name(bad_t), rval_type_name(exp_t),
						bad_rve->fpos.s_line, bad_rve->fpos.s_col);
		else
			yyerror("BUG: unexpected null \"bad\" expression\n");
		rve_destroy(ret);
		ret=0;
	}
	return ret;
}


/** check if the expression is an int.
 * if the expression does not evaluate to an int return -1 and
 * log an error.
 * @return 0 success, no warnings; 1 success but warnings; -1 on error */
static int rval_expr_int_check(struct rval_expr *rve)
{
	struct rval_expr* bad_rve;
	enum rval_type type, bad_t, exp_t;

	if (rve==0){
		yyerror("invalid expression");
		return -1;
	}else if (!rve_check_type(&type, rve, &bad_rve, &bad_t ,&exp_t)){
		if (bad_rve)
			yyerror_at(&rve->fpos, "bad expression: type mismatch:"
						" %s instead of %s at (%d,%d)",
						rval_type_name(bad_t), rval_type_name(exp_t),
						bad_rve->fpos.s_line, bad_rve->fpos.s_col);
		else
			yyerror("BUG: unexpected null \"bad\" expression\n");
		return -1;
	}else if (type!=RV_INT && type!=RV_NONE){
		warn_at(&rve->fpos, "non-int expression (you might want to use"
				" casts)\n");
		return 1;
	}
	return 0;
}


/** warn if the expression is constant.
 * @return 0 on success (no warning), 1 when warning */
static int warn_ct_rve(struct rval_expr *rve, char* name)
{
	if (rve && rve_is_constant(rve)){
		warn_at(&rve->fpos, "constant value in %s%s",
				name?name:"expression", name?"(...)":"");
		return 1;
	}
	return 0;
}


static struct name_lst* mk_name_lst(char* host, int flags)
{
	struct name_lst* l;
	if (host==0) return 0;
	l=pkg_malloc(sizeof(struct name_lst));
	if (l==0) {
		PKG_MEM_CRITICAL;
	} else {
		l->name=host;
		l->flags=flags;
		l->next=0;
	}
	return l;
}


static struct socket_id* mk_listen_id(char* host, int proto, int port)
{
	struct socket_id* l;
	if (host==0) return 0;
	l=pkg_malloc(sizeof(struct socket_id));
	if (l==0) {
		PKG_MEM_CRITICAL;
	} else {
		l->addr_lst=mk_name_lst(host, 0);
		if (l->addr_lst==0){
			pkg_free(l);
			return 0;
		}
		l->flags=0;
		l->port=port;
		l->proto=proto;
		l->next=0;
	}
	return l;
}


static void free_name_lst(struct name_lst* lst)
{
	struct name_lst* tmp;

	while(lst){
		tmp=lst;
		lst=lst->next;
		pkg_free(tmp);
	}
}


static struct socket_id* mk_listen_id2(struct name_lst* addr_l, int proto,
										int port)
{
	struct socket_id* l;
	if (addr_l==0) return 0;
	l=pkg_malloc(sizeof(struct socket_id));
	if (l==0) {
		PKG_MEM_CRITICAL;
	} else {
		l->flags=addr_l->flags;
		l->port=port;
		l->proto=proto;
		l->addr_lst=addr_l;
		l->next=0;
	}
	return l;
}


static void free_socket_id(struct socket_id* i)
{
	free_name_lst(i->addr_lst);
	pkg_free(i);
}


static void free_socket_id_lst(struct socket_id* lst)
{
	struct socket_id* tmp;

	while(lst){
		tmp=lst;
		lst=lst->next;
		free_socket_id(tmp);
	}
}


/** create a temporary case statmenet structure.
 *  *err will be filled in case of error (return == 0):
 *   -1 - non constant expression
 *   -2 - expression error (bad type)
 *   -10 - memory allocation error
 */
static struct case_stms* mk_case_stm(struct rval_expr* ct, int is_re,
											struct action* a, int* err)
{
	struct case_stms* s;
	struct rval_expr* bad_rve;
	enum rval_type type, bad_t, exp_t;
	enum match_str_type t;

	t=MATCH_UNKNOWN;
	if (ct){
		/* if ct!=0 => case, else if ct==0 is a default */
		if (!rve_is_constant(ct)){
			yyerror_at(&ct->fpos, "non constant expression in case");
			*err=-1;
			return 0;
		}
		if (rve_check_type(&type, ct, &bad_rve, &bad_t, &exp_t)!=1){
			yyerror_at(&ct->fpos, "bad expression: type mismatch:"
							" %s instead of %s at (%d,%d)",
							rval_type_name(bad_t), rval_type_name(exp_t),
							bad_rve->fpos.s_line, bad_rve->fpos.s_col);
			*err=-2;
			return 0;
		}
		if (is_re)
			t=MATCH_RE;
		else if (type==RV_STR)
			t=MATCH_STR;
		else
			t=MATCH_INT;
	}

	s=pkg_malloc(sizeof(*s));
	if (s==0) {
		yyerror("internal error: memory allocation failure");
		*err=-10;
	} else {
		memset(s, 0, sizeof(*s));
		s->ct_rve=ct;
		s->type=t;
		s->actions=a;
		s->next=0;
		s->append=0;
	}
	return s;
}


/*
 * @return 0 on success, -1 on error.
 */
static int case_check_type(struct case_stms* stms)
{
	struct case_stms* c;
	struct case_stms* s;

	for(c=stms; c ; c=c->next){
		if (!c->ct_rve) continue;
		for (s=c->next; s; s=s->next){
			if (!s->ct_rve) continue;
			if ((s->type!=c->type) &&
				!(	(c->type==MATCH_STR || c->type==MATCH_RE) &&
					(s->type==MATCH_STR || s->type==MATCH_RE) ) ){
					yyerror_at(&s->ct_rve->fpos, "type mismatch in case");
					return -1;
			}
		}
	}
	return 0;
}


/*
 * @return 0 on success, -1 on error.
 */
static int case_check_default(struct case_stms* stms)
{
	struct case_stms* c;
	int default_no;

	default_no=0;
	for(c=stms; c ; c=c->next)
		if (c->ct_rve==0) default_no++;
	return (default_no<=1)?0:-1;
}



/** fixes the parameters and the type of a module function call.
 * It is done here instead of fix action, to have quicker feedback
 * on error cases (e.g. passing a non constant to a function with a
 * declared fixup)
 * The rest of the fixup is done inside do_action().
 * @param a - filled module function call (MODULE*_T) action structure
 *            complete with parameters, starting at val[2] and parameter
 *            number at val[1].
 * @return 0 on success, -1 on error (it will also print the error msg.).
 *
 */
static int mod_f_params_pre_fixup(struct action* a)
{
	ksr_cmd_export_t* cmd_exp;
	action_u_t* params;
	int param_no;
	struct rval_expr* rve;
	struct rvalue* rv;
	int r;
	str s;

	cmd_exp = a->val[0].u.data;
	param_no = a->val[1].u.number;
	params = &a->val[2];

	switch(cmd_exp->param_no) {
		case 0:
			a->type = MODULE0_T;
			break;
		case 1:
			a->type = MODULE1_T;
			break;
		case 2:
			a->type = MODULE2_T;
			break;
		case 3:
			a->type = MODULE3_T;
			break;
		case 4:
			a->type = MODULE4_T;
			break;
		case 5:
			a->type = MODULE5_T;
			break;
		case 6:
			a->type = MODULE6_T;
			break;
		case VAR_PARAM_NO:
			a->type = MODULEX_T;
			break;
		default:
			yyerror("function %s: bad definition"
					" (invalid number of parameters)", cmd_exp->name);
			return -1;
	}

	if ( cmd_exp->fixup) {
		if (is_fparam_rve_fixup(cmd_exp->fixup))
			/* mark known fparam rve safe fixups */
			cmd_exp->fixup_flags  |= FIXUP_F_FPARAM_RVE;
		else if (!(cmd_exp->fixup_flags & FIXUP_F_FPARAM_RVE) &&
				 cmd_exp->free_fixup == 0) {
			/* v0 or v1 functions that have fixups and no coresp. fixup_free
			   functions, need constant, string params.*/
			for (r=0; r < param_no; r++) {
				rve=params[r].u.data;
				if (!rve_is_constant(rve)) {
					yyerror_at(&rve->fpos, "function %s: parameter %d is not"
								" constant\n", cmd_exp->name, r+1);
					return -1;
				}
				if ((rv = rval_expr_eval(0, 0, rve)) == 0 ||
						rval_get_str(0, 0, &s, rv, 0) < 0 ) {
					/* out of mem or bug ? */
					rval_destroy(rv);
					yyerror_at(&rve->fpos, "function %s: bad parameter %d"
									" expression\n", cmd_exp->name, r+1);
					return -1;
				}
				rval_destroy(rv);
				rve_destroy(rve);
				params[r].type = STRING_ST; /* asciiz */
				params[r].u.string = s.s;
				params[r].u.str.len = s.len; /* not used right now */
			}
		}
	}/* else
		if no fixups are present, the RVEs can be transformed
		into strings at runtime, allowing seamless var. use
		even with old functions.
		Further optimizations -> in fix_actions()
		*/
	return 0;
}



/** frees a filled module function call action structure.
 * @param a - filled module function call action structure
 *            complete with parameters, starting at val[2] and parameter
 *            number at val[1].
 */
static void free_mod_func_action(struct action* a)
{
	action_u_t* params;
	int param_no;
	int r;

	param_no = a->val[1].u.number;
	params = &a->val[2];

	for (r=0; r < param_no; r++)
		if (params[r].u.data)
			rve_destroy(params[r].u.data);
	pkg_free(a);
}



/*
int main(int argc, char ** argv)
{
	if (yyparse()!=0)
		fprintf(stderr, "parsing error\n");
}
*/
