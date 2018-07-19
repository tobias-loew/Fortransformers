/* niceprintf.h -- contains constants and macros from the output filter
   for the generated C code.  We use macros for increased speed, less
   function overhead.  */

#define MAX_OUTPUT_SIZE 6000	/* Number of chars on one output line PLUS
				   the length of the longest string
				   printed using   nice_printf   */

extern int indent;		/* Number of spaces to indent; this is a
                        temporary fix */
extern int tab_size;		/* Number of spaces in each tab */


inline void next_tab(FILE* fp)
{
    indent += tab_size;
}

inline void prev_tab(FILE* fp)
{
    indent -= tab_size;
}



static const int tab_length = 4;
