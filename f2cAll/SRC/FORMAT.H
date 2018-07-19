#define DEF_C_LINE_LENGTH 120
#define DEF_C_COMMENT_LINE_LENGTH 100
/* actual max will be 79 */

extern int c_output_line_length;	/* max # chars per line in C source
					   code */
extern int c_comment_output_line_length;	/* max comment # chars per line in C source
					   code */

char *wr_ardecls(FILE *outfile, struct Dimblock *dimp, long size);
void wr_output_values(FILE *outfile, Namep namep, chainp values);
void list_init_data(FILE **Infile, char *Inname, FILE *outfile);
void wr_one_init(FILE *outfile, char *varname, chainp *Values, int keepit);
int do_init_data(FILE *outfile, FILE *infile);
chainp data_value(FILE *infile, ftnint offset, int type);

