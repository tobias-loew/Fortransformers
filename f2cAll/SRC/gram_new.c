
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
/* #line 207 "gram.in" */

#include <string>
#include "defs.h"
#include "func_def.h"
#include "p1defs.h"

static int nstars;			/* Number of labels in an
					   alternate return CALL */
static int datagripe;
int ndim;
int ndim_type;  // dimension for all variables of the type
static int vartype;
extern std::string var_derived_type_name;
extern std::string var_derived_type_extends_name;
int new_dcl;
static ftnint varleng;
struct Dims dims[MAXDIM+1];
struct Dims dims_type[MAXDIM+1];
bool in_dims_type = false;
extern struct Labelblock **labarray;	/* Labels in an alternate
						   return CALL */
extern int maxlablist;
extern bool in_member_access = false;
/* The next two variables are used to verify that each statement might be reached
   during runtime.   lastwasbranch   is tested only in the defintion of the
   stat:   nonterminal. */

int lastwasbranch = NO;
static int thiswasbranch = NO;
extern ftnint yystno;
extern flag intonly;
static chainp datastack;
extern long laststfcn, thisstno;
extern int can_include;	/* for netlib */

//ftnint convci();
//Addrp nextdata();
//expptr mklogcon(), mkaddcon(), mkrealcon(), mkstrcon(), mkbitcon();
//expptr mkcxcon();
//struct Listblock *mklist();
//struct Listblock *mklist();
//struct Impldoblock *mkiodo();
//Extsym *comblock();
#define ESNULL (Extsym *)0
#define NPNULL (Namep)0
#define LBNULL (struct Listblock *)0
//extern void freetemps(), make_param();


 static void
pop_datastack() {
	chainp d0 = datastack;
	if (d0->datap)
		curdtp = (chainp)d0->datap;
	datastack = d0->nextp;
	d0->nextp = 0;
	frchain(&d0);
	}



/* Line 189 of yacc.c  */
/* #line 135 "gram.tab.in" */

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     SEOS = 1,
     SCOMMENT = 2,
     SLABEL = 3,
     SUNKNOWN = 4,
     SHOLLERITH = 5,
     SICON = 6,
     SRCON = 7,
     SDCON = 8,
     SBITCON = 9,
     SOCTCON = 10,
     SHEXCON = 11,
     STRUE = 12,
     SFALSE = 13,
     SNAME = 14,
     SNAMEEQ = 15,
     SFIELD = 16,
     SSCALE = 17,
     SINCLUDE = 18,
     SLET = 19,
     SASSIGN = 20,
     SAUTOMATIC = 21,
     SBACKSPACE = 22,
     SBLOCK = 23,
     SCALL = 24,
     SCHARACTER = 25,
     SCLOSE = 26,
     SCOMMON = 27,
     SCOMPLEX = 28,
     SCONTINUE = 29,
     SDATA = 30,
     SDCOMPLEX = 31,
     SDIMENSION = 32,
     SDO = 33,
     SDOUBLE = 34,
     SELSE = 35,
     SELSEIF = 36,
     SEND = 37,
     SENDFILE = 38,
     SENDIF = 39,
     SENTRY = 40,
     SEQUIV = 41,
     SEXTERNAL = 42,
     SFORMAT = 43,
     SFUNCTION = 44,
     SGOTO = 45,
     SASGOTO = 46,
     SCOMPGOTO = 47,
     SARITHIF = 48,
     SLOGIF = 49,
     SIMPLICIT = 50,
     SINQUIRE = 51,
     SINTEGER = 52,
     SINTRINSIC = 53,
     SLOGICAL = 54,
     SNAMELIST = 55,
     SOPEN = 56,
     SPARAM = 57,
     SPAUSE = 58,
     SPRINT = 59,
     SPROGRAM = 60,
     SPUNCH = 61,
     SREAD = 62,
     SREAL = 63,
     SRETURN = 64,
     SREWIND = 65,
     SSAVE = 66,
     SSTATIC = 67,
     SSTOP = 68,
     SSUBROUTINE = 69,
     STHEN = 70,
     STO = 71,
     SUNDEFINED = 72,
     SWRITE = 73,
     SLPAR = 74,
     SRPAR = 75,
     SEQUALS = 76,
     SCOLON = 77,
     SCOMMA = 78,
     SCURRENCY = 79,
     SPLUS = 80,
     SMINUS = 81,
     SSTAR = 82,
     SSLASH = 83,
     SPOWER = 84,
     SCONCAT = 85,
     SAND = 86,
     SOR = 87,
     SNEQV = 88,
     SEQV = 89,
     SNOT = 90,
     SEQ = 91,
     SLT = 92,
     SGT = 93,
     SLE = 94,
     SGE = 95,
     SNE = 96,
     SENDDO = 97,
     SWHILE = 98,
     SSLASHD = 99,
     SSELECT = 100,
     SCASE = 101,
     SCASEDEFAULT = 102,
     SENDSELECT = 103,
     SINTENT = 104,
     SIN = 105,
     SOUT = 106,
     SINOUT = 107,
     SVALUE = 108,
     SLPARSLASH = 109,
     SSLASHRPAR = 110,
     SMODULE = 111,
     SCONTAINS = 112,
     SPURE = 113,
     SELEMENTAL = 114,
     SBIND = 115,
     STYPE = 116,
     SUSE = 117,
     SENDFUNCTION = 118,
     SENDSUBROUTINE = 119,
     SENDMODULE = 120,
     SENDPROGRAM = 121,
     SENDTYPE = 122,
     SMEMBER = 123,
     SCYCLE = 124,
     SEXIT = 125,
     SPRIVATE = 126,
     SPUBLIC = 127,
     SDOCONCURRENT = 128,
     SORDER = 129,
     SRESHAPE = 130,
     SINTERFACE = 131,
     SENDINTERFACE = 132,
     SALLOCATABLE = 133,
     SALLOCATE = 134,
     SSOURCE = 135,
     SSTAT = 136,
     SDOT = 137,
     SOPTIONAL = 138,
     SPOINTER = 139,
     SDEALLOCATE = 140,
     SCLASS = 141,
     SEXTENDS = 142,
     SKIND = 143,
     SRESULT = 144,
     SLEN = 145,
     SC_INT = 146,
     SC_SHORT = 147,
     SC_LONG = 148,
     SC_LONG_LONG = 149,
     SC_SIGNED_CHAR = 150,
     SC_SIZE_T = 151,
     SC_INT8_T = 152,
     SC_INT16_T = 153,
     SC_INT32_T = 154,
     SC_INT64_T = 155,
     SC_INT128_T = 156,
     SC_INT_LEAST8_T = 157,
     SC_INT_LEAST16_T = 158,
     SC_INT_LEAST32_T = 159,
     SC_INT_LEAST64_T = 160,
     SC_INT_LEAST128_T = 161,
     SC_INT_FAST8_T = 162,
     SC_INT_FAST16_T = 163,
     SC_INT_FAST32_T = 164,
     SC_INT_FAST64_T = 165,
     SC_INT_FAST128_T = 166,
     SC_INTMAX_T = 167,
     SC_INTPTR_T = 168,
     SC_PTRDIFF_T = 169,
     SC_FLOAT = 170,
     SC_DOUBLE = 171,
     SC_LONG_DOUBLE = 172,
     SC_FLOAT128 = 173,
     SC_FLOAT_COMPLEX = 174,
     SC_DOUBLE_COMPLEX = 175,
     SC_LONG_DOUBLE_COMPLEX = 176,
     SC_FLOAT128_COMPLEX = 177,
     SC_BOOL = 178,
     SC_CHAR = 179,
     SINT64 = 180,
     SREAL128 = 181,
     SONLY = 182,
     SOPCALL = 258
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
/* #line 272 "gram.in" */

	int ival;
    ii_val_t ii_val;
	ftnint lval;
	char *charpval;
	chainp chval;
	tagptr tagval;
	expptr expval;
	struct Labelblock *labval;
	struct Nameblock *namval;
	struct Eqvchain *eqvval;
	Extsym *extval;
	


/* Line 214 of yacc.c  */
/* #line 372 "gram.tab.in" */
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
/* #line 384 "gram.tab.in" */

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2419

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  186
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  198
/* YYNRULES -- Number of rules.  */
#define YYNRULES  503
/* YYNRULES -- Number of states.  */
#define YYNSTATES  904

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   258

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,   185
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     8,    11,    14,    17,    21,    24,
      29,    34,    39,    43,    47,    50,    53,    55,    57,    61,
      66,    70,    71,    72,    81,    82,    83,    92,    96,   100,
     102,   104,   105,   106,   113,   114,   115,   122,   123,   126,
     127,   131,   132,   134,   136,   137,   140,   141,   144,   146,
     149,   150,   152,   153,   154,   156,   157,   159,   161,   162,
     164,   167,   171,   173,   177,   179,   183,   184,   187,   191,
     193,   197,   199,   201,   203,   205,   207,   209,   211,   213,
     215,   217,   219,   221,   223,   226,   228,   231,   233,   236,
     238,   244,   251,   261,   262,   275,   278,   284,   291,   297,
     302,   307,   308,   312,   313,   318,   319,   321,   322,   325,
     326,   339,   341,   343,   345,   346,   352,   354,   356,   358,
     360,   362,   364,   366,   368,   371,   372,   375,   376,   381,
     382,   385,   387,   389,   391,   393,   395,   397,   399,   401,
     403,   405,   407,   409,   414,   419,   421,   422,   426,   432,
     435,   437,   439,   441,   443,   445,   447,   449,   451,   453,
     455,   457,   459,   461,   463,   465,   467,   469,   471,   473,
     475,   477,   479,   481,   483,   485,   487,   489,   491,   493,
     495,   497,   499,   501,   503,   505,   507,   509,   510,   516,
     523,   524,   525,   531,   532,   533,   537,   538,   543,   545,
     546,   551,   558,   559,   564,   571,   575,   580,   586,   590,
     592,   596,   600,   604,   608,   612,   616,   620,   624,   626,
     630,   634,   638,   639,   646,   647,   648,   649,   652,   656,
     658,   662,   667,   669,   672,   674,   678,   680,   682,   684,
     688,   692,   693,   694,   709,   713,   715,   719,   721,   723,
     724,   728,   730,   734,   737,   739,   745,   747,   751,   752,
     755,   756,   760,   761,   764,   768,   770,   774,   776,   778,
     780,   782,   786,   788,   792,   796,   797,   798,   802,   803,
     806,   808,   812,   813,   814,   820,   821,   828,   833,   835,
     836,   839,   841,   845,   847,   851,   853,   855,   858,   863,
     865,   869,   870,   871,   873,   875,   877,   881,   885,   887,
     891,   893,   897,   899,   901,   905,   909,   913,   917,   920,
     924,   928,   932,   936,   940,   943,   947,   952,   955,   956,
     962,   963,   967,   968,   972,   982,   984,   986,   988,   990,
     992,   994,   996,   998,  1000,  1003,  1008,  1014,  1016,  1020,
    1026,  1030,  1031,  1033,  1035,  1037,  1039,  1041,  1043,  1045,
    1047,  1049,  1051,  1053,  1055,  1057,  1059,  1063,  1065,  1067,
    1071,  1075,  1079,  1083,  1086,  1090,  1092,  1100,  1105,  1107,
    1110,  1113,  1120,  1123,  1126,  1127,  1135,  1137,  1140,  1142,
    1146,  1152,  1156,  1157,  1161,  1167,  1175,  1177,  1181,  1186,
    1188,  1193,  1200,  1207,  1213,  1219,  1224,  1230,  1233,  1235,
    1237,  1239,  1241,  1243,  1254,  1256,  1260,  1265,  1269,  1273,
    1275,  1277,  1279,  1281,  1285,  1289,  1297,  1305,  1306,  1308,
    1312,  1314,  1318,  1320,  1323,  1325,  1327,  1329,  1331,  1335,
    1338,  1341,  1343,  1347,  1351,  1353,  1357,  1358,  1359,  1360,
    1362,  1365,  1368,  1371,  1374,  1377,  1380,  1383,  1387,  1392,
    1397,  1399,  1403,  1407,  1409,  1411,  1413,  1417,  1419,  1421,
    1423,  1425,  1427,  1431,  1435,  1437,  1441,  1443,  1445,  1447,
    1450,  1453,  1456,  1458,  1462,  1467,  1472,  1474,  1478,  1480,
    1486,  1488,  1490,  1492,  1496,  1500,  1504,  1508,  1512,  1516,
    1520,  1526,  1532,  1538
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     187,     0,    -1,    -1,   187,   188,     3,    -1,   189,   190,
      -1,   189,   222,    -1,   189,   336,    -1,   189,    20,   221,
      -1,   189,    39,    -1,   189,   120,   212,   362,    -1,   189,
     121,   212,   362,    -1,   189,   123,   212,   362,    -1,   189,
     122,   212,    -1,   189,   124,   212,    -1,   189,   134,    -1,
     189,     6,    -1,     1,    -1,     5,    -1,    62,   209,   214,
      -1,    62,   209,   214,   215,    -1,    25,   209,   214,    -1,
      -1,    -1,   208,    71,   209,   211,   218,   191,   203,   192,
      -1,    -1,    -1,   208,    46,   209,   211,   218,   193,   203,
     194,    -1,    42,   211,   218,    -1,   113,   210,    16,    -1,
     114,    -1,   133,    -1,    -1,    -1,   196,   117,   197,    76,
      16,    77,    -1,    -1,    -1,   198,   146,   199,    76,   213,
      77,    -1,    -1,   201,   195,    -1,    -1,   200,   202,   195,
      -1,    -1,   200,    -1,   239,    -1,    -1,   205,   116,    -1,
      -1,   206,   115,    -1,   204,    -1,   207,   204,    -1,    -1,
     207,    -1,    -1,    -1,   213,    -1,    -1,    16,    -1,    16,
      -1,    -1,   211,    -1,    76,    77,    -1,    76,   216,    77,
      -1,   217,    -1,   216,    80,   217,    -1,    16,    -1,    16,
      78,    16,    -1,    -1,    76,    77,    -1,    76,   219,    77,
      -1,   220,    -1,   219,    80,   220,    -1,   213,    -1,    84,
      -1,     7,    -1,   223,    -1,   256,    -1,   258,    -1,   259,
      -1,   260,    -1,   263,    -1,   302,    -1,   295,    -1,   310,
      -1,   128,    -1,   128,    16,    -1,   129,    -1,   129,    16,
      -1,    68,    -1,    68,   272,    -1,    45,    -1,    59,   313,
      76,   274,    77,    -1,   239,   213,   313,   238,   288,   255,
      -1,   239,    79,    79,   213,   313,   238,   288,   255,   230,
      -1,    -1,   239,    80,   224,   235,    79,    79,   213,   313,
     238,   288,   255,   230,    -1,   118,    16,    -1,   118,   229,
      79,    79,    16,    -1,   223,    80,   213,   288,   255,   230,
      -1,   223,   101,   266,   268,   101,    -1,   117,    76,    16,
      77,    -1,   144,    76,    16,    77,    -1,    -1,    80,   227,
     225,    -1,    -1,   226,    80,   228,   225,    -1,    -1,   226,
      -1,    -1,    78,   317,    -1,    -1,    78,   132,    76,   317,
      80,   279,    80,   231,   131,    78,   279,    77,    -1,   107,
      -1,   108,    -1,   109,    -1,    -1,   106,    76,   234,   232,
      77,    -1,   110,    -1,    59,    -1,    68,    -1,   128,    -1,
     129,    -1,   135,    -1,   140,    -1,    44,    -1,    34,   286,
      -1,    -1,   236,   233,    -1,    -1,   235,    80,   237,   233,
      -1,    -1,   240,   242,    -1,   241,    -1,    54,    -1,    65,
      -1,    30,    -1,    36,    -1,    33,    -1,    56,    -1,    27,
      -1,    74,    -1,    34,    -1,    23,    -1,    69,    -1,   118,
      76,    16,    77,    -1,   143,    76,    16,    77,    -1,   254,
      -1,    -1,    76,   243,   244,    -1,   363,   317,   364,   250,
      77,    -1,   246,    77,    -1,   180,    -1,   181,    -1,   173,
      -1,   177,    -1,   172,    -1,   176,    -1,   175,    -1,   179,
      -1,   148,    -1,   168,    -1,   165,    -1,   166,    -1,   167,
      -1,   164,    -1,   163,    -1,   160,    -1,   161,    -1,   162,
      -1,   159,    -1,   158,    -1,   155,    -1,   156,    -1,   157,
      -1,   154,    -1,   169,    -1,   170,    -1,   150,    -1,   174,
      -1,   178,    -1,   151,    -1,   171,    -1,   149,    -1,   152,
      -1,   153,    -1,   182,    -1,   183,    -1,   245,    -1,    -1,
     145,    78,   247,   245,   248,    -1,   147,    78,   363,   317,
     364,   250,    -1,    -1,    -1,    80,   249,   147,    78,   317,
      -1,    -1,    -1,    80,   251,   252,    -1,    -1,   145,    78,
     253,   245,    -1,   245,    -1,    -1,    84,   363,   317,   364,
      -1,    84,   363,    76,    84,    77,   364,    -1,    -1,    84,
     363,   317,   364,    -1,    84,   363,    76,    84,    77,   364,
      -1,    29,   313,   283,    -1,    29,   313,   257,   283,    -1,
     256,   352,   257,   352,   283,    -1,   256,    80,   283,    -1,
      87,    -1,    85,    16,    85,    -1,    44,   313,   213,    -1,
     258,    80,   213,    -1,    55,   313,   213,    -1,   259,    80,
     213,    -1,    43,   313,   261,    -1,   260,    80,   261,    -1,
      76,   262,    77,    -1,   326,    -1,   262,    80,   326,    -1,
      32,   264,   265,    -1,   263,   352,   265,    -1,    -1,   266,
     285,    85,   267,   268,    85,    -1,    -1,    -1,    -1,   269,
     270,    -1,   268,    80,   270,    -1,   271,    -1,   331,    84,
     271,    -1,   213,    76,   314,    77,    -1,   331,    -1,   324,
     331,    -1,   273,    -1,   272,    80,   273,    -1,   213,    -1,
     257,    -1,   275,    -1,   274,    80,   275,    -1,   213,    78,
     317,    -1,    -1,    -1,   213,    78,   132,   276,    76,   317,
      80,   279,    80,   277,   131,    78,   279,    77,    -1,   278,
      80,   317,    -1,   317,    -1,   111,   278,   112,    -1,   323,
      -1,   317,    -1,    -1,   239,    79,    79,    -1,   280,    -1,
     282,    80,   280,    -1,   213,   288,    -1,   326,    -1,    76,
     285,    80,   339,    77,    -1,   284,    -1,   285,    80,   284,
      -1,    -1,   287,   288,    -1,    -1,    76,   289,    77,    -1,
      -1,   290,   291,    -1,   289,    80,   291,    -1,   292,    -1,
     317,    79,   292,    -1,    79,    -1,    84,    -1,   317,    -1,
     294,    -1,   293,    80,   294,    -1,     8,    -1,    52,   313,
     304,    -1,   295,    80,   304,    -1,    -1,    -1,    80,   297,
      55,    -1,    -1,    79,    79,    -1,    16,    -1,   299,    80,
      16,    -1,    -1,    -1,    80,   301,   184,    79,   299,    -1,
      -1,   119,   296,   298,    16,   303,   300,    -1,   305,    76,
     307,    77,    -1,   305,    -1,    -1,   306,   239,    -1,   308,
      -1,   307,    80,   308,    -1,   309,    -1,   309,    83,   309,
      -1,    16,    -1,    57,    -1,   310,   311,    -1,    85,   213,
      85,   312,    -1,   213,    -1,   312,    80,   213,    -1,    -1,
      -1,   316,    -1,   328,    -1,   317,    -1,   137,    78,   317,
      -1,   138,    78,   317,    -1,   315,    -1,   316,    80,   315,
      -1,   318,    -1,    76,   317,    77,    -1,   327,    -1,   332,
      -1,   317,   324,   317,    -1,   317,    84,   317,    -1,   317,
      85,   317,    -1,   317,    86,   317,    -1,   324,   317,    -1,
     317,   325,   317,    -1,   317,    91,   317,    -1,   317,    90,
     317,    -1,   317,    89,   317,    -1,   317,    88,   317,    -1,
      92,   317,    -1,   317,    87,   317,    -1,   317,    76,   314,
      77,    -1,   317,   320,    -1,    -1,   111,   319,   281,   282,
     112,    -1,    -1,   125,   321,   317,    -1,    -1,   139,   322,
     317,    -1,    76,   317,    80,   213,    78,   317,    80,   317,
      77,    -1,    82,    -1,    83,    -1,    93,    -1,    95,    -1,
      94,    -1,    97,    -1,    96,    -1,    98,    -1,   213,    -1,
     213,   329,    -1,   213,    76,   314,    77,    -1,   213,    76,
     314,    77,   329,    -1,   213,    -1,   330,    79,   330,    -1,
     330,    79,   330,    79,   330,    -1,    76,   328,    77,    -1,
      -1,   317,    -1,   213,    -1,   332,    -1,    14,    -1,    15,
      -1,     7,    -1,     8,    -1,     9,    -1,    10,    -1,   333,
      -1,    13,    -1,    12,    -1,    11,    -1,   335,    -1,    76,
     334,    77,    -1,   326,    -1,   332,    -1,   334,   324,   334,
      -1,   334,    84,   334,    -1,   334,    85,   334,    -1,   334,
      86,   334,    -1,   324,   334,    -1,   334,    87,   334,    -1,
     348,    -1,    35,   362,   363,   294,   364,   352,   344,    -1,
      35,   362,   352,   344,    -1,    99,    -1,   338,   348,    -1,
     338,    72,    -1,    38,   362,    76,   317,    77,    72,    -1,
      37,   362,    -1,    41,   362,    -1,    -1,   102,   362,   337,
     103,    76,   317,    77,    -1,   105,    -1,   103,   360,    -1,
     104,    -1,   130,   362,   343,    -1,    51,   362,    76,   317,
      77,    -1,   213,    78,   361,    -1,    -1,    80,   317,   340,
      -1,   213,    78,   317,    79,   317,    -1,   213,    78,   317,
      79,   317,    79,   317,    -1,   341,    -1,   342,    80,   341,
      -1,    76,   342,   340,    77,    -1,   339,    -1,   100,    76,
     317,    77,    -1,    75,   362,    76,   314,    77,   314,    -1,
     141,    76,   317,    80,   317,    77,    -1,   136,   362,    76,
     314,    77,    -1,   142,   362,    76,   314,    77,    -1,   350,
     318,    78,   317,    -1,    22,   362,   349,    73,   213,    -1,
      31,   362,    -1,   351,    -1,   345,    -1,   347,    -1,   346,
      -1,   365,    -1,    50,   362,    76,   317,    77,   294,    80,
     294,    80,   294,    -1,   353,    -1,   353,    76,    77,    -1,
     353,    76,   354,    77,    -1,    66,   362,   330,    -1,   356,
     362,   330,    -1,   126,    -1,   127,    -1,     8,    -1,    21,
      -1,    47,   362,   294,    -1,    48,   362,   213,    -1,    48,
     362,   213,   352,    76,   293,    77,    -1,    49,   362,    76,
     293,    77,   352,   317,    -1,    -1,    80,    -1,    26,   362,
     213,    -1,   355,    -1,   354,    80,   355,    -1,   317,    -1,
      84,   294,    -1,    60,    -1,    70,    -1,   317,    -1,   357,
      -1,   357,    79,   357,    -1,   357,    79,    -1,    79,   357,
      -1,   358,    -1,   359,    80,   358,    -1,    76,   359,    77,
      -1,   317,    -1,   361,    80,   317,    -1,    -1,    -1,    -1,
     366,    -1,   367,   372,    -1,   367,   335,    -1,   367,    84,
      -1,   367,    86,    -1,   369,   372,    -1,   376,   372,    -1,
     376,   371,    -1,   376,   372,   378,    -1,   376,   371,    80,
     378,    -1,   376,   372,    80,   378,    -1,   377,    -1,   377,
      80,   380,    -1,   368,   362,   383,    -1,    24,    -1,    67,
      -1,    40,    -1,   370,   362,   383,    -1,    53,    -1,    58,
      -1,    28,    -1,   335,    -1,    84,    -1,    76,   334,    77,
      -1,    76,   373,    77,    -1,   374,    -1,   373,    80,   374,
      -1,   334,    -1,    84,    -1,    86,    -1,   375,   317,    -1,
     375,    84,    -1,   375,    86,    -1,    17,    -1,    64,   362,
     383,    -1,    61,   362,   334,   383,    -1,    61,   362,    84,
     383,    -1,   379,    -1,   378,    80,   379,    -1,   326,    -1,
      76,   378,    80,   339,    77,    -1,   318,    -1,   382,    -1,
     381,    -1,   318,    80,   318,    -1,   318,    80,   382,    -1,
     382,    80,   318,    -1,   382,    80,   382,    -1,   381,    80,
     318,    -1,   381,    80,   382,    -1,    76,   317,    77,    -1,
      76,   318,    80,   339,    77,    -1,    76,   382,    80,   339,
      77,    -1,    76,   381,    80,   339,    77,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   329,   329,   330,   333,   338,   339,   359,   367,   372,
     377,   382,   387,   394,   402,   404,   411,   416,   444,   446,
     450,   453,   453,   453,   459,   459,   459,   461,   467,   470,
     474,   485,   485,   485,   487,   487,   487,   489,   489,   491,
     491,   495,   496,   502,   504,   504,   507,   507,   510,   512,
     516,   517,   521,   525,   528,   532,   534,   537,   541,   542,
     546,   547,   550,   551,   554,   555,   559,   560,   563,   567,
     569,   573,   580,   596,   604,   605,   606,   607,   608,   609,
     610,   611,   612,   613,   614,   615,   616,   617,   620,   622,
     624,   628,   639,   654,   654,   679,   681,   683,   707,   719,
     721,   725,   725,   727,   727,   730,   732,   736,   740,   742,
     742,   748,   749,   750,   754,   754,   759,   764,   769,   774,
     779,   784,   789,   794,   799,   807,   807,   812,   812,   819,
     821,   825,   831,   832,   833,   834,   835,   836,   837,   838,
     839,   840,   841,   842,   843,   847,   849,   849,   857,   859,
     863,   864,   865,   866,   867,   868,   869,   870,   871,   872,
     873,   874,   875,   876,   877,   878,   879,   880,   881,   882,
     883,   884,   885,   886,   887,   888,   889,   890,   891,   892,
     893,   894,   895,   896,   897,   898,   906,   908,   908,   911,
     914,   917,   917,   920,   923,   923,   927,   927,   929,   934,
     935,   964,   969,   970,  1004,  1008,  1010,  1012,  1014,  1018,
    1020,  1024,  1026,  1030,  1032,  1036,  1037,  1040,  1053,  1057,
    1064,  1065,  1069,  1083,  1094,  1096,  1098,  1098,  1099,  1102,
    1104,  1109,  1114,  1115,  1123,  1124,  1127,  1134,  1137,  1138,
    1141,  1151,  1151,  1151,  1167,  1169,  1173,  1189,  1192,  1198,
    1200,  1204,  1206,  1212,  1216,  1228,  1244,  1250,  1256,  1256,
    1262,  1263,  1266,  1266,  1267,  1270,  1281,  1292,  1305,  1307,
    1310,  1312,  1316,  1320,  1322,  1325,  1328,  1328,  1331,  1334,
    1339,  1340,  1343,  1346,  1346,  1349,  1349,  1352,  1353,  1359,
    1359,  1363,  1364,  1367,  1369,  1373,  1383,  1384,  1387,  1401,
    1403,  1408,  1436,  1437,  1442,  1450,  1453,  1456,  1473,  1475,
    1480,  1481,  1486,  1487,  1488,  1490,  1492,  1494,  1496,  1501,
    1503,  1506,  1509,  1511,  1513,  1515,  1520,  1524,  1530,  1530,
    1536,  1536,  1539,  1539,  1544,  1562,  1563,  1566,  1567,  1568,
    1569,  1570,  1571,  1574,  1576,  1579,  1581,  1586,  1605,  1610,
    1617,  1622,  1623,  1626,  1634,  1637,  1638,  1639,  1640,  1641,
    1642,  1643,  1652,  1655,  1658,  1663,  1664,  1668,  1669,  1670,
    1672,  1674,  1676,  1678,  1683,  1687,  1688,  1695,  1700,  1702,
    1704,  1705,  1707,  1709,  1711,  1711,  1713,  1715,  1717,  1719,
    1726,  1730,  1737,  1739,  1744,  1754,  1766,  1769,  1774,  1783,
    1784,  1789,  1795,  1826,  1831,  1838,  1841,  1843,  1844,  1845,
    1846,  1847,  1848,  1850,  1852,  1854,  1856,  1862,  1864,  1866,
    1868,  1872,  1876,  1885,  1887,  1889,  1891,  1899,  1900,  1903,
    1907,  1909,  1913,  1914,  1918,  1920,  1933,  1937,  1942,  1947,
    1952,  1959,  1961,  1965,  1968,  1970,  1975,  1989,  1993,  1997,
    2001,  2002,  2004,  2006,  2008,  2009,  2011,  2013,  2015,  2017,
    2023,  2025,  2029,  2032,  2034,  2036,  2040,  2043,  2045,  2047,
    2051,  2057,  2065,  2070,  2074,  2075,  2078,  2080,  2082,  2084,
    2086,  2088,  2092,  2096,  2104,  2111,  2120,  2122,  2126,  2128,
    2132,  2134,  2136,  2139,  2141,  2143,  2145,  2147,  2149,  2156,
    2158,  2160,  2162,  2167
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SEOS", "SCOMMENT", "SLABEL", "SUNKNOWN",
  "SHOLLERITH", "SICON", "SRCON", "SDCON", "SBITCON", "SOCTCON", "SHEXCON",
  "STRUE", "SFALSE", "SNAME", "SNAMEEQ", "SFIELD", "SSCALE", "SINCLUDE",
  "SLET", "SASSIGN", "SAUTOMATIC", "SBACKSPACE", "SBLOCK", "SCALL",
  "SCHARACTER", "SCLOSE", "SCOMMON", "SCOMPLEX", "SCONTINUE", "SDATA",
  "SDCOMPLEX", "SDIMENSION", "SDO", "SDOUBLE", "SELSE", "SELSEIF", "SEND",
  "SENDFILE", "SENDIF", "SENTRY", "SEQUIV", "SEXTERNAL", "SFORMAT",
  "SFUNCTION", "SGOTO", "SASGOTO", "SCOMPGOTO", "SARITHIF", "SLOGIF",
  "SIMPLICIT", "SINQUIRE", "SINTEGER", "SINTRINSIC", "SLOGICAL",
  "SNAMELIST", "SOPEN", "SPARAM", "SPAUSE", "SPRINT", "SPROGRAM", "SPUNCH",
  "SREAD", "SREAL", "SRETURN", "SREWIND", "SSAVE", "SSTATIC", "SSTOP",
  "SSUBROUTINE", "STHEN", "STO", "SUNDEFINED", "SWRITE", "SLPAR", "SRPAR",
  "SEQUALS", "SCOLON", "SCOMMA", "SCURRENCY", "SPLUS", "SMINUS", "SSTAR",
  "SSLASH", "SPOWER", "SCONCAT", "SAND", "SOR", "SNEQV", "SEQV", "SNOT",
  "SEQ", "SLT", "SGT", "SLE", "SGE", "SNE", "SENDDO", "SWHILE", "SSLASHD",
  "SSELECT", "SCASE", "SCASEDEFAULT", "SENDSELECT", "SINTENT", "SIN",
  "SOUT", "SINOUT", "SVALUE", "SLPARSLASH", "SSLASHRPAR", "SMODULE",
  "SCONTAINS", "SPURE", "SELEMENTAL", "SBIND", "STYPE", "SUSE",
  "SENDFUNCTION", "SENDSUBROUTINE", "SENDMODULE", "SENDPROGRAM",
  "SENDTYPE", "SMEMBER", "SCYCLE", "SEXIT", "SPRIVATE", "SPUBLIC",
  "SDOCONCURRENT", "SORDER", "SRESHAPE", "SINTERFACE", "SENDINTERFACE",
  "SALLOCATABLE", "SALLOCATE", "SSOURCE", "SSTAT", "SDOT", "SOPTIONAL",
  "SPOINTER", "SDEALLOCATE", "SCLASS", "SEXTENDS", "SKIND", "SRESULT",
  "SLEN", "SC_INT", "SC_SHORT", "SC_LONG", "SC_LONG_LONG",
  "SC_SIGNED_CHAR", "SC_SIZE_T", "SC_INT8_T", "SC_INT16_T", "SC_INT32_T",
  "SC_INT64_T", "SC_INT128_T", "SC_INT_LEAST8_T", "SC_INT_LEAST16_T",
  "SC_INT_LEAST32_T", "SC_INT_LEAST64_T", "SC_INT_LEAST128_T",
  "SC_INT_FAST8_T", "SC_INT_FAST16_T", "SC_INT_FAST32_T",
  "SC_INT_FAST64_T", "SC_INT_FAST128_T", "SC_INTMAX_T", "SC_INTPTR_T",
  "SC_PTRDIFF_T", "SC_FLOAT", "SC_DOUBLE", "SC_LONG_DOUBLE", "SC_FLOAT128",
  "SC_FLOAT_COMPLEX", "SC_DOUBLE_COMPLEX", "SC_LONG_DOUBLE_COMPLEX",
  "SC_FLOAT128_COMPLEX", "SC_BOOL", "SC_CHAR", "SINT64", "SREAL128",
  "SONLY", "SOPCALL", "$accept", "program", "stat", "thislabel", "entry",
  "$@1", "$@2", "$@3", "$@4", "routine_postfix", "$@5", "$@6", "$@7",
  "$@8", "routine_postfixes", "$@9", "$@10", "routine_postfixes_opt",
  "routine_prefix", "$@11", "$@12", "routine_prefixes",
  "routine_prefixes_opt", "new_proc", "new_module", "entryname",
  "opt_name", "name", "progname", "progarglist", "progargs", "progarg",
  "arglist", "args", "arg", "filename", "spec", "dcl", "$@13", "type_attr",
  "type_attrs", "$@14", "$@15", "opt_type_attrs", "opt_init_list", "$@16",
  "inout", "attribute", "$@17", "attributes", "$@18", "$@19", "new_dcl",
  "type", "typespec", "typename", "type_lengspec", "$@20",
  "type_lengspec_helper", "type_lengspec_name_kwd", "type_lengspec_name",
  "$@21", "type_lengspec_opt_len", "$@22", "type_lengspec_opt_kind",
  "$@23", "type_lengspec_opt_kind_helper", "$@24", "lengspec",
  "lengspec_or_dims", "common", "comblock", "external", "intrinsic",
  "equivalence", "equivset", "equivlist", "data", "in_data", "datalist",
  "datainit", "datapop", "vallist", "$@25", "val", "value", "savelist",
  "saveitem", "paramlist", "paramitem", "$@26", "$@27",
  "const_array_items", "const_array", "paraminit", "opt_type",
  "paraminits", "var", "datavar", "datavarlist", "dims_type", "$@28",
  "dims", "dimlist", "$@29", "dim", "ubound", "labellist", "label",
  "implicit", "opt_intrinsic", "$@30", "opt_dcolon", "only_names",
  "opt_onlys", "$@31", "use", "$@32", "implist", "imptype", "$@33",
  "letgroups", "letgroup", "letter", "namelist", "namelistentry",
  "namelistlist", "in_dcl", "funarglist", "funarg", "funargs", "expr",
  "uexpr", "$@34", "expr_member_access", "$@35", "$@36", "expr_do_list",
  "addop", "relop", "lhs", "lhs_base", "in_substring", "substring",
  "opt_expr", "simple", "simple_const", "bit_const", "fexpr",
  "unpar_fexpr", "exec", "$@37", "logif", "dospec", "wherespecs",
  "doconcloop", "doconcloops", "doconcspec", "dospecw", "write_cxx",
  "pointer_integer", "allocate_cxx", "iffable", "assignlabel", "let",
  "goto", "opt_comma", "call", "callarglist", "callarg", "stop", "caseval",
  "casespec", "caselist", "casedecl", "exprlist", "end_spec", "intonlyon",
  "intonlyoff", "io", "io1", "iofmove", "fmkwd", "iofctl", "ctlkwd",
  "infmt", "ioctl", "ctllist", "ioclause", "nameeq", "read", "print",
  "inlist", "inelt", "outlist", "out2", "other", "in_ioctl", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,     1,     2,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   258
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   186,   187,   187,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   189,   190,   190,
     190,   191,   192,   190,   193,   194,   190,   190,   190,   190,
     190,   196,   197,   195,   198,   199,   195,   201,   200,   202,
     200,   203,   203,   204,   205,   204,   206,   204,   207,   207,
     208,   208,   209,   210,   211,   212,   212,   213,   214,   214,
     215,   215,   216,   216,   217,   217,   218,   218,   218,   219,
     219,   220,   220,   221,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   223,   223,   224,   223,   223,   223,   223,   223,   225,
     225,   227,   226,   228,   226,   229,   229,   230,   230,   231,
     230,   232,   232,   232,   234,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   236,   235,   237,   235,   238,
     239,   240,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   242,   243,   242,   244,   244,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   246,   247,   246,   246,
     248,   249,   248,   250,   251,   250,   253,   252,   252,   254,
     254,   254,   255,   255,   255,   256,   256,   256,   256,   257,
     257,   258,   258,   259,   259,   260,   260,   261,   262,   262,
     263,   263,   264,   265,   266,   267,   269,   268,   268,   270,
     270,   271,   271,   271,   272,   272,   273,   273,   274,   274,
     275,   276,   277,   275,   278,   278,   279,   280,   280,   281,
     281,   282,   282,   283,   284,   284,   285,   285,   287,   286,
     288,   288,   290,   289,   289,   291,   291,   291,   292,   292,
     293,   293,   294,   295,   295,   296,   297,   296,   298,   298,
     299,   299,   300,   301,   300,   303,   302,   304,   304,   306,
     305,   307,   307,   308,   308,   309,   310,   310,   311,   312,
     312,   313,   314,   314,   315,   315,   315,   315,   316,   316,
     317,   317,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   319,   318,
     321,   320,   322,   320,   323,   324,   324,   325,   325,   325,
     325,   325,   325,   326,   326,   326,   326,   327,   328,   328,
     329,   330,   330,   331,   331,   332,   332,   332,   332,   332,
     332,   332,   333,   333,   333,   334,   334,   335,   335,   335,
     335,   335,   335,   335,   335,   336,   336,   336,   336,   336,
     336,   336,   336,   336,   337,   336,   336,   336,   336,   336,
     338,   339,   340,   340,   341,   341,   342,   342,   343,   344,
     344,   345,   346,   347,   347,   348,   348,   348,   348,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   349,   350,   351,   351,   351,   351,   352,   352,   353,
     354,   354,   355,   355,   356,   356,   357,   358,   358,   358,
     358,   359,   359,   360,   361,   361,   362,   363,   364,   365,
     366,   366,   366,   366,   366,   366,   366,   366,   366,   366,
     366,   366,   367,   368,   368,   368,   369,   370,   370,   370,
     371,   371,   372,   372,   373,   373,   374,   374,   374,   374,
     374,   374,   375,   376,   377,   377,   378,   378,   379,   379,
     380,   380,   380,   381,   381,   381,   381,   381,   381,   382,
     382,   382,   382,   383
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     3,     2,     2,     2,     3,     2,     4,
       4,     4,     3,     3,     2,     2,     1,     1,     3,     4,
       3,     0,     0,     8,     0,     0,     8,     3,     3,     1,
       1,     0,     0,     6,     0,     0,     6,     0,     2,     0,
       3,     0,     1,     1,     0,     2,     0,     2,     1,     2,
       0,     1,     0,     0,     1,     0,     1,     1,     0,     1,
       2,     3,     1,     3,     1,     3,     0,     2,     3,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     2,     1,     2,     1,
       5,     6,     9,     0,    12,     2,     5,     6,     5,     4,
       4,     0,     3,     0,     4,     0,     1,     0,     2,     0,
      12,     1,     1,     1,     0,     5,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     0,     2,     0,     4,     0,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     4,     1,     0,     3,     5,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     5,     6,
       0,     0,     5,     0,     0,     3,     0,     4,     1,     0,
       4,     6,     0,     4,     6,     3,     4,     5,     3,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     3,
       3,     3,     0,     6,     0,     0,     0,     2,     3,     1,
       3,     4,     1,     2,     1,     3,     1,     1,     1,     3,
       3,     0,     0,    14,     3,     1,     3,     1,     1,     0,
       3,     1,     3,     2,     1,     5,     1,     3,     0,     2,
       0,     3,     0,     2,     3,     1,     3,     1,     1,     1,
       1,     3,     1,     3,     3,     0,     0,     3,     0,     2,
       1,     3,     0,     0,     5,     0,     6,     4,     1,     0,
       2,     1,     3,     1,     3,     1,     1,     2,     4,     1,
       3,     0,     0,     1,     1,     1,     3,     3,     1,     3,
       1,     3,     1,     1,     3,     3,     3,     3,     2,     3,
       3,     3,     3,     3,     2,     3,     4,     2,     0,     5,
       0,     3,     0,     3,     9,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     4,     5,     1,     3,     5,
       3,     0,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     3,
       3,     3,     3,     2,     3,     1,     7,     4,     1,     2,
       2,     6,     2,     2,     0,     7,     1,     2,     1,     3,
       5,     3,     0,     3,     5,     7,     1,     3,     4,     1,
       4,     6,     6,     5,     5,     4,     5,     2,     1,     1,
       1,     1,     1,    10,     1,     3,     4,     3,     3,     1,
       1,     1,     1,     3,     3,     7,     7,     0,     1,     3,
       1,     3,     1,     2,     1,     1,     1,     1,     3,     2,
       2,     1,     3,     3,     1,     3,     0,     0,     0,     1,
       2,     2,     2,     2,     2,     2,     2,     3,     4,     4,
       1,     3,     3,     1,     1,     1,     3,     1,     1,     1,
       1,     1,     3,     3,     1,     3,     1,     1,     1,     2,
       2,     2,     1,     3,     4,     4,     1,     3,     1,     5,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       5,     5,     5,     0
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     1,    16,    17,     0,    50,     3,    15,     0,
     422,   446,   141,   463,    52,   446,   138,   469,   301,   134,
     446,   222,   136,   140,   446,   135,   446,   446,     8,   465,
     446,     0,   301,   301,    89,   446,   446,   446,   446,   446,
     301,   467,   132,   301,   137,   296,   468,   301,   434,   446,
      52,   446,   133,   446,   464,    87,   142,   435,   139,   446,
     378,   446,     0,   388,   386,    53,    29,   105,   275,    55,
      55,    55,    55,    55,   419,   420,    83,    85,   446,    30,
      14,   446,     0,   446,     0,     4,    48,     0,     0,    51,
       0,     5,    74,    43,   199,   131,   427,    76,    77,    78,
     427,    81,    80,    82,     6,     0,   409,   411,   410,   375,
       0,   408,   414,   446,   412,   449,     0,   446,     0,   446,
       0,   460,    73,     7,     0,    58,     0,     0,   407,   224,
     427,   382,     0,   383,    57,    66,    54,     0,     0,     0,
       0,     0,     0,     0,   289,     0,     0,     0,    58,   503,
     351,     0,   209,   236,   237,    88,   234,     0,   384,     0,
     387,     0,    95,     0,   101,   106,     0,   276,   278,    56,
     446,   446,    12,   446,    13,    84,    86,     0,     0,     0,
       0,     0,    45,    47,     0,    49,    43,    52,    52,     0,
     224,     0,    93,   301,   146,   447,   130,   145,   428,     0,
       0,     0,     0,   428,   224,   289,     0,   297,   380,   379,
     357,   358,   359,   360,   364,   363,   362,   355,   356,     0,
     335,   336,     0,   328,   347,     0,   310,     0,   312,   313,
     361,     0,   351,     0,   452,   453,   343,     0,   367,   368,
       0,   365,   450,   503,     0,   454,   503,   471,   365,   456,
     455,     0,   421,     0,    59,    20,   429,   260,     0,   205,
     220,     0,     0,     0,     0,     0,    27,     0,   215,   211,
     272,   423,   424,     0,     0,     0,   273,   288,     0,   213,
       0,     0,   503,   503,   365,    18,   483,   352,   310,   417,
       0,     0,   302,     0,     0,   436,   437,   441,     0,    28,
       0,     0,   103,     0,     0,     0,     0,     9,    10,    11,
       0,   389,   302,     0,   302,     0,     0,     0,   260,   226,
       0,   125,   129,   447,     0,   208,   427,   212,   214,   216,
     221,   274,     0,     0,   324,   249,   302,     0,     0,     0,
       0,     0,     0,     0,     0,   337,   339,   338,   341,   340,
     342,   330,   332,   327,     0,     0,     0,   318,   415,     0,
     432,     0,   430,   418,   482,   477,   478,   476,     0,   474,
       0,   302,   344,   373,     0,     0,     0,     0,     0,   462,
     476,   466,     0,     0,     0,   488,   457,   486,     0,   310,
     461,   492,   491,     0,   262,   253,   206,     0,   256,     0,
     254,     0,     0,   399,   377,   448,     0,    67,    72,    71,
       0,    69,     0,   218,     0,     0,   270,     0,     0,     0,
     290,     0,     0,   238,     0,   485,   484,     0,    19,   210,
     235,     0,     0,     0,   308,   303,   305,   304,     0,     0,
     440,   439,   443,     0,   143,     0,     0,   102,     0,     0,
     277,   279,   285,     0,   396,   392,     0,     0,     0,   144,
      66,    66,   202,     0,     0,   301,     0,     0,   260,     0,
       0,   158,   181,   176,   179,   182,   183,   173,   170,   171,
     172,   169,   168,   165,   166,   167,   164,   163,   160,   161,
     162,   159,   174,   175,   180,   154,   152,   177,   156,   155,
     153,   178,   157,   150,   151,   184,   185,   147,   186,     0,
       0,     0,   448,     0,     0,   311,     0,     0,     0,   315,
     316,   317,   325,   323,   322,   321,   320,     0,     0,   314,
     319,   405,   433,   416,     0,   366,   473,     0,   480,   481,
     479,     0,   304,   370,   371,   372,   374,   369,   472,   458,
       0,   459,     0,     0,   310,     0,     0,     0,     0,     0,
     406,     0,     0,     0,     0,   225,     0,     0,   427,     0,
      68,     0,   217,     0,     0,   427,     0,     0,   390,   295,
       0,   291,   293,     0,    90,     0,   366,    64,    60,     0,
      62,     0,     0,   302,   351,   351,     0,   438,   442,     0,
       0,   104,    96,   282,     0,     0,     0,   403,     0,   404,
      24,    21,   447,   107,     0,    98,   353,   227,   229,     0,
     232,   354,   129,     0,   127,   258,   123,   117,   118,     0,
     116,   119,   120,   121,   122,   126,   202,   187,   447,   149,
     448,     0,   200,   207,   299,   298,     0,     0,   251,     0,
     248,   247,   326,   331,   333,   431,   476,   475,   345,   350,
       0,   487,   311,     0,     0,     0,   310,   494,   310,   498,
     310,   496,   261,     0,   267,   268,   263,   265,   269,     0,
     257,   226,     0,   444,   391,     0,   381,    70,   219,     0,
       0,   271,     0,   287,     0,     0,   241,   240,   239,     0,
      61,     0,   306,   307,   401,   309,   348,     0,     0,     0,
     283,   286,     0,   347,   392,   397,   398,   402,    37,    37,
       0,     0,    97,   228,   302,   353,   233,     0,   260,     0,
       0,   124,   260,   114,    91,     0,     0,   193,   448,     0,
     250,     0,     0,   329,   351,   346,   343,     0,   347,     0,
       0,     0,   264,     0,     0,     0,   400,     0,   376,   425,
     426,     0,   292,   294,     0,    65,    63,   351,   385,    99,
     100,     0,     0,     0,   393,    39,    31,    25,    22,     0,
     448,     0,   108,     0,   230,   232,   202,   301,   128,   259,
       0,   190,   448,   194,     0,   201,   300,     0,   252,     0,
     489,   500,   502,   501,   266,   269,   255,   223,   445,     0,
       0,   349,     0,   394,    31,    38,     0,     0,    26,    23,
       0,   203,     0,   231,   107,   129,   111,   112,   113,     0,
     191,   188,   193,     0,   148,     0,     0,     0,     0,     0,
      40,    32,    35,   448,     0,    92,   260,   115,     0,   189,
       0,   198,   195,     0,   413,     0,   280,   284,   395,     0,
       0,   204,     0,   202,     0,   196,     0,     0,     0,     0,
       0,     0,     0,   107,     0,     0,     0,     0,   245,   242,
     281,     0,     0,   109,    94,   192,   197,     0,     0,   246,
       0,    33,    36,     0,   334,   244,     0,     0,     0,     0,
       0,     0,   243,   110
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     5,     6,    85,   719,   819,   718,   818,   815,
     816,   859,   817,   860,   775,   776,   814,   777,    86,    87,
      88,    89,    90,   125,   161,   254,   170,   224,   255,   428,
     589,   590,   266,   410,   411,   123,    91,    92,   321,   447,
     165,   301,   448,   166,   722,   893,   829,   635,   790,   466,
     467,   730,   468,    93,    94,    95,   196,   323,   507,   508,
     509,   735,   831,   848,   794,   833,   852,   875,   197,   613,
      96,   154,    97,    98,    99,   268,   412,   100,   129,   260,
     261,   681,   463,   464,   617,   618,   155,   156,   422,   423,
     764,   890,   877,   868,   648,   517,   649,   259,   398,   399,
     731,   732,   395,   561,   562,   676,   677,   415,   416,   101,
     168,   304,   306,   857,   711,   771,   102,   603,   276,   277,
     278,   580,   581,   582,   103,   207,   645,   127,   433,   434,
     435,   225,   288,   335,   353,   527,   528,   651,   227,   355,
     238,   228,   437,   372,   438,   620,   229,   230,   240,   284,
     104,   293,   105,   403,   606,   454,   455,   311,   404,   106,
     107,   108,   109,   253,   110,   111,   199,   112,   361,   362,
     113,   296,   297,   298,   160,   684,   124,   263,   568,   114,
     115,   116,   117,   118,   119,   249,   242,   368,   369,   370,
     120,   121,   386,   387,   390,   391,   667,   286
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -808
static const yytype_int16 yypact[] =
{
    -808,   224,  -808,  -808,  -808,    75,  1422,  -808,  -808,    79,
    -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,
    -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,
    -808,   109,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,
    -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,
    -808,  -808,  -808,  -808,  -808,    53,  -808,  -808,  -808,  -808,
    -808,  -808,    72,  -808,  -808,  -808,  -808,    66,   134,   167,
     167,   167,   167,   167,  -808,  -808,   202,   210,  -808,  -808,
    -808,  -808,   157,  -808,   175,  -808,  -808,   141,   169,   770,
     107,  -808,   -13,    40,   -10,  -808,    24,   208,   218,   246,
      28,   247,  -808,   219,  -808,   981,  -808,  -808,  -808,  -808,
     967,  -808,   257,  -808,  -808,  -808,  1614,  -808,   273,  -808,
    1341,   270,  -808,  -808,   350,   109,   109,    53,  -808,  -808,
      32,  -808,   283,  -808,  -808,   284,  -808,   289,   109,   360,
     109,   300,   301,   305,  -808,   109,   307,  1697,   109,  -808,
     967,   368,  -808,  -808,  -808,   306,  -808,   309,  -808,  1208,
    -808,   372,  -808,   373,  -808,   311,   313,  -808,   316,  -808,
    -808,  -808,  -808,  -808,  -808,  -808,  -808,   320,   324,   967,
     325,   386,  -808,  -808,   327,  -808,  -808,  -808,  -808,   109,
    -808,   326,  -808,  -808,  -808,  -808,  -808,  -808,   109,   223,
     109,   109,   289,  -808,  -808,  -808,   109,  -808,  -808,  -808,
    -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,   967,
    -808,  -808,   967,  -808,  -808,  2145,   328,   967,  -808,  -808,
    -808,  1083,   967,   664,  -808,  -808,   334,  1708,  -808,  -808,
     607,   408,  -808,  -808,   664,  -808,  -808,  -808,    33,   332,
      81,  1570,  -808,   341,  -808,  -808,  -808,   343,   109,  -808,
    -808,    23,    10,   360,   967,    68,  -808,   109,  -808,  -808,
    -808,  -808,   -12,   360,   967,   967,  -808,   345,   439,  -808,
     109,  1708,  -808,   607,  -808,   346,  -808,  2145,  -808,  -808,
     338,    53,   256,   314,   967,  2145,   349,  -808,     0,  -808,
     347,    37,  -808,   351,   374,   356,   421,  -808,  -808,  -808,
     109,  -808,   256,  1722,   256,   361,   109,   109,   343,  -808,
     109,  -808,  -808,  2161,  1581,  -808,   362,  -808,  -808,  -808,
    -808,  -808,   358,  1745,  1301,   439,   256,   967,   967,   967,
     967,   967,   967,   967,   967,  -808,  -808,  -808,  -808,  -808,
    -808,  -808,  -808,  -808,   967,   967,   967,    57,  -808,   360,
    2145,    94,  -808,  -808,  -808,  -808,  -808,   432,   135,  -808,
    1119,   256,  -808,   359,  1708,  1708,  1708,  1708,  1708,  -808,
     443,  -808,    56,    56,    56,  -808,   366,  -808,  1570,    38,
    -808,   379,   380,   109,  -808,  -808,  -808,    23,  -808,   106,
    -808,   385,   387,  -808,  -808,  -808,  1769,  -808,  -808,  -808,
     155,  -808,   160,  -808,   388,   161,  -808,  1792,  1816,   447,
    -808,   389,   182,  -808,   462,  -808,  -808,    29,  -808,  -808,
    -808,   390,   392,   394,  -808,   396,  1839,  -808,   395,   401,
    -808,   967,  -808,  1208,  -808,   402,   403,  -808,    37,   468,
    -808,  -808,  -808,   407,  -808,   409,   410,   967,   413,  -808,
     284,   284,   412,   104,  1718,  -808,   121,   269,   343,   416,
     419,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,
    -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,
    -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,
    -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,   422,
     967,  1225,  2145,   109,   109,  -808,   423,  1592,   424,    57,
      57,    57,   288,  1301,   775,   483,   483,   967,   967,   103,
    2168,  2145,  -808,  -808,  1255,    47,  -808,   664,  -808,  -808,
    2145,   426,   430,   359,   359,   359,   162,   231,  -808,   366,
     420,   366,    56,  1863,   431,   442,   444,  1570,  1570,  1570,
    -808,   200,  1169,   451,    23,  -808,   967,   967,   362,   440,
    -808,    17,  -808,   109,   360,   362,   360,   360,  -808,  -808,
     225,  -808,   452,   628,  -808,   109,  -808,   445,  -808,   229,
    -808,   967,   967,   256,   653,   967,   967,  -808,  -808,   520,
     525,  -808,  -808,   463,   967,   967,   473,  -808,  1886,  -808,
    -808,  -808,  -808,   474,  1718,  -808,   475,  -808,  -808,   977,
     469,  -808,  -808,   476,  -808,  -808,  -808,  -808,  -808,   480,
    -808,  -808,  -808,  -808,  -808,  -808,   412,  -808,  -808,  -808,
    2145,   481,  -808,  -808,  -808,   493,   482,   967,  -808,    64,
    2145,  -808,  -808,  -808,  -808,  -808,   607,  -808,   484,  -808,
      56,  -808,    44,  1570,  1570,  1570,    48,  -808,    50,  -808,
      51,  -808,  -808,  1169,  -808,  -808,  -808,  -808,  1910,    23,
    -808,  -808,  1933,  2145,   495,    10,  -808,  -808,  -808,   244,
     967,  -808,   504,  -808,   447,   447,  -808,  2145,  -808,   569,
    -808,   571,  2145,  2145,  -808,  -808,   510,  1957,   513,   514,
    -808,  -808,  1980,   407,  2004,  -808,  -808,  -808,   589,   589,
    1603,   935,  -808,  -808,   256,  -808,  -808,  1718,   343,   109,
     269,  -808,   343,  -808,  -808,  2236,   967,   521,  -808,   109,
    -808,  1660,  1592,  -808,   967,  -808,   258,   529,   387,   530,
     532,   533,  -808,  1423,   535,   118,  -808,   967,  -808,  -808,
    2145,   360,  -808,  -808,   526,  -808,  -808,   967,  -808,  -808,
    -808,   429,   967,   967,  -808,   612,   470,  -808,  -808,  1559,
    2145,   547,  2145,   549,  -808,  -808,   412,  -808,  -808,  -808,
     222,   548,  2145,  -808,   554,  -808,  -808,   109,  -808,   430,
    -808,  -808,  -808,  -808,  -808,  2145,  -808,  -808,  2145,   550,
     967,  -808,   567,  2027,   470,  -808,   534,   501,  -808,  -808,
     572,  -808,   967,  -808,   474,  -808,  -808,  -808,  -808,   573,
    -808,  -808,   521,  2200,  -808,   574,   360,  2051,   637,   967,
    -808,  -808,  -808,  -808,  2074,  -808,   343,  -808,   508,  -808,
     578,  -808,  -808,   967,  -808,   546,  -808,   579,  2145,   582,
     594,  -808,   546,   412,   605,  -808,  2098,   967,   604,   669,
     670,   109,   615,   474,   967,  2236,   967,    85,  2145,  -808,
    -808,   610,   620,  -808,  -808,  2145,  -808,  2121,   967,  -808,
     570,  -808,  -808,   575,  -808,  2145,   622,   624,   546,   546,
     630,   632,  -808,  -808
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,  -109,
    -808,  -808,  -808,  -808,  -808,  -808,  -808,    -6,   626,  -808,
    -808,  -808,  -808,   -15,  -808,   -26,   203,   -31,   568,  -808,
    -808,    16,  -138,  -808,   147,  -808,  -808,  -808,  -808,   271,
    -808,  -808,  -808,  -808,  -726,  -808,  -808,    -9,  -808,  -808,
    -808,  -808,  -605,   -74,  -808,  -808,  -808,  -808,  -808,  -712,
    -808,  -808,  -808,  -808,  -110,  -808,  -808,  -808,  -808,  -620,
    -808,   -97,  -808,  -808,  -808,   523,  -808,  -808,  -808,   519,
     536,  -808,    52,  -808,   117,     7,  -808,   446,  -808,   153,
    -808,  -808,  -808,  -807,    -1,  -808,  -808,  -188,  -545,   352,
    -808,  -808,  -312,  -808,  -808,    69,    -2,   170,  -137,  -808,
    -808,  -808,  -808,  -808,  -808,  -808,  -808,  -808,   538,  -808,
    -808,  -808,    59,    60,  -808,  -808,  -808,   -29,  -292,   165,
    -808,   553,  -101,  -808,  -808,  -808,  -808,  -808,    74,  -808,
    -174,  -808,  -364,    96,  -142,  -591,   -82,  -808,  -122,    91,
    -808,  -808,  -808,  -382,    49,   156,  -808,  -808,    77,  -808,
    -808,  -808,   660,  -808,  -808,  -808,   -87,  -808,  -808,   233,
    -808,  -273,   330,  -808,  -808,  -808,    22,  -194,  -480,  -808,
    -808,  -808,  -808,  -808,  -808,  -808,   236,  -808,   232,  -808,
    -808,  -808,   -42,  -523,  -808,   382,  -239,   -66
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -500
static const yytype_int16 yytable[] =
{
     136,   324,   271,   137,   138,   135,   462,   542,   289,   226,
     325,   144,   392,   204,   145,   186,   734,   728,   146,   680,
     456,   440,   458,   791,   153,   283,   134,   -75,   726,   661,
     258,   -79,   642,   134,   239,   148,  -470,   126,   239,   134,
    -447,  -490,   128,   262,   518,   587,   130,  -499,   131,   132,
    -472,  -493,   133,  -497,  -495,   872,   134,   139,   140,   141,
     142,   143,   193,  -472,  -427,   239,   194,   189,   203,   134,
     396,   147,   134,   149,   195,   150,   385,   442,     7,   541,
     443,   157,   162,   158,   134,   236,   122,   400,   190,   236,
     363,   900,   901,   413,   136,   256,   257,   134,   845,   397,
     177,   408,   326,   178,   198,   180,   588,   269,   203,   272,
     401,   367,   203,  -470,   279,   373,   236,   136,   557,   191,
     192,   851,   380,  -472,  -499,   134,   405,  -472,  -493,   510,
    -497,  -495,   383,   336,   680,   232,   785,   661,   151,   243,
     152,   246,   163,   339,   742,   407,   164,   884,   159,   556,
     389,   239,   408,   187,   445,   239,   636,   383,   318,   424,
     737,   384,   239,   886,   322,   888,   824,   257,   597,   327,
     328,   533,   316,   317,   534,   332,   743,   379,   188,   336,
     381,   446,   351,   169,   614,   414,   564,   337,   338,   339,
     237,   565,   307,   308,   237,   309,   352,   889,   614,   239,
     623,   624,   236,   807,   420,   615,   236,   241,   385,   385,
     385,   248,   536,   236,   167,   537,   425,   426,   175,   236,
     846,   237,   532,   400,     2,     3,   176,   257,   351,     4,
     236,   402,   570,   179,   409,   571,   236,   572,   575,   513,
     573,   576,   352,   873,   220,   221,   374,   375,   376,   421,
     236,   181,   543,   544,   545,   546,   547,   182,   795,   584,
     153,   516,   585,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   134,   171,   172,   173,   174,   672,   747,   453,
     673,   749,   750,   751,   183,   136,   136,   554,   200,   465,
     460,   461,   239,   239,   239,   239,   239,   754,   201,   354,
     821,   704,   693,   625,   206,   694,   700,   237,   151,   701,
     152,   237,   832,   626,   378,   374,   375,   376,   237,   669,
     671,   759,   610,   611,   576,   643,   202,   205,   627,   826,
     827,   828,   219,   231,   371,  -351,   567,   628,   220,   221,
     549,   550,   551,   236,   236,   236,   236,   236,   222,   244,
     251,   236,   236,   236,   245,   237,   250,   378,   252,   264,
     265,   354,   560,   861,   336,   267,   236,   223,   270,   354,
     220,   221,   337,   338,   339,   629,   273,   274,   385,   630,
     799,   275,   621,   280,   290,   292,   291,   354,   299,   300,
     400,   302,   303,   431,   432,   305,   310,   631,   632,   688,
     312,   314,   315,   163,   633,   320,   356,   354,   354,   634,
     371,  -451,   382,   351,   393,   656,   786,   439,   720,   394,
     789,   419,   427,   429,   444,   669,   671,   352,   441,   450,
     449,   354,   783,   616,   354,   451,   622,   452,   459,   691,
     692,   378,   203,   514,   736,   376,   552,   378,   237,   237,
     237,   237,   237,   706,   378,   239,   666,   668,   670,   558,
     559,   566,    12,   579,   574,   567,    16,   583,   591,    19,
     592,   593,    22,    23,   595,    25,   594,   596,   599,   600,
     354,   685,   257,   644,   602,   604,   385,   607,   690,   605,
     609,   354,   354,    42,   637,    44,   612,   638,   378,   639,
     660,   652,   646,   658,    52,   400,   236,   659,    56,   535,
     354,   663,   686,    58,   220,   221,   374,   375,   376,   377,
     548,   236,   664,   699,   665,   220,   221,   374,   375,   376,
     377,   679,   621,   236,   863,   695,   708,   621,   619,   586,
     409,   709,   236,   710,   220,   221,   374,   375,   376,   377,
     716,   724,   721,   727,   421,   729,   733,   184,   738,   336,
     744,   740,   666,   668,   670,   220,   221,   337,   338,   339,
     340,   341,   342,   739,   713,   757,   345,   346,   347,   348,
     349,   350,    84,   616,   761,   765,   354,   587,   725,   767,
     769,   770,   -41,   354,   354,   354,   354,   354,   354,   354,
     354,   793,   810,   354,   354,   354,   800,   801,   351,   802,
     803,   237,   806,   812,   354,   -42,   -34,   378,   378,   378,
     378,   378,   352,   822,   809,   811,   823,   354,   830,   746,
     836,   834,   748,   748,   748,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   134,   621,   838,   842,   746,   843,
     847,   841,   853,   856,   402,   864,   865,   867,   870,   869,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   134,
     871,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     134,   364,   354,   874,   879,   880,   881,   891,   619,   220,
     221,   374,   375,   376,   377,   883,   616,   892,   787,   854,
     898,   896,   899,   287,   219,   840,   897,   902,   796,   903,
     220,   221,   295,   778,   354,   185,   285,   766,   687,   601,
     222,   788,   849,   330,   354,   329,   319,   354,   354,   219,
     378,   723,   313,   755,   784,   220,   221,   430,   698,   223,
     281,   798,   752,   331,   689,   222,   220,   221,   365,   563,
     366,   804,   354,   762,   745,   763,   354,   354,   825,   705,
     696,   715,   758,   774,   223,   209,   835,   655,     0,   657,
     555,   354,   333,   598,     0,   334,   354,   354,     0,     0,
     357,   354,     0,     0,   360,   287,   354,     0,   354,     0,
     431,   432,     0,    12,     0,     0,     0,    16,     0,     0,
      19,   619,     0,    22,    23,     0,    25,     0,     0,     0,
       0,     0,     0,     0,     0,   354,     0,   406,     0,     0,
       0,     0,     0,     0,    42,     0,    44,   417,   418,     0,
       0,     0,     0,     0,   354,    52,     0,     0,     0,    56,
     882,     0,     0,     0,    58,   436,     0,   295,     0,     0,
       0,   336,     0,     0,   354,     0,   354,   220,   221,   337,
     338,   339,   340,   341,     0,   436,   354,   436,   345,   346,
     347,   348,   349,   350,     0,     0,     0,   512,     0,   354,
       0,     0,   354,     0,     0,   -46,   -44,   354,   184,   436,
     519,   520,   521,   522,   523,   524,   525,   526,     0,     0,
     351,     0,     0,     0,     0,     0,     0,   529,   530,   531,
       0,   354,     0,    84,   352,     0,     0,     0,   354,     0,
       0,     0,     0,   540,   436,     0,     0,     0,     0,     0,
       0,     0,   354,     0,     0,     0,     0,     0,     0,     0,
     354,   553,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   134,   354,     0,     0,     0,     0,     0,     0,   354,
       0,   354,     0,     0,     0,     0,     0,     0,     0,   354,
       0,     0,     0,     0,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   134,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   134,   295,     0,   295,     0,     0,     0,
       0,     0,    10,    11,     0,    13,     0,    15,     0,    17,
     608,   219,    20,     0,     0,     0,     0,   220,   221,     0,
       0,    29,     0,     0,     0,     0,     0,   222,    35,    36,
      37,    38,     0,     0,    41,     0,     0,     0,     0,    46,
       0,    48,    49,   219,     0,    51,   223,    53,    54,   220,
     221,    57,     0,   208,     0,     0,    59,     0,     0,   222,
       0,     0,     0,   640,   333,     0,     0,   781,     0,     0,
     650,     0,     0,     0,     0,     0,     0,     0,   223,     0,
     653,   654,     0,     0,     0,     0,     0,   360,     0,     0,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   134,
       0,     0,     0,     0,     0,     0,     0,    74,    75,     0,
       0,     0,     0,     0,     0,   678,     0,    81,     0,   682,
     683,     0,    82,    83,     0,     0,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   134,   697,     0,     0,     0,
       0,     0,     0,     0,   702,   703,   436,   436,   287,   707,
       0,     0,     0,     0,     0,     0,     0,   712,   714,   219,
     358,     0,     0,     0,     0,   220,   221,   359,     0,     0,
       0,     0,     0,     0,     0,   222,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   134,     0,     0,     0,     0,
       0,     0,     0,     0,   223,   219,     0,     0,     0,     0,
     741,   220,   221,   538,     0,   539,     0,     0,     0,     0,
       0,   222,     0,     0,     0,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   134,     0,   678,     0,     0,     0,
     223,     0,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   134,     0,   760,     0,   219,     0,     0,   674,     0,
       0,   220,   221,   675,     0,     0,     0,     0,     0,     0,
       0,   222,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   134,     0,   780,   782,     0,     0,   436,     0,     0,
     223,     0,     0,     0,   219,     0,     0,   294,     0,   792,
     220,   221,     0,     0,     0,   650,     0,   287,     0,     0,
     222,   219,     0,     0,     0,     0,   805,   220,   221,   641,
     808,     0,     0,     0,     0,     0,     0,   222,     0,   223,
     287,     0,     0,     0,     0,   813,   714,     0,     0,     0,
       0,   219,   333,     0,     0,     0,   223,   220,   221,   359,
       0,     0,     0,     0,     0,     0,     0,   222,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   134,     0,     0,
       0,     0,     0,   837,     0,     0,   223,     0,     0,     0,
       0,     0,     0,     0,     0,   844,     0,   336,     0,     0,
       0,     0,     0,   220,   221,   337,   338,   339,   340,     0,
       0,     0,   858,     0,   345,   346,   347,   348,   349,   350,
       0,     0,     0,     0,     0,     0,   866,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   233,     0,     0,
     878,     0,     0,   220,   221,   247,   351,   885,     8,   887,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   134,
     352,   895,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,     0,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,     0,    51,    52,    53,    54,
      55,    56,    57,     0,     0,     0,    58,    59,     0,   219,
       0,     0,     0,     0,     0,   220,   221,   675,     0,     0,
       0,     0,     0,     0,     0,   222,     0,     0,     0,     0,
       0,    60,     0,     0,    61,    62,    63,    64,     0,     0,
       0,     0,     0,     0,   223,    65,    66,   -46,   -44,     0,
      67,    68,    69,    70,    71,    72,    73,     0,    74,    75,
      76,    77,    78,     0,     0,    79,    80,     0,    81,     0,
       0,     0,     0,    82,    83,    84,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   134,     0,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   134,     0,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   134,     0,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   134,     0,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   134,
       0,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     134,     0,     0,     0,     0,   219,     0,     0,     0,     0,
       0,   220,   221,   820,     0,     0,   388,     0,     0,     0,
       0,   222,   220,   221,     0,     0,     0,   511,     0,     0,
       0,     0,   222,   220,   221,     0,     0,     0,   647,     0,
     223,     0,     0,   222,   220,   221,     0,     0,     0,   779,
       0,   223,     0,     0,   222,   220,   221,     0,     0,     0,
     233,     0,   223,     0,     0,   222,   220,   221,   234,     0,
     235,     0,     0,   223,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   134,   223,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   134,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   134,     0,   336,   515,     0,     0,
     797,     0,   220,   221,   337,   338,   339,   340,   341,   342,
     343,   344,     0,   345,   346,   347,   348,   349,   350,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   281,     0,     0,     0,     0,     0,   220,
     221,   282,     0,     0,   281,   351,     0,     0,     0,     0,
     220,   221,     0,     0,     0,     0,     0,     0,   336,   352,
     220,   221,   457,     0,   220,   221,   337,   338,   339,   340,
     341,   342,   343,   344,     0,   345,   346,   347,   348,   349,
     350,   336,   515,     0,     0,     0,     0,   220,   221,   337,
     338,   339,   340,   341,   342,   343,   344,     0,   345,   346,
     347,   348,   349,   350,     0,   336,   569,   351,     0,     0,
       0,   220,   221,   337,   338,   339,   340,   341,   342,   343,
     344,   352,   345,   346,   347,   348,   349,   350,   336,   577,
     351,     0,     0,     0,   220,   221,   337,   338,   339,   340,
     341,   342,   343,   344,   352,   345,   346,   347,   348,   349,
     350,     0,   336,   578,   351,     0,     0,     0,   220,   221,
     337,   338,   339,   340,   341,   342,   343,   344,   352,   345,
     346,   347,   348,   349,   350,   336,     0,   351,  -352,     0,
       0,   220,   221,   337,   338,   339,   340,   341,   342,   343,
     344,   352,   345,   346,   347,   348,   349,   350,     0,   336,
     662,   351,     0,     0,     0,   220,   221,   337,   338,   339,
     340,   341,   342,   343,   344,   352,   345,   346,   347,   348,
     349,   350,   336,   717,   351,     0,     0,     0,   220,   221,
     337,   338,   339,   340,   341,   342,   343,   344,   352,   345,
     346,   347,   348,   349,   350,     0,   336,     0,   351,   753,
       0,     0,   220,   221,   337,   338,   339,   340,   341,   342,
     343,   344,   352,   345,   346,   347,   348,   349,   350,   336,
     756,   351,     0,     0,     0,   220,   221,   337,   338,   339,
     340,   341,   342,   343,   344,   352,   345,   346,   347,   348,
     349,   350,     0,   336,   768,   351,     0,     0,     0,   220,
     221,   337,   338,   339,   340,   341,   342,   343,   344,   352,
     345,   346,   347,   348,   349,   350,   336,     0,   351,   772,
       0,     0,   220,   221,   337,   338,   339,   340,   341,   342,
     343,   344,   352,   345,   346,   347,   348,   349,   350,     0,
     336,     0,   351,     0,   773,     0,   220,   221,   337,   338,
     339,   340,   341,   342,   343,   344,   352,   345,   346,   347,
     348,   349,   350,   336,     0,   351,   839,     0,     0,   220,
     221,   337,   338,   339,   340,   341,   342,   343,   344,   352,
     345,   346,   347,   348,   349,   350,     0,   336,     0,   351,
       0,   855,     0,   220,   221,   337,   338,   339,   340,   341,
     342,   343,   344,   352,   345,   346,   347,   348,   349,   350,
     336,     0,   351,     0,   862,     0,   220,   221,   337,   338,
     339,   340,   341,   342,   343,   344,   352,   345,   346,   347,
     348,   349,   350,     0,   336,     0,   351,     0,   876,     0,
     220,   221,   337,   338,   339,   340,   341,   342,   343,   344,
     352,   345,   346,   347,   348,   349,   350,   336,   894,   351,
       0,     0,     0,   220,   221,   337,   338,   339,   340,   341,
     342,   343,   344,   352,   345,   346,   347,   348,   349,   350,
       0,   336,     0,   351,     0,     0,     0,   220,   221,   337,
     338,   339,   340,   341,   342,   343,   344,   352,   345,   346,
     347,   348,   349,   350,   336,     0,   351,     0,     0,     0,
     220,   221,   337,   338,   339,   340,     0,     0,     0,     0,
     352,  -500,  -500,  -500,  -500,  -500,  -500,     0,     0,     0,
     351,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   352,     0,     0,     0,     0,     0,
       0,     0,     0,   351,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   469,   352,   470,   471,
     472,   473,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,   496,   497,   498,   499,   500,   501,
     502,   503,   504,   505,   506,   850,     0,     0,   471,   472,
     473,   474,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   497,   498,   499,   500,   501,   502,
     503,   504,   505,   506,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506
};

static const yytype_int16 yycheck[] =
{
      31,   195,   139,    32,    33,    31,   318,   371,   150,   110,
     198,    40,   251,   100,    43,    89,   636,   622,    47,   564,
     312,   294,   314,   735,    55,   147,    16,     3,   619,   552,
     127,     3,   512,    16,   116,    50,     3,    15,   120,    16,
       8,     3,    20,   130,   336,    16,    24,     3,    26,    27,
       3,     3,    30,     3,     3,   862,    16,    35,    36,    37,
      38,    39,    93,    16,    76,   147,    76,    80,    80,    16,
     258,    49,    16,    51,    84,    53,   250,    77,     3,   371,
      80,    59,    16,    61,    16,   116,     7,   261,   101,   120,
     232,   898,   899,   267,   125,   126,   127,    16,   824,    76,
      78,    84,   199,    81,    80,    83,    77,   138,    80,   140,
     100,   233,    80,    80,   145,   237,   147,   148,    80,    79,
      80,   833,   244,    76,    80,    16,   263,    80,    80,   323,
      80,    80,    76,    76,   679,   113,   727,   660,    85,   117,
      87,   119,    76,    86,    80,    77,    80,   873,    76,   388,
     251,   233,    84,    46,   117,   237,   468,    76,   189,   281,
     640,    80,   244,   875,   193,    80,   786,   198,   441,   200,
     201,    77,   187,   188,    80,   206,   112,   243,    71,    76,
     246,   144,   125,    16,    80,   272,    80,    84,    85,    86,
     116,    85,   170,   171,   120,   173,   139,   112,    80,   281,
      79,    80,   233,    85,   278,   101,   237,   116,   382,   383,
     384,   120,    77,   244,    80,    80,   282,   283,    16,   250,
     825,   147,   359,   397,     0,     1,    16,   258,   125,     5,
     261,   262,    77,    76,   265,    80,   267,    77,    77,   326,
      80,    80,   139,   863,    82,    83,    84,    85,    86,   280,
     281,    76,   374,   375,   376,   377,   378,   116,   738,    77,
     291,   335,    80,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    70,    71,    72,    73,    77,   660,   310,
      80,   663,   664,   665,   115,   316,   317,   388,    80,   320,
     316,   317,   374,   375,   376,   377,   378,   679,    80,   225,
     780,   593,    77,    34,    85,    80,    77,   233,    85,    80,
      87,   237,   792,    44,   240,    84,    85,    86,   244,   558,
     559,    77,   460,   461,    80,   513,    80,    80,    59,   107,
     108,   109,    76,    76,    76,    79,    78,    68,    82,    83,
     382,   383,   384,   374,   375,   376,   377,   378,    92,    76,
      80,   382,   383,   384,   118,   281,   120,   283,     8,    76,
      76,   287,   393,   843,    76,    76,   397,   111,     8,   295,
      82,    83,    84,    85,    86,   106,    76,    76,   552,   110,
     744,    76,   464,    76,    16,    76,    80,   313,    16,    16,
     564,    80,    79,   137,   138,    79,    76,   128,   129,   573,
      76,    76,    16,    76,   135,    79,    78,   333,   334,   140,
      76,     3,    80,   125,    73,   537,   728,   103,   612,    76,
     732,    76,    76,    85,    77,   664,   665,   139,    79,    55,
      79,   357,   724,   464,   360,    79,   465,    16,    77,   576,
     577,   367,    80,    85,   638,    86,    80,   373,   374,   375,
     376,   377,   378,   595,   380,   537,   557,   558,   559,    80,
      80,    76,    23,    16,    76,    78,    27,    78,    78,    30,
      78,    77,    33,    34,    79,    36,    80,    76,    76,    76,
     406,   568,   513,   514,    16,    78,   660,    77,   575,    80,
      77,   417,   418,    54,    78,    56,    84,    78,   424,    77,
      80,    77,    79,    77,    65,   679,   537,    77,    69,    77,
     436,    80,    72,    74,    82,    83,    84,    85,    86,    87,
      77,   552,    80,    78,    80,    82,    83,    84,    85,    86,
      87,    80,   614,   564,   846,    83,    16,   619,   464,    77,
     571,    16,   573,    80,    82,    83,    84,    85,    86,    87,
      77,    76,    78,    84,   585,    79,    76,   118,    77,    76,
      76,    79,   663,   664,   665,    82,    83,    84,    85,    86,
      87,    88,    89,    80,   605,    80,    93,    94,    95,    96,
      97,    98,   143,   614,    80,    16,   512,    16,   619,    79,
      77,    77,     3,   519,   520,   521,   522,   523,   524,   525,
     526,    80,    76,   529,   530,   531,    77,    77,   125,    77,
      77,   537,    77,   184,   540,     3,   146,   543,   544,   545,
     546,   547,   139,    76,   761,   767,    77,   553,    80,   660,
      80,    77,   663,   664,   665,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,   727,    79,   146,   679,    77,
      77,   117,    78,    16,   685,   147,    78,   111,    76,    80,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      76,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,   608,    78,    80,    16,    16,    77,   614,    82,
      83,    84,    85,    86,    87,    80,   727,    77,   729,   836,
      78,   131,    78,   150,    76,   814,   131,    77,   739,    77,
      82,    83,   159,   719,   640,    89,   148,   701,   571,   448,
      92,   730,   832,   204,   650,   202,   190,   653,   654,    76,
     656,   614,   179,   681,   727,    82,    83,   291,   585,   111,
      76,   742,   673,   205,   574,    92,    82,    83,    84,   397,
      86,   753,   678,   694,   658,   695,   682,   683,   787,   594,
     132,   605,   685,   714,   111,   105,   797,   534,    -1,   537,
     388,   697,   219,   443,    -1,   222,   702,   703,    -1,    -1,
     227,   707,    -1,    -1,   231,   232,   712,    -1,   714,    -1,
     137,   138,    -1,    23,    -1,    -1,    -1,    27,    -1,    -1,
      30,   727,    -1,    33,    34,    -1,    36,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   741,    -1,   264,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    -1,    56,   274,   275,    -1,
      -1,    -1,    -1,    -1,   760,    65,    -1,    -1,    -1,    69,
     871,    -1,    -1,    -1,    74,   292,    -1,   294,    -1,    -1,
      -1,    76,    -1,    -1,   780,    -1,   782,    82,    83,    84,
      85,    86,    87,    88,    -1,   312,   792,   314,    93,    94,
      95,    96,    97,    98,    -1,    -1,    -1,   324,    -1,   805,
      -1,    -1,   808,    -1,    -1,   115,   116,   813,   118,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,   354,   355,   356,
      -1,   837,    -1,   143,   139,    -1,    -1,    -1,   844,    -1,
      -1,    -1,    -1,   370,   371,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   858,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     866,   388,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,   878,    -1,    -1,    -1,    -1,    -1,    -1,   885,
      -1,   887,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   895,
      -1,    -1,    -1,    -1,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,   441,    -1,   443,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    24,    -1,    26,    -1,    28,
     457,    76,    31,    -1,    -1,    -1,    -1,    82,    83,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    92,    47,    48,
      49,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    58,
      -1,    60,    61,    76,    -1,    64,   111,    66,    67,    82,
      83,    70,    -1,    72,    -1,    -1,    75,    -1,    -1,    92,
      -1,    -1,    -1,   510,   511,    -1,    -1,   132,    -1,    -1,
     517,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
     527,   528,    -1,    -1,    -1,    -1,    -1,   534,    -1,    -1,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,    -1,
      -1,    -1,    -1,    -1,    -1,   562,    -1,   136,    -1,   566,
     567,    -1,   141,   142,    -1,    -1,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,   583,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   591,   592,   593,   594,   595,   596,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   604,   605,    76,
      77,    -1,    -1,    -1,    -1,    82,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,    76,    -1,    -1,    -1,    -1,
     647,    82,    83,    84,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    -1,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    -1,   673,    -1,    -1,    -1,
     111,    -1,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    -1,   690,    -1,    76,    -1,    -1,    79,    -1,
      -1,    82,    83,    84,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    -1,   720,   721,    -1,    -1,   724,    -1,    -1,
     111,    -1,    -1,    -1,    76,    -1,    -1,    79,    -1,   736,
      82,    83,    -1,    -1,    -1,   742,    -1,   744,    -1,    -1,
      92,    76,    -1,    -1,    -1,    -1,   753,    82,    83,    84,
     757,    -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,   111,
     767,    -1,    -1,    -1,    -1,   772,   773,    -1,    -1,    -1,
      -1,    76,   779,    -1,    -1,    -1,   111,    82,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    -1,    -1,
      -1,    -1,    -1,   810,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   822,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    82,    83,    84,    85,    86,    87,    -1,
      -1,    -1,   839,    -1,    93,    94,    95,    96,    97,    98,
      -1,    -1,    -1,    -1,    -1,    -1,   853,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    -1,
     867,    -1,    -1,    82,    83,    84,   125,   874,     6,   876,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
     139,   888,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    64,    65,    66,    67,
      68,    69,    70,    -1,    -1,    -1,    74,    75,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    82,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    99,    -1,    -1,   102,   103,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   113,   114,   115,   116,    -1,
     118,   119,   120,   121,   122,   123,   124,    -1,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,    -1,   136,    -1,
      -1,    -1,    -1,   141,   142,   143,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    -1,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    -1,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    -1,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    -1,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      -1,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    82,    83,    84,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    92,    82,    83,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    92,    82,    83,    -1,    -1,    -1,    76,    -1,
     111,    -1,    -1,    92,    82,    83,    -1,    -1,    -1,    76,
      -1,   111,    -1,    -1,    92,    82,    83,    -1,    -1,    -1,
      76,    -1,   111,    -1,    -1,    92,    82,    83,    84,    -1,
      86,    -1,    -1,   111,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,   111,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    -1,    76,    77,    -1,    -1,
      80,    -1,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    -1,    93,    94,    95,    96,    97,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    82,
      83,    84,    -1,    -1,    76,   125,    -1,    -1,    -1,    -1,
      82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    76,   139,
      82,    83,    80,    -1,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    -1,    93,    94,    95,    96,    97,
      98,    76,    77,    -1,    -1,    -1,    -1,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    -1,    93,    94,
      95,    96,    97,    98,    -1,    76,    77,   125,    -1,    -1,
      -1,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,   139,    93,    94,    95,    96,    97,    98,    76,    77,
     125,    -1,    -1,    -1,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,   139,    93,    94,    95,    96,    97,
      98,    -1,    76,    77,   125,    -1,    -1,    -1,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,   139,    93,
      94,    95,    96,    97,    98,    76,    -1,   125,    79,    -1,
      -1,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,   139,    93,    94,    95,    96,    97,    98,    -1,    76,
      77,   125,    -1,    -1,    -1,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,   139,    93,    94,    95,    96,
      97,    98,    76,    77,   125,    -1,    -1,    -1,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,   139,    93,
      94,    95,    96,    97,    98,    -1,    76,    -1,   125,    79,
      -1,    -1,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,   139,    93,    94,    95,    96,    97,    98,    76,
      77,   125,    -1,    -1,    -1,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,   139,    93,    94,    95,    96,
      97,    98,    -1,    76,    77,   125,    -1,    -1,    -1,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,   139,
      93,    94,    95,    96,    97,    98,    76,    -1,   125,    79,
      -1,    -1,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,   139,    93,    94,    95,    96,    97,    98,    -1,
      76,    -1,   125,    -1,    80,    -1,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,   139,    93,    94,    95,
      96,    97,    98,    76,    -1,   125,    79,    -1,    -1,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,   139,
      93,    94,    95,    96,    97,    98,    -1,    76,    -1,   125,
      -1,    80,    -1,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,   139,    93,    94,    95,    96,    97,    98,
      76,    -1,   125,    -1,    80,    -1,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,   139,    93,    94,    95,
      96,    97,    98,    -1,    76,    -1,   125,    -1,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
     139,    93,    94,    95,    96,    97,    98,    76,    77,   125,
      -1,    -1,    -1,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,   139,    93,    94,    95,    96,    97,    98,
      -1,    76,    -1,   125,    -1,    -1,    -1,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,   139,    93,    94,
      95,    96,    97,    98,    76,    -1,   125,    -1,    -1,    -1,
      82,    83,    84,    85,    86,    87,    -1,    -1,    -1,    -1,
     139,    93,    94,    95,    96,    97,    98,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   145,   139,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   145,    -1,    -1,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   187,     0,     1,     5,   188,   189,     3,     6,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    64,    65,    66,    67,    68,    69,    70,    74,    75,
      99,   102,   103,   104,   105,   113,   114,   118,   119,   120,
     121,   122,   123,   124,   126,   127,   128,   129,   130,   133,
     134,   136,   141,   142,   143,   190,   204,   205,   206,   207,
     208,   222,   223,   239,   240,   241,   256,   258,   259,   260,
     263,   295,   302,   310,   336,   338,   345,   346,   347,   348,
     350,   351,   353,   356,   365,   366,   367,   368,   369,   370,
     376,   377,     7,   221,   362,   209,   362,   313,   362,   264,
     362,   362,   362,   362,    16,   211,   213,   313,   313,   362,
     362,   362,   362,   362,   313,   313,   313,   362,   209,   362,
     362,    85,    87,   213,   257,   272,   273,   362,   362,    76,
     360,   210,    16,    76,    80,   226,   229,    80,   296,    16,
     212,   212,   212,   212,   212,    16,    16,   362,   362,    76,
     362,    76,   116,   115,   118,   204,   239,    46,    71,    80,
     101,    79,    80,   213,    76,    84,   242,   254,    80,   352,
      80,    80,    80,    80,   352,    80,    85,   311,    72,   348,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    76,
      82,    83,    92,   111,   213,   317,   318,   324,   327,   332,
     333,    76,   362,    76,    84,    86,   213,   324,   326,   332,
     334,   335,   372,   362,    76,   372,   362,    84,   335,   371,
     372,    80,     8,   349,   211,   214,   213,   213,   257,   283,
     265,   266,   352,   363,    76,    76,   218,    76,   261,   213,
       8,   294,   213,    76,    76,    76,   304,   305,   306,   213,
      76,    76,    84,   334,   335,   214,   383,   317,   318,   330,
      16,    80,    76,   337,    79,   317,   357,   358,   359,    16,
      16,   227,    80,    79,   297,    79,   298,   362,   362,   362,
      76,   343,    76,   317,    76,    16,   209,   209,   213,   266,
      79,   224,   313,   243,   363,   283,   257,   213,   213,   261,
     265,   304,   213,   317,   317,   319,    76,    84,    85,    86,
      87,    88,    89,    90,    91,    93,    94,    95,    96,    97,
      98,   125,   139,   320,   324,   325,    78,   317,    77,    84,
     317,   354,   355,   330,    17,    84,    86,   334,   373,   374,
     375,    76,   329,   334,    84,    85,    86,    87,   324,   383,
     334,   383,    80,    76,    80,   326,   378,   379,    76,   318,
     380,   381,   382,    73,    76,   288,   283,    76,   284,   285,
     326,   100,   213,   339,   344,   294,   317,    77,    84,   213,
     219,   220,   262,   326,   352,   293,   294,   317,   317,    76,
     239,   213,   274,   275,   334,   383,   383,    76,   215,    85,
     273,   137,   138,   314,   315,   316,   317,   328,   330,   103,
     357,    79,    77,    80,    77,   117,   144,   225,   228,    79,
      55,    79,    16,   213,   341,   342,   314,    80,   314,    77,
     211,   211,   288,   268,   269,   213,   235,   236,   238,   145,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   244,   245,   246,
     363,    76,   317,   352,    85,    77,   239,   281,   314,   317,
     317,   317,   317,   317,   317,   317,   317,   321,   322,   317,
     317,   317,   294,    77,    80,    77,    77,    80,    84,    86,
     317,   314,   328,   334,   334,   334,   334,   334,    77,   378,
     378,   378,    80,   317,   318,   381,   382,    80,    80,    80,
     213,   289,   290,   285,    80,    85,    76,    78,   364,    77,
      77,    80,    77,    80,    76,    77,    80,    77,    77,    16,
     307,   308,   309,    78,    77,    80,    77,    16,    77,   216,
     217,    78,    78,    77,    80,    79,    76,   357,   358,    76,
      76,   225,    16,   303,    78,    80,   340,    77,   317,    77,
     218,   218,    84,   255,    80,   101,   213,   270,   271,   324,
     331,   332,   313,    79,    80,    34,    44,    59,    68,   106,
     110,   128,   129,   135,   140,   233,   288,    78,    78,    77,
     317,    84,   364,   283,   213,   312,    79,    76,   280,   282,
     317,   323,    77,   317,   317,   355,   334,   374,    77,    77,
      80,   379,    77,    80,    80,    80,   318,   382,   318,   382,
     318,   382,    77,    80,    79,    84,   291,   292,   317,    80,
     284,   267,   317,   317,   361,   352,    72,   220,   326,   293,
     352,   294,   294,    77,    80,    83,   132,   317,   275,    78,
      77,    80,   317,   317,   314,   315,   330,   317,    16,    16,
      80,   300,   317,   213,   317,   341,    77,    77,   193,   191,
     363,    78,   230,   270,    76,   213,   331,    84,   238,    79,
     237,   286,   287,    76,   255,   247,   363,   364,    77,    80,
      79,   317,    80,   112,    76,   329,   213,   339,   213,   339,
     339,   339,   291,    79,   339,   268,    77,    80,   344,    77,
     317,    80,   308,   309,   276,    16,   217,    79,    77,    77,
      77,   301,    79,    80,   340,   200,   201,   203,   203,    76,
     317,   132,   317,   314,   271,   331,   288,   213,   233,   288,
     234,   245,   317,    80,   250,   364,   213,    80,   280,   328,
      77,    77,    77,    77,   292,   317,    77,    85,   317,   294,
      76,   330,   184,   317,   202,   195,   196,   198,   194,   192,
      84,   364,    76,    77,   255,   313,   107,   108,   109,   232,
      80,   248,   364,   251,    77,   213,    80,   317,    79,    79,
     195,   117,   146,    77,   317,   230,   238,    77,   249,   250,
     145,   245,   252,    78,   294,    80,    16,   299,   317,   197,
     199,   364,    80,   288,   147,    78,   317,   111,   279,    80,
      76,    76,   279,   255,    78,   253,    80,   278,   317,    80,
      16,    16,   213,    80,   230,   317,   245,   317,    80,   112,
     277,    77,    77,   231,    77,   317,   131,   131,    78,    78,
     279,   279,    77,    77
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
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
  int yytoken;
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

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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
  *++yyvsp = yylval;

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
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:

/* Line 1455 of yacc.c  */
/* #line 334 "gram.in" */
    {
/* stat:   is the nonterminal for Fortran statements */

		  lastwasbranch = NO; ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
/* #line 340 "gram.in" */
    { /* forbid further statement function definitions... */
		  if (parstate == INDATA && laststfcn != thisstno)
			parstate = INEXEC;
		  thisstno++;
		  if((yyvsp[(1) - (2)].labval) && ((yyvsp[(1) - (2)].labval)->labelno==dorange))
			enddo((yyvsp[(1) - (2)].labval)->labelno, false, 1);
		  if(lastwasbranch && thislabel==NULL)
			warn("statement cannot be reached");
		  lastwasbranch = thiswasbranch;
		  thiswasbranch = NO;
		  if((yyvsp[(1) - (2)].labval))
			{
			if((yyvsp[(1) - (2)].labval)->labtype == LABFORMAT)
				err("label already that of a format");
			else
				(yyvsp[(1) - (2)].labval)->labtype = LABEXEC;
			}
		  freetemps();
		;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
/* #line 360 "gram.in" */
    { if (can_include)
			doinclude( (yyvsp[(3) - (3)].charpval), true );
		  else {
			fprintf(diagfile, "Cannot open file %s\n", (yyvsp[(3) - (3)].charpval));
			done(1);
			}
		;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
/* #line 368 "gram.in" */
    { if ((yyvsp[(1) - (2)].labval))
			lastwasbranch = NO;
		  endproc(); /* lastwasbranch = NO; -- set in endproc() */
		;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
/* #line 373 "gram.in" */
    { if ((yyvsp[(1) - (4)].labval))
			lastwasbranch = NO;
		  endproc(); /* lastwasbranch = NO; -- set in endproc() */
		;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
/* #line 378 "gram.in" */
    { if ((yyvsp[(1) - (4)].labval))
			lastwasbranch = NO;
		  endproc(); /* lastwasbranch = NO; -- set in endproc() */
		;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
/* #line 383 "gram.in" */
    { if ((yyvsp[(1) - (4)].labval))
			lastwasbranch = NO;
		  endproc(); /* lastwasbranch = NO; -- set in endproc() */
		;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
/* #line 388 "gram.in" */
    {  
        exendmodule();
       // if ($1)
		//	lastwasbranch = NO;
       //     endproc(); /* lastwasbranch = NO; -- set in endproc() */
		;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
/* #line 395 "gram.in" */
    { 
         if ((yyvsp[(1) - (3)].labval))
			lastwasbranch = NO;
            endtype(); /* lastwasbranch = NO; -- set in endproc() */

        //enddcl(); endproc(); 
		;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
/* #line 403 "gram.in" */
    { in_interface_definition = false;;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
/* #line 405 "gram.in" */
    { extern void unclassifiable();
		  unclassifiable();

/* flline flushes the current line, ignoring the rest of the text there */

		  flline(); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
/* #line 412 "gram.in" */
    { flline();  needkwd = NO;  inioctl = NO;
		  yyerrok; yyclearin; ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
/* #line 417 "gram.in" */
    {
		if(yystno != 0)
			{
			(yyval.labval) = thislabel =  mklabel(yystno);
			if( ! headerdone ) {
				if (procclass == CLUNKNOWN)
					procclass = CLMAIN;
				puthead(CNULL, procclass);
				}
			if(thislabel->labdefined)
				execerr("label %s already defined",
					convic(thislabel->stateno) );
			else	{
				if(thislabel->blklevel!=0 && thislabel->blklevel<(unsigned int)blklevel
				    && thislabel->labtype!=LABFORMAT)
					warn1("there is a branch to label %s from outside block",
					      convic( (ftnint) (thislabel->stateno) ) );
				thislabel->blklevel = blklevel;
				thislabel->labdefined = YES;
				if(thislabel->labtype != LABFORMAT)
					p1_label((long)(thislabel - labeltab));
				}
			}
		else    (yyval.labval) = thislabel = NULL;
		;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
/* #line 445 "gram.in" */
    {startproc((yyvsp[(3) - (3)].extval), CLMAIN); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
/* #line 447 "gram.in" */
    {	warn("ignoring arguments to main program");
			/* hashclear(); */
			startproc((yyvsp[(3) - (4)].extval), CLMAIN); ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
/* #line 451 "gram.in" */
    { if((yyvsp[(3) - (3)].extval)) NO66("named BLOCKDATA");
		  startproc((yyvsp[(3) - (3)].extval), CLBLOCK); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
/* #line 453 "gram.in" */
    { needkwd = 1; /*&&*/;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
/* #line 453 "gram.in" */
    {needkwd = 0;;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
/* #line 454 "gram.in" */
    { entrypt(CLPROC, TYSUBR | (yyvsp[(1) - (8)].ival), (ftnint) 0,  (yyvsp[(4) - (8)].extval), (yyvsp[(5) - (8)].chval), (yyvsp[(7) - (8)].chval)); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
/* #line 459 "gram.in" */
    { needkwd = 1; /*&&*/;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
/* #line 459 "gram.in" */
    {needkwd = 0;;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
/* #line 460 "gram.in" */
    { entrypt(CLPROC, (yyvsp[(1) - (8)].ival), varleng, (yyvsp[(4) - (8)].extval), (yyvsp[(5) - (8)].chval), (yyvsp[(7) - (8)].chval)); ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
/* #line 462 "gram.in" */
    { if(parstate==OUTSIDE || procclass==CLMAIN
			|| procclass==CLBLOCK)
				execerr("misplaced entry statement", CNULL);
		  entrypt(CLENTRY, 0, (ftnint) 0, (yyvsp[(2) - (3)].extval), (yyvsp[(3) - (3)].chval), 0);
		;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
/* #line 468 "gram.in" */
    { exmodule(token); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
/* #line 471 "gram.in" */
    {enddcl(); endproc(); excontains(); ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
/* #line 475 "gram.in" */
    { /*assert(false);*/ in_interface_definition = true;;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
/* #line 485 "gram.in" */
    { needkwd = 1; ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
/* #line 485 "gram.in" */
    { needkwd = 0; ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
/* #line 485 "gram.in" */
    {needkwd = 1; (yyval.namval) = 0;;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
/* #line 487 "gram.in" */
    { needkwd = 1; ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
/* #line 487 "gram.in" */
    { needkwd = 0; ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
/* #line 487 "gram.in" */
    {needkwd = 1; (yyval.namval) = (yyvsp[(5) - (6)].namval);;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
/* #line 489 "gram.in" */
    { needkwd = 1; ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
/* #line 490 "gram.in" */
    { (yyval.chval) = ((yyvsp[(2) - (2)].namval) ? mkchain((char *)(yyvsp[(2) - (2)].namval),CHNULL) : CHNULL ); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
/* #line 491 "gram.in" */
    { needkwd = 1; ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
/* #line 492 "gram.in" */
    { if((yyvsp[(3) - (3)].namval)) /*$1 =*/ (yyval.chval) = mkchain((char *)(yyvsp[(3) - (3)].namval), (yyvsp[(1) - (3)].chval)); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
/* #line 495 "gram.in" */
    {(yyval.chval) = 0;;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
/* #line 497 "gram.in" */
    {(yyval.chval) = (yyvsp[(1) - (1)].chval);;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
/* #line 502 "gram.in" */
    {(yyval.ival)=(yyvsp[(1) - (1)].ival);;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
/* #line 504 "gram.in" */
    { needkwd = 1; ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
/* #line 505 "gram.in" */
    {(yyval.ival)=TY_PREFIX_ELEMENTAL;;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
/* #line 507 "gram.in" */
    { needkwd = 1; ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
/* #line 508 "gram.in" */
    {(yyval.ival)=TY_PREFIX_PURE;;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
/* #line 511 "gram.in" */
    {(yyval.ival) = (yyvsp[(1) - (1)].ival);;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
/* #line 513 "gram.in" */
    {(yyval.ival) = (yyvsp[(1) - (2)].ival) | (yyvsp[(2) - (2)].ival);;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
/* #line 516 "gram.in" */
    {needkwd = 0; (yyval.ival) = TYUNKNOWN;;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
/* #line 518 "gram.in" */
    {(yyval.ival) = (yyvsp[(1) - (1)].ival);;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
/* #line 521 "gram.in" */
    { newproc(); ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
/* #line 525 "gram.in" */
    { newmodule(); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
/* #line 529 "gram.in" */
    { (yyval.extval) = newentry((yyvsp[(1) - (1)].namval), 1); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
/* #line 538 "gram.in" */
    { (yyval.namval) = mkname(token); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
/* #line 541 "gram.in" */
    { (yyval.extval) = NULL; ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
/* #line 559 "gram.in" */
    { (yyval.chval) = 0; ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
/* #line 561 "gram.in" */
    { NO66(" () argument list");
		  (yyval.chval) = 0; ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
/* #line 564 "gram.in" */
    {(yyval.chval) = (yyvsp[(2) - (3)].chval); ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
/* #line 568 "gram.in" */
    { (yyval.chval) = ((yyvsp[(1) - (1)].namval) ? mkchain((char *)(yyvsp[(1) - (1)].namval),CHNULL) : CHNULL ); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
/* #line 570 "gram.in" */
    { if((yyvsp[(3) - (3)].namval)) /*$1 =*/ (yyval.chval) = mkchain((char *)(yyvsp[(3) - (3)].namval), (yyvsp[(1) - (3)].chval)); ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
/* #line 574 "gram.in" */
    { if(!in_interface_definition){
              if((yyvsp[(1) - (1)].namval)->vstg!=STGUNKNOWN && (yyvsp[(1) - (1)].namval)->vstg!=STGARG)
			      dclerr("name declared as argument after use", (yyvsp[(1) - (1)].namval));
		      (yyvsp[(1) - (1)].namval)->vstg = STGARG;
          }
		;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
/* #line 581 "gram.in" */
    { NO66("altenate return argument");

/* substars   means that '*'ed formal parameters should be replaced.
   This is used to specify alternate return labels; in theory, only
   parameter slots which have '*' should accept the statement labels.
   This compiler chooses to ignore the '*'s in the formal declaration, and
   always return the proper value anyway.

   This variable is only referred to in   proc.c   */

		  (yyval.namval) = 0;  substars = YES; ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
/* #line 597 "gram.in" */
    {
		char *s;
		s = copyn(toklen+1, token);
		s[toklen] = '\0';
		(yyval.charpval) = s;
		;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
/* #line 604 "gram.in" */
    { /* reset type global dimension */ ndim_type = 0; ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
/* #line 618 "gram.in" */
    { NO66("SAVE statement");
		  saveall = YES; ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
/* #line 621 "gram.in" */
    { NO66("SAVE statement"); ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
/* #line 623 "gram.in" */
    { fmtstmt(thislabel); setfmt(thislabel); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
/* #line 625 "gram.in" */
    { NO66("PARAMETER statement"); ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
/* #line 629 "gram.in" */
    { 
          if(!in_interface_definition){
              settype((yyvsp[(2) - (6)].namval), (yyvsp[(1) - (6)].ival), (yyvsp[(6) - (6)].expval), 0);
		      if(ndim>0) {
                setbound((yyvsp[(2) - (6)].namval),ndim,dims, (yyvsp[(6) - (6)].expval));
              } else if(ndim_type>0) {
                setbound((yyvsp[(2) - (6)].namval),ndim_type,dims_type, (yyvsp[(6) - (6)].expval));
              }
          }
		;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
/* #line 640 "gram.in" */
    { 
          if(!in_interface_definition){
              settype((yyvsp[(4) - (9)].namval), (yyvsp[(1) - (9)].ival), (yyvsp[(8) - (9)].expval), 0);
		      if(ndim>0) {
                setbound((yyvsp[(4) - (9)].namval),ndim,dims, (yyvsp[(8) - (9)].expval));
              } else if(ndim_type>0) {
                setbound((yyvsp[(4) - (9)].namval),ndim_type,dims_type, (yyvsp[(8) - (9)].expval));
              }
              if((yyvsp[(9) - (9)].expval))
                {
                          make_param((Paramblock *)(yyvsp[(4) - (9)].namval), 0, (yyvsp[(9) - (9)].expval));
                }
            }
		;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
/* #line 654 "gram.in" */
    { ndim_type = 0; needkwd = 1; ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
/* #line 655 "gram.in" */
    { 
          if(!in_interface_definition){
              (yyval.ii_val).first = (yyvsp[(1) - (12)].ival);
              (yyval.ii_val).second = (yyvsp[(4) - (12)].ival);
              settype((yyvsp[(7) - (12)].namval), (yyvsp[(1) - (12)].ival), (yyvsp[(11) - (12)].expval), (yyvsp[(4) - (12)].ival));
		      if(ndim>0) {
                setbound((yyvsp[(7) - (12)].namval),ndim,dims, (yyvsp[(11) - (12)].expval));
              } else if(ndim_type>0) {
                setbound((yyvsp[(7) - (12)].namval),ndim_type,dims_type, (yyvsp[(11) - (12)].expval));
              }
              if(/*($4 & ATTRIBUTE_PARAMETER) &&*/ (yyvsp[(12) - (12)].expval))
                {
                          make_param((Paramblock *)(yyvsp[(7) - (12)].namval), (yyvsp[(4) - (12)].ival), (yyvsp[(12) - (12)].expval));
                }
                else if(((yyvsp[(4) - (12)].ival) & ATTRIBUTE_PARAMETER) /*|| $12*/)
                {
			        err("parameter requires initialization2");
                }
              if((yyvsp[(4) - (12)].ival) & ATTRIBUTE_SAVE)
                {
		            (yyvsp[(7) - (12)].namval)->vsave = YES;
                }
            }
		;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
/* #line 680 "gram.in" */
    {newtype(token);;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
/* #line 682 "gram.in" */
    {newtype(token);;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
/* #line 684 "gram.in" */
    { 
          if(!in_interface_definition){
              (yyval.ii_val) = (yyvsp[(1) - (6)].ii_val);
              settype2((yyvsp[(3) - (6)].namval), (yyvsp[(1) - (6)].ii_val), (yyvsp[(5) - (6)].expval));
		      if(ndim>0) {
                setbound((yyvsp[(3) - (6)].namval),ndim,dims, (yyvsp[(5) - (6)].expval));
              } else if(ndim_type>0) {
                setbound((yyvsp[(3) - (6)].namval),ndim_type,dims_type, (yyvsp[(5) - (6)].expval));
              }
              if(/*($1.second & ATTRIBUTE_PARAMETER) &&*/ (yyvsp[(6) - (6)].expval))
                {
                          make_param((Paramblock *)(yyvsp[(3) - (6)].namval), (yyvsp[(1) - (6)].ii_val).second, (yyvsp[(6) - (6)].expval));
                }
                else if(((yyvsp[(1) - (6)].ii_val).second & ATTRIBUTE_PARAMETER) /*|| $6*/)
                {
			        err("parameter requires initialization1");
                }
              if((yyvsp[(1) - (6)].ii_val).second & ATTRIBUTE_SAVE)
                {
		            (yyvsp[(3) - (6)].namval)->vsave = YES;
                }
            }
		;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
/* #line 708 "gram.in" */
    {
          if(!in_interface_definition){
             if (new_dcl == 2) {
			    err("attempt to give DATA in type-declaration");
			    new_dcl = 1;
			    }
            }
		;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
/* #line 722 "gram.in" */
    {var_derived_type_extends_name = token;;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
/* #line 725 "gram.in" */
    {needkwd=1;;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
/* #line 727 "gram.in" */
    {needkwd=1;;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
/* #line 730 "gram.in" */
    {needkwd=0;;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
/* #line 736 "gram.in" */
    {(yyval.expval)= 0;;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
/* #line 741 "gram.in" */
    {(yyval.expval) = (yyvsp[(2) - (2)].expval);;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
/* #line 742 "gram.in" */
    {needkwd  =  1;;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
/* #line 743 "gram.in" */
    { 
         (yyval.expval) = call1(TYUNKNOWN, "ft::transpose", (yyvsp[(4) - (12)].expval));
        ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
/* #line 748 "gram.in" */
    {(yyval.ival)= ATTRIBUTE_IN;;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
/* #line 749 "gram.in" */
    {(yyval.ival)= ATTRIBUTE_OUT;;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
/* #line 750 "gram.in" */
    {(yyval.ival)= ATTRIBUTE_INOUT;;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
/* #line 754 "gram.in" */
    { needkwd = 1; ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
/* #line 755 "gram.in" */
    {
           (yyval.ival) = (yyvsp[(4) - (5)].ival);
        ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
/* #line 760 "gram.in" */
    {
           (yyval.ival) = ATTRIBUTE_VALUE;
        ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
/* #line 765 "gram.in" */
    {
           (yyval.ival) = ATTRIBUTE_PARAMETER;
        ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
/* #line 770 "gram.in" */
    {
           (yyval.ival) = ATTRIBUTE_SAVE;
        ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
/* #line 775 "gram.in" */
    {
           (yyval.ival) = ATTRIBUTE_PRIVATE;
        ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
/* #line 780 "gram.in" */
    {
           (yyval.ival) = ATTRIBUTE_PUBLIC;
        ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
/* #line 785 "gram.in" */
    {
           (yyval.ival) = ATTRIBUTE_ALLOCATABLE;
        ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
/* #line 790 "gram.in" */
    {
           (yyval.ival) = ATTRIBUTE_OPTIONAL;
        ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
/* #line 795 "gram.in" */
    {
           (yyval.ival) = ATTRIBUTE_EXTERNAL;
        ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
/* #line 800 "gram.in" */
    {
           (yyval.ival) = 0;
        ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
/* #line 807 "gram.in" */
    { needkwd = 1; ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
/* #line 808 "gram.in" */
    {
           (yyval.ival) = (yyvsp[(2) - (2)].ival);
        ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
/* #line 812 "gram.in" */
    { needkwd = 1; ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
/* #line 813 "gram.in" */
    {
           (yyval.ival) = (yyvsp[(1) - (4)].ival) | (yyvsp[(4) - (4)].ival);
        ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
/* #line 819 "gram.in" */
    { new_dcl = 2; ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
/* #line 822 "gram.in" */
    { varleng = (yyvsp[(2) - (2)].lval); ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
/* #line 826 "gram.in" */
    { varleng = ((yyvsp[(1) - (1)].ival)<0 || ONEOF((yyvsp[(1) - (1)].ival),M(TYLOGICAL)|M(TYLONG))
				? 0 : typesize[(yyvsp[(1) - (1)].ival)]);
		  vartype = (yyvsp[(1) - (1)].ival); ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
/* #line 831 "gram.in" */
    { (yyval.ival) = TYLONG; ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
/* #line 832 "gram.in" */
    { (yyval.ival) = tyreal; ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
/* #line 833 "gram.in" */
    { ++complex_seen; (yyval.ival) = tycomplex; ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
/* #line 834 "gram.in" */
    { (yyval.ival) = TYDREAL; /*dp*/ ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
/* #line 835 "gram.in" */
    { ++dcomplex_seen; NOEXT("DOUBLE COMPLEX statement"); (yyval.ival) = TYDCOMPLEX; ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
/* #line 836 "gram.in" */
    { (yyval.ival) = TYLOGICAL; ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
/* #line 837 "gram.in" */
    { NO66("CHARACTER statement"); (yyval.ival) = TYCHAR; ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
/* #line 838 "gram.in" */
    { (yyval.ival) = TYUNKNOWN; ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
/* #line 839 "gram.in" */
    { (yyval.ival) = TYUNKNOWN; ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
/* #line 840 "gram.in" */
    { NOEXT("AUTOMATIC statement"); (yyval.ival) = - STGAUTO; ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
/* #line 841 "gram.in" */
    { NOEXT("STATIC statement"); (yyval.ival) = - STGBSS; ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
/* #line 842 "gram.in" */
    { (yyval.ival) = TY_DERIVED; var_derived_type_name = token;;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
/* #line 843 "gram.in" */
    { (yyval.ival) = TY_DERIVED; var_derived_type_name = token;;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
/* #line 847 "gram.in" */
    {(yyval.lval) = (yyvsp[(1) - (1)].lval);;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
/* #line 849 "gram.in" */
    {trykwd=1;;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
/* #line 849 "gram.in" */
    { (yyval.lval) = (yyvsp[(3) - (3)].lval);;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
/* #line 857 "gram.in" */
    {(yyval.lval) = (yyvsp[(2) - (5)].expval)->constblock.Const.ci;;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
/* #line 859 "gram.in" */
    { (yyval.lval) = (yyvsp[(1) - (2)].lval);;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
/* #line 863 "gram.in" */
    { (yyval.lval) = length_from_name("c_bool");;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
/* #line 864 "gram.in" */
    { (yyval.lval) = length_from_name("c_char");;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
/* #line 865 "gram.in" */
    { (yyval.lval) = length_from_name("c_double");;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
/* #line 866 "gram.in" */
    { (yyval.lval) = length_from_name("c_double_complex");;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
/* #line 867 "gram.in" */
    { (yyval.lval) = length_from_name("c_float");;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
/* #line 868 "gram.in" */
    { (yyval.lval) = length_from_name("c_float_complex");;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
/* #line 869 "gram.in" */
    { (yyval.lval) = length_from_name("c_float128");;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
/* #line 870 "gram.in" */
    { (yyval.lval) = length_from_name("c_float128_complex");;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
/* #line 871 "gram.in" */
    { (yyval.lval) = length_from_name("c_int");;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
/* #line 872 "gram.in" */
    { (yyval.lval) = length_from_name("c_int_fast128_t");;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
/* #line 873 "gram.in" */
    { (yyval.lval) = length_from_name("c_int_fast16_t");;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
/* #line 874 "gram.in" */
    { (yyval.lval) = length_from_name("c_int_fast32_t");;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
/* #line 875 "gram.in" */
    { (yyval.lval) = length_from_name("c_int_fast64_t");;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
/* #line 876 "gram.in" */
    { (yyval.lval) = length_from_name("c_int_fast8_t");;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
/* #line 877 "gram.in" */
    { (yyval.lval) = length_from_name("c_int_least128_t");;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
/* #line 878 "gram.in" */
    { (yyval.lval) = length_from_name("c_int_least16_t");;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
/* #line 879 "gram.in" */
    { (yyval.lval) = length_from_name("c_int_least32_t");;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
/* #line 880 "gram.in" */
    { (yyval.lval) = length_from_name("c_int_least64_t");;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
/* #line 881 "gram.in" */
    { (yyval.lval) = length_from_name("c_int_least8_t");;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
/* #line 882 "gram.in" */
    { (yyval.lval) = length_from_name("c_int128_t");;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
/* #line 883 "gram.in" */
    { (yyval.lval) = length_from_name("c_int16_t");;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
/* #line 884 "gram.in" */
    { (yyval.lval) = length_from_name("c_int32_t");;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
/* #line 885 "gram.in" */
    { (yyval.lval) = length_from_name("c_int64_t");;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
/* #line 886 "gram.in" */
    { (yyval.lval) = length_from_name("c_int8_t");;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
/* #line 887 "gram.in" */
    { (yyval.lval) = length_from_name("c_intmax_t");;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
/* #line 888 "gram.in" */
    { (yyval.lval) = length_from_name("c_intptr_t");;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
/* #line 889 "gram.in" */
    { (yyval.lval) = length_from_name("c_long");;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
/* #line 890 "gram.in" */
    { (yyval.lval) = length_from_name("c_long_double");;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
/* #line 891 "gram.in" */
    { (yyval.lval) = length_from_name("c_long_double_complex");;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
/* #line 892 "gram.in" */
    { (yyval.lval) = length_from_name("c_long_long");;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
/* #line 893 "gram.in" */
    { (yyval.lval) = length_from_name("c_ptrdiff_t");;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
/* #line 894 "gram.in" */
    { (yyval.lval) = length_from_name("c_short");;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
/* #line 895 "gram.in" */
    { (yyval.lval) = length_from_name("c_signed_char");;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
/* #line 896 "gram.in" */
    { (yyval.lval) = length_from_name("c_size_t");;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
/* #line 897 "gram.in" */
    { (yyval.lval) = length_from_name("int64");;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
/* #line 898 "gram.in" */
    { (yyval.lval) = length_from_name("real128");;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
/* #line 906 "gram.in" */
    { (yyval.lval) = (yyvsp[(1) - (1)].lval);;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
/* #line 908 "gram.in" */
    {trykwd=1; /* jdslgjks*/;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
/* #line 908 "gram.in" */
    { (yyval.lval) = (yyvsp[(4) - (5)].lval);;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
/* #line 911 "gram.in" */
    { (yyval.lval) = 1;;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
/* #line 917 "gram.in" */
    {trykwd=1;;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
/* #line 923 "gram.in" */
    {trykwd=1;;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
/* #line 927 "gram.in" */
    {trykwd=1;;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
/* #line 934 "gram.in" */
    { (yyval.lval) = varleng; ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
/* #line 936 "gram.in" */
    {
		expptr p;
		p = (yyvsp[(3) - (4)].expval);
		NO66("length specification *n");
		if( ! ISICON(p) || p->constblock.Const.ci <= 0 )
			{
			(yyval.lval) = 0;
            if(!cxx)
            {
			    dclerr("length must be a positive integer constant",
				    NPNULL);
            }
        }
		else {
			if (vartype == TYCHAR)
				(yyval.lval) = p->constblock.Const.ci;
			else switch((int)p->constblock.Const.ci) {
				case 1:	(yyval.lval) = 1; break;
				case 2: (yyval.lval) = typesize[TYSHORT];	break;
				case 4: (yyval.lval) = typesize[TYLONG];	break;
				case 8: (yyval.lval) = typesize[TYDREAL];	break;
				case 16: (yyval.lval) = typesize[TYDCOMPLEX]; break;
				default:
					dclerr("invalid length",NPNULL);
					(yyval.lval) = varleng;
				}
			}
		;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
/* #line 965 "gram.in" */
    { NO66("length specification *(*)"); (yyval.lval) = -1; ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
/* #line 969 "gram.in" */
    { (yyval.expval) = mkintcon(varleng); ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
/* #line 971 "gram.in" */
    {
            if(cxx){
                (yyval.expval) = (yyvsp[(3) - (4)].expval);
            } else {
		    expptr p;
		    p = (yyvsp[(3) - (4)].expval);
		    NO66("length specification *n");
		    if( ! ISICON(p) || p->constblock.Const.ci <= 0 )
			    {
			    (yyval.expval) = mkintcon(0);
                if(!cxx)
                {
			        dclerr("length must be a positive integer constant",
				        NPNULL);
                }
            }
		    else {
			    if (vartype == TYCHAR)
				    (yyval.expval) = mkintcon(p->constblock.Const.ci);
			    else switch((int)p->constblock.Const.ci) {
				    case 1:	(yyval.expval) = mkintcon(1); break;
				    case 2: (yyval.expval) = mkintcon(typesize[TYSHORT]);	break;
				    case 4: (yyval.expval) = mkintcon(typesize[TYLONG]);	break;
				    case 8: (yyval.expval) = mkintcon(typesize[TYDREAL]);	break;
				    case 16: (yyval.expval) = mkintcon(typesize[TYDCOMPLEX]); break;
				    default:
					    dclerr("invalid length",NPNULL);
					    (yyval.expval) = mkintcon(varleng);
				    }
			    }
		    }
        ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
/* #line 1005 "gram.in" */
    { NO66("length specification *(*)"); (yyval.expval) = mkintcon(-1); ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
/* #line 1009 "gram.in" */
    { incomm( (yyval.extval) = comblock("") , (yyvsp[(3) - (3)].namval) ); ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
/* #line 1011 "gram.in" */
    { (yyval.extval) = (yyvsp[(3) - (4)].extval);  incomm((yyvsp[(3) - (4)].extval), (yyvsp[(4) - (4)].namval)); ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
/* #line 1013 "gram.in" */
    { (yyval.extval) = (yyvsp[(3) - (5)].extval);  incomm((yyvsp[(3) - (5)].extval), (yyvsp[(5) - (5)].namval)); ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
/* #line 1015 "gram.in" */
    { incomm((yyvsp[(1) - (3)].extval), (yyvsp[(3) - (3)].namval)); ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
/* #line 1019 "gram.in" */
    { (yyval.extval) = comblock(""); ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
/* #line 1021 "gram.in" */
    { (yyval.extval) = comblock(token); ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
/* #line 1025 "gram.in" */
    { setext((yyvsp[(3) - (3)].namval)); ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
/* #line 1027 "gram.in" */
    { setext((yyvsp[(3) - (3)].namval)); ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
/* #line 1031 "gram.in" */
    { NO66("INTRINSIC statement"); setintr((yyvsp[(3) - (3)].namval)); ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
/* #line 1033 "gram.in" */
    { setintr((yyvsp[(3) - (3)].namval)); ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
/* #line 1041 "gram.in" */
    {
		struct Equivblock *p;
		if(nequiv >= maxequiv)
			many("equivalences", 'q', maxequiv);
		p  =  & eqvclass[nequiv++];
		p->eqvinit = NO;
		p->eqvbottom = 0;
		p->eqvtop = 0;
		p->equivs = (yyvsp[(2) - (3)].eqvval);
		;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
/* #line 1054 "gram.in" */
    { (yyval.eqvval)=ALLOC(Eqvchain);
		  (yyval.eqvval)->eqvitem.eqvlhs = (struct Primblock *)(yyvsp[(1) - (1)].expval);
		;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
/* #line 1058 "gram.in" */
    { (yyval.eqvval)=ALLOC(Eqvchain);
		  (yyval.eqvval)->eqvitem.eqvlhs = (struct Primblock *) (yyvsp[(3) - (3)].expval);
		  (yyval.eqvval)->eqvnextp = (yyvsp[(1) - (3)].eqvval);
		;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
/* #line 1069 "gram.in" */
    { if(parstate == OUTSIDE)
			{
			newproc();
			startproc(ESNULL, CLMAIN);
			}
		  if(parstate < INDATA)
			{
			enddcl();
			parstate = INDATA;
			datagripe = 1;
			}
		;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
/* #line 1084 "gram.in" */
    { if(!cxx){
          ftnint junk;
		  if(nextdata(&junk) != NULL)
			err("too few initializers");
            }
		  frdata((yyvsp[(2) - (6)].chval));
		  frrpl();
		;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
/* #line 1094 "gram.in" */
    { frchain(&datastack); curdtp = 0; ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
/* #line 1096 "gram.in" */
    { pop_datastack(); ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
/* #line 1098 "gram.in" */
    { toomanyinit = NO; ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
/* #line 1103 "gram.in" */
    { dataval(ENULL, (yyvsp[(1) - (1)].expval)); ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
/* #line 1105 "gram.in" */
    { dataval((yyvsp[(1) - (3)].expval), (yyvsp[(3) - (3)].expval)); ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
/* #line 1110 "gram.in" */
    { 
		  (yyval.expval) = mkexpr(OPINIT, (expptr)(yyvsp[(1) - (4)].namval), convert_to_init_list((expptr)mklist((yyvsp[(3) - (4)].chval)))); 
        ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
/* #line 1116 "gram.in" */
    { if( (yyvsp[(1) - (2)].ival)==OPMINUS && ISCONST((yyvsp[(2) - (2)].expval)) )
			consnegop((Constp)(yyvsp[(2) - (2)].expval));
		  (yyval.expval) = (yyvsp[(2) - (2)].expval);
		;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
/* #line 1128 "gram.in" */
    { int k;
		  (yyvsp[(1) - (1)].namval)->vsave = YES;
		  k = (yyvsp[(1) - (1)].namval)->vstg;
		if( ! ONEOF(k, M(STGUNKNOWN)|M(STGBSS)|M(STGINIT)) )
			dclerr("can only save static variables", (yyvsp[(1) - (1)].namval));
		;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
/* #line 1142 "gram.in" */
    { if((yyvsp[(1) - (3)].namval)->vclass == CLUNKNOWN)
			make_param((Paramblock *)(yyvsp[(1) - (3)].namval), ATTRIBUTE_PARAMETER, (yyvsp[(3) - (3)].expval));
		  else dclerr("cannot make into parameter", (yyvsp[(1) - (3)].namval));
		;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
/* #line 1151 "gram.in" */
    {int kasjhgjkasdgkj = 42;;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
/* #line 1151 "gram.in" */
    {needkwd  =  1;;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
/* #line 1152 "gram.in" */
    { 
        if((yyvsp[(1) - (14)].namval)->vclass ==  CLUNKNOWN)
			make_param((Paramblock *)(yyvsp[(1) - (14)].namval), ATTRIBUTE_PARAMETER, (yyvsp[(6) - (14)].expval));
		  else dclerr("cannot make into parameter", (yyvsp[(1) - (14)].namval));
        ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
/* #line 1190 "gram.in" */
    { (yyval.chval) = mkchain((char *)(yyvsp[(1) - (1)].expval), CHNULL); ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
/* #line 1193 "gram.in" */
    { (yyval.chval) = mkchain((char *)(yyvsp[(1) - (1)].expval), CHNULL); ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
/* #line 1198 "gram.in" */
    {needwkey_if_seen_type_dcolon=0;;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
/* #line 1205 "gram.in" */
    { (yyval.chval) = (yyvsp[(1) - (1)].chval); ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
/* #line 1207 "gram.in" */
    { (yyval.chval) = hookup((yyvsp[(3) - (3)].chval), (yyvsp[(1) - (3)].chval)); ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
/* #line 1213 "gram.in" */
    { if(ndim>0) setbound((yyvsp[(1) - (2)].namval), ndim, dims, 0); ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
/* #line 1217 "gram.in" */
    { Namep np;
		  np = ( (struct Primblock *) (yyvsp[(1) - (1)].expval)) -> namep;
		  vardcl(np);
		  if(np->vstg == STGCOMMON)
			extsymtab[np->vardesc.varno].extinit = YES;
		  else if(np->vstg==STGEQUIV)
			eqvclass[np->vardesc.varno].eqvinit = YES;
		  else if(np->vstg!=STGINIT && np->vstg!=STGBSS)
			dclerr("inconsistent storage classes", np);
		  (yyval.chval) = mkchain((char *)(yyvsp[(1) - (1)].expval), CHNULL);
		;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
/* #line 1229 "gram.in" */
    { chainp p; struct Impldoblock *q;
		pop_datastack();
		q = ALLOC(Impldoblock);
		q->tag = TIMPLDO;
		(q->varnp = (Namep) ((yyvsp[(4) - (5)].chval)->datap))->vimpldovar = 1;
		p = (yyvsp[(4) - (5)].chval)->nextp;
		if(p)  { q->implb = (expptr)(p->datap); p = p->nextp; }
		if(p)  { q->impub = (expptr)(p->datap); p = p->nextp; }
		if(p)  { q->impstep = (expptr)(p->datap); }
		frchain2( (yyvsp[(4) - (5)].chval) );
		(yyval.chval) = mkchain((char *)q, CHNULL);
		q->datalist = hookup((yyvsp[(2) - (5)].chval), (yyval.chval));
		;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
/* #line 1245 "gram.in" */
    { if (!datastack)
			curdtp = 0;
		  datastack = mkchain((char *)curdtp, datastack);
		  curdtp = (yyvsp[(1) - (1)].chval); curdtelt = 0;
		  ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
/* #line 1251 "gram.in" */
    { (yyval.chval) = hookup((yyvsp[(1) - (3)].chval), (yyvsp[(3) - (3)].chval)); ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
/* #line 1256 "gram.in" */
    {in_dims_type = true; ndim_type = 0;;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
/* #line 1258 "gram.in" */
    {in_dims_type = false; ndim_type = ndim; ndim = 0;;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
/* #line 1262 "gram.in" */
    { ndim = 0; ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
/* #line 1266 "gram.in" */
    { ndim = 0; ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
/* #line 1271 "gram.in" */
    {
          Dims* cur_dims = in_dims_type ? dims_type : dims;
		  if(ndim == maxdim)
			err("too many dimensions");
		  else if(ndim < maxdim)
			{ cur_dims[ndim].lb = 0;
			  cur_dims[ndim].ub = (yyvsp[(1) - (1)].expval);
			}
		  ++ndim;
		;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
/* #line 1282 "gram.in" */
    {
          Dims* cur_dims = in_dims_type ? dims_type : dims;
		  if(ndim == maxdim)
			err("too many dimensions");
		  else if(ndim < maxdim)
			{ cur_dims[ndim].lb = (yyvsp[(1) - (3)].expval);
			  cur_dims[ndim].ub = (yyvsp[(3) - (3)].expval);
			}
		  ++ndim;
		;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
/* #line 1293 "gram.in" */
    {
          Dims* cur_dims = in_dims_type ? dims_type : dims;
		  if(ndim == maxdim)
			err("too many dimensions");
		  else if(ndim < maxdim)
			{ cur_dims[ndim].lb = nullptr;
			  cur_dims[ndim].ub = nullptr;
			}
		  ++ndim;
		;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
/* #line 1306 "gram.in" */
    { (yyval.expval) = 0; ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
/* #line 1311 "gram.in" */
    { nstars = 1; labarray[0] = (yyvsp[(1) - (1)].labval); ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
/* #line 1313 "gram.in" */
    { if(nstars < maxlablist)  labarray[nstars++] = (yyvsp[(3) - (3)].labval); ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
/* #line 1317 "gram.in" */
    { (yyval.labval) = execlab( convci(toklen, token) ); ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
/* #line 1321 "gram.in" */
    { NO66("IMPLICIT statement"); ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
/* #line 1328 "gram.in" */
    {needkwd = 1; /*oiuqwe*/;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
/* #line 1346 "gram.in" */
    {needkwd=1;;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
/* #line 1349 "gram.in" */
    { exuse(token); ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
/* #line 1354 "gram.in" */
    { if (vartype != TYUNKNOWN)
			dclerr("-- expected letter range",NPNULL);
		  setimpl(vartype, varleng, 'a', 'z'); ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
/* #line 1359 "gram.in" */
    { needkwd = 1; ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
/* #line 1368 "gram.in" */
    { setimpl(vartype, varleng, (yyvsp[(1) - (1)].ival), (yyvsp[(1) - (1)].ival)); ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
/* #line 1370 "gram.in" */
    { setimpl(vartype, varleng, (yyvsp[(1) - (3)].ival), (yyvsp[(3) - (3)].ival)); ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
/* #line 1374 "gram.in" */
    { if(toklen!=1 || token[0]<'a' || token[0]>'z')
			{
			dclerr("implicit item must be single letter", NPNULL);
			(yyval.ival) = 0;
			}
		  else (yyval.ival) = token[0];
		;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
/* #line 1388 "gram.in" */
    {
		if((yyvsp[(2) - (4)].namval)->vclass == CLUNKNOWN)
			{
			(yyvsp[(2) - (4)].namval)->vclass = CLNAMELIST;
			(yyvsp[(2) - (4)].namval)->vtype = TYINT;
			(yyvsp[(2) - (4)].namval)->vstg = STGBSS;
			(yyvsp[(2) - (4)].namval)->varxptr.namelist = (yyvsp[(4) - (4)].chval);
			(yyvsp[(2) - (4)].namval)->vardesc.varno = ++lastvarno;
			}
		else dclerr("cannot be a namelist name", (yyvsp[(2) - (4)].namval));
		;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
/* #line 1402 "gram.in" */
    { (yyval.chval) = mkchain((char *)(yyvsp[(1) - (1)].namval), CHNULL); ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
/* #line 1404 "gram.in" */
    { (yyval.chval) = hookup((yyvsp[(1) - (3)].chval), mkchain((char *)(yyvsp[(3) - (3)].namval), CHNULL)); ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
/* #line 1408 "gram.in" */
    { switch(parstate)
			{
            case OUTSIDE:
                parstate = INTYPEDCL;
                break;

		//	case OUTSIDE:
        //    	newproc();
		//			startproc(ESNULL, CLMAIN);
			case INSIDE:	parstate = INDCL;
			case INDCL:	break;
			case INTYPEDCL:	break;

			case INDATA:
				if (datagripe) {
					errstr(
				"Statement order error: declaration after DATA",
						CNULL);
					datagripe = 0;
					}
				break;

			default:
				dclerr("declaration among executables", NPNULL);
			}
		;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
/* #line 1436 "gram.in" */
    { (yyval.chval) = 0; ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
/* #line 1438 "gram.in" */
    { (yyval.chval) = revchain((yyvsp[(1) - (1)].chval)); ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
/* #line 1443 "gram.in" */
    { 
            Listblock * l = mklist((yyvsp[(1) - (1)].chval));
            l->tag = TSPECIALLIST;
            l->vtype = VTSPECIALLIST_SUBARRAY;
            (yyval.expval) = (expptr)l;
        ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
/* #line 1451 "gram.in" */
    { (yyval.expval) = (yyvsp[(1) - (1)].expval); ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
/* #line 1454 "gram.in" */
    { (yyval.expval) = mkexpr(OP_OPT_SOURCE, (yyvsp[(3) - (3)].expval), ENULL);; ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
/* #line 1457 "gram.in" */
    { (yyval.expval) = mkexpr(OP_OPT_STAT, (yyvsp[(3) - (3)].expval), ENULL);; ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
/* #line 1474 "gram.in" */
    { (yyval.chval) = mkchain((char *)(yyvsp[(1) - (1)].expval), CHNULL); ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
/* #line 1476 "gram.in" */
    { (yyval.chval) = mkchain((char *)(yyvsp[(3) - (3)].expval), (yyvsp[(1) - (3)].chval)); ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
/* #line 1481 "gram.in" */
    { (yyval.expval) = (yyvsp[(2) - (3)].expval); if ((yyval.expval)->tag == TPRIM)
                    (yyval.expval)->primblock.parenused = 1; ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
/* #line 1489 "gram.in" */
    { (yyval.expval) = mkexpr((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].expval), (yyvsp[(3) - (3)].expval)); ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
/* #line 1491 "gram.in" */
    { (yyval.expval) = mkexpr(OPSTAR, (yyvsp[(1) - (3)].expval), (yyvsp[(3) - (3)].expval)); ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
/* #line 1493 "gram.in" */
    { (yyval.expval) = mkexpr(OPSLASH, (yyvsp[(1) - (3)].expval), (yyvsp[(3) - (3)].expval)); ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
/* #line 1495 "gram.in" */
    { (yyval.expval) = mkexpr(OPPOWER, (yyvsp[(1) - (3)].expval), (yyvsp[(3) - (3)].expval)); ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
/* #line 1497 "gram.in" */
    { if((yyvsp[(1) - (2)].ival) == OPMINUS)
            (yyval.expval) = mkexpr(OPNEG, (yyvsp[(2) - (2)].expval), ENULL);
          else 	(yyval.expval) = (yyvsp[(2) - (2)].expval);
        ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
/* #line 1502 "gram.in" */
    { (yyval.expval) = mkexpr((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].expval), (yyvsp[(3) - (3)].expval)); ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
/* #line 1504 "gram.in" */
    { NO66(".EQV. operator");
          (yyval.expval) = mkexpr(OPEQV, (yyvsp[(1) - (3)].expval),(yyvsp[(3) - (3)].expval)); ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
/* #line 1507 "gram.in" */
    { NO66(".NEQV. operator");
          (yyval.expval) = mkexpr(OPNEQV, (yyvsp[(1) - (3)].expval), (yyvsp[(3) - (3)].expval)); ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
/* #line 1510 "gram.in" */
    { (yyval.expval) = mkexpr(OPOR, (yyvsp[(1) - (3)].expval), (yyvsp[(3) - (3)].expval)); ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
/* #line 1512 "gram.in" */
    { (yyval.expval) = mkexpr(OPAND, (yyvsp[(1) - (3)].expval), (yyvsp[(3) - (3)].expval)); ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
/* #line 1514 "gram.in" */
    { (yyval.expval) = mkexpr(OPNOT, (yyvsp[(2) - (2)].expval), ENULL); ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
/* #line 1516 "gram.in" */
    { NO66("concatenation operator //");
          (yyval.expval) = mkexpr(cxx ? OPSTRCAT : OPCONCAT, (yyvsp[(1) - (3)].expval), (yyvsp[(3) - (3)].expval));
           ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
/* #line 1521 "gram.in" */
    { 
          (yyval.expval) = mkexpr(OPCALL, (yyvsp[(1) - (4)].expval), (expptr)mklist((yyvsp[(3) - (4)].chval))); 
        ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
/* #line 1525 "gram.in" */
    { (yyval.expval) = mkopmember(OPMEMBER, (yyvsp[(1) - (2)].expval), (yyvsp[(2) - (2)].expval)); ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
/* #line 1530 "gram.in" */
    {needwkey_if_seen_type_dcolon=1;;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
/* #line 1531 "gram.in" */
    {(yyval.expval) = convert_to_init_list((expptr)mklist(revchain((yyvsp[(4) - (5)].chval))));;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
/* #line 1536 "gram.in" */
    {in_member_access = true; ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
/* #line 1537 "gram.in" */
    { in_member_access = false; (yyval.expval) = (yyvsp[(3) - (3)].expval); ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
/* #line 1539 "gram.in" */
    {in_member_access = true; ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
/* #line 1540 "gram.in" */
    { in_member_access = false; (yyval.expval) = (yyvsp[(3) - (3)].expval); ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
/* #line 1545 "gram.in" */
    {
        chainp p = mkchain((char *)(yyvsp[(8) - (9)].expval), CHNULL);
        p = mkchain((char *)(yyvsp[(6) - (9)].expval), p);
        p = mkchain((char *)(yyvsp[(4) - (9)].namval), p);
        p = mkchain((char *)(yyvsp[(2) - (9)].expval), p);
        Listblock * l = mklist(p);
        l->tag = TSPECIALLIST;
        l->vtype = VTSPECIALLIST_IMPLIED_DO;
        (yyval.expval) = (expptr)l;
    ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
/* #line 1562 "gram.in" */
    { (yyval.ival) = OPPLUS; ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
/* #line 1563 "gram.in" */
    { (yyval.ival) = OPMINUS; ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
/* #line 1566 "gram.in" */
    { (yyval.ival) = OPEQ; ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
/* #line 1567 "gram.in" */
    { (yyval.ival) = OPGT; ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
/* #line 1568 "gram.in" */
    { (yyval.ival) = OPLT; ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
/* #line 1569 "gram.in" */
    { (yyval.ival) = OPGE; ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
/* #line 1570 "gram.in" */
    { (yyval.ival) = OPLE; ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
/* #line 1571 "gram.in" */
    { (yyval.ival) = OPNE; ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
/* #line 1575 "gram.in" */
    { (yyval.expval) = mkprim((yyvsp[(1) - (1)].namval), LBNULL, CHNULL); ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
/* #line 1577 "gram.in" */
    { NO66("substring operator :");
          (yyval.expval) = mkprim((yyvsp[(1) - (2)].namval), LBNULL, (yyvsp[(2) - (2)].chval)); ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
/* #line 1580 "gram.in" */
    { (yyval.expval) = mkprim((yyvsp[(1) - (4)].namval), mklist((yyvsp[(3) - (4)].chval)), CHNULL); ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
/* #line 1582 "gram.in" */
    { NO66("substring operator :");
          (yyval.expval) = mkprim((yyvsp[(1) - (5)].namval), mklist((yyvsp[(3) - (5)].chval)), (yyvsp[(5) - (5)].chval)); ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
/* #line 1587 "gram.in" */
    { (yyval.expval) = mkprim((yyvsp[(1) - (1)].namval), LBNULL, CHNULL); ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
/* #line 1606 "gram.in" */
    { (yyval.chval) = mkchain((char *)(yyvsp[(1) - (3)].expval), 
                mkchain((char *)(yyvsp[(3) - (3)].expval),
                mkchain((char *)0,CHNULL  ))); ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
/* #line 1611 "gram.in" */
    { (yyval.chval) = mkchain((char *)(yyvsp[(1) - (5)].expval), 
                mkchain((char *)(yyvsp[(3) - (5)].expval),
                mkchain((char *)(yyvsp[(5) - (5)].expval),CHNULL  ))); ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
/* #line 1618 "gram.in" */
    { (yyval.chval) = (yyvsp[(2) - (3)].chval) ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
/* #line 1622 "gram.in" */
    { (yyval.expval) = 0; ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
/* #line 1627 "gram.in" */
    {
        /* TODO
         if($1->vclass == CLPARAM)
            $$ = (expptr) cpexpr(
                ( (Paramblock *) ($1) ) -> paramval);
        */
        ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
/* #line 1637 "gram.in" */
    { (yyval.expval) = mklogcon(1); ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
/* #line 1638 "gram.in" */
    { (yyval.expval) = mklogcon(0); ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
/* #line 1639 "gram.in" */
    { (yyval.expval) = mkstrcon(toklen, token); ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
/* #line 1640 "gram.in" */
    { (yyval.expval) = mkintcon( convci(toklen, token) ); ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
/* #line 1641 "gram.in" */
    { (yyval.expval) = mkrealcon(tyreal, token); ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
/* #line 1642 "gram.in" */
    { (yyval.expval) = mkrealcon(TYDREAL, token); ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
/* #line 1653 "gram.in" */
    { NOEXT("hex constant");
          (yyval.expval) = mkbitcon(4, toklen, token); ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
/* #line 1656 "gram.in" */
    { NOEXT("octal constant");
          (yyval.expval) = mkbitcon(3, toklen, token); ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
/* #line 1659 "gram.in" */
    { NOEXT("binary constant");
          (yyval.expval) = mkbitcon(1, toklen, token); ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
/* #line 1665 "gram.in" */
    { (yyval.expval) = (yyvsp[(2) - (3)].expval); ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
/* #line 1671 "gram.in" */
    { (yyval.expval) = mkexpr((yyvsp[(2) - (3)].ival), (yyvsp[(1) - (3)].expval), (yyvsp[(3) - (3)].expval)); ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
/* #line 1673 "gram.in" */
    { (yyval.expval) = mkexpr(OPSTAR, (yyvsp[(1) - (3)].expval), (yyvsp[(3) - (3)].expval)); ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
/* #line 1675 "gram.in" */
    { (yyval.expval) = mkexpr(OPSLASH, (yyvsp[(1) - (3)].expval), (yyvsp[(3) - (3)].expval)); ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
/* #line 1677 "gram.in" */
    { (yyval.expval) = mkexpr(OPPOWER, (yyvsp[(1) - (3)].expval), (yyvsp[(3) - (3)].expval)); ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
/* #line 1679 "gram.in" */
    { if((yyvsp[(1) - (2)].ival) == OPMINUS)
            (yyval.expval) = mkexpr(OPNEG, (yyvsp[(2) - (2)].expval), ENULL);
          else	(yyval.expval) = (yyvsp[(2) - (2)].expval);
        ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
/* #line 1684 "gram.in" */
    { NO66("concatenation operator //");
          (yyval.expval) = mkexpr(cxx ? OPSTRCAT : OPCONCAT, (yyvsp[(1) - (3)].expval), (yyvsp[(3) - (3)].expval)); ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
/* #line 1689 "gram.in" */
    {
		if((yyvsp[(4) - (7)].labval)->labdefined)
			execerr("no backward DO loops", CNULL);
		(yyvsp[(4) - (7)].labval)->blklevel = blklevel+1;
		exdo((yyvsp[(4) - (7)].labval)->labelno, NPNULL, (yyvsp[(7) - (7)].chval));
		;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
/* #line 1696 "gram.in" */
    {
		exdo((int)(ctls - ctlstack - 2), NPNULL, (yyvsp[(4) - (4)].chval));
		NOEXT("DO without label");
		;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
/* #line 1701 "gram.in" */
    { exenddo(NPNULL); ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
/* #line 1703 "gram.in" */
    { exendif();  thiswasbranch = NO; ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
/* #line 1706 "gram.in" */
    { exelif((yyvsp[(4) - (6)].expval)); lastwasbranch = NO; ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
/* #line 1708 "gram.in" */
    { exelse(); lastwasbranch = NO; ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
/* #line 1710 "gram.in" */
    { exendif(); lastwasbranch = NO; ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
/* #line 1711 "gram.in" */
    { needkwd = 1; ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
/* #line 1712 "gram.in" */
    { exselect((yyvsp[(6) - (7)].expval)); ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
/* #line 1714 "gram.in" */
    { exendselect(); ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
/* #line 1716 "gram.in" */
    { excase((yyvsp[(2) - (2)].chval)); ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
/* #line 1718 "gram.in" */
    { excase(0); ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
/* #line 1720 "gram.in" */
    {
		exdoconc((int)(ctls - ctlstack - 2), (yyvsp[(3) - (3)].chval));
		;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
/* #line 1727 "gram.in" */
    { exif((yyvsp[(4) - (5)].expval)); ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
/* #line 1731 "gram.in" */
    { (yyval.chval) = mkchain((char *)(yyvsp[(1) - (3)].namval), (yyvsp[(3) - (3)].chval)); ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
/* #line 1737 "gram.in" */
    { (yyval.chval) = nullptr; ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
/* #line 1740 "gram.in" */
    { (yyval.chval) = mkchain((char *)(yyvsp[(2) - (3)].expval), (yyvsp[(3) - (3)].chval)); ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
/* #line 1745 "gram.in" */
    {
            chainp p = nullptr;
            p = mkchain(nullptr, nullptr);        // no step
            p = mkchain((char *)(yyvsp[(5) - (5)].expval), p);
            p = mkchain((char *)(yyvsp[(3) - (5)].expval), p);
            p = mkchain((char *)(yyvsp[(1) - (5)].namval), p);
            (yyval.chval) = p;
        ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
/* #line 1755 "gram.in" */
    {
            chainp p = nullptr;
            p = mkchain((char *)(yyvsp[(7) - (7)].expval), nullptr);        // no step
            p = mkchain((char *)(yyvsp[(5) - (7)].expval), p);
            p = mkchain((char *)(yyvsp[(3) - (7)].expval), p);
            p = mkchain((char *)(yyvsp[(1) - (7)].namval), p);
            (yyval.chval) = p;
        ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
/* #line 1767 "gram.in" */
    { (yyval.chval) = mkchain((char *)(yyvsp[(1) - (1)].chval), nullptr); ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
/* #line 1770 "gram.in" */
    { (yyval.chval) =     hookup((yyvsp[(1) - (3)].chval), mkchain((char *)(yyvsp[(3) - (3)].chval), nullptr)) ; ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
/* #line 1775 "gram.in" */
    { 
            chainp p = nullptr;
            p = mkchain((char *)(yyvsp[(3) - (4)].chval), nullptr);
            p = mkchain((char *)(yyvsp[(2) - (4)].chval), p);
            (yyval.chval) = p;
        ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
/* #line 1785 "gram.in" */
    { (yyval.chval) = mkchain(CNULL, (chainp)(yyvsp[(3) - (4)].expval)); ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
/* #line 1790 "gram.in" */
    { 
            exwrite((yyvsp[(4) - (6)].chval), (yyvsp[(6) - (6)].chval));
        ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
/* #line 1796 "gram.in" */
    { 
            putexpr(call2(TYUNKNOWN, "ft::pointer_integer", (yyvsp[(3) - (6)].expval), (yyvsp[(5) - (6)].expval)));
        ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
/* #line 1827 "gram.in" */
    { 
           exallocate((yyvsp[(4) - (5)].chval), false);
        ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
/* #line 1832 "gram.in" */
    { 
           exallocate((yyvsp[(4) - (5)].chval), true);
        ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
/* #line 1840 "gram.in" */
    { exequals((struct Primblock *)(yyvsp[(2) - (4)].expval), (yyvsp[(4) - (4)].expval)); ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
/* #line 1842 "gram.in" */
    { exassign((yyvsp[(5) - (5)].namval), (yyvsp[(3) - (5)].labval)); ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
/* #line 1849 "gram.in" */
    { inioctl = NO; ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
/* #line 1851 "gram.in" */
    { exarif((yyvsp[(4) - (10)].expval), (yyvsp[(6) - (10)].labval), (yyvsp[(8) - (10)].labval), (yyvsp[(10) - (10)].labval));  thiswasbranch = YES; ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
/* #line 1853 "gram.in" */
    { excall((yyvsp[(1) - (1)].namval), LBNULL, 0, labarray); ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
/* #line 1855 "gram.in" */
    { excall((yyvsp[(1) - (3)].namval), LBNULL, 0, labarray); ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
/* #line 1857 "gram.in" */
    { if(nstars < maxlablist)
			excall((yyvsp[(1) - (4)].namval), mklist(revchain((yyvsp[(3) - (4)].chval))), nstars, labarray);
		  else
			many("alternate returns", 'l', maxlablist);
		;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
/* #line 1863 "gram.in" */
    { exreturn((yyvsp[(3) - (3)].expval));  thiswasbranch = YES; ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
/* #line 1865 "gram.in" */
    { exstop((yyvsp[(1) - (3)].ival), (yyvsp[(3) - (3)].expval));  thiswasbranch = (yyvsp[(1) - (3)].ival); ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
/* #line 1867 "gram.in" */
    { excycle(); ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
/* #line 1869 "gram.in" */
    { exexit(); ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
/* #line 1873 "gram.in" */
    { (yyval.labval) = mklabel( convci(toklen, token) ); ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
/* #line 1877 "gram.in" */
    { if(parstate == OUTSIDE)
			{
			newproc();
			startproc(ESNULL, CLMAIN);
			}
		;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
/* #line 1886 "gram.in" */
    { exgoto((yyvsp[(3) - (3)].labval));  thiswasbranch = YES; ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
/* #line 1888 "gram.in" */
    { exasgoto((yyvsp[(3) - (3)].namval));  thiswasbranch = YES; ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
/* #line 1890 "gram.in" */
    { exasgoto((yyvsp[(3) - (7)].namval));  thiswasbranch = YES; ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
/* #line 1892 "gram.in" */
    { if(nstars < maxlablist)
			putcmgo(putx(fixtype((yyvsp[(7) - (7)].expval))), nstars, labarray);
		  else
			many("labels in computed GOTO list", 'l', maxlablist);
		;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
/* #line 1904 "gram.in" */
    { nstars = 0; (yyval.namval) = (yyvsp[(3) - (3)].namval); ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
/* #line 1908 "gram.in" */
    { (yyval.chval) = (yyvsp[(1) - (1)].expval) ? mkchain((char *)(yyvsp[(1) - (1)].expval),CHNULL) : CHNULL; ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
/* #line 1910 "gram.in" */
    { (yyval.chval) = (yyvsp[(3) - (3)].expval) ? mkchain((char *)(yyvsp[(3) - (3)].expval), (yyvsp[(1) - (3)].chval)) : (yyvsp[(1) - (3)].chval); ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
/* #line 1915 "gram.in" */
    { if(nstars < maxlablist) labarray[nstars++] = (yyvsp[(2) - (2)].labval); (yyval.expval) = 0; ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
/* #line 1919 "gram.in" */
    { (yyval.ival) = 0; ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
/* #line 1921 "gram.in" */
    { (yyval.ival) = 2; ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
/* #line 1934 "gram.in" */
    { (yyval.expval) = (expptr)(yyvsp[(1) - (1)].expval); ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
/* #line 1938 "gram.in" */
    { 
            // single case as single element list
		    (yyval.chval) = mkchain((char *)(yyvsp[(1) - (1)].expval), CHNULL);
        ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
/* #line 1943 "gram.in" */
    { 
            // bounded range
		    (yyval.chval) = mkchain((char *)(yyvsp[(1) - (3)].expval), CHNULL);
        ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
/* #line 1948 "gram.in" */
    { 
            // bounded range
		    (yyval.chval) = mkchain((char *)(yyvsp[(1) - (2)].expval), CHNULL);
        ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
/* #line 1953 "gram.in" */
    { 
            // bounded range
		    (yyval.chval) = mkchain((char *)(yyvsp[(2) - (2)].expval), CHNULL);
        ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
/* #line 1960 "gram.in" */
    { (yyval.chval) = (yyvsp[(1) - (1)].chval); ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
/* #line 1962 "gram.in" */
    { (yyval.chval) = hookup((yyvsp[(1) - (3)].chval), (yyvsp[(3) - (3)].chval)); ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
/* #line 1965 "gram.in" */
    {(yyval.chval)=(yyvsp[(2) - (3)].chval);;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
/* #line 1969 "gram.in" */
    { (yyval.chval) = mkchain((char *)(yyvsp[(1) - (1)].expval), CHNULL); ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
/* #line 1971 "gram.in" */
    { (yyval.chval) = hookup((yyvsp[(1) - (3)].chval), mkchain((char *)(yyvsp[(3) - (3)].expval),CHNULL) ); ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
/* #line 1975 "gram.in" */
    { 
            if(parstate == OUTSIDE)
			{
			newproc();
			startproc(ESNULL, CLMAIN);
			}

/* This next statement depends on the ordering of the state table encoding */

		    if(parstate < INDATA) enddcl();
            ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
/* #line 1989 "gram.in" */
    { intonly = YES; ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
/* #line 1993 "gram.in" */
    { intonly = NO; ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
/* #line 1998 "gram.in" */
    { endio(); ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
/* #line 2003 "gram.in" */
    { ioclause(IOSUNIT, (yyvsp[(2) - (2)].expval)); endioctl(); ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
/* #line 2005 "gram.in" */
    { ioclause(IOSUNIT, ENULL); endioctl(); ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
/* #line 2007 "gram.in" */
    { ioclause(IOSUNIT, IOSTDERR); endioctl(); ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
/* #line 2010 "gram.in" */
    { doio(CHNULL); ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
/* #line 2012 "gram.in" */
    { doio(CHNULL); ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
/* #line 2014 "gram.in" */
    { doio(revchain((yyvsp[(3) - (3)].chval))); ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
/* #line 2016 "gram.in" */
    { doio(revchain((yyvsp[(4) - (4)].chval))); ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
/* #line 2018 "gram.in" */
    { doio(revchain((yyvsp[(4) - (4)].chval))); ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
/* #line 2024 "gram.in" */
    { doio(CHNULL); ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
/* #line 2026 "gram.in" */
    { doio(revchain((yyvsp[(3) - (3)].chval))); ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
/* #line 2033 "gram.in" */
    { iostmt = IOBACKSPACE; ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
/* #line 2035 "gram.in" */
    { iostmt = IOREWIND; ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
/* #line 2037 "gram.in" */
    { iostmt = IOENDFILE; ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
/* #line 2044 "gram.in" */
    { iostmt = IOINQUIRE; ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
/* #line 2046 "gram.in" */
    { iostmt = IOOPEN; ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
/* #line 2048 "gram.in" */
    { iostmt = IOCLOSE; ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
/* #line 2052 "gram.in" */
    {
		ioclause(IOSUNIT, ENULL);
		ioclause(IOSFMT, (yyvsp[(1) - (1)].expval));
		endioctl();
		;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
/* #line 2058 "gram.in" */
    {
		ioclause(IOSUNIT, ENULL);
		ioclause(IOSFMT, ENULL);
		endioctl();
		;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
/* #line 2066 "gram.in" */
    {
		  ioclause(IOSUNIT, (yyvsp[(2) - (3)].expval));
		  endioctl();
		;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
/* #line 2071 "gram.in" */
    { endioctl(); ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
/* #line 2079 "gram.in" */
    { ioclause(IOSPOSITIONAL, (yyvsp[(1) - (1)].expval)); ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
/* #line 2081 "gram.in" */
    { ioclause(IOSPOSITIONAL, ENULL); ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
/* #line 2083 "gram.in" */
    { ioclause(IOSPOSITIONAL, IOSTDERR); ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
/* #line 2085 "gram.in" */
    { ioclause((yyvsp[(1) - (2)].ival), (yyvsp[(2) - (2)].expval)); ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
/* #line 2087 "gram.in" */
    { ioclause((yyvsp[(1) - (2)].ival), ENULL); ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
/* #line 2089 "gram.in" */
    { ioclause((yyvsp[(1) - (2)].ival), IOSTDERR); ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
/* #line 2093 "gram.in" */
    { (yyval.ival) = iocname(); ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
/* #line 2097 "gram.in" */
    { iostmt = IOREAD; ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
/* #line 2105 "gram.in" */
    {
		iostmt = IOWRITE;
		ioclause(IOSUNIT, ENULL);
		ioclause(IOSFMT, (yyvsp[(3) - (4)].expval));
		endioctl();
		;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
/* #line 2112 "gram.in" */
    {
		iostmt = IOWRITE;
		ioclause(IOSUNIT, ENULL);
		ioclause(IOSFMT, ENULL);
		endioctl();
		;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
/* #line 2121 "gram.in" */
    { (yyval.chval) = mkchain((char *)(yyvsp[(1) - (1)].tagval), CHNULL); ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
/* #line 2123 "gram.in" */
    { (yyval.chval) = mkchain((char *)(yyvsp[(3) - (3)].tagval), (yyvsp[(1) - (3)].chval)); ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
/* #line 2127 "gram.in" */
    { (yyval.tagval) = (tagptr) (yyvsp[(1) - (1)].expval); ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
/* #line 2129 "gram.in" */
    { (yyval.tagval) = (tagptr) mkiodo((yyvsp[(4) - (5)].chval),revchain((yyvsp[(2) - (5)].chval))); ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
/* #line 2133 "gram.in" */
    { (yyval.chval) = mkchain((char *)(yyvsp[(1) - (1)].expval), CHNULL); ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
/* #line 2135 "gram.in" */
    { (yyval.chval) = mkchain((char *)(yyvsp[(1) - (1)].tagval), CHNULL); ;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
/* #line 2140 "gram.in" */
    { (yyval.chval) = mkchain((char *)(yyvsp[(3) - (3)].expval), mkchain((char *)(yyvsp[(1) - (3)].expval), CHNULL) ); ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
/* #line 2142 "gram.in" */
    { (yyval.chval) = mkchain((char *)(yyvsp[(3) - (3)].tagval), mkchain((char *)(yyvsp[(1) - (3)].expval), CHNULL) ); ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
/* #line 2144 "gram.in" */
    { (yyval.chval) = mkchain((char *)(yyvsp[(3) - (3)].expval), mkchain((char *)(yyvsp[(1) - (3)].tagval), CHNULL) ); ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
/* #line 2146 "gram.in" */
    { (yyval.chval) = mkchain((char *)(yyvsp[(3) - (3)].tagval), mkchain((char *)(yyvsp[(1) - (3)].tagval), CHNULL) ); ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
/* #line 2148 "gram.in" */
    { (yyval.chval) = mkchain((char *)(yyvsp[(3) - (3)].expval), (yyvsp[(1) - (3)].chval)); ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
/* #line 2150 "gram.in" */
    { (yyval.chval) = mkchain((char *)(yyvsp[(3) - (3)].tagval), (yyvsp[(1) - (3)].chval)); ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
/* #line 2157 "gram.in" */
    { (yyval.tagval) = (tagptr) (yyvsp[(2) - (3)].expval); ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
/* #line 2159 "gram.in" */
    { (yyval.tagval) = (tagptr) mkiodo((yyvsp[(4) - (5)].chval), mkchain((char *)(yyvsp[(2) - (5)].expval), CHNULL) ); ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
/* #line 2161 "gram.in" */
    { (yyval.tagval) = (tagptr) mkiodo((yyvsp[(4) - (5)].chval), mkchain((char *)(yyvsp[(2) - (5)].tagval), CHNULL) ); ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
/* #line 2163 "gram.in" */
    { (yyval.tagval) = (tagptr) mkiodo((yyvsp[(4) - (5)].chval), revchain((yyvsp[(2) - (5)].chval))); ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
/* #line 2167 "gram.in" */
    { startioctl(); ;}
    break;



/* Line 1455 of yacc.c  */
/* #line 6172 "gram.tab.in" */
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
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

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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

  *++yyvsp = yylval;


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

#if !defined(yyoverflow) || YYERROR_VERBOSE
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
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



