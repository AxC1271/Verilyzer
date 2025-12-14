/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NAND = 258,
     WIRE = 259,
     AND = 260,
     OR = 261,
     NOR = 262,
     XOR = 263,
     XNOR = 264,
     BUF = 265,
     NOT = 266,
     DFF = 267,
     OUTPUT = 268,
     INPUT = 269,
     MODULE = 270,
     ENDMODULE = 271,
     _NAME = 272,
     _STRING = 273,
     _NUMBER = 274,
     _BASENUMBER = 275
   };
#endif
/* Tokens.  */
#define NAND 258
#define WIRE 259
#define AND 260
#define OR 261
#define NOR 262
#define XOR 263
#define XNOR 264
#define BUF 265
#define NOT 266
#define DFF 267
#define OUTPUT 268
#define INPUT 269
#define MODULE 270
#define ENDMODULE 271
#define _NAME 272
#define _STRING 273
#define _NUMBER 274
#define _BASENUMBER 275




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 27 "parse.y"
{
    char *id;
    unsigned long ln;
}
/* Line 1529 of yacc.c.  */
#line 94 "parse.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

