/****************************************************************
Copyright 1990, 1991, 1992, 1993 by AT&T Bell Laboratories, Bellcore.

Permission to use, copy, modify, and distribute this software
and its documentation for any purpose and without fee is hereby
granted, provided that the above copyright notice appear in all
copies and that both that the copyright notice and this
permission notice and warranty disclaimer appear in supporting
documentation, and that the names of AT&T Bell Laboratories or
Bellcore or any of their entities not be used in advertising or
publicity pertaining to distribution of the software without
specific, written prior permission.

AT&T and Bellcore disclaim all warranties with regard to this
software, including all implied warranties of merchantability
and fitness.  In no event shall AT&T or Bellcore be liable for
any special, indirect or consequential damages or any damages
whatsoever resulting from loss of use, data or profits, whether
in an action of contract, negligence or other tortious action,
arising out of or in connection with the use or performance of
this software.
****************************************************************/
#ifndef __DEFS_H__ 
#define __DEFS_H__ 
#include <math.h>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <boost/optional.hpp>
#include "sysdep.h"

#include "ftypes.h"
#include "defines.h"
#include "machdefs.h"

#define MAXDIM 20
#define MAXINCLUDES 10
#define MAXLITERALS 200		/* Max number of constants in the literal
				   pool */
#define MAXTOKENLEN 302		/* length of longest token */
#define MAXCTL 20
#define MAXHASH 1511
#define MAXSTNO 801
#define MAXEXT 200
#define MAXEQUIV 150
#define MAXLABLIST 258		/* Max number of labels in an alternate
				   return CALL or computed GOTO */
#define MAXCONTIN 199		/* Max continuation lines */

/* These are the primary pointer types used in the compiler */

typedef union Expression *expptr, *tagptr;
typedef struct Chain *chainp;
typedef struct Addrblock *Addrp;
typedef struct Constblock *Constp;
typedef struct Exprblock *Exprp;
typedef struct Nameblock *Namep;
struct ii_val_t{
    int first;
    int second;
};

extern FILEP opf(char *fn, char *mode);
extern FILEP infile;
extern FILEP diagfile;
extern FILEP textfile;
extern FILEP asmfile;
extern FILEP c_file;		/* output file for all functions; extern
				   declarations will have to be prepended */
extern FILEP pass1_file;	/* Temp file to hold the function bodies
				   read on pass 1 */
extern FILEP expr_file;		/* Debugging file */
extern FILEP initfile;		/* Intermediate data file pointer */
extern FILEP blkdfile;		/* BLOCK DATA file */

extern int current_ftn_file;
extern int maxcontin;

extern char *blkdfname, *initfname, *sortfname;
extern long int headoffset;	/* Since the header block requires data we
				   don't know about until AFTER each
				   function has been processed, we keep a
				   pointer to the current (dummy) header
				   block (at the top of the assembly file)
				   here */

extern char main_alias[];	/* name given to PROGRAM psuedo-op */
extern char token [ ];
extern int toklen;
extern long lineno;
extern char *infname;
extern int needkwd;
extern int needwkey_if_seen_type_dcolon;
extern int trykwd;
extern struct Labelblock *thislabel;

/* Used to allow runtime expansion of internal tables.  In particular,
   these values can exceed their associated constants */

extern int maxctl;
extern int maxequiv;
extern int maxstno;
extern int maxhash;
extern int maxext;

extern flag nowarnflag;
extern flag ftn66flag;		/* Generate warnings when weird f77
				   features are used (undeclared dummy
				   procedure, non-char initialized with
				   string, 1-dim subscript in EQUIV) */
extern flag no66flag;		/* Generate an error when a generic
				   function (f77 feature) is used */
extern flag noextflag;		/* Generate an error when an extension to
				   Fortran 77 is used (hex/oct/bin
				   constants, automatic, static, double
				   complex types) */
extern flag zflag;		/* enable double complex intrinsics */
extern flag shiftcase;
extern flag undeftype;
extern flag shortsubs;		/* Use short subscripts on arrays? */
extern flag onetripflag;	/* if true, always execute DO loop body */
extern flag dolimitconstflag; /* make do limit const instead of creating temporary variable */
extern flag no_var_pooling; /* do not pool local variables */
extern flag unused_vars; /* output unused variables */
extern flag symbolic_params; /* use symbolic params/const */
extern flag cxx; /* use modern c++ (implies a lot) */
extern flag checksubs;
extern flag debugflag;
extern int nerr;
extern int nwarn;
extern flag srng; /* use static rng */
extern flag use_member_op; /* use member_op () for strust member access */

extern bool in_interface_definition;
extern int parstate;
extern flag headerdone;		/* True iff the current procedure's header
				   data has been written */
extern int blklevel;
extern flag saveall;
extern flag substars;		/* True iff some formal parameter is an
				   asterisk */
extern int impltype[ ];
extern ftnint implleng[ ];
extern int implstg[ ];

extern int tycomplex, tyint, tyioint, tyreal;
extern int tylog, tylogical;	/* TY____ of the implementation of   logical.
				   This will be LONG unless '-2' is given
				   on the command line */
extern int type_choice[];
extern char *typename_[];

extern int typesize[];	/* size (in bytes) of an object of each
				   type.  Indexed by TY___ macros */
extern int typealign[];
typedef std::tuple<Namep, std::string, int /* for backwards comp */> proctype_t;
extern proctype_t proctype;	/* Type of return value in this procedure */
extern char * procname;	/* External name of the procedure, or last ENTRY name */
extern int rtvlabel[ ];	/* Return value labels, indexed by TY___ macros */
extern Addrp retslot;
extern Addrp xretslot[];
extern int cxslot;	/* Complex return argument slot (frame pointer offset)*/
extern int chslot;	/* Character return argument slot (fp offset) */
extern int chlgslot;	/* Argument slot for length of character buffer */
extern int procclass;	/* Class of the current procedure:  either CLPROC,
			   CLMAIN, CLBLOCK or CLUNKNOWN */

// enumerate and name common-includes
enum includes_enum {
    include_common_params,
    include_common_data,

    includes__SIZE
};

static constexpr char const* mapped_includes[includes__SIZE] = {
    "common_params.inc",
    "common_data.inc",
};




extern bool seen_includes[includes__SIZE];
extern std::string module_name;
extern bool seen_contains;

extern ftnint procleng;	/* Length of function return value (e.g. char
			   string length).  If this is -1, then the length is
			   not known at compile time */
extern int nentry;	/* Number of entry points (other than the original
			   function call) into this procedure */
extern flag multitype;	/* YES iff there is more than one return value
			   possible */
extern int blklevel;
extern long lastiolabno;
extern int lastlabno;
extern int lastvarno;
extern int lastargslot;	/* integer offset pointing to the next free
			   location for an argument to the current routine */
extern int argloc;
extern int autonum[];		/* for numbering
				   automatic variables, e.g. temporaries */
extern int retlabel;
extern int ret0label;
extern int dorange;		/* Number of the label which terminates
				   the innermost DO loop */
extern int regnum[ ];		/* Numbers of DO indicies named in
				   regnamep   (below) */
extern Namep regnamep[ ];	/* List of DO indicies in registers */
extern int maxregvar;		/* number of elts in   regnamep   */
extern int highregvar;		/* keeps track of the highest register
				   number used by DO index allocator */
extern int nregvar;		/* count of DO indicies in registers */

extern chainp templist[];
extern int maxdim;
extern chainp earlylabs;
extern chainp holdtemps;
extern struct Entrypoint *entries;
extern struct Rplblock *rpllist;
extern struct Chain *curdtp;
extern ftnint curdtelt;
extern chainp allargs;		/* union of args in entries */
extern int nallargs;		/* total number of args */
extern int nallchargs;		/* total number of character args */
extern flag toomanyinit;	/* True iff too many initializers in a
				   DATA statement */

extern flag inioctl;
extern int iostmt;
extern Addrp ioblkp;
extern int nioctl;
extern int nequiv;
extern int eqvstart;	/* offset to eqv number to guarantee uniqueness
			   and prevent <something> from going negative */
extern int nintnames;

/* Chain of tagged blocks */

struct Chain
	{
	chainp nextp;
	char * datap;		/* Tagged block */
	};

extern chainp chains;

/* Recall that   field   is intended to hold four-bit characters */

/* This structure exists only to defeat the type checking */

struct Headblock
	{
	field tag;
	field vtype;
	field vclass;
	field vstg;
	expptr vleng;		/* Expression for length of char string -
				   this may be a constant, or an argument
				   generated by mkarg() */
	} ;

/* Control construct info (for do loops, else, etc) */

struct loop_ctrl {
    unsigned dostepsign = {};	/* 0 - variable, 1 - pos, 2 - neg */
    Namep donamep = {};		/* DO index variable */
	expptr domax = {};		/* constant or temp variable holding MAX
				   loop value; or expr of while(expr) */
	expptr dostep = {};		/* expression */

};

struct Ctlframe
	{
    unsigned ctltype = {};
    unsigned dowhile = {};
    bool select = {};
    bool first_case = {};
    bool case_ = {};
    bool needs_breakout_for = {};
    unsigned int where_count = {};
    int ctlabels[4] = {};	/* Control labels, defined below */
	int dolabel = {};		/* label marking end of this DO loop */

    

    std::vector<loop_ctrl> loop_ctrls;

	Namep loopname = {};
	};
#define endlabel ctlabels[0]
#define elselabel ctlabels[1]
#define dobodylabel ctlabels[1]
#define doposlabel ctlabels[2]
#define doneglabel ctlabels[3]
extern struct Ctlframe *ctls;		/* Keeps info on DO and BLOCK IF
					   structures - this is the stack
					   bottom */
extern struct Ctlframe *ctlstack;	/* Pointer to current nesting
					   level */
extern struct Ctlframe *lastctl;	/* Point to end of
					   dynamically-allocated array */

typedef struct {
	int type;
	chainp cp;
	} Atype;

typedef struct {
	int defined, dnargs, nargs, changes;
	Atype atypes[1];
	} Argtypes;

/* External Symbols */

struct Extsym
	{
	char *fextname;		/* Fortran version of external name */
	char *cextname;		/* C version of external name */
	field extstg;		/* STG -- should be COMMON, UNKNOWN or EXT
				   */
	unsigned extype:4;	/* for transmitting type to output routines */
	unsigned used_here:1;	/* Boolean - true on the second pass
				   through a function if the block has
				   been referenced */
	unsigned exused:1;	/* Has been used (for help with error msgs
				   about externals typed differently in
				   different modules) */
	unsigned exproto:1;	/* type specified in a .P file */
	unsigned extinit:1;	/* Procedure has been defined,
				   or COMMON has DATA */
	unsigned extseen:1;	/* True if previously referenced */
	chainp extp;		/* List of identifiers in the common
				   block for this function, stored as
				   Namep (hash table pointers) */
	chainp allextp;		/* List of lists of identifiers; we keep one
				   list for each layout of this common block */
	int curno;		/* current number for this common block,
				   used for constructing appending _nnn
				   to the common block name */
	int maxno;		/* highest curno value for this common block */
	ftnint extleng;
	ftnint maxleng;
	Argtypes *arginfo;
	};
typedef struct Extsym Extsym;

extern Extsym *extsymtab;	/* External symbol table */
extern Extsym *nextext;
extern Extsym *lastext;
extern int complex_seen, dcomplex_seen;

/* Statement labels */

struct Labelblock
	{
	int labelno;		/* Internal label */
	unsigned blklevel:8;	/* level of nesting , for branch-in-loop
				   checking */
	unsigned labused:1;
	unsigned fmtlabused:1;
	unsigned labinacc:1;	/* inaccessible? (i.e. has its scope
				   vanished) */
	unsigned labdefined:1;	/* YES or NO */
	unsigned labtype:2;	/* LAB{FORMAT,EXEC,etc} */
	ftnint stateno;		/* Original label */
	char *fmtstring;	/* format string */
	};

extern struct Labelblock *labeltab;	/* Label table - keeps track of
					   all labels, including undefined */
extern struct Labelblock *labtabend;
extern struct Labelblock *highlabtab;

/* Entry point list */

struct Entrypoint
	{
	struct Entrypoint *entnextp;
	Extsym *entryname;	/* Name of this ENTRY */
	chainp arglist;
	int typelabel;			/* Label for function exit; this
					   will return the proper type of
					   object */
	Namep enamep;			/* External name */
    std::string alt_result;
	};

/* Primitive block, or Primary block.  This is a general template returned
   by the parser, which will be interpreted in context.  It is a template
   for an identifier (variable name, function name), parenthesized
   arguments (array subscripts, function parameters) and substring
   specifications. */

struct Primblock
	{
	field tag;
	field vtype;
	unsigned parenused:1;		/* distinguish (a) from a */
	Namep namep;			/* Pointer to structure Nameblock */
	struct Listblock *argsp;

//    ! implement multi-dimension index/stride access
	expptr fcharp;			/* first-char-index-pointer (in
					   substring) */
    expptr lcharp;			/* last-char-index-pointer (in
                            substring) */
    expptr scharp;			/* stride ) */
};


struct Hashentry
	{
	int hashval;
	Namep varp;
	};
extern struct Hashentry *hashtab;	/* Hash table */
extern struct Hashentry *lasthash;
extern int hash_counter;

extern struct Hashentry *type_hashtab;	/* Hash table */
extern struct Hashentry *type_lasthash;
extern int type_hash_counter;

extern struct Hashentry *member_hashtab;	/* Hash table */
extern struct Hashentry *member_lasthash;
extern int member_hash_counter;


extern std::vector<std::string> global_includes;
extern std::vector<std::string> proto_includes;

struct Intrpacked	/* bits for intrinsic function description */
	{
	unsigned f1:3;
	unsigned f2:4;
	unsigned f3:7;
	unsigned f4:1;
	};

struct derived_type {
    std::string name;
    std::string name_super;

    std::vector<Namep> members;
};

struct Nameblock
	{
	field tag;
	field vtype;
	field vclass;
	field vstg;
	expptr vleng;		/* length of character string, if applicable */
	char *fvarname;		/* name in the Fortran source */
	char *cvarname;		/* name in the resulting C */

    chainp paramval;

	chainp vlastdim;	/* datap points to new_vars entry for the */
				/* system variable, if any, storing the final */
				/* dimension; we zero the datap if this */
				/* variable is needed */
	unsigned vprocclass:3;	/* P____ macros - selects the   varxptr
				   field below */
	unsigned vdovar:1;	/* "is it a DO variable?" for register
				   and multi-level loop	checking */
	unsigned vdcldone:1;	/* "do I think I'm done?" - set when the
				   context is sufficient to determine its
				   status */
	unsigned vadjdim:1;	/* "adjustable dimension?" - needed for
				   information about copies */
	unsigned vsave:1;
	unsigned vimpldovar:1;	/* used to prevent erroneous error messages
				   for variables used only in DATA stmt
				   implicit DOs */
	unsigned vis_assigned:1;/* True if this variable has had some
				   label ASSIGNED to it; hence
				   varxptr.assigned_values is valid */
	unsigned vimplstg:1;	/* True if storage type is assigned implicitly;
				   this allows a COMMON variable to participate
				   in a DIMENSION before the COMMON declaration.
				   */
	unsigned vcommequiv:1;	/* True if EQUIVALENCEd onto STGCOMMON */
	unsigned vfmt_asg:1;	/* True if char *var_fmt needed */
	unsigned vpassed:1;	/* True if passed as a character-variable arg */
	unsigned vknownarg:1;	/* True if seen in a previous entry point */
	unsigned visused:1;	/* True if variable is referenced -- so we */
				/* can omit variables that only appear in DATA */
	unsigned vnamelist:1;	/* Appears in a NAMELIST */
	unsigned vimpltype:1;	/* True if implicitly typed and not
				   invoked as a function or subroutine
				   (so we can consistently type procedures
				   declared external and passed as args
				   but never invoked).
				   */
	unsigned vtypewarned:1;	/* so we complain just once about
				   changed types of external procedures */
	unsigned vinftype:1;	/* so we can restore implicit type to a
				   procedure if it is invoked as a function
				   after being given a different type by -it */
	unsigned vinfproc:1;	/* True if -it infers this to be a procedure */
	unsigned vcalled:1;	/* has been invoked */
	unsigned vdimfinish:1;	/* need to invoke dim_finish() */
	unsigned vrefused:1;	/* Need to #define name_ref (for -s) */
    unsigned vsubscrused : 1;	/* Need to #define name_subscr (for -2) */

    unsigned intent_in : 1;
    unsigned intent_out : 1;
    unsigned intent_value : 1;
    unsigned is_optional : 1;
    unsigned is_allocatable : 1;
    unsigned is_toplevel : 1;
    unsigned is_invoked : 1;
    unsigned is_external : 1;

    char* derived_type_name;

    int hash_index;
/* The   vardesc   union below is used to store the number of an intrinsic
   function (when vstg == STGINTR and vprocclass == PINTRINSIC), or to
   store the index of this external symbol in   extsymtab   (when vstg ==
   STGEXT and vprocclass == PEXTERNAL) */

	union	{
		int varno;		/* Return variable for a function.
					   This is used when a function is
					   assigned a return value.  Also
					   used to point to the COMMON
					   block, when this is a field of
					   that block.  Also points to
					   EQUIV block when STGEQUIV */
		struct Intrpacked intrdesc;	/* bits for intrinsic function*/
		} vardesc;
	struct Dimblock *vdim;	/* points to the dimensions if they exist */
	ftnint voffset;		/* offset in a storage block (the variable
				   name will be "v.%d", voffset in a
				   common blck on the vax).  Also holds
				   pointers for automatic variables.  When
				   STGEQUIV, this is -(offset from array
				   base) */
	union	{
		chainp namelist;	/* points to names in the NAMELIST,
					   if this is a NAMELIST name */
		chainp vstfdesc;	/* points to (formals, expr) pair */
		chainp assigned_values;	/* list of integers, each being a
					   statement label assigned to
					   this variable in the current function */
		} varxptr;
	int argno;		/* for multiple entries */
	Argtypes *arginfo;
	};


/* PARAMETER statements */

//struct Paramblock
//	{
//	field tag;
//	field vtype;
//	field vclass;
//	field vstg;
//	expptr vleng;
//	char *fvarname;
//	char *cvarname;
//	chainp paramval;
//	} ;

using Paramblock = Nameblock;

/* Expression block */

struct Exprblock
	{
	field tag;
	field vtype;
	field vclass;
	field vstg;
	expptr vleng;		/* in the case of a character expression, this
				   value is inherited from the children */
	unsigned opcode;
	expptr leftp;
	expptr rightp;
	};


union Constant
	{
	struct {
		char *ccp0;
		ftnint blanks;
		} ccp1;
	ftnint ci;		/* Constant long integer */
	double cd[2];
	char *cds[2];
	};
#define ccp ccp1.ccp0

struct Constblock
	{
	field tag;
	field vtype;
	field vclass;
	field vstg;		/* vstg = 1 when using Const.cds */
	expptr vleng;
	union Constant Const;
	};


struct Listblock
	{
	field tag;
	field vtype;
	chainp listp;
	};



/* Address block - this is the FINAL form of identifiers before being
   sent to pass 2.  We'll want to add the original identifier here so that it can
   be preserved in the translation.

   An example identifier is q.7.  The "q" refers to the storage class
   (field vstg), the 7 to the variable number (int memno). */

struct Addrblock
	{
	field tag;
	field vtype;
	field vclass;
	field vstg;
	expptr vleng;
	/* put union...user here so the beginning of an Addrblock
	 * is the same as a Constblock.
	 */
	union {
	    Namep name;		/* contains a pointer into the hash table */
	    char ident[IDENT_LEN + 1];	/* C string form of identifier */
	    char *Charp;
	    union Constant Const;	/* Constant value */
	    struct {
		double dfill[2];
		field vstg1;
		} kludge;	/* so we can distinguish string vs binary
				 * floating-point constants */
	} user;
	long memno;		/* when vstg == STGCONST, this is the
				   numeric part of the assembler label
				   where the constant value is stored */
	expptr memoffset;	/* used in subscript computations, usually */
	unsigned istemp:1;	/* used in stack management of temporary
				   variables */
	unsigned isarray:1;	/* used to show that memoffset is
				   meaningful, even if zero */
	unsigned ntempelt:10;	/* for representing temporary arrays, as
				   in concatenation */
	unsigned dbl_builtin:1;	/* builtin to be declared double */
	unsigned charleng:1;	/* so saveargtypes can get i/o calls right */
	unsigned cmplx_sub:1;	/* used in complex arithmetic under -s */
	unsigned skip_offset:1;	/* used in complex arithmetic under -s */
    unsigned parenused : 1;	/* distinguish (a) from a */
    unsigned is_retval : 1;	/* is return-value */
    ftnint varleng;		/* holds a copy of a constant length which
				   is stored in the   vleng   field (e.g.
				   a double is 8 bytes) */
	int uname_tag;		/* Tag describing which of the unions()
				   below to use */
    char *Field;		/* field name when dereferencing a struct */
    char * derived_type_name;		/* field name when dereferencing a struct */
}; /* struct Addrblock */


/* Errorbock - placeholder for errors, to allow the compilation to
   continue */

struct Errorblock
	{
	field tag;
	field vtype;
	};


/* Implicit DO block, especially related to DATA statements.  This block
   keeps track of the compiler's location in the implicit DO while it's
   running.  In particular, the   isactive and isbusy   flags tell where
   it is */

struct Impldoblock
	{
	field tag;
	unsigned isactive:1;
	unsigned isbusy:1;
	Namep varnp;
	Constp varvp;
	chainp impdospec;
	expptr implb;
	expptr impub;
	expptr impstep;
	ftnint impdiff;
	ftnint implim;
	struct Chain *datalist;
	};


/* Each of these components has a first field called   tag.   This union
   exists just for allocation simplicity */

union Expression
	{
	field tag;
	struct Addrblock addrblock;
	struct Constblock constblock;
	struct Errorblock errorblock;
	struct Exprblock exprblock;
	struct Headblock headblock;
	struct Impldoblock impldoblock;
	struct Listblock listblock;
	struct Nameblock nameblock;
	/*struct*/ Paramblock paramblock;
	struct Primblock primblock;
	} ;



struct Dimblock
	{
	int ndim;
	expptr nelt;		/* This is NULL if the array is unbounded */
	expptr baseoffset;	/* a constant or local variable holding
				   the offset in this procedure */
	expptr basexpr;		/* expression for comuting the offset, if
				   it's not constant.  If this is
				   non-null, the register named in
				   baseoffset will get initialized to this
				   value in the procedure's prolog */

    enum {dim_expptr_count = 4};
	struct
		{
		expptr dimsize;	/* constant or register holding the size
				   of this dimension */
		expptr dimexpr;	/* as above in basexpr, this is an
				   expression for computing a variable
				   dimension */
        expptr lb, ub;
		} dims[1];	/* Dimblocks are allocated with enough
				   space for this to become dims[ndim] */
	};


/* Statement function identifier stack - this holds the name and value of
   the parameters in a statement function invocation.  For example,

	f(x,y,z)=x+y+z
		.
		.
	y = f(1,2,3)

   generates a stack of depth 3, with <x 1>, <y 2>, <z 3> AT THE INVOCATION, NOT
   at the definition */

struct Rplblock	/* name replacement block */
	{
	struct Rplblock *rplnextp;
	Namep rplnp;		/* Name of the formal parameter */
	expptr rplvp;		/* Value of the actual parameter */
	expptr rplxp;		/* Initialization of temporary variable,
				   if required; else null */
	int rpltag;		/* Tag on the value of the actual param */
	};



/* Equivalence block */

struct Equivblock
	{
	struct Eqvchain *equivs;	/* List (Eqvchain) of primblocks
					   holding variable identifiers */
	flag eqvinit;
	long int eqvtop;
	long int eqvbottom;
	int eqvtype;
	} ;
#define eqvleng eqvtop

extern struct Equivblock *eqvclass;


struct Eqvchain
	{
	struct Eqvchain *eqvnextp;
	union
		{
		struct Primblock *eqvlhs;
		Namep eqvname;
		} eqvitem;
	long int eqvoffset;
	} ;



/* For allocation purposes only, and to keep lint quiet.  In particular,
   don't count on the tag being able to tell you which structure is used */


/* There is a tradition in Fortran that the compiler not generate the same
   bit pattern more than is necessary.  This structure is used to do just
   that; if two integer constants have the same bit pattern, just generate
   it once.  This could be expanded to optimize without regard to type, by
   removing the type check in   putconst()   */

struct Literal
	{
	short littype;
	short litnum;			/* numeric part of the assembler
					   label for this constant value */
	int lituse;		/* usage count */
	union	{
		ftnint litival;
		double litdval[2];
		ftnint litival2[2];	/* length, nblanks for strings */
		} litval;
	char *cds[2];
	};

extern struct Literal *litpool;
extern int maxliterals, nliterals;
extern char Letters[];
#define letter(x) Letters[x]

struct Dims { expptr lb, ub; };


/* popular functions with non integer return values */


int *ckalloc();
char *addunder(char *s, bool force);
char *varstr(), *nounder();
char *copyn(int n, char *s);
char *copys(char *s);
chainp revchain(chainp next);
chainp hookup(chainp x, chainp y);
chainp mkchain(char * p, chainp q);

ftnint convci(int n, char *s);
char *convic(ftnint n);
char *setdoto();
double convcd();
struct Labelblock *mklabel(ftnint l);
struct Labelblock *execlab();
Extsym *mkext(char *f, char *s);
Extsym *newentry(Namep v, int substmsg);
expptr addrof(expptr p);
expptr call0(int type, char *name);
expptr call1(int type, char *name, expptr arg);
expptr call2(int type, char *name, expptr arg1, expptr arg2);
expptr call3(int type, char *name, expptr arg1, expptr arg2, expptr arg3);
expptr call4(int type, char *name, expptr arg1, expptr arg2, expptr arg3, expptr arg4);

Addrp builtin(int t, char *s, int dbi);
Addrp autovar();
Addrp mktmpn(int nelt, int type, expptr lengp);
Addrp mktmp0(int type, expptr lengp);
Addrp mktmp(int type, expptr lengp);
Addrp mkplace(Namep np);
Addrp putconst(Constp p);
Addrp memversion(Namep np);
Addrp mkaddr(Namep p);
expptr mkprim(Namep v0, struct Listblock *args, chainp substr);
expptr mklhs(Primblock *p, int subkeep);
expptr mkexpr(int opcode, expptr lp, expptr rp);
expptr convert_to_init_list(expptr e);
expptr mkopmember(int opcode, expptr lp, expptr rp);
expptr mkconv(int t, expptr p);
expptr mkfunct(expptr p0);
expptr fixexpr(Exprp p);
expptr fixtype(tagptr p);
expptr errnode(), mkaddcon();
expptr putcxop(expptr p);
expptr mkintcon(ftnint l);
tagptr cpexpr(tagptr p);
ftnint lmin(ftnint a, ftnint b);
ftnint lmax(ftnint a, ftnint b);
ftnint iarrlen(Namep q);
char *dbconst();
char *flconst(char *buf, char *x);
extern void freetemps();
void make_param(Paramblock *p, int attribute, expptr expr);
void putex1 ();
void puteq(expptr lp, expptr rp);
expptr putx (expptr);
expptr putsteq(Addrp a, Addrp b);
expptr putassign(expptr lp, expptr rp);
extern int forcedouble;		/* force real functions to double */
extern int doin_setbound;	/* special handling for array bounds */
extern int Ansi;

char * cds(char *s, char *z0);
char *string_num(char *pfx, long n);
char *dtos(double x);
extern char *cpstring();
std::string c_type_decl(int type, int is_extern, bool is_const_param);
std::string cpp_type_decl(Namep type, int is_extern, bool is_const_param);
std::string cpp2_type_decl(proctype_t type, int is_extern, bool is_const_param);
std::string cpp3_type_decl(int type, const char* name, int is_extern, bool is_const_param);
extern char hextoi_tab[];
#define hextoi(x) hextoi_tab[(x) & 0xff]
extern char *casttypes[], *ftn_types[], *protorettypes[], *usedcasts[];
extern int Castargs, infertypes;
extern FILE *protofile;
void inferdcl(Namep np, int type);
extern void exit(), save_argtypes();
void protowrite(FILE *protofile, proctype_t type, char *name, struct Entrypoint *e, chainp lengths);
extern char binread[], binwrite[], textread[], textwrite[];
extern char *ei_first, *ei_last, *ei_next;
extern char *wh_first, *wh_last, *wh_next;
extern void putwhile(expptr p);
extern char *halign;
extern flag keepsubs;
#ifdef TYQUAD
extern flag use_tyquad;
#endif
extern int n_keywords, n_st_fields;
extern char *c_keywords[], *st_fields[];
extern int n_cpp_keywords;
extern char *cpp_keywords[];
extern char *fortran_externals_keywords[];
extern int n_fortran_externals_keywords;

struct conseval_t
{
    conseval_t(){}
    conseval_t(int n)
        :state(state_::int_)
        , n(n)
    {}

    conseval_t(double d)
        :state(state_::double_)
        , d(d)
    {}

    enum class state_{
        nothing,
        int_,
        double_
    };

    conseval_t abs() const{ 
        switch(state)
        {\
        default: return conseval_t{};
        case state_::int_: return conseval_t{::abs(n)};
        case state_::double_: return conseval_t{::abs(d)};
        }
    }


    conseval_t pow(conseval_t const& r) const{ 
        switch(state)
        {
        default: return conseval_t{};
        case state_::int_: 
            switch(r.state)
            {
            default: return conseval_t{};
            case state_::int_: 
                {
                    return conseval_t{(int)::pow(n,r.n)};
                }
                break;
            case state_::double_: return conseval_t{::pow(n,r.d)};
            }
        case state_::double_: 
            switch(r.state)
            {
            default: return conseval_t{};
            case state_::int_: return conseval_t{::pow(d,r.n)};
            case state_::double_: return conseval_t{::pow(d,r.d)};
            }
        }
    }



#define MAKE_UNARY_OP(op)    conseval_t operator op() const{ \
        switch(state)\
        {\
        default: return conseval_t{};\
        case state_::int_: return conseval_t{op n};\
        case state_::double_: return conseval_t{op d};\
        }\
    }

#define MAKE_UNARY_OP_INT(op)    conseval_t operator op() const{ \
        switch(state)\
        {\
        default: return conseval_t{};\
        case state_::int_: return conseval_t{op n};\
        }\
    }

    MAKE_UNARY_OP(!);
    MAKE_UNARY_OP_INT(~);
    MAKE_UNARY_OP(-);


#define MAKE_BINARY_OP(op)    conseval_t operator op(conseval_t const& r) const{ \
        switch(state)\
        {\
        default: return conseval_t{};\
        case state_::int_: \
            switch(r.state)\
            {\
            default: return conseval_t{};\
            case state_::int_: return conseval_t{n op r.n};\
            case state_::double_: return conseval_t{n op r.d};\
            }\
        case state_::double_: \
            switch(r.state)\
            {\
            default: return conseval_t{};\
            case state_::int_: return conseval_t{d op r.n};\
            case state_::double_: return conseval_t{d op r.d};\
            }\
        }\
    }

#define MAKE_INT_BINARY_OP(op)    conseval_t operator op(conseval_t const& r) const{ \
        switch(state)\
        {\
        default: return conseval_t{};\
        case state_::int_: \
            switch(r.state)\
            {\
            default: return conseval_t{};\
            case state_::int_: return conseval_t{n op r.n};\
            }\
        }\
    }

    MAKE_BINARY_OP(&&);
    MAKE_BINARY_OP(|| );
    MAKE_BINARY_OP(== );
    MAKE_BINARY_OP(!= );
    MAKE_BINARY_OP(+ );
    MAKE_BINARY_OP(- );
    MAKE_BINARY_OP(* );
    MAKE_BINARY_OP(/ );
    MAKE_INT_BINARY_OP(&);
    MAKE_INT_BINARY_OP(|);
    MAKE_INT_BINARY_OP(^);
    MAKE_INT_BINARY_OP(<<);
    MAKE_INT_BINARY_OP(>>);
//    MAKE_BINARY_OP();

    state_ state{};
    int n{};
    double d{};
};




// types used for data-initialization
//struct lambda_init_item_data_t {
//    long long i = {};
//    double real_d = {};
//    double img_d = {};
//    std::string s;
//    void* addr = {};
//};

struct lambda_init_item_t {
    int type = {};
    int offset = {};
    int index = {};
    expptr index_expr = {};
    expptr data_expr = {};
    std::string data;
    std::string derived_type_name;
};

template<typename T>
std::string to_string(T&& t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}

struct lambda_init_t {
    bool map_required = {};
    bool lambda_required = {};
    std::vector<lambda_init_item_t> items;
};

using lambda_init_map_t = std::map<std::string, lambda_init_t>;


#endif // __DEFS_H__ 
