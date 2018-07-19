struct iob_data {
	struct iob_data *next;
	char *type;
	char *name;
	char *fields[1];
	};
struct io_setup {
	char **fields;
	int nelt, type;
	};

struct defines {
	struct defines *next;
	char defname[1];
	};

typedef struct iob_data iob_data;
typedef struct io_setup io_setup;
typedef struct defines defines;

extern iob_data *iob_list;
extern struct Addrblock *io_structs[9];
void def_start(FILE *outfile, char *s1, char *s2, char *post);
void new_iob_data(io_setup *ios, char *name);
extern void other_undefs();
char *tostring(char *s, int n);
