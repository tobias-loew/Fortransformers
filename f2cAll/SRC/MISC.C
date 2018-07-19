/****************************************************************
Copyright 1990, 1992, 1993 by AT&T Bell Laboratories and Bellcore.

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

#include "defs.h"
#include "func_def.h"
#include <set>
#include <map>

extern bool in_type_definition;
extern bool in_member_access; 
int in_vector(char *str, char **keywds, int n);
int newlabel();
void add_extern_to_list(Addrp addr, chainp *list_store);



int oneof_stg (Namep name, int stg, int mask)
 //Namep name;
 //int stg, mask;
{
	if (stg == STGCOMMON && name) {
		if ((mask & M(STGEQUIV)))
			return name->vcommequiv;
		if ((mask & M(STGCOMMON)))
			return !name->vcommequiv;
		}
	return ONEOF(stg, mask);
	}


/* op_assign -- given a binary opcode, return the associated assignment
   operator */

int op_assign (int opcode)
//int opcode;
{
    int retval = -1;

    switch (opcode) {
        case OPPLUS: retval = OPPLUSEQ; break;
	case OPMINUS: retval = OPMINUSEQ; break;
	case OPSTAR: retval = OPSTAREQ; break;
	case OPSLASH: retval = OPSLASHEQ; break;
	case OPMOD: retval = OPMODEQ; break;
	case OPLSHIFT: retval = OPLSHIFTEQ; break;
	case OPRSHIFT: retval = OPRSHIFTEQ; break;
	case OPBITAND: retval = OPBITANDEQ; break;
	case OPBITXOR: retval = OPBITXOREQ; break;
	case OPBITOR: retval = OPBITOREQ; break;
	default:
	    erri ("op_assign:  bad opcode '%d'", opcode);
	    break;
    } /* switch */

    return retval;
} /* op_assign */


 char *
Alloc(int n)	/* error-checking version of malloc */
		/* ckalloc initializes memory to 0; Alloc does not */
 //int n;
{
	char errbuf[32];
	register char *rv;

	rv = (char*)malloc(n);
	if (!rv) {
		sprintf(errbuf, "malloc(%d) failure!", n);
		Fatal(errbuf);
		}
	return rv;
	}


void cpn(int n, char *a, char *b)
//register int n;
//register char *a, *b;
{
	while(--n >= 0)
		*b++ = *a++;
}



int eqn(int n, char *a, char *b)
//register int n;
//register char *a, *b;
{
	while(--n >= 0)
		if(*a++ != *b++)
			return(NO);
	return(YES);
}







int cmpstr(char *a, char *b, ftnint la, ftnint lb)	/* compare two strings */
//register char *a, *b;
//ftnint la, lb;
{
	register char *aend, *bend;
	aend = a + la;
	bend = b + lb;


	if(la <= lb)
	{
		while(a < aend)
			if(*a != *b)
				return( *a - *b );
			else
			{
				++a;
				++b;
			}

		while(b < bend)
			if(*b != ' ')
				return(' ' - *b);
			else
				++b;
	}

	else
	{
		while(b < bend)
			if(*a != *b)
				return( *a - *b );
			else
			{
				++a;
				++b;
			}
		while(a < aend)
			if(*a != ' ')
				return(*a - ' ');
			else
				++a;
	}
	return(0);
}


/* hookup -- Same as LISP NCONC, that is a destructive append of two lists */

chainp hookup(chainp x, chainp y)
//register chainp x, y;
{
	register chainp p;

	if(x == NULL)
		return(y);

	for(p = x ; p->nextp ; p = p->nextp)
		;
	p->nextp = y;
	return(x);
}



struct Listblock *mklist(chainp p)
//chainp p;
{
	register struct Listblock *q;

	q = ALLOC(Listblock);
	q->tag = TLIST;
	q->listp = p;
	return(q);
}


chainp mkchain(char * p, chainp q)
//register char * p;
//register chainp q;
{
	register chainp r;

	if(chains)
	{
		r = chains;
		chains = chains->nextp;
	}
	else
		r = ALLOC(Chain);

	r->datap = p;
	r->nextp = q;
	return(r);
}

 chainp
revchain(chainp next)
// register chainp next;
{
	register chainp p, prev = 0;

	while(p = next) {
		next = p->nextp;
		p->nextp = prev;
		prev = p;
		}
	return prev;
	}


/* addunder -- turn a cvarname into an external name */
/* The cvarname may already end in _ (to avoid C keywords); */
/* if not, it has room for appending an _. */

 char *
addunder(char *s, bool force)
// register char *s;
{
     if(cxx && !force)
     {
         return s;
     }
	register int c, i;
	char *s0 = s;

	i = 0;
	while(c = *s++)
		if (c == '_')
			i++;
		else
			i = 0;
	if (!i) {
		*s-- = 0;
		*s = '_';
		}
	return( s0 );
	}


/* copyn -- return a new copy of the input Fortran-string */

char *copyn(int n, char *s)
//register int n;
//register char *s;
{
	register char *p, *q;

	p = q = (char *) Alloc(n);
	while(--n >= 0)
		*q++ = *s++;
	return(p);
}



/* copys -- return a new copy of the input C-string */

char *copys(char *s)
//char *s;
{
	return( copyn( strlen(s)+1 , s) );
}



/* convci -- Convert Fortran-string to integer; assumes that input is a
   legal number, with no trailing blanks */

ftnint convci(int n, char *s)
//register int n;
//register char *s;
{
	ftnint sum;
	sum = 0;
	while(n-- > 0)
		sum = 10*sum + (*s++ - '0');
	return(sum);
}

/* convic - Convert Integer constant to string */

char *convic(ftnint n)
//ftnint n;
{
	static char s[20];
	register char *t;

	s[19] = '\0';
	t = s+19;

	do	{
		*--t = '0' + n%10;
		n /= 10;
	} while(n > 0);

	return(t);
}


// support for fortran include of files containing COMMON-blocks
// the c++ structs and static variables have to be defined manually
bool check_include(std::string name, includes_enum include, std::string& include_prefix)
{
    // namespace of global static variables (e.g. "fg" for fortran-globals)
    static const std::string global_ns = "fg::";

    using map_inc_t = std::map<std::string, std::string>;

    // define  one map per Fortran include file
    // e.g. for include file params.inc
    static const map_inc_t common_params =
    {
//    var-name,    static variable of struct
    {"par1",       "PARAMS."},
    {"par2",     "PARAMS."},

    {"name",      "USER."},
    {"id",        "USER."},
    //...
    };

    // e.g. for include file data.inc
    static const map_inc_t common_data =
    {
        //    var-name,    static variable of struct
        { "init1",       "INIT." },
        { "init2",     "INIT." },

        //...
    };


    static const std::map<includes_enum, map_inc_t const*> maps = {
        {include_common_params, &common_params },
        {include_common_data, &common_data },
    };

    static const bool valid_check = [&]()
    {
        std::set<std::string> tester;
        for(auto&& m : maps)
        {
            for(auto&& e : *m.second)
            {
                if(tester.count(e.first))
                {
                    assert(false);
                    return false;
                }
                tester.insert(e.first);
            }
        }
        return true;
    }();

    auto map = maps.find(include);
    if(map != maps.end())
    {
        auto it = map->second->find(name);
        if(it != map->second->end())
        {
            include_prefix = global_ns + it->second;
            return true;
        }
    }
    return false;
}


/* mkname -- add a new identifier to the environment, including the closed
   hash table. */

Namep mkname(char *s)
//register char *s;
{
    if(in_interface_definition) {
        return nullptr;
    }

	struct Hashentry *hp;
	register Namep q;
	register int c, hash, i, j;
	register char *t;
	char *s0;
	char errbuf[64];

	hash = i = j = 0;
    s0 = s;
    std::string include_prefix;

    if(!in_type_definition && !in_member_access)
    {
        // check for commebs.inc, etc.
        for(int include = 0; include < includes__SIZE; ++include)
        {
            if(seen_includes[include])
            {
                if(check_include(s0, (includes_enum)include, include_prefix))
                {
                    break;
                }
            }
        }
    }

    while(c = *s++) {
        hash += c;
        if(!cxx && c == '_')
            i = 2;
    }

    if(include_prefix.empty())
    {

        if(!i && in_vector(s0, c_keywords, n_keywords) >= 0)
        {
            i = 1;
        }
        if(in_vector(s0, cpp_keywords, n_cpp_keywords) >= 0)
        {
            j = 1;
        }
    }


    hash %= maxhash;

/* Add the name to the closed hash table */
    auto local_hashtab = in_type_definition
        ? type_hashtab
        : in_member_access 
            ? member_hashtab 
            : hashtab
        ;

    auto local_lasthash = in_type_definition
        ? type_lasthash
        : in_member_access
            ? member_lasthash
            : lasthash
        ;

    hp = local_hashtab + hash;

    while(q = hp->varp)
        if(hash == hp->hashval && !strcmp(s0, q->fvarname))
            return(q);
        else if(++hp >= local_lasthash)
            hp = local_hashtab;

    if(++nintnames >= maxhash-1)
		many("names", 'n', maxhash);	/* Fatal error */
	hp->varp = q = ALLOC(Nameblock);
    hp->varp->hash_index = hash_counter++;

	hp->hashval = hash;
	q->tag = TNAME;	/* TNAME means the tag type is NAME */
	c = s - s0;
	if (c > 7 && noextflag) {
		sprintf(errbuf, "\"%.35s%s\" over 6 characters long", s0,
			c > 36 ? "..." : "");
		errext(errbuf);
		}


    bool ft_external = false;
    if(include_prefix.empty())
    {
        if(cxx && i == 0)
        {
            if(in_vector(s0, fortran_externals_keywords, n_fortran_externals_keywords) >= 0)
            {
                ft_external = true;
            }
        }
    }


	q->fvarname = strcpy(mem(c,0), s0);

    if(include_prefix.empty())
    {
        t = q->cvarname = mem(c + (cxx?(i?4:0):i) + 1 + j + (ft_external?10:0), 0);
    }
    else
    {
        t = q->cvarname = mem(c + include_prefix.length(), 0);
    }
    s = s0;
	/* add __ to the end of any name containing _ and to any C keyword */
	while(*t = *s++)
		t++;
	if (i) {
        if(cxx)
        {
            std::string s1 = "ft::";
            s1 += s0;
            for(int j_ = 0; j_ < j; ++j_)
            {
                s1 += '_';
            }
            strcpy(q->cvarname, s1.c_str());
        }
        else
        {
            do *t++ = '_';
            while(--i > 0);
            *t = 0;
        }
    }
    else if(ft_external)
    {
        std::string s1 = "FORT_EXT(";
        s1 += s0;
        s1 += ")";
        std::transform(s1.begin(), s1.end(), s1.begin(), ::toupper);
        strcpy(q->cvarname, s1.c_str());
    }
    else if(!include_prefix.empty())
    {
        std::string s1 = include_prefix;
        s1 += s0;
        strcpy(q->cvarname, s1.c_str());
    }
    return(q);
}


struct Labelblock *mklabel(ftnint l)
//ftnint l;
{
	register struct Labelblock *lp;

	if(l <= 0)
		return(NULL);

	for(lp = labeltab ; lp < highlabtab ; ++lp)
		if(lp->stateno == l)
			return(lp);

	if(++highlabtab > labtabend)
		many("statement labels", 's', maxstno);

	lp->stateno = l;
	lp->labelno = newlabel();
	lp->blklevel = 0;
	lp->labused = NO;
	lp->fmtlabused = NO;
	lp->labdefined = NO;
	lp->labinacc = NO;
	lp->labtype = LABUNKNOWN;
	lp->fmtstring = 0;
	return(lp);
}


int newlabel()
{
	return( ++lastlabno );
}


/* this label appears in a branch context */

struct Labelblock *execlab(ftnint stateno)
//ftnint stateno;
{
	register struct Labelblock *lp;

	if(lp = mklabel(stateno))
	{
		if(lp->labinacc)
			warn1("illegal branch to inner block, statement label %s",
			    convic(stateno) );
		else if(lp->labdefined == NO)
			lp->blklevel = blklevel;
		if(lp->labtype == LABFORMAT)
			err("may not branch to a format");
		else
			lp->labtype = LABEXEC;
	}
	else
		execerr("illegal label %s", convic(stateno));

	return(lp);
}


/* find or put a name in the external symbol table */

Extsym *mkext(char *f, char *s)
//char *f, *s;
{
	Extsym *p;

	for(p = extsymtab ; p<nextext ; ++p)
		if(!strcmp(s,p->cextname))
			return( p );

	if(nextext >= lastext)
		many("external symbols", 'x', maxext);

	nextext->fextname = strcpy(gmem(strlen(f)+1,0), f);
	nextext->cextname = f == s
				? nextext->fextname
				: strcpy(gmem(strlen(s)+1,0), s);
	nextext->extstg = STGUNKNOWN;
	nextext->extp = 0;
	nextext->allextp = 0;
	nextext->extleng = 0;
	nextext->maxleng = 0;
	nextext->extinit = 0;
	nextext->curno = nextext->maxno = 0;
	return( nextext++ );
}


Addrp builtin(int t, char *s, int dbi)
//int t, dbi;
//char *s;
{
	register Extsym *p;
	register Addrp q;
	extern chainp used_builtins;

	p = mkext(s,s);
	if(p->extstg == STGUNKNOWN)
		p->extstg = STGEXT;
	else if(p->extstg != STGEXT)
	{
		errstr("improper use of builtin %s", s);
		return(0);
	}

	q = ALLOC(Addrblock);
	q->tag = TADDR;
	q->vtype = t;
	q->vclass = CLPROC;
	q->vstg = STGEXT;
	q->memno = p - extsymtab;
	q->dbl_builtin = dbi;

/* A NULL pointer here tells you to use   memno   to check the external
   symbol table */

	q -> uname_tag = UNAM_EXTERN;

/* Add to the list of used builtins */

	if (dbi >= 0)
		add_extern_to_list (q, &used_builtins);
	return(q);
}



void add_extern_to_list (Addrp addr, chainp *list_store)
//Addrp addr;
//chainp *list_store;
{
    chainp last = CHNULL;
    chainp list;
    int memno;

    if (list_store == (chainp *) NULL || addr == (Addrp) NULL)
	return;

    list = *list_store;
    memno = addr -> memno;

    for (;list; last = list, list = list -> nextp) {
	Addrp this_ = (Addrp) (list -> datap);

	if (this_ -> tag == TADDR && this_ -> uname_tag == UNAM_EXTERN &&
		this_ -> memno == memno)
	    return;
    } /* for */

    if (*list_store == CHNULL)
	*list_store = mkchain((char *)cpexpr((expptr)addr), CHNULL);
    else
	last->nextp = mkchain((char *)cpexpr((expptr)addr), CHNULL);

} /* add_extern_to_list */


void frchain(chainp* const p)
//register chainp *p;
{
    register chainp q;

    if(p == 0 || *p == 0)
        return;

    for(q = *p; q->nextp; q = q->nextp)
        ;
    q->nextp = chains;
    chains = *p;
    *p = 0;
}

void frchain2(chainp p)
//register chainp *p;
{
    register chainp q;

    if(p == 0 )
        return;

    for(q = p; q->nextp; q = q->nextp)
        ;
    q->nextp = chains;
    chains = p;
}

 void
     frexchain(chainp *p)
      //register chainp *p;
 {
     register chainp q, r;

     if(q = *p) {
         for(;; q = r) {
             frexpr((expptr)q->datap);
             if(!(r = q->nextp))
                 break;
         }
         q->nextp = chains;
         chains = *p;
         *p = 0;
     }
 }


tagptr cpblock(int n, char * p)
//register int n;
//register char * p;
{
	register ptr q;

	memcpy((char *)(q = ckalloc(n)), (char *)p, n);
	return( (tagptr) q);
}



ftnint lmax(ftnint a, ftnint b)
//ftnint a, b;
{
	return( a>b ? a : b);
}

ftnint lmin(ftnint a, ftnint b)
//ftnint a, b;
{
	return(a < b ? a : b);
}




int maxtype(int t1, int t2)
//int t1, t2;
{
	int t;

	t = t1 >= t2 ? t1 : t2;
	if(t==TYCOMPLEX && (t1==TYDREAL || t2==TYDREAL) )
		t = TYDCOMPLEX;
	return(t);
}



/* return log base 2 of n if n a power of 2; otherwise -1 */
int log_2(ftnint n)
//ftnint n;
{
	int k;

	/* trick based on binary representation */

	if(n<=0 || (n & (n-1))!=0)
		return(-1);

	for(k = 0 ;  n >>= 1  ; ++k)
		;
	return(k);
}



void frrpl()
{
	struct Rplblock *rp;

	while(rpllist)
	{
		rp = rpllist->rplnextp;
		free( (charptr) rpllist);
		rpllist = rp;
	}
}



/* Call a Fortran function with an arbitrary list of arguments */

int callk_kludge;

expptr callk(int type, char *name, chainp args)
//int type;
//char *name;
//chainp args;
{
	register expptr p;

	p = mkexpr(OPCALL,
		(expptr)builtin(callk_kludge ? callk_kludge : type, name, 0),
		(expptr)args);
	p->exprblock.vtype = type;
	return(p);
}



expptr call4(int type, char *name, expptr arg1, expptr arg2, expptr arg3, expptr arg4)
//int type;
//char *name;
//expptr arg1, arg2, arg3, arg4;
{
	struct Listblock *args;
	args = mklist( mkchain((char *)arg1,
			mkchain((char *)arg2,
				mkchain((char *)arg3,
	    				mkchain((char *)arg4, CHNULL)) ) ) );
	return( callk(type, name, (chainp)args) );
}




expptr call3(int type, char *name, expptr arg1, expptr arg2, expptr arg3)
//int type;
//char *name;
//expptr arg1, arg2, arg3;
{
	struct Listblock *args;
	args = mklist( mkchain((char *)arg1,
			mkchain((char *)arg2,
				mkchain((char *)arg3, CHNULL) ) ) );
	return( callk(type, name, (chainp)args) );
}





expptr call2(int type, char *name, expptr arg1, expptr arg2)
//int type;
//char *name;
//expptr arg1, arg2;
{
	struct Listblock *args;

	args = mklist( mkchain((char *)arg1, mkchain((char *)arg2, CHNULL) ) );
	return( callk(type,name, (chainp)args) );
}




expptr call1(int type, char *name, expptr arg)
//int type;
//char *name;
//expptr arg;
{
	return( callk(type,name, (chainp)mklist(mkchain((char *)arg,CHNULL)) ));
}


expptr call0(int type, char *name)
//int type;
//char *name;
{
	return( callk(type, name, CHNULL) );
}



struct Impldoblock *mkiodo(chainp dospec, chainp list)
//chainp dospec, list;
{
	register struct Impldoblock *q;

	q = ALLOC(Impldoblock);
	q->tag = TIMPLDO;
	q->impdospec = dospec;
	q->datalist = list;
	return(q);
}




/* ckalloc -- Allocate 1 memory unit of size   n,   checking for out of
   memory error */

ptr ckalloc(int n)
//register int n;
{
	register ptr p;
	p = (ptr)calloc(1, (unsigned) n);
	if (p || !n)
		return(p);
	fprintf(stderr, "failing to get %d bytes\n",n);
	Fatal("out of memory");
	/* NOT REACHED */ return 0;
}



int isaddr(expptr p)
//register expptr p;
{
	if(p->tag == TADDR)
		return(YES);
	if(p->tag == TEXPR)
		switch(p->exprblock.opcode)
		{
		case OPCOMMA:
			return( isaddr(p->exprblock.rightp) );

		case OPASSIGN:
		case OPASSIGNI:
		case OPPLUSEQ:
		case OPMINUSEQ:
		case OPSLASHEQ:
		case OPMODEQ:
		case OPLSHIFTEQ:
		case OPRSHIFTEQ:
		case OPBITANDEQ:
		case OPBITXOREQ:
		case OPBITOREQ:
			return( isaddr(p->exprblock.leftp) );
		}
	return(NO);
}




int isstatic(expptr p)
//register expptr p;
{
	extern int useauto;
	if(p->headblock.vleng && !ISCONST(p->headblock.vleng))
		return(NO);

	switch(p->tag)
	{
	case TCONST:
		return(YES);

	case TADDR:
		if(!useauto && ONEOF(p->addrblock.vstg,MSKSTATIC) &&
		    ISCONST(p->addrblock.memoffset))
			return(YES);

	default:
		return(NO);
	}
}



/* addressable -- return True iff it is a constant value, or can be
   referenced by constant values */

int addressable(expptr p)
//register expptr p;
{
	switch(p->tag)
	{
	case TCONST:
		return(YES);

	case TADDR:
		return( addressable(p->addrblock.memoffset) );

	default:
		return(NO);
	}
}


/* isnegative_const -- returns true if the constant is negative.  Returns
   false for imaginary and nonnumeric constants */

int isnegative_const (struct Constblock *cp)
//struct Constblock *cp;
{
    int retval;

    if (cp == NULL)
	return 0;

    switch (cp -> vtype) {
	case TYINT1:
        case TYSHORT:
	case TYLONG:
#ifdef TYQUAD
	case TYQUAD:
#endif
	    retval = cp -> Const.ci < 0;
	    break;
	case TYREAL:
	case TYDREAL:
		retval = cp->vstg ? *cp->Const.cds[0] == '-'
				  :  cp->Const.cd[0] < 0.0;
	    break;
	default:

	    retval = 0;
	    break;
    } /* switch */

    return retval;
} /* isnegative_const */

void negate_const(Constp cp)
 //Constp cp;
{
    if (cp == (struct Constblock *) NULL)
	return;

    switch (cp -> vtype) {
	case TYINT1:
	case TYSHORT:
	case TYLONG:
#ifdef TYQUAD
	case TYQUAD:
#endif
	    cp -> Const.ci = - cp -> Const.ci;
	    break;
	case TYCOMPLEX:
	case TYDCOMPLEX:
		if (cp->vstg)
		    switch(*cp->Const.cds[1]) {
			case '-':
				++cp->Const.cds[1];
				break;
			case '0':
				break;
			default:
				--cp->Const.cds[1];
			}
		else
	    		cp->Const.cd[1] = -cp->Const.cd[1];
		/* no break */
	case TYREAL:
	case TYDREAL:
		if (cp->vstg)
		    switch(*cp->Const.cds[0]) {
			case '-':
				++cp->Const.cds[0];
				break;
			case '0':
				break;
			default:
				--cp->Const.cds[0];
			}
		else
	    		cp->Const.cd[0] = -cp->Const.cd[0];
	    break;
	case TYCHAR:
	case TYLOGICAL1:
	case TYLOGICAL2:
	case TYLOGICAL:
	    erri ("negate_const:  can't negate type '%d'", cp -> vtype);
	    break;
	default:
	    erri ("negate_const:  bad type '%d'",
		    cp -> vtype);
	    break;
    } /* switch */
} /* negate_const */

void ffilecopy (FILE *infp, FILE *outfp)
//FILE *infp, *outfp;
{
    while (!feof (infp)) {
	register int c = getc (infp);
	if (!feof (infp))
	putc (c, outfp);
    } /* while */
} /* ffilecopy */


/* in_vector -- verifies whether   str   is in c_keywords.
   If so, the index is returned else  -1  is returned.
   c_keywords must be in alphabetical order (as defined by strcmp).
*/

int in_vector(char *str, char **keywds, int n)
//char *str; char **keywds; register int n;
{
	register char **K = keywds;
	register int n1, t;

	do {
		n1 = n >> 1;
		if (!(t = strcmp(str, K[n1])))
			return K - keywds + n1;
		if (t < 0)
			n = n1;
		else {
			n -= ++n1;
			K += n1;
			}
		}
		while(n > 0);

	return -1;
	} /* in_vector */


int is_negatable (Constp Const)
//Constp Const;
{
    int retval = 0;
    if (Const != (Constp) NULL)
	switch (Const -> vtype) {
	    case TYINT1:
		retval = Const -> Const.ci >= -BIGGEST_CHAR;
		break;
	    case TYSHORT:
	        retval = Const -> Const.ci >= -BIGGEST_SHORT;
	        break;
	    case TYLONG:
#ifdef TYQUAD
	    case TYQUAD:
#endif
	        retval = Const -> Const.ci >= -BIGGEST_LONG;
	        break;
	    case TYREAL:
	    case TYDREAL:
	    case TYCOMPLEX:
	    case TYDCOMPLEX:
	        retval = 1;
	        break;
	    case TYLOGICAL1:
	    case TYLOGICAL2:
	    case TYLOGICAL:
	    case TYCHAR:
	    case TYSUBR:
	    default:
	        retval = 0;
	        break;
	} /* switch */

    return retval;
} /* is_negatable */

void backup(char *fname, char *bname)
 //char *fname, *bname;
{
	FILE *b, *f;
	static char couldnt[] = "Couldn't open %.80s";

	if (!(f = fopen(fname, binread))) {
		warn1(couldnt, fname);
		return;
		}
	if (!(b = fopen(bname, binwrite))) {
		warn1(couldnt, bname);
		return;
		}
	ffilecopy(f, b);
	fclose(f);
	fclose(b);
	}


/* struct_eq -- returns YES if structures have the same field names and
   types, NO otherwise */

int struct_eq (chainp s1, chainp s2)
//chainp s1, s2;
{
    struct Dimblock *d1, *d2;
    Constp cp1, cp2;

    if (s1 == CHNULL && s2 == CHNULL)
	return YES;
    for(; s1 && s2; s1 = s1->nextp, s2 = s2->nextp) {
	register Namep v1 = (Namep) s1 -> datap;
	register Namep v2 = (Namep) s2 -> datap;

	if (v1 == (Namep) NULL || v1 -> tag != TNAME ||
		v2 == (Namep) NULL || v2 -> tag != TNAME)
	    return NO;

	if (v1->vtype != v2->vtype || v1->vclass != v2->vclass
		|| strcmp(v1->fvarname, v2->fvarname))
	    return NO;

	/* compare dimensions (needed for comparing COMMON blocks) */

	if (d1 = v1->vdim) {
		if (!(cp1 = (Constp)d1->nelt) || cp1->tag != TCONST)
			return NO;
		if (!(d2 = v2->vdim))
			if (cp1->Const.ci == 1)
				continue;
			else
				return NO;
		if (!(cp2 = (Constp)d2->nelt) || cp2->tag != TCONST
		||  cp1->Const.ci != cp2->Const.ci)
			return NO;
		}
	else if ((d2 = v2->vdim) && (!(cp2 = (Constp)d2->nelt)
				|| cp2->tag != TCONST
				|| cp2->Const.ci != 1))
		return NO;
    } /* while s1 != CHNULL && s2 != CHNULL */

    return s1 == CHNULL && s2 == CHNULL;
} /* struct_eq */


int length_from_name(const char* name)
{
    struct type_length_name_t {
        std::string name;
        int length_index;
    };
    static const type_length_name_t type_length_names[] = {
        {"c_int", 4},
        {"c_short", 2},
        {"c_long", 4},
        {"c_long_long", 8},
        {"c_signed_char", 1},
        {"c_size_t", 128 },
        {"c_int8_t", 1},
        {"c_int16_t", 2},
        {"c_int32_t", 4},
        {"c_int64_t", 8},
        {"c_int128_t", 16},
        {"c_int_least8_t", 1},
        {"c_int_least16_t", 2},
        {"c_int_least32_t", 4},
        {"c_int_least64_t", 8},
        {"c_int_least128_t", 16},
        {"c_int_fast8_t", 1},
        {"c_int_fast16_t", 2},
        {"c_int_fast32_t", 4},
        {"c_int_fast64_t", 8},
        {"c_int_fast128_t", 16},
        {"c_intmax_t", 16},
        {"c_intptr_t", 128 },
        {"c_ptrdiff_t", 128 },
        {"c_float", 4},
        {"c_double", 8},
        {"c_long_double", 8},
        {"c_float128", 16},
        {"c_float_complex", 8},
        {"c_double_complex", 16},
        {"c_long_double_complex", 16},
        {"c_float128_complex", 32},
        {"c_bool", 1},
        {"c_char", 1},
        { "int64", 8 },
        { "real128", 16 },
    };

    for (auto&& item : type_length_names) {
        if (item.name == name) {
            return item.length_index;
        }
    }

    assert(false);
    return 1;
}
