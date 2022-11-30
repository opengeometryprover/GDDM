/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "filterJGEXtoFOF.y"

/* Livro C - 1988_KernighanRitchie_TheCProgrammingLanguage2ed.pdf
 *                                 
 * Para compilar                    
 * i) Bison                         
 * > bison -d .y
 *                                  
 * O que produz                     
 * ".tab.c" e ".tab.h" 
 * ii) Flex            
 * > flex -4.l 
 *                     
 * O que produz                     
 *                                  
 * "lex.yy.c"                       
 * iii) gcc                         
 *                                  
 * gcc lex.yy.c .tab.c -lfl -lm -o 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int yylex(void);
/* int warning(char *); */
int yyerror(char *);

void oncircle(char *);
void reflection(char *,char *,char *);
void upstr(char *);
void cleanName(char *);

 
extern int yylex();
extern FILE *yyin;
extern FILE *yyout;

typedef struct idVariables {
  char variable[20];
} IdVariables;

IdVariables listIdVariables[1000];
int numIdVariables=0;

typedef struct propositions {
  char proposition[150];
} Propositions;

char proposition[600];
char newProposition[600];

Propositions listPropositions[1000];
int numPropositions=0;
int numHypotheses=0;

int numMedPoints=1;
char midpointsIds[20];
char newPointsIds1[20],newPointsIds2[20];
  
char fileName[60];
char fullFileName[60];
char outputFileName[60];
char conjectureName[60];
// to be able to convert from ASCI string to integer
char numero[10];
 
// variáveis auxiliares
int mpi,aux;

 

#line 147 "filterJGEXtoFOF.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "filterJGEXtoFOF.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_POINT = 3,                      /* POINT  */
  YYSYMBOL_MIDPOINT = 4,                   /* MIDPOINT  */
  YYSYMBOL_FOOT = 5,                       /* FOOT  */
  YYSYMBOL_PERPENDICULAR = 6,              /* PERPENDICULAR  */
  YYSYMBOL_ONLINE = 7,                     /* ONLINE  */
  YYSYMBOL_LINE = 8,                       /* LINE  */
  YYSYMBOL_CIRCLE = 9,                     /* CIRCLE  */
  YYSYMBOL_ONCIRCLE = 10,                  /* ONCIRCLE  */
  YYSYMBOL_ONRCIRCLE = 11,                 /* ONRCIRCLE  */
  YYSYMBOL_CYCLIC = 12,                    /* CYCLIC  */
  YYSYMBOL_RECTANGLE = 13,                 /* RECTANGLE  */
  YYSYMBOL_REFLECTION = 14,                /* REFLECTION  */
  YYSYMBOL_ORTHOCENTER = 15,               /* ORTHOCENTER  */
  YYSYMBOL_CIRCUMCENTER = 16,              /* CIRCUMCENTER  */
  YYSYMBOL_ONTLINE = 17,                   /* ONTLINE  */
  YYSYMBOL_PARALLELOGRAM = 18,             /* PARALLELOGRAM  */
  YYSYMBOL_QUADRANGLE = 19,                /* QUADRANGLE  */
  YYSYMBOL_COLLINEAR = 20,                 /* COLLINEAR  */
  YYSYMBOL_PARALLEL = 21,                  /* PARALLEL  */
  YYSYMBOL_LCTANGENT = 22,                 /* LCTANGENT  */
  YYSYMBOL_CONTRIANGLE = 23,               /* CONTRIANGLE  */
  YYSYMBOL_TRIANGLE = 24,                  /* TRIANGLE  */
  YYSYMBOL_RTRIANGLE = 25,                 /* RTRIANGLE  */
  YYSYMBOL_PETRIANGLE = 26,                /* PETRIANGLE  */
  YYSYMBOL_SIMTRIANGLE = 27,               /* SIMTRIANGLE  */
  YYSYMBOL_ISOTRIANGLE = 28,               /* ISOTRIANGLE  */
  YYSYMBOL_EQTRIANGLE = 29,                /* EQTRIANGLE  */
  YYSYMBOL_SQUARE = 30,                    /* SQUARE  */
  YYSYMBOL_NSQUARE = 31,                   /* NSQUARE  */
  YYSYMBOL_PSQUARE = 32,                   /* PSQUARE  */
  YYSYMBOL_EQDISTANCE = 33,                /* EQDISTANCE  */
  YYSYMBOL_ONBLINE = 34,                   /* ONBLINE  */
  YYSYMBOL_ONPLINE = 35,                   /* ONPLINE  */
  YYSYMBOL_ONALINE = 36,                   /* ONALINE  */
  YYSYMBOL_ANGLEBISECTOR = 37,             /* ANGLEBISECTOR  */
  YYSYMBOL_INCENTER = 38,                  /* INCENTER  */
  YYSYMBOL_LRATIO = 39,                    /* LRATIO  */
  YYSYMBOL_PRATIO = 40,                    /* PRATIO  */
  YYSYMBOL_TRATIO = 41,                    /* TRATIO  */
  YYSYMBOL_RATIO = 42,                     /* RATIO  */
  YYSYMBOL_PENTAGON = 43,                  /* PENTAGON  */
  YYSYMBOL_INTERSECTIONCC = 44,            /* INTERSECTIONCC  */
  YYSYMBOL_INTERSECTIONLC = 45,            /* INTERSECTIONLC  */
  YYSYMBOL_INTERSECTIONLL = 46,            /* INTERSECTIONLL  */
  YYSYMBOL_INTERSECTIONLT = 47,            /* INTERSECTIONLT  */
  YYSYMBOL_INTERSECTIONPP = 48,            /* INTERSECTIONPP  */
  YYSYMBOL_SANGLE = 49,                    /* SANGLE  */
  YYSYMBOL_EQANGLE = 50,                   /* EQANGLE  */
  YYSYMBOL_RTRAPEZOID = 51,                /* RTRAPEZOID  */
  YYSYMBOL_TRAPEZOID = 52,                 /* TRAPEZOID  */
  YYSYMBOL_SYM = 53,                       /* SYM  */
  YYSYMBOL_SEPARADOR = 54,                 /* SEPARADOR  */
  YYSYMBOL_SHOW = 55,                      /* SHOW  */
  YYSYMBOL_NOSHOW = 56,                    /* NOSHOW  */
  YYSYMBOL_FINALIZA = 57,                  /* FINALIZA  */
  YYSYMBOL_VARIABLE = 58,                  /* VARIABLE  */
  YYSYMBOL_NUMERO = 59,                    /* NUMERO  */
  YYSYMBOL_60_n_ = 60,                     /* '\n'  */
  YYSYMBOL_YYACCEPT = 61,                  /* $accept  */
  YYSYMBOL_linha = 62,                     /* linha  */
  YYSYMBOL_comando = 63,                   /* comando  */
  YYSYMBOL_circle = 64,                    /* circle  */
  YYSYMBOL_variaveis = 65,                 /* variaveis  */
  YYSYMBOL_66_1 = 66                       /* $@1  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
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
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  114
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   328

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  61
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  6
/* YYNRULES -- Number of rules.  */
#define YYNRULES  68
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  274

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   314


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      60,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      55,    56,    57,    58,    59
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    98,    98,    99,   100,   101,   104,   106,   118,   138,
     152,   164,   195,   209,   235,   249,   263,   299,   321,   322,
     335,   338,   341,   343,   345,   367,   403,   456,   509,   511,
     525,   528,   554,   560,   590,   679,   736,   752,   774,   796,
     818,   832,   854,   878,   897,   919,   949,   990,  1013,  1026,
    1134,  1176,  1199,  1213,  1227,  1229,  1248,  1267,  1287,  1288,
    1289,  1290,  1293,  1328,  1356,  1370,  1392,  1398,  1397
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "POINT", "MIDPOINT",
  "FOOT", "PERPENDICULAR", "ONLINE", "LINE", "CIRCLE", "ONCIRCLE",
  "ONRCIRCLE", "CYCLIC", "RECTANGLE", "REFLECTION", "ORTHOCENTER",
  "CIRCUMCENTER", "ONTLINE", "PARALLELOGRAM", "QUADRANGLE", "COLLINEAR",
  "PARALLEL", "LCTANGENT", "CONTRIANGLE", "TRIANGLE", "RTRIANGLE",
  "PETRIANGLE", "SIMTRIANGLE", "ISOTRIANGLE", "EQTRIANGLE", "SQUARE",
  "NSQUARE", "PSQUARE", "EQDISTANCE", "ONBLINE", "ONPLINE", "ONALINE",
  "ANGLEBISECTOR", "INCENTER", "LRATIO", "PRATIO", "TRATIO", "RATIO",
  "PENTAGON", "INTERSECTIONCC", "INTERSECTIONLC", "INTERSECTIONLL",
  "INTERSECTIONLT", "INTERSECTIONPP", "SANGLE", "EQANGLE", "RTRAPEZOID",
  "TRAPEZOID", "SYM", "SEPARADOR", "SHOW", "NOSHOW", "FINALIZA",
  "VARIABLE", "NUMERO", "'\\n'", "$accept", "linha", "comando", "circle",
  "variaveis", "$@1", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-60)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-68)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -1,   -59,   -18,     3,     4,    43,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   -60,   -60,   -60,   -60,   -60,   -60,   -60,    60,
     -60,   -60,   -60,   108,   113,   110,   111,   112,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   -60,   -60,   -18,   -60,   160,   161,
     162,   163,   -60,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   -60,   -60,   208,
     209,   -60,   210,   -60,   211,   212,   213,   -60,   214,   215,
     216,   217,   218,   -60,   219,   -60,   220,   -60,   -60,   -60,
     221,   -60,   -60,   222,   -60,   -60,   223,   -60,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   239,   240,   241,   -60,   -60,   -60,   -60,   -60,
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   242,   243,
     -60,   -60,   -60,   244,   -60,   -60,   245,   246,   247,   249,
     250,   251,   252,   253,   254,   -60,   255,   -60,   -60,   256,
     257,   258,   259,   260,   261,   -60,   -60,   -60,   -60,   263,
     264,   265,   -60,   -60,   -60,   -60,   -60,   -60,   -60,   266,
     267,   -60,   268,   -60
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,    20,    21,    58,    59,    60,     2,     0,
       3,    18,     5,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     1,     4,     0,     6,     0,     0,
       0,     0,    54,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     7,     0,
       0,    10,    63,    13,     0,     0,     0,    32,     0,     0,
       0,     0,     0,    19,     0,    53,     0,    22,    29,    30,
       0,    31,    50,     0,    27,    26,     0,    16,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     8,     9,    64,    11,
      12,    41,    33,    34,    14,    24,    23,    48,     0,     0,
      25,    52,    15,     0,    42,    35,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    36,     0,    39,    40,     0,
       0,     0,     0,     0,     0,    28,    46,    45,    43,     0,
       0,     0,    51,    49,    17,    55,    56,    57,    44,     0,
      38,    47,     0,    37
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -60,   -60,   269,   -60,    55,   -60
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    59,    60,    61,    64,   116
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
       1,    62,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      63,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
     114,    65,    66,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    67,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   -67,   117,   118,   119,
     120,   167,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,     0,   245,   246,   247,   248,
     249,   250,   251,     0,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,     0,   265,   266,
     267,   268,   269,   270,   271,   272,   273,     0,   115
};

static const yytype_int8 yycheck[] =
{
       1,    60,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      58,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
       0,    58,    58,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    58,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    54,    58,    58,
      58,   116,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    -1,    59,    58,    58,    58,
      58,    58,    58,    -1,    59,    59,    59,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    -1,    59,    59,
      59,    58,    58,    58,    58,    58,    58,    -1,    59
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    62,
      63,    64,    60,    58,    65,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,     0,    63,    66,    54,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    65,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    59,    58,    58,    58,    58,
      58,    58,    59,    59,    59,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    59,    59,    59,    58,    58,
      58,    58,    58,    58
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    61,    62,    62,    62,    62,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    63,    63,    63,
      63,    63,    64,    64,    64,    64,    65,    66,    65
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     3,     4,     5,     5,
       4,     5,     5,     4,     5,     5,     4,     7,     1,     4,
       1,     1,     4,     5,     5,     5,     4,     4,     6,     4,
       4,     4,     4,     5,     5,     5,     5,     9,     7,     5,
       5,     5,     5,     6,     7,     6,     6,     8,     5,     7,
       4,     7,     5,     4,     3,     7,     7,     7,     1,     1,
       1,     1,     3,     4,     5,     4,     1,     0,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
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
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
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
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
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
  case 5: /* linha: error '\n'  */
#line 101 "filterJGEXtoFOF.y"
                           {yyerrok;}
#line 1366 "filterJGEXtoFOF.tab.c"
    break;

  case 6: /* comando: POINT variaveis SEPARADOR  */
#line 105 "filterJGEXtoFOF.y"
                  { /* Builds the initial list of points */ }
#line 1372 "filterJGEXtoFOF.tab.c"
    break;

  case 7: /* comando: MIDPOINT VARIABLE VARIABLE VARIABLE  */
#line 107 "filterJGEXtoFOF.y"
                  {
		    upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"midp(");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);		    
		  }
#line 1388 "filterJGEXtoFOF.tab.c"
    break;

  case 8: /* comando: FOOT VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 119 "filterJGEXtoFOF.y"
                  {
		    upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\nperp(");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,") & coll(");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);		    
		  }
#line 1412 "filterJGEXtoFOF.tab.c"
    break;

  case 9: /* comando: PERPENDICULAR VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 139 "filterJGEXtoFOF.y"
                  {
		    upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\nperp(");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);		    
		  }
#line 1430 "filterJGEXtoFOF.tab.c"
    break;

  case 10: /* comando: ONLINE VARIABLE VARIABLE VARIABLE  */
#line 153 "filterJGEXtoFOF.y"
                  {
		    upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\ncoll(");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);		    
		  }
#line 1446 "filterJGEXtoFOF.tab.c"
    break;

  case 11: /* comando: ONRCIRCLE VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 165 "filterJGEXtoFOF.y"
                  { /* circle E O A with EO _|_ AO */
		    upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    newPointsIds1[0]='\0';
		    strcpy(newPointsIds1,"NWPNT");
		    mpi=5;
		    aux=numMedPoints;
		     do {
		      newPointsIds1[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    strcpy(listIdVariables[numIdVariables++].variable,newPointsIds1);
		    strcpy(proposition,"\ncircle(");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds1);
		    strcat(proposition,") & perp(");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);		    
		  }
#line 1481 "filterJGEXtoFOF.tab.c"
    break;

  case 12: /* comando: CYCLIC VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 196 "filterJGEXtoFOF.y"
                  {		   
		    upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\ncyclic(");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);		 
		  }
#line 1499 "filterJGEXtoFOF.tab.c"
    break;

  case 13: /* comando: ONCIRCLE VARIABLE VARIABLE VARIABLE  */
#line 210 "filterJGEXtoFOF.y"
                  {
		    /* oncircle v1 v2 v3 -> circle(v2,v3,v1,n1) */
		    // one new point (uses the midpoint counter)
		    upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    newPointsIds1[0]='\0';
		    strcpy(newPointsIds1,"NWPNT");
		    mpi=5;
		    aux=numMedPoints;
		     do {
		      newPointsIds1[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    strcpy(listIdVariables[numIdVariables++].variable,newPointsIds1);
		    // building the proposition circle
		    strcpy(proposition,"\ncircle(");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds1);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 1529 "filterJGEXtoFOF.tab.c"
    break;

  case 14: /* comando: ONTLINE VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 236 "filterJGEXtoFOF.y"
                  {
		    upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\nperp(");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 1547 "filterJGEXtoFOF.tab.c"
    break;

  case 15: /* comando: ONPLINE VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 250 "filterJGEXtoFOF.y"
                  {
		    upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\npara(");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 1565 "filterJGEXtoFOF.tab.c"
    break;

  case 16: /* comando: ONBLINE VARIABLE VARIABLE VARIABLE  */
#line 264 "filterJGEXtoFOF.y"
                  { /* Point on Bisection (bisectriz) 
		     * onbline C A B =
		     *   midpoint(M,A,B) & perp(A,B,M,C)
		     */
		    upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    // one new point (uses the midpoint counter)
		    upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    newPointsIds1[0]='\0';
		    strcpy(newPointsIds1,"NWPNT");
		    mpi=5;
		    aux=numMedPoints;
		     do {
		      newPointsIds1[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    strcpy(listIdVariables[numIdVariables++].variable,newPointsIds1);
		    // Building the command
		    strcpy(proposition,"\nmidpoint(");
		    strcat(proposition,newPointsIds1);
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,") & perp(");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds1);
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	

		  }
#line 1605 "filterJGEXtoFOF.tab.c"
    break;

  case 17: /* comando: ONALINE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 300 "filterJGEXtoFOF.y"
                  {
		    upstr((yyvsp[-5].string));upstr((yyvsp[-4].string));upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\neqangle(");
		    strcat(proposition,(yyvsp[-5].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 1631 "filterJGEXtoFOF.tab.c"
    break;

  case 19: /* comando: COLLINEAR VARIABLE VARIABLE VARIABLE  */
#line 323 "filterJGEXtoFOF.y"
                 { 
		    upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\ncoll(");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 1648 "filterJGEXtoFOF.tab.c"
    break;

  case 20: /* comando: SHOW  */
#line 335 "filterJGEXtoFOF.y"
                     {
		       numHypotheses=numPropositions;
		     }
#line 1656 "filterJGEXtoFOF.tab.c"
    break;

  case 21: /* comando: NOSHOW  */
#line 338 "filterJGEXtoFOF.y"
                       {
	                 numHypotheses=0;
		       }
#line 1664 "filterJGEXtoFOF.tab.c"
    break;

  case 22: /* comando: TRIANGLE VARIABLE VARIABLE VARIABLE  */
#line 342 "filterJGEXtoFOF.y"
                  { /* not relevant */ }
#line 1670 "filterJGEXtoFOF.tab.c"
    break;

  case 23: /* comando: QUADRANGLE VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 344 "filterJGEXtoFOF.y"
                  { /* ignore */ }
#line 1676 "filterJGEXtoFOF.tab.c"
    break;

  case 24: /* comando: PARALLELOGRAM VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 346 "filterJGEXtoFOF.y"
                  {
		    upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\npara(");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,") & para(");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 1702 "filterJGEXtoFOF.tab.c"
    break;

  case 25: /* comando: SQUARE VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 368 "filterJGEXtoFOF.y"
                  { /*
		     * Square, two consecutive equal angles and the
		     * corresponding sides equal in length
		     * SQUARE A B C D =
		     *  eqangle(B,A,A,D,A,D,D,C) &
		     *  cong(A,D,D,C) 
		     */
		    upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\neqangle(");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,") & cong(");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 1742 "filterJGEXtoFOF.tab.c"
    break;

  case 26: /* comando: PSQUARE VARIABLE VARIABLE VARIABLE  */
#line 404 "filterJGEXtoFOF.y"
                  { /*
		     * PSQUARE A B C =
		     *  AB || CD & AD || BC & AB=BC & AB _|_ BC
		     */
		    upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    // one new point (uses the midpoint counter)
		    upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    newPointsIds1[0]='\0';
		    strcpy(newPointsIds1,"NWPNT");
		    mpi=5;
		    aux=numMedPoints;
		     do {
		      newPointsIds1[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    strcpy(listIdVariables[numIdVariables++].variable,newPointsIds1);
		    // Building the command
		    strcpy(proposition,"\npara(");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds1);
		    strcat(proposition,") & para(");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds1);
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,") & cong(");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,") & perp(");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 1799 "filterJGEXtoFOF.tab.c"
    break;

  case 27: /* comando: NSQUARE VARIABLE VARIABLE VARIABLE  */
#line 457 "filterJGEXtoFOF.y"
                  { /*
		     * NSQUARE A B C =
		     *  AB || CD & AD || BC & AB=BC & AB _|_ BC
		     */
		    upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    // one new point (uses the midpoint counter)
		    upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    newPointsIds1[0]='\0';
		    strcpy(newPointsIds1,"NWPNT");
		    mpi=5;
		    aux=numMedPoints;
		     do {
		      newPointsIds1[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    strcpy(listIdVariables[numIdVariables++].variable,newPointsIds1);
		    // Building the command
		    strcpy(proposition,"\npara(");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds1);
		    strcat(proposition,") & para(");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds1);
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,") & cong(");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,") & perp(");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);
		  }
#line 1856 "filterJGEXtoFOF.tab.c"
    break;

  case 28: /* comando: PENTAGON VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 510 "filterJGEXtoFOF.y"
                  { /* Ignore */ }
#line 1862 "filterJGEXtoFOF.tab.c"
    break;

  case 29: /* comando: RTRIANGLE VARIABLE VARIABLE VARIABLE  */
#line 512 "filterJGEXtoFOF.y"
                  { /* Triangle rectangle in the 1st point */
		    upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\nperp(");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 1880 "filterJGEXtoFOF.tab.c"
    break;

  case 30: /* comando: PETRIANGLE VARIABLE VARIABLE VARIABLE  */
#line 526 "filterJGEXtoFOF.y"
                  { /* Triangle ??? - ignore */
		  }
#line 1887 "filterJGEXtoFOF.tab.c"
    break;

  case 31: /* comando: ISOTRIANGLE VARIABLE VARIABLE VARIABLE  */
#line 529 "filterJGEXtoFOF.y"
                  { /*
		     * Isosceles Triangle - two congruente sides
		     *  ISOTRIANGLE A B C =
		     *   cong(A,B,B,C) | cong(A,B,A,C)
		     */
		    upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\n(cong(");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,") | cong(");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,"))");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 1917 "filterJGEXtoFOF.tab.c"
    break;

  case 32: /* comando: REFLECTION VARIABLE VARIABLE VARIABLE  */
#line 555 "filterJGEXtoFOF.y"
                  {	
		    upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));	 
		    reflection((yyvsp[-2].string),(yyvsp[-1].string),(yyvsp[0].string));
		    strcpy(listPropositions[numPropositions++].proposition,newProposition);
		  }
#line 1927 "filterJGEXtoFOF.tab.c"
    break;

  case 33: /* comando: ORTHOCENTER VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 561 "filterJGEXtoFOF.y"
                  {
		    upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\nperp(");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,") & perp(");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,") & perp(");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 1961 "filterJGEXtoFOF.tab.c"
    break;

  case 34: /* comando: CIRCUMCENTER VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 591 "filterJGEXtoFOF.y"
                  {
		    upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    
		    // Builds the new midpoint Id
		    strcpy(midpointsIds,"MIDPNT");
		    mpi=6;
		    aux=numMedPoints;
		     do {
		      midpointsIds[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    // Add a new quantified variable
		    strcpy(listIdVariables[numIdVariables++].variable,midpointsIds);
		    // builds the midp & perp for that point
		    strcpy(proposition,"\nmidp(");
		    strcat(proposition,midpointsIds);
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,") & perp(");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,midpointsIds);
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,")");
		    // add that to the list of propositions
		    strcpy(listPropositions[numPropositions++].proposition,proposition);
		    // Builds the new midpoint Id
		    strcpy(midpointsIds,"MIDPNT");
		    mpi=6;
		    aux=numMedPoints;
		     do {
		      midpointsIds[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    // Add a new quantified variable
		    strcpy(listIdVariables[numIdVariables++].variable,midpointsIds);
		    // builds the midp & perp for that point
		    strcpy(proposition,"midp(");
		    strcat(proposition,midpointsIds);
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,") & perp(");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,midpointsIds);
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,")");
		    // add that to the list of propositions
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		    // Builds the new midpoint Id
		    strcpy(midpointsIds,"MIDPNT");
		    mpi=6;
		    aux=numMedPoints;
		     do {
		      midpointsIds[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    // Add a new quantified variable
		    strcpy(listIdVariables[numIdVariables++].variable,midpointsIds);
		    // builds the midp & perp for that point
		    strcpy(proposition,"midp(");
		    strcat(proposition,midpointsIds);
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,") & perp(");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,midpointsIds);
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,")");
		    // add that to the list of propositions
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 2054 "filterJGEXtoFOF.tab.c"
    break;

  case 35: /* comando: INCENTER VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 680 "filterJGEXtoFOF.y"
                  { /*
		     * INCENTER I A B C =
		     * eqangle(I,A,A,B) & eqangle(I,B,B,C) & eqangle(I,C,C,A)
		     */
		    upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\neqangle(");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,") & eqangle(");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,") & eqangle(");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 2115 "filterJGEXtoFOF.tab.c"
    break;

  case 36: /* comando: SANGLE VARIABLE VARIABLE VARIABLE NUMERO  */
#line 737 "filterJGEXtoFOF.y"
                  {
		    upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));
		    strcpy(proposition,"\nangle(");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    sprintf(numero,"%d",(yyvsp[0].number));
		    strcat(proposition,numero);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		    //fprintf(yyout,"eqangle(%s,%s,%s,%s,%s,%s) &",$2,$3,$4,$5,$6,$7);
		  }
#line 2135 "filterJGEXtoFOF.tab.c"
    break;

  case 37: /* comando: EQANGLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 753 "filterJGEXtoFOF.y"
                  {
		    upstr((yyvsp[-7].string));upstr((yyvsp[-6].string));upstr((yyvsp[-5].string));upstr((yyvsp[-4].string));upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\neqangle(");
		    strcat(proposition,(yyvsp[-7].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-6].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-5].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 2161 "filterJGEXtoFOF.tab.c"
    break;

  case 38: /* comando: EQANGLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 775 "filterJGEXtoFOF.y"
                  { /* EQANGLE A B C D E F = EQANGLE A B B C D E E F */
		    upstr((yyvsp[-5].string));upstr((yyvsp[-4].string));upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\neqangle(");
		    strcat(proposition,(yyvsp[-5].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 2187 "filterJGEXtoFOF.tab.c"
    break;

  case 39: /* comando: RTRAPEZOID VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 797 "filterJGEXtoFOF.y"
                  { /* Trapezoid A B C D, AB _|_ AD AB || CD */
		    upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\nperp(");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,") & para(");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 2213 "filterJGEXtoFOF.tab.c"
    break;

  case 40: /* comando: TRAPEZOID VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 819 "filterJGEXtoFOF.y"
                  { /* Trapezoid A B C D, AB || CD */
		    upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcat(proposition,"\npara(");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 2231 "filterJGEXtoFOF.tab.c"
    break;

  case 41: /* comando: RECTANGLE VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 833 "filterJGEXtoFOF.y"
                  { /* Trapezoid A B C D, AB || CD & AD || BC */
		    upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcat(proposition,"\npara(");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,") & para(");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 2257 "filterJGEXtoFOF.tab.c"
    break;

  case 42: /* comando: ANGLEBISECTOR VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 855 "filterJGEXtoFOF.y"
                  { /*
		      ANGLE_BISECTOR E C D A = eqangle(A,D,D,E,E,D,D,C)
		    */
		    upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\neqangle(");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 2285 "filterJGEXtoFOF.tab.c"
    break;

  case 43: /* comando: INTERSECTIONLL VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 879 "filterJGEXtoFOF.y"
                  { /* Intersection Line Line */

		    upstr((yyvsp[-4].string));upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\ncoll(");
		    strcat(proposition,(yyvsp[-4].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,") & coll(");
		    strcat(proposition,(yyvsp[-4].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 2308 "filterJGEXtoFOF.tab.c"
    break;

  case 44: /* comando: INTERSECTIONLT VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 898 "filterJGEXtoFOF.y"
                  { /* Intersection E C D A A B 
		     * point E is in line CD
		     * 
		     */

		    upstr((yyvsp[-5].string));upstr((yyvsp[-4].string));upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));
		    strcpy(proposition,"\ncoll(");
		    strcat(proposition,(yyvsp[-5].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,") & coll(");
		    strcat(proposition,(yyvsp[-5].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 2334 "filterJGEXtoFOF.tab.c"
    break;

  case 45: /* comando: INTERSECTIONLC VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 920 "filterJGEXtoFOF.y"
                  { /* Intersection Line Circle */

		    upstr((yyvsp[-4].string));upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    newPointsIds1[0]='\0';
		    strcpy(newPointsIds1,"NWPNT");
		    mpi=5          ;
		    aux=numMedPoints;
		     do {
		      newPointsIds1[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    strcpy(listIdVariables[numIdVariables++].variable,newPointsIds1);
		    strcpy(proposition,"\ncoll(");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));
		    strcat(proposition,") & circle(");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds1);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 2368 "filterJGEXtoFOF.tab.c"
    break;

  case 46: /* comando: INTERSECTIONCC VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 950 "filterJGEXtoFOF.y"
                  { /* intersection Circle Circle */

		    upstr((yyvsp[-4].string));upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    newPointsIds1[0]='\0';
		    strcpy(newPointsIds1,"NWPNT");
		    mpi=5          ;
		    aux=numMedPoints;
		     do {
		      newPointsIds1[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    strcpy(listIdVariables[numIdVariables++].variable,newPointsIds1);
		    newPointsIds2[0]='\0';
		    strcpy(newPointsIds2,"NWPNT");
		    mpi=5          ;
		    aux=numMedPoints;
		     do {
		      newPointsIds2[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    strcpy(listIdVariables[numIdVariables++].variable,newPointsIds2);
		    strcpy(proposition,"\ncircle(");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds1);
		    strcat(proposition,") & circle(");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds2);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 2413 "filterJGEXtoFOF.tab.c"
    break;

  case 47: /* comando: INTERSECTIONPP VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 991 "filterJGEXtoFOF.y"
                  { /* Intersection of two parallel lines to form a rectangle */

		    upstr((yyvsp[-6].string));upstr((yyvsp[-5].string));upstr((yyvsp[-4].string));upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\npara(");
		    strcat(proposition,(yyvsp[-4].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-5].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-6].string));
		    strcat(proposition,") & para(");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-6].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 2440 "filterJGEXtoFOF.tab.c"
    break;

  case 48: /* comando: PARALLEL VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 1014 "filterJGEXtoFOF.y"
                  {
		    strcpy(proposition,"\npara(");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 2457 "filterJGEXtoFOF.tab.c"
    break;

  case 49: /* comando: SIMTRIANGLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 1027 "filterJGEXtoFOF.y"
                  { /* Simetric triangles */
		    upstr((yyvsp[-5].string));upstr((yyvsp[-4].string));upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\neqangle(");
		    // <A = <1 & <B = <2
		    strcat(proposition,(yyvsp[-4].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-5].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-5].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));strcat(proposition,") & (eqangle(");
	    	    strcat(proposition,(yyvsp[-5].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));strcat(proposition,") |\neqangle(");
		    // <A = <1 & <B = <3
		    strcat(proposition,(yyvsp[-4].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-5].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-5].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));strcat(proposition,")) & (eqangle(");
	    	    strcat(proposition,(yyvsp[-5].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));strcat(proposition,") |\neqangle(");
		    // <A = <2 & <B = <1
		    strcat(proposition,(yyvsp[-4].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-5].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-5].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));strcat(proposition,")) & (eqangle(");
	    	    strcat(proposition,(yyvsp[-5].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));strcat(proposition,") |\neqangle(");
		    // <A = <2 & <B = <3
		    strcat(proposition,(yyvsp[-4].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-5].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-5].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));strcat(proposition,")) & (eqangle(");
	    	    strcat(proposition,(yyvsp[-5].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));strcat(proposition,") |\neqangle(");
		    // <A = <3 ; <B = <1
		    strcat(proposition,(yyvsp[-4].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-5].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-5].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));strcat(proposition,")) & (eqangle(");
	    	    strcat(proposition,(yyvsp[-5].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));strcat(proposition,") |\neqangle(");
		    // <A = <3 ; <B = <2
		    strcat(proposition,(yyvsp[-4].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-5].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-5].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));strcat(proposition,")) & eqangle(");
	    	    strcat(proposition,(yyvsp[-5].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));strcat(proposition,")"); 
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 2569 "filterJGEXtoFOF.tab.c"
    break;

  case 50: /* comando: EQTRIANGLE VARIABLE VARIABLE VARIABLE  */
#line 1135 "filterJGEXtoFOF.y"
                  { /*
		     * Equilateral triangle
		     * EQ_TRIANGLE C A B 
		     *  = eqangle(B,A,A,C,A,B,B,C) & eqangle(A,B,B,C,A,C,C,B)
		     */
		    upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\neqangle(");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,") & eqangle(");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 2615 "filterJGEXtoFOF.tab.c"
    break;

  case 51: /* comando: CONTRIANGLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 1177 "filterJGEXtoFOF.y"
                  { /*
		     * Congruence of triangles
		     * CONTRIANGLE A B C D E F
		     *  = contri(A,B,C,D,E,F)
		     */
		    upstr((yyvsp[-5].string));upstr((yyvsp[-4].string));upstr((yyvsp[-3].string));
		    upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\ncontri(");
		    strcat(proposition,(yyvsp[-5].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 2642 "filterJGEXtoFOF.tab.c"
    break;

  case 52: /* comando: EQDISTANCE VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 1200 "filterJGEXtoFOF.y"
                  {
		    upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\ncong(");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 2660 "filterJGEXtoFOF.tab.c"
    break;

  case 53: /* comando: LCTANGENT VARIABLE VARIABLE VARIABLE  */
#line 1214 "filterJGEXtoFOF.y"
                  {
		    upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"\nperp(");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 2678 "filterJGEXtoFOF.tab.c"
    break;

  case 54: /* comando: LINE VARIABLE VARIABLE  */
#line 1228 "filterJGEXtoFOF.y"
                  { /* not relevant */ }
#line 2684 "filterJGEXtoFOF.tab.c"
    break;

  case 55: /* comando: LRATIO VARIABLE VARIABLE VARIABLE VARIABLE NUMERO NUMERO  */
#line 1230 "filterJGEXtoFOF.y"
                  { /* not relevant ??? */
		    char numero[10];
		    upstr((yyvsp[-5].string));upstr((yyvsp[-4].string));upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));
		    strcpy(proposition,"\nratio(");
		    strcat(proposition,(yyvsp[-5].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,") = ");
		    sprintf(numero,"%d",(yyvsp[-1].number));
		    strcat(proposition,numero);
		    strcat(proposition,"/");
		    sprintf(numero,"%d", (yyvsp[0].number));
		    strcat(proposition,numero);
		    strcpy(listPropositions[numPropositions++].proposition,proposition); }
#line 2707 "filterJGEXtoFOF.tab.c"
    break;

  case 56: /* comando: PRATIO VARIABLE VARIABLE VARIABLE VARIABLE NUMERO NUMERO  */
#line 1249 "filterJGEXtoFOF.y"
                  { /* not relevant ??? */
		    upstr((yyvsp[-5].string));upstr((yyvsp[-4].string));upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));
		    strcpy(proposition,"\nratio(");
		    strcat(proposition,(yyvsp[-5].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,") = ");
		    sprintf (numero,"%d",(yyvsp[-1].number));
		    strcat(proposition,numero);
		    strcat(proposition,"/");
		    //		    strcat(proposition,$7);
		    sprintf(numero,"%d", (yyvsp[0].number));
		    strcat(proposition,numero);
		    strcpy(listPropositions[numPropositions++].proposition,proposition); }
#line 2730 "filterJGEXtoFOF.tab.c"
    break;

  case 57: /* comando: RATIO VARIABLE VARIABLE VARIABLE VARIABLE NUMERO NUMERO  */
#line 1268 "filterJGEXtoFOF.y"
                  { /* AB/CD = n1/n2 */
		    char numero[10];
		    upstr((yyvsp[-5].string));upstr((yyvsp[-4].string));upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));
		    strcpy(proposition,"\nratio(");
		    strcat(proposition,(yyvsp[-5].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-4].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,") = ");
		    sprintf(numero,"%d",(yyvsp[-1].number));
		    strcat(proposition,numero);
		    strcat(proposition,"/");
		    sprintf(numero,"%d", (yyvsp[0].number));
		    strcat(proposition,numero);
		    strcpy(listPropositions[numPropositions++].proposition,proposition);
		  }
#line 2754 "filterJGEXtoFOF.tab.c"
    break;

  case 58: /* comando: FINALIZA  */
#line 1287 "filterJGEXtoFOF.y"
                         { /* */ }
#line 2760 "filterJGEXtoFOF.tab.c"
    break;

  case 59: /* comando: VARIABLE  */
#line 1288 "filterJGEXtoFOF.y"
                         { fprintf(yyout,"%s ",(yyvsp[0].string)); }
#line 2766 "filterJGEXtoFOF.tab.c"
    break;

  case 62: /* circle: CIRCLE VARIABLE VARIABLE  */
#line 1294 "filterJGEXtoFOF.y"
                  { /* circle centre point -> circle center point point point */
		    // two new points (uses the midpoint counter)
		    upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    newPointsIds1[0]='\0';
		    newPointsIds2[0]='\0';
		    strcpy(newPointsIds1,"NWPNT");
		    mpi=5          ;
		    aux=numMedPoints;
		     do {
		      newPointsIds1[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    strcpy(listIdVariables[numIdVariables++].variable,newPointsIds1);
		    strcpy(newPointsIds2,"NWPNT");
		    mpi=5          ;
		    aux=numMedPoints;
		     do {
		      newPointsIds2[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    // adding the two new points to the list of points
		    strcpy(listIdVariables[numIdVariables++].variable,newPointsIds2);
		    // building the proposition circle
		    strcpy(proposition,"\ncircle(");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds1);
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds2);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 2805 "filterJGEXtoFOF.tab.c"
    break;

  case 63: /* circle: CIRCLE VARIABLE VARIABLE VARIABLE  */
#line 1329 "filterJGEXtoFOF.y"
                  {/* circle centre point point 
                      -> circle center point point point */
		    // one new point (uses the midpoint counter)
		    
		    upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    newPointsIds1[0]='\0';
		    strcpy(newPointsIds1,"NWPNT");
		    mpi=5          ;
		    aux=numMedPoints;
		     do {
		      newPointsIds1[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    // adding the new point to the list of points
		    strcpy(listIdVariables[numIdVariables++].variable,newPointsIds1);
		    // building the proposition circle
		    strcpy(proposition,"\ncircle(");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds2);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);			    
		  }
#line 2837 "filterJGEXtoFOF.tab.c"
    break;

  case 64: /* circle: CIRCLE VARIABLE VARIABLE VARIABLE VARIABLE  */
#line 1357 "filterJGEXtoFOF.y"
                  {
		    upstr((yyvsp[-3].string));upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcpy(proposition,"circle(");
		    strcat(proposition,(yyvsp[-3].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
#line 2855 "filterJGEXtoFOF.tab.c"
    break;

  case 65: /* circle: SYM VARIABLE VARIABLE VARIABLE  */
#line 1371 "filterJGEXtoFOF.y"
                  { /* SYM C B A = coll(C,B,A) & cong(A,B,A,C) */
		    upstr((yyvsp[-2].string));upstr((yyvsp[-1].string));upstr((yyvsp[0].string));
		    strcat(proposition,"\ncoll(");
		    strcat(proposition,(yyvsp[-2].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,") & cong(");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[0].string));
		    strcat(proposition,",");
		    strcat(proposition,(yyvsp[-1].string));
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);
		  }
#line 2879 "filterJGEXtoFOF.tab.c"
    break;

  case 66: /* variaveis: VARIABLE  */
#line 1393 "filterJGEXtoFOF.y"
                 {
		   upstr((yyvsp[0].string));
		   strcpy(listIdVariables[numIdVariables++].variable,(yyvsp[0].string));
		 }
#line 2888 "filterJGEXtoFOF.tab.c"
    break;

  case 67: /* $@1: %empty  */
#line 1398 "filterJGEXtoFOF.y"
                 {
		   upstr((yyvsp[0].string));
		   strcpy(listIdVariables[numIdVariables++].variable,(yyvsp[0].string));
		 }
#line 2897 "filterJGEXtoFOF.tab.c"
    break;


#line 2901 "filterJGEXtoFOF.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
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
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1405 "filterJGEXtoFOF.y"



int main(int argc, char *argv[]) {
  
  if (argc < 2) {
    printf("Error: usage ./jgexToTPTP <full-name-of-a-JGEX-file>\n\n");
    return(1);
  }

  
  strncpy(fileName,argv[1],strlen(argv[1])-4);
  strcpy(fullFileName,argv[1]);
  
  yyin = fopen(fullFileName,"r");

  strcpy(outputFileName,fileName);
  strcpy(conjectureName,fileName);
 
  cleanName(conjectureName);

  strcat(outputFileName,".p");

  yyout = fopen(outputFileName,"w");
  
  yyparse();

  /*
    If there isn't a 'SHOW' statement, the .gex file is not a
    conjecture, so the convertion to TPTP format is not done
   */
  if (numHypotheses==0) {

    char command[50];
    strcpy(command, "rm -f ");
    strcat(command,outputFileName);
    system(command);
    return(1);   // error code, not a conjecture
  }
  // if numHypotheses > 0 then proceed

  
  // Include the Axions
  fprintf(yyout,"\n%%----Include Geometry Deductive Database Method axioms\ninclude('geometryDeductiveDatabaseMethod.ax').\n\nfof(exemplo%s,conjecture,( ! [ ",conjectureName);
  // Universal quantified Points
  for (int i=0;i<numIdVariables;i++) {
    fprintf(yyout,"%s",listIdVariables[i].variable);
    if (i<numIdVariables-1) {
      fprintf(yyout,",");
    }
    else {
      fprintf(yyout," ");
    }
  }
  
  fprintf(yyout,"] :\n");
  
  // Parenthesis embracing all the conjecture
  fprintf(yyout,"(");
  // Parenthesis embracing the hypothesis
  fprintf(yyout,"(");
  for (int i=0;i<numHypotheses;i++) {
    fprintf(yyout,"%s",listPropositions[i].proposition);
    if (i<numHypotheses-1) {
      fprintf(yyout," & ");
    }
    else {
      fprintf(yyout,"\n");
    }
  }
  // Closing the parenthesis in the hypothesis and opening it in the conclusions 
  fprintf(yyout,")\n=>\n(");
  for (int i=numHypotheses;i<numPropositions;i++) {
    fprintf(yyout,"%s",listPropositions[i].proposition);
    if (i<numPropositions-1) {
      fprintf(yyout," & ");
    }
    else {
      fprintf(yyout,"\n");
    }
  }
  // Close the parenthesis on the conclusions
  fprintf(yyout,")\n");
  // Close the parenthesis on the conjecture
  fprintf(yyout,")\n");
  // Close all
  fprintf(yyout,")).");
  
  fclose(yyout);
}

int yyerror(char *s) {
  // printf("%s\n",s);
};

/*
 * Given that the filename will be used as part of the name of the
 * conjecture in the fof statment it must obey to TPTP rules. It
 * cannot contain other symbols not letters and/or digits
 * --> conjectureName
 * <-- conjectureName (without non-letters and non-digits symbols
 */
void cleanName(char conjectureName[]){
  char aux[60];
  int pos=0,posAux=0;
  int fileNameLength=strlen(conjectureName);

  while (pos < fileNameLength) {
    if (conjectureName[pos] >= 'a' && conjectureName[pos] <= 'z' ||
	conjectureName[pos] >= 'A' && conjectureName[pos] <= 'Z' ||
	conjectureName[pos] >= '0' && conjectureName[pos] <= '9') {
      aux[posAux++] = conjectureName[pos];
    }
    pos++;
  }
  aux[posAux]='\0';
  strcpy(conjectureName,aux);
}


void oncircle(char arg[]){
  char arg1[6],arg2[6],arg3[6];
  int j,k;

  for (int i=0;i<numPropositions;i++) {
    if (!strncmp(listPropositions[i].proposition,"circle(",7)){
      j=0;
      k=0;
      while(listPropositions[i].proposition[7+k] != ','){
	arg1[j++]=listPropositions[i].proposition[7+k++];
      }
      arg1[j]='\0';
      j=0;k++;
      while(listPropositions[i].proposition[7+k] != ','){
	arg2[j++]=listPropositions[i].proposition[7+k++];
      }
      arg2[j]='\0';
      j=0;k++;
      while(listPropositions[i].proposition[7+k] != ','){
	arg3[j++]=listPropositions[i].proposition[7+k++];
      }
      arg3[j]='\0';

      //      printf("--- %s,%s,%s,%s ---",arg,arg1,arg2,arg3);
      newProposition[0]='\0';
      strncat(newProposition,listPropositions[i].proposition,7);
      strcat(newProposition,arg1);
      strcat(newProposition,",");
      strcat(newProposition,arg2);
      strcat(newProposition,",");
      strcat(newProposition,arg3);
      strcat(newProposition,",");
      strcat(newProposition,arg);
      strcat(newProposition,")");
    }
  }
}


void reflection(char arg1[],char arg2[],char arg3[]) {
  newProposition[0]='\0';
  strcat(newProposition,"\ncoll(");
  strcat(newProposition,arg2);
  strcat(newProposition,",");
  strcat(newProposition,arg3);
  strcat(newProposition,",");
  strcat(newProposition,arg1);
  strcat(newProposition,") & cong(");
  strcat(newProposition,arg2);
  strcat(newProposition,",");
  strcat(newProposition,arg3);
  strcat(newProposition,",");
  strcat(newProposition,arg2);
  strcat(newProposition,",");
  strcat(newProposition,arg1);
  strcat(newProposition,")");  
}

void upstr(char * temp) {
  char * name;
  name = strtok(temp,":");

  // Convert to upper case
  char *s = name;
  while (*s) {
    *s = toupper((unsigned char) *s);
    s++;
  }
}
