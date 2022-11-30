/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_FILTERJGEXTOFOF_TAB_H_INCLUDED
# define YY_YY_FILTERJGEXTOFOF_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    POINT = 258,                   /* POINT  */
    MIDPOINT = 259,                /* MIDPOINT  */
    FOOT = 260,                    /* FOOT  */
    PERPENDICULAR = 261,           /* PERPENDICULAR  */
    ONLINE = 262,                  /* ONLINE  */
    LINE = 263,                    /* LINE  */
    CIRCLE = 264,                  /* CIRCLE  */
    ONCIRCLE = 265,                /* ONCIRCLE  */
    ONRCIRCLE = 266,               /* ONRCIRCLE  */
    CYCLIC = 267,                  /* CYCLIC  */
    RECTANGLE = 268,               /* RECTANGLE  */
    REFLECTION = 269,              /* REFLECTION  */
    ORTHOCENTER = 270,             /* ORTHOCENTER  */
    CIRCUMCENTER = 271,            /* CIRCUMCENTER  */
    ONTLINE = 272,                 /* ONTLINE  */
    PARALLELOGRAM = 273,           /* PARALLELOGRAM  */
    QUADRANGLE = 274,              /* QUADRANGLE  */
    COLLINEAR = 275,               /* COLLINEAR  */
    PARALLEL = 276,                /* PARALLEL  */
    LCTANGENT = 277,               /* LCTANGENT  */
    CONTRIANGLE = 278,             /* CONTRIANGLE  */
    TRIANGLE = 279,                /* TRIANGLE  */
    RTRIANGLE = 280,               /* RTRIANGLE  */
    PETRIANGLE = 281,              /* PETRIANGLE  */
    SIMTRIANGLE = 282,             /* SIMTRIANGLE  */
    ISOTRIANGLE = 283,             /* ISOTRIANGLE  */
    EQTRIANGLE = 284,              /* EQTRIANGLE  */
    SQUARE = 285,                  /* SQUARE  */
    NSQUARE = 286,                 /* NSQUARE  */
    PSQUARE = 287,                 /* PSQUARE  */
    EQDISTANCE = 288,              /* EQDISTANCE  */
    ONBLINE = 289,                 /* ONBLINE  */
    ONPLINE = 290,                 /* ONPLINE  */
    ONALINE = 291,                 /* ONALINE  */
    ANGLEBISECTOR = 292,           /* ANGLEBISECTOR  */
    INCENTER = 293,                /* INCENTER  */
    LRATIO = 294,                  /* LRATIO  */
    PRATIO = 295,                  /* PRATIO  */
    TRATIO = 296,                  /* TRATIO  */
    RATIO = 297,                   /* RATIO  */
    PENTAGON = 298,                /* PENTAGON  */
    INTERSECTIONCC = 299,          /* INTERSECTIONCC  */
    INTERSECTIONLC = 300,          /* INTERSECTIONLC  */
    INTERSECTIONLL = 301,          /* INTERSECTIONLL  */
    INTERSECTIONLT = 302,          /* INTERSECTIONLT  */
    INTERSECTIONPP = 303,          /* INTERSECTIONPP  */
    SANGLE = 304,                  /* SANGLE  */
    EQANGLE = 305,                 /* EQANGLE  */
    RTRAPEZOID = 306,              /* RTRAPEZOID  */
    TRAPEZOID = 307,               /* TRAPEZOID  */
    SYM = 308,                     /* SYM  */
    SEPARADOR = 309,               /* SEPARADOR  */
    SHOW = 310,                    /* SHOW  */
    NOSHOW = 311,                  /* NOSHOW  */
    FINALIZA = 312,                /* FINALIZA  */
    VARIABLE = 313,                /* VARIABLE  */
    NUMERO = 314                   /* NUMERO  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 77 "filterJGEXtoFOF.y"

   int number;
   char *string;

#line 128 "filterJGEXtoFOF.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_FILTERJGEXTOFOF_TAB_H_INCLUDED  */
