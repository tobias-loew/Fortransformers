/* nice_printf -- same arguments as fprintf.

	All output which is to become C code must be directed through this
   function.  For now, no buffering is done.  Later on, every line of
   output will be filtered to accomodate the style definitions (e.g. one
   statement per line, spaces between function names and argument lists,
   etc.)
*/
#include "niceprintf.h"

extern int nice_printf ();


/* Definitions for the opcode table.  The table is indexed by the macros
   which are #defined in   defines.h   */

#define UNARY_OP 01
#define BINARY_OP 02

#define SPECIAL_FMT NULL

#define is_unary_op(x) (opcode_table[x].type == UNARY_OP)
#define is_binary_op(x) (opcode_table[x].type == BINARY_OP)
#define op_precedence(x) (opcode_table[x].prec)
#define op_format(x) (opcode_table[x].format)

/* _assoc_table -- encodes left-associativity and right-associativity
   information; indexed by precedence level.  Only 2, 3, 14 are
   right-associative.  Source:  Kernighan & Ritchie, p. 49 */

extern char _assoc_table[];

#define is_right_assoc(x) (_assoc_table [x])
#define is_left_assoc(x) (! _assoc_table [x])


typedef struct {
    int type;			/* UNARY_OP or BINARY_OP */
    int prec;			/* Precedence level, useful for adjusting
				   number of parens to insert.  Zero is a
				   special level, and 2, 3, 14 are
				   right-associative */
    char *format;
} table_entry;


extern char *fl_fmt_string;	/* Float constant format string */
extern char *db_fmt_string;	/* Double constant format string */
extern char *cm_fmt_string;	/* Complex constant format string */
extern char *dcm_fmt_string;	/* Double Complex constant format string */

extern int indent;		/* Number of spaces to indent; this is a
				   temporary fix */
extern int tab_size;		/* Number of spaces in each tab */
extern int in_string;

extern table_entry opcode_table[];


void out_addr(FILE *fp, struct Addrblock *addrp);
void out_const(FILE *fp, Constp cp);
void expr_out(FILE *fp, expptr e);
void out_init();
void out_name(FILE *fp, Namep namep);
void extern_out(FILE *fp, Extsym *extsym);
void out_asgoto(FILE *outfile, expptr expr);
void out_if(FILE *outfile, expptr expr);
void out_else(FILE *outfile);
void elif_out(FILE *outfile, expptr expr);
void endif_out(FILE *outfile);
void end_else_out(FILE *outfile);
void compgoto_out(FILE *outfile, expptr index, expptr labels);
void out_for(FILE *outfile, expptr init, expptr test, expptr inc);
void out_end_for(FILE *outfile, long needs_breakout_for, int loops_count);
void out_switch(FILE *outfile, expptr value);
void out_end_switch(FILE *outfile);
void out_case(FILE *outfile, expptr case_, expptr lower, expptr upper);
void out_case_open(FILE *outfile);
void out_end_case(FILE *outfile);
void out_and_free_statement();
