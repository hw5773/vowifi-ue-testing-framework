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

#ifndef YY_SETTINGS_PARSER_SETTINGS_SETTINGS_PARSER_H_INCLUDED
# define YY_SETTINGS_PARSER_SETTINGS_SETTINGS_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef SETTINGS_PARSER_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define SETTINGS_PARSER_DEBUG 1
#  else
#   define SETTINGS_PARSER_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define SETTINGS_PARSER_DEBUG 1
# endif /* ! defined YYDEBUG */
#endif  /* ! defined SETTINGS_PARSER_DEBUG */
#if SETTINGS_PARSER_DEBUG
extern int settings_parser_debug;
#endif

/* Token type.  */
#ifndef SETTINGS_PARSER_TOKENTYPE
# define SETTINGS_PARSER_TOKENTYPE
  enum settings_parser_tokentype
  {
    NAME = 258,
    STRING = 259,
    DOT = 260,
    COMMA = 261,
    COLON = 262,
    NEWLINE = 263,
    STRING_ERROR = 264
  };
#endif
/* Tokens.  */
#define NAME 258
#define STRING 259
#define DOT 260
#define COMMA 261
#define COLON 262
#define NEWLINE 263
#define STRING_ERROR 264

/* Value type.  */
#if ! defined SETTINGS_PARSER_STYPE && ! defined SETTINGS_PARSER_STYPE_IS_DECLARED
union SETTINGS_PARSER_STYPE
{
#line 85 "settings/settings_parser.y"

	char *s;
	struct section_t *sec;
	struct kv_t *kv;
	array_t *refs;

#line 90 "settings/settings_parser.h"

};
typedef union SETTINGS_PARSER_STYPE SETTINGS_PARSER_STYPE;
# define SETTINGS_PARSER_STYPE_IS_TRIVIAL 1
# define SETTINGS_PARSER_STYPE_IS_DECLARED 1
#endif



int settings_parser_parse (parser_helper_t *ctx);
/* "%code provides" blocks.  */
#line 74 "settings/settings_parser.y"

	#define YY_DECL \
		int settings_parser_lex(SETTINGS_PARSER_STYPE *yylval, void *yyscanner)
	YY_DECL;

#line 108 "settings/settings_parser.h"

#endif /* !YY_SETTINGS_PARSER_SETTINGS_SETTINGS_PARSER_H_INCLUDED  */
