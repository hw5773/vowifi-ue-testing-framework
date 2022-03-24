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

#ifndef YY_CONF_PARSER_PARSER_PARSER_H_INCLUDED
# define YY_CONF_PARSER_PARSER_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef CONF_PARSER_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define CONF_PARSER_DEBUG 1
#  else
#   define CONF_PARSER_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define CONF_PARSER_DEBUG 1
# endif /* ! defined YYDEBUG */
#endif  /* ! defined CONF_PARSER_DEBUG */
#if CONF_PARSER_DEBUG
extern int conf_parser_debug;
#endif

/* Token type.  */
#ifndef CONF_PARSER_TOKENTYPE
# define CONF_PARSER_TOKENTYPE
  enum conf_parser_tokentype
  {
    STRING = 258,
    EQ = 259,
    SPACES = 260,
    NEWLINE = 261,
    CONFIG_SETUP = 262,
    CONN = 263,
    CA = 264,
    STRING_ERROR = 265
  };
#endif
/* Tokens.  */
#define STRING 258
#define EQ 259
#define SPACES 260
#define NEWLINE 261
#define CONFIG_SETUP 262
#define CONN 263
#define CA 264
#define STRING_ERROR 265

/* Value type.  */
#if ! defined CONF_PARSER_STYPE && ! defined CONF_PARSER_STYPE_IS_DECLARED
union CONF_PARSER_STYPE
{
#line 78 "parser/parser.y"

	char *s;
	conf_parser_section_t t;

#line 90 "parser/parser.h"

};
typedef union CONF_PARSER_STYPE CONF_PARSER_STYPE;
# define CONF_PARSER_STYPE_IS_TRIVIAL 1
# define CONF_PARSER_STYPE_IS_DECLARED 1
#endif



int conf_parser_parse (parser_helper_t *ctx);
/* "%code provides" blocks.  */
#line 67 "parser/parser.y"

	#define YY_DECL \
		int conf_parser_lex(CONF_PARSER_STYPE *yylval, void *yyscanner)
	YY_DECL;

#line 108 "parser/parser.h"

#endif /* !YY_CONF_PARSER_PARSER_PARSER_H_INCLUDED  */
