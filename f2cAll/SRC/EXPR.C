/****************************************************************
Copyright 1990, 1991, 1992, 1993 by AT&T Bell Laboratories and Bellcore.

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
#include "output.h"
#include "names.h"
#include "assert.h"

typedef struct {
    double dreal, dimag;
} dcomplex;

LOCAL void conspower(Constp p, Constp ap, ftnint n);
LOCAL void consbinop(int opcode, int type, Constp cpp, Constp app, Constp bpp);
LOCAL void zdiv(dcomplex *c, dcomplex *a, dcomplex *b);
LOCAL expptr fold(expptr e), mkpower(expptr p);
LOCAL expptr stfcall(Namep np, struct Listblock *actlist);

expptr opconv(expptr p, int t);
int inregister(Namep np);
expptr subcheck(Namep np, expptr p);
void consnegop(Constp p);

#ifndef stfcall_MAX
#define stfcall_MAX 144
#endif


extern char dflttype[26];
extern int htype;

/* little routines to create constant blocks */

Constp mkconst(int t)
//register int t;
{
	register Constp p;

	p = ALLOC(Constblock);
	p->tag = TCONST;
	p->vtype = t;
	return(p);
}


/* mklogcon -- Make Logical Constant */

expptr mklogcon(int l)
//register int l;
{
	register Constp  p;

	p = mkconst(tylog);
	p->Const.ci = l;
	return( (expptr) p );
}



/* mkintcon -- Make Integer Constant */

expptr mkintcon(ftnint l)
{
	register Constp p;

	p = mkconst(tyint);
	p->Const.ci = l;
	return( (expptr) p );
}




/* mkaddcon -- Make Address Constant, given integer value */

expptr mkaddcon(long l)
//register long l;
{
	register Constp p;

	p = mkconst(TYADDR);
	p->Const.ci = l;
	return( (expptr) p );
}



/* mkrealcon -- Make Real Constant.  The type t is assumed
   to be TYREAL or TYDREAL */

expptr mkrealcon(int t, char *d)
 //register int t;
 //char *d;
{
	register Constp p;

	p = mkconst(t);
	p->Const.cds[0] = cds(d,CNULL);
	p->vstg = 1;
	return( (expptr) p );
}


/* mkbitcon -- Make bit constant.  Reads the input string, which is
   assumed to correctly specify a number in base 2^shift (where   shift
   is the input parameter).   shift   may not exceed 4, i.e. only binary,
   quad, octal and hex bases may be input.  Constants may not exceed 32
   bits, or whatever the size of (struct Constblock).ci may be. */

expptr mkbitcon(int shift, int leng, char *s)
//int shift;
//int leng;
//char *s;
{
	register Constp p;
	register long x;

	p = mkconst(TYLONG);
	x = 0;
	while(--leng >= 0)
		if(*s != ' ')
			x = (x << shift) | hextoi(*s++);
	/* mwm wanted to change the type to short for short constants,
	 * but this is dangerous -- there is no syntax for long constants
	 * with small values.
	 */
	p->Const.ci = x;
	return( (expptr) p );
}





/* mkstrcon -- Make string constant.  Allocates storage and initializes
   the memory for a copy of the input Fortran-string. */

expptr mkstrcon(int l,char const *v)
//int l;
//register char *v;
{
	register Constp p;
	register char *s;

	p = mkconst(TYCHAR);
	p->vleng = ICON(l);
	p->Const.ccp = s = (char *) ckalloc(l+1);
	p->Const.ccp1.blanks = 0;
	while(--l >= 0)
		*s++ = *v++;
	*s = '\0';
	return( (expptr) p );
}



/* mkcxcon -- Make complex contsant.  A complex number is a pair of
   values, each of which may be integer, real or double. */

expptr mkcxcon(expptr realp, expptr imagp)
//register expptr realp, imagp;
{
	int rtype, itype;
	register Constp p;
	expptr errnode();

	rtype = realp->headblock.vtype;
	itype = imagp->headblock.vtype;

	if( ISCONST(realp) && ISNUMERIC(rtype) && ISCONST(imagp) && ISNUMERIC(itype) )
	{
		p = mkconst( (rtype==TYDREAL||itype==TYDREAL)
				? TYDCOMPLEX : tycomplex);
		if (realp->constblock.vstg || imagp->constblock.vstg) {
			p->vstg = 1;
			p->Const.cds[0] = ISINT(rtype)
				? string_num("", realp->constblock.Const.ci)
				: realp->constblock.vstg
					? realp->constblock.Const.cds[0]
					: dtos(realp->constblock.Const.cd[0]);
			p->Const.cds[1] = ISINT(itype)
				? string_num("", imagp->constblock.Const.ci)
				: imagp->constblock.vstg
					? imagp->constblock.Const.cds[0]
					: dtos(imagp->constblock.Const.cd[0]);
			}
		else {
			p->Const.cd[0] = ISINT(rtype)
				? realp->constblock.Const.ci
				: realp->constblock.Const.cd[0];
			p->Const.cd[1] = ISINT(itype)
				? imagp->constblock.Const.ci
				: imagp->constblock.Const.cd[0];
			}
	}
	else
	{
		err("invalid complex constant");
		p = (Constp)errnode();
	}

	frexpr(realp);
	frexpr(imagp);
	return( (expptr) p );
}


/* errnode -- Allocate a new error block */

expptr errnode()
{
	struct Errorblock *p;
	p = ALLOC(Errorblock);
	p->tag = TERROR;
	p->vtype = TYERROR;
	return( (expptr) p );
}





/* mkconv -- Make type conversion.  Cast expression   p   into type   t.
   Note that casting to a character copies only the first sizeof(char)
   bytes. */

expptr mkconv(int t, expptr p)
//register int t;
//register expptr p;
{
	register expptr q;
	register int pt, charwarn = 1;
	//expptr opconv();

	if (t >= 100) {
		t -= 100;
		charwarn = 0;
		}
	if(t==TYUNKNOWN || t==TYERROR)
    {
        if(cxx)
        {
            // assume types match
            return p;
        }
        badtype("mkconv", t);
    }
	pt = p->headblock.vtype;

/* Casting to the same type is a no-op */

	if(t == pt)
		return(p);

/* If we're casting a constant which is not in the literal table ... */

	else if( ISCONST(p) && pt!=TYADDR && pt != TYCHAR)
	{
		if (ISINT(t) && ISINT(pt) || ISREAL(t) && ISREAL(pt)) {
			/* avoid trouble with -i2 */
			p->headblock.vtype = t;
			return p;
			}
		q = (expptr) mkconst(t);
		consconv(t, &q->constblock, &p->constblock );
		frexpr(p);
	}
	else {
		if (pt == TYCHAR && t != TYADDR && charwarn
				&& (!halign || p->tag != TADDR
				|| p->addrblock.uname_tag != UNAM_CONST))
			warn(
		 "ichar([first char. of] char. string) assumed for conversion to numeric");
		q = opconv(p, t);
		}

	if(t == TYCHAR)
		q->constblock.vleng = ICON(1);
	return(q);
}



/* opconv -- Convert expression   p   to type   t   using the main
   expression evaluator; returns an OPCONV expression, I think  14-jun-88 mwm */

expptr opconv(expptr p, int t)
//expptr p;
//int t;
{
	register expptr q;

	if (t == TYSUBR)
		err("illegal use of subroutine name");
	q = mkexpr(OPCONV, p, ENULL);
	q->headblock.vtype = t;
	return(q);
}



/* addrof -- Create an ADDR expression operation */

expptr addrof(expptr p)
//expptr p;
{
	return( mkexpr(OPADDR, p, ENULL) );
}



/* cpexpr - Returns a new copy of input expression   p   */

tagptr cpexpr(tagptr p)
//register tagptr p;
{
	register tagptr e;
	int tag;
	register chainp ep, pp;
	//tagptr cpblock();

/* This table depends on the ordering of the T macros, e.g. TNAME */

	static int blksize[ ] =
	{
		0,
		sizeof(struct Nameblock),
		sizeof(struct Constblock),
		sizeof(struct Exprblock),
		sizeof(struct Addrblock),
		sizeof(struct Primblock),
		sizeof(struct Listblock),
		sizeof(struct Impldoblock),
		sizeof(struct Errorblock),
        sizeof(struct Listblock)
    };

	if(p == NULL)
		return(NULL);

/* TNAMEs are special, and don't get copied.  Each name in the current
   symbol table has a unique TNAME structure. */

	if( (tag = p->tag) == TNAME)
		return(p);

	e = cpblock(blksize[p->tag], (char *)p);

	switch(tag)
	{
	case TCONST:
		if(e->constblock.vtype == TYCHAR)
		{
            //if(!cxx)
            {
                e->constblock.Const.ccp =
                    copyn((int)e->constblock.vleng->constblock.Const.ci+1,
                        e->constblock.Const.ccp);
                e->constblock.vleng =
                    (expptr) cpexpr(e->constblock.vleng);
            }
		}
	case TERROR:
		break;

	case TEXPR:
		e->exprblock.leftp =  (expptr) cpexpr(p->exprblock.leftp);
		e->exprblock.rightp = (expptr) cpexpr(p->exprblock.rightp);
		break;

	case TLIST:
		if(pp = p->listblock.listp)
		{
			ep = e->listblock.listp =
			    mkchain((char *)cpexpr((tagptr)pp->datap), CHNULL);
			for(pp = pp->nextp ; pp ; pp = pp->nextp)
				ep = ep->nextp =
				    mkchain((char *)cpexpr((tagptr)pp->datap),
						CHNULL);
		}
		break;

	case TADDR:
		e->addrblock.vleng = (expptr)  cpexpr(e->addrblock.vleng);
		e->addrblock.memoffset = (expptr)cpexpr(e->addrblock.memoffset);
		e->addrblock.istemp = NO;
		break;

	case TPRIM:
		e->primblock.argsp = (struct Listblock *)
		    cpexpr((expptr)e->primblock.argsp);
		e->primblock.fcharp = (expptr) cpexpr(e->primblock.fcharp);
        e->primblock.lcharp = (expptr)cpexpr(e->primblock.lcharp);
        e->primblock.scharp = (expptr) cpexpr(e->primblock.scharp);
		break;

    case TSPECIALLIST:
        if(pp = p->listblock.listp)
        {
            ep = e->listblock.listp =
                mkchain((char *)cpexpr((tagptr)pp->datap), CHNULL);
            for(pp = pp->nextp; pp; pp = pp->nextp)
                ep = ep->nextp =
                mkchain((char *)cpexpr((tagptr)pp->datap),
                    CHNULL);
        }
        break;


	default:
		badtag("cpexpr", tag);
	}

	return(e);
}

/* frexpr -- Free expression -- frees up memory used by expression   p   */

void frexpr(tagptr p)
{
	register chainp q;

	if(p == NULL)
		return;

	switch(p->tag)
	{
	case TCONST:
		if( ISCHAR(p) )
		{
			free( (charptr) (p->constblock.Const.ccp) );
			frexpr(p->constblock.vleng);
		}
		break;

	case TADDR:
		if (p->addrblock.vtype != TY_DERIVED && 
            p->addrblock.vtype != TYINTPTR &&
            p->addrblock.vtype > TYERROR)	/* i/o block */
			break;
		frexpr(p->addrblock.vleng);
		frexpr(p->addrblock.memoffset);
		break;

	case TERROR:
		break;

/* TNAME blocks don't get free'd - probably because they're pointed to in
   the hash table. 14-Jun-88 -- mwm */

	case TNAME:
		return;

	case TPRIM:
		frexpr((expptr)p->primblock.argsp);
		frexpr(p->primblock.fcharp);
        frexpr(p->primblock.lcharp);
        frexpr(p->primblock.scharp);
		break;

	case TEXPR:
		frexpr(p->exprblock.leftp);
		if(p->exprblock.rightp)
			frexpr(p->exprblock.rightp);
		break;

    case TLIST:
        //for(q = p->listblock.listp; q; q = q->nextp)
        //    frexpr((tagptr)q->datap);
        frchain(&(p->listblock.listp));
        break;

    case TSPECIALLIST:
        //for(q = p->listblock.listp; q; q = q->nextp)
        //    frexpr((tagptr)q->datap);
        frchain(&(p->listblock.listp));
        break;

	default:
		badtag("frexpr", p->tag);
	}

	free( (charptr) p );
}

 void
wronginf(Namep np)
 //Namep np;
{
	int c, k;
	warn1("fixing wrong type inferred for %.65s", np->fvarname);
	np->vinftype = 0;
	c = letter(np->fvarname[0]);
	if ((np->vtype = impltype[c]) == TYCHAR
	&& (k = implleng[c]))
		np->vleng = ICON(k);
	}

/* fix up types in expression; replace subtrees and convert
   names to address blocks */

expptr fixtype(tagptr p)
//register tagptr p;
{

	if(p == 0)
		return(0);

	switch(p->tag)
	{
	case TCONST:
		if(ONEOF(p->constblock.vtype,MSKINT|MSKLOGICAL|MSKADDR|
		    MSKREAL) )
			return( (expptr) p);

		return( (expptr) putconst((Constp)p) );

	case TADDR:
		p->addrblock.memoffset = fixtype(p->addrblock.memoffset);
		return( (expptr) p);

    case TERROR:
        return((expptr)p);

    case TNAME:
        return((expptr)p);

    default:
		badtag("fixtype", p->tag);

/* This case means that   fixexpr   can't call   fixtype   with any expr,
   only a subexpr of its parameter. */

	case TEXPR:
		return( fixexpr((Exprp)p) );

    case TSPECIALLIST:
    case TLIST:

        if(p->listblock.listp)
        {
            for(auto pp = p->listblock.listp; pp; pp = pp->nextp)
            {
                pp->datap = (char*)fixtype((expptr)pp->datap);
            }
        }
		return( (expptr) p );
        break;


    //case TNAME:
    //    {
    //        if(!symbolic_params){
    //            badtag("fixtype", p->tag);
    //        }
    //        return((expptr)p);
    //    }
    //    break;

	case TPRIM:
		if(p->primblock.argsp && p->primblock.namep->vclass!=CLVAR)
		{
			if(p->primblock.namep->vtype == TYSUBR)
			{
				err("function invocation of subroutine");
				return( errnode() );
			}
			else {
				if (p->primblock.namep->vinftype)
					wronginf(p->primblock.namep);
				return( mkfunct(p) );
				}
		}
        //else if(symbolic_params && p->primblock.namep->vclass == CLPARAM)
        //{
        //    return cpexpr((expptr)p->primblock.namep);
        //}

/* The lack of args makes   p   a function name, substring reference
   or variable name. */

		else	return mklhs((struct Primblock *) p, keepsubs);
	}
}


 int
badchleng(expptr p)// register expptr p;
{
	if (!p->headblock.vleng) {
		if (p->headblock.tag == TADDR
		&& p->addrblock.uname_tag == UNAM_NAME)
			errstr("bad use of character*(*) variable %.60s",
				p->addrblock.user.name->fvarname);
		else
			err("Bad use of character*(*)");
		return 1;
		}
	return 0;
	}


 static expptr
cplenexpr(expptr p)
 //expptr p;
{
	expptr rv;

	if (badchleng(p))
		return ICON(1);
	rv = cpexpr(p->headblock.vleng);
	if (ISCONST(p) && p->constblock.vtype == TYCHAR)
		rv->constblock.Const.ci += p->constblock.Const.ccp1.blanks;
	return rv;
	}


/* special case tree transformations and cleanups of expression trees.
   Parameter   p   should have a TEXPR tag at its root, else an error is
   returned */

expptr fixexpr(Exprp p)
//register Exprp p;
{
	expptr lp;
	register expptr rp;
	register expptr q;
	int opcode, ltype, rtype, ptype, mtype;

	if( ISERROR(p) )
		return( (expptr) p );
	else if(p->tag != TEXPR)
		badtag("fixexpr", p->tag);
	opcode = p->opcode;

/* First set the types of the left and right subexpressions */

	lp = p->leftp;
	if (!ISCONST(lp) || lp->constblock.vtype != TYCHAR)
		lp = p->leftp = fixtype(lp);
	ltype = lp->headblock.vtype;

    if(!cxx)
    {
        if(opcode==OPASSIGN && lp->tag!=TADDR)
        {
            err("left side of assignment must be variable");
            frexpr((expptr)p);
            return( errnode() );
        }
    }

	if(rp = p->rightp)
	{
		if (!ISCONST(rp) || rp->constblock.vtype != TYCHAR)
			rp = p->rightp = fixtype(rp);
		rtype = rp->headblock.vtype;
	}
	else
		rtype = 0;

	if(ltype==TYERROR || rtype==TYERROR)
	{
		frexpr((expptr)p);
		return( errnode() );
	}

/* Now work on the whole expression */

	/* force folding if possible */

	if( ISCONST(lp) && (rp==NULL || ISCONST(rp)) )
	{
		q = opcode == OPCONV && lp->constblock.vtype == p->vtype
			? lp : mkexpr(opcode, lp, rp);

/* mkexpr is expected to reduce constant expressions */

		if( ISCONST(q) ) {
			p->leftp = p->rightp = 0;
			frexpr((expptr)p);
			return(q);
			}
		free( (charptr) q );	/* constants did not fold */
	}

	if( (ptype = cktype(opcode, ltype, rtype)) == TYERROR)
	{
		frexpr((expptr)p);
		return( errnode() );
	}

	if (ltype == TYCHAR && ISCONST(lp))
		p->leftp =  lp = (expptr)putconst((Constp)lp);
	if (rtype == TYCHAR && ISCONST(rp))
		p->rightp = rp = (expptr)putconst((Constp)rp);

	switch(opcode)
	{
	case OPCONCAT:
        if (cxx) {

            if (ltype != ptype && (ltype < TYINT1 || ptype > TYDREAL))
                p->leftp = fixtype(mkconv(ptype, lp));
            if (rtype != ptype && (rtype < TYINT1 || ptype > TYDREAL))
                p->rightp = fixtype(mkconv(ptype, rp));
            break;
        }
        else {
            if (p->vleng == NULL && !cxx)
                p->vleng = mkexpr(OPPLUS, cplenexpr(lp),
                    cplenexpr(rp));
        }
		break;

	case OPASSIGN:
		if (rtype == TYREAL || ISLOGICAL(ptype))
			break;
	case OPPLUSEQ:
	case OPSTAREQ:
		if(ltype == rtype)
			break;
		if( ! ISCONST(rp) && ISREAL(ltype) && ISREAL(rtype) )
			break;
		if( ISCOMPLEX(ltype) || ISCOMPLEX(rtype) )
			break;
		if( ONEOF(ltype, MSKADDR|MSKINT) && ONEOF(rtype, MSKADDR|MSKINT)
		    && typesize[ltype]>=typesize[rtype] )
			    break;

/* Cast the right hand side to match the type of the expression */

		p->rightp = fixtype( mkconv(ptype, rp) );
		break;

	case OPSLASH:
		if( ISCOMPLEX(rtype) )
		{
			p = (Exprp) call2(ptype,

/* Handle double precision complex variables */

			    ptype == TYCOMPLEX ? "c_div" : "z_div",
			    mkconv(ptype, lp), mkconv(ptype, rp) );
			break;
		}
	case OPPLUS:
	case OPMINUS:
	case OPSTAR:
	case OPMOD:
		if(ptype==TYDREAL && ( (ltype==TYREAL && ! ISCONST(lp) ) ||
		    (rtype==TYREAL && ! ISCONST(rp) ) ))
			break;
		if( ISCOMPLEX(ptype) )
			break;

/* Cast both sides of the expression to match the type of the whole
   expression.  */

		if(ltype != ptype && (ltype < TYINT1 || ptype > TYDREAL))
			p->leftp = fixtype(mkconv(ptype,lp));
		if(rtype != ptype && (rtype < TYINT1 || ptype > TYDREAL))
			p->rightp = fixtype(mkconv(ptype,rp));
		break;

	case OPPOWER:
		return( mkpower((expptr)p) );

	case OPLT:
	case OPLE:
	case OPGT:
	case OPGE:
	case OPEQ:
	case OPNE:
		if(ltype == rtype)
			break;
		if (htype) {
			if (ltype == TYCHAR) {
				p->leftp = fixtype(mkconv(rtype,lp));
				break;
				}
			if (rtype == TYCHAR) {
				p->rightp = fixtype(mkconv(ltype,rp));
				break;
				}
			}
		mtype = cktype(OPMINUS, ltype, rtype);
		if(mtype==TYDREAL && ( (ltype==TYREAL && ! ISCONST(lp)) ||
		    (rtype==TYREAL && ! ISCONST(rp)) ))
			break;
		if( ISCOMPLEX(mtype) )
			break;
		if(ltype != mtype)
			p->leftp = fixtype(mkconv(mtype,lp));
		if(rtype != mtype)
			p->rightp = fixtype(mkconv(mtype,rp));
		break;

	case OPCONV:
		ptype = cktype(OPCONV, p->vtype, ltype);
		if(lp->tag==TEXPR && lp->exprblock.opcode==OPCOMMA
		 && !ISCOMPLEX(ptype))
		{
			lp->exprblock.rightp =
			    fixtype( mkconv(ptype, lp->exprblock.rightp) );
			free( (charptr) p );
			p = (Exprp) lp;
		}
		break;

	case OPADDR:
		if(lp->tag==TEXPR && lp->exprblock.opcode==OPADDR)
			Fatal("addr of addr");
		break;

	case OPCOMMA:
	case OPQUEST:
	case OPCOLON:
		break;

	case OPMIN:
	case OPMAX:
	case OPMIN2:
	case OPMAX2:
	case OPDMIN:
	case OPDMAX:
	case OPABS:
	case OPDABS:
		ptype = p->vtype;
		break;

	default:
		break;
	}

	p->vtype = ptype;
	return((expptr) p);
}


/* fix an argument list, taking due care for special first level cases */

int fixargs(int doput, struct Listblock *p0)
//int doput;	/* doput is true if constants need to be passed by reference */
//struct Listblock *p0;
{
	register chainp p;
	register tagptr q, t;
	register int qtag;
	int nargs;
	Addrp mkscalar();

	nargs = 0;
	if(p0)
		for(p = p0->listp ; p ; p = p->nextp)
		{
			++nargs;
			q = (tagptr)p->datap;
			qtag = q->tag;
			if(qtag == TCONST)
			{

/* Call putconst() to store values in a constant table.  Since even
   constants must be passed by reference, this can optimize on the storage
   required */
                if(cxx)
                {
                    p->datap = (char *)fixtype(q);
                }
                else
                {
                    p->datap = doput ? (char *)putconst((Constp)q)
                        : (char *)q;
                }
			}

/* Take a function name and turn it into an Addr.  This only happens when
   nothing else has figured out the function beforehand */

			else if(qtag==TPRIM && q->primblock.argsp==0 &&
			    q->primblock.namep->vclass==CLPROC &&
			    q->primblock.namep->vprocclass != PTHISPROC)
				p->datap = (char *)mkaddr(q->primblock.namep);

			else if(qtag==TPRIM && q->primblock.argsp==0 &&
			    q->primblock.namep->vdim!=NULL)
				p->datap = (char *)mkscalar(q->primblock.namep);

			else if(qtag==TPRIM && q->primblock.argsp==0 &&
			    q->primblock.namep->vdovar &&
			    (t = (tagptr) memversion(q->primblock.namep)) )
				p->datap = (char *)fixtype(t);
			else
				p->datap = (char *)fixtype(q);
		}
	return(nargs);
}



/* mkscalar -- only called by   fixargs   above, and by some routines in
   io.c */

Addrp mkscalar(Namep np)
//register Namep np;
{
	register Addrp ap;

	vardcl(np);
	ap = mkaddr(np);

	/* The prolog causes array arguments to point to the
	 * (0,...,0) element, unless subscript checking is on.
	 */
    if(!cxx)
    {
        if( !checksubs && np->vstg==STGARG)
        {
            register struct Dimblock *dp;
            dp = np->vdim;
            frexpr(ap->memoffset);
            ap->memoffset = mkexpr(OPSTAR,
                (np->vtype==TYCHAR ?
                cpexpr(np->vleng) :
                (tagptr)ICON(typesize[np->vtype]) ),
                cpexpr(dp->baseoffset) );
        }
    }
	return(ap);
}


 static void
adjust_arginfo(Namep np)	/* adjust arginfo to omit the length arg for the
			   arg that we now know to be a character-valued
			   function */
 //register Namep np;
{
	struct Entrypoint *ep;
	register chainp args;
	Argtypes *at;

	for(ep = entries; ep; ep = ep->entnextp)
		for(args = ep->arglist; args; args = args->nextp)
			if (np == (Namep)args->datap
			&& (at = ep->entryname->arginfo))
				--at->nargs;
	}



expptr mkfunct(expptr p0)
// expptr p0;
{
	register struct Primblock *p = (struct Primblock *)p0;
	struct Entrypoint *ep;
	Addrp ap;
	Extsym *extp;
	register Namep np;
	register expptr q;
	//expptr intrcall();
	extern chainp new_procs;
	int k, nargs;
	int class_;

	if(p->tag != TPRIM)
		return( errnode() );

	np = p->namep;
	class_ = np->vclass;


	if(class_ == CLUNKNOWN)
	{
		np->vclass = class_ = CLPROC;
		if(np->vstg == STGUNKNOWN)
		{
			if(np->vtype!=TYSUBR && (k = intrfunct(np->fvarname))
				&& (zflag || !(*(struct Intrpacked *)&k).f4
					|| dcomplex_seen))
			{
				np->vstg = STGINTR;
				np->vardesc.varno = k;
				np->vprocclass = PINTRINSIC;
			}
			else
			{
				extp = mkext(np->fvarname,
					addunder(np->cvarname, false));
				extp->extstg = STGEXT;
				np->vstg = STGEXT;
				np->vardesc.varno = extp - extsymtab;
				np->vprocclass = PEXTERNAL;
			}
		}
		else if(np->vstg==STGARG)
		{
		    if(np->vtype == TYCHAR) {
			adjust_arginfo(np);
			if (np->vpassed) {
				char wbuf[160], *who;
				who = np->fvarname;
				sprintf(wbuf, "%s%s%s\n\t%s%s%s",
					"Character-valued dummy procedure ",
					who, " not declared EXTERNAL.",
			"Code may be wrong for previous function calls having ",
					who, " as a parameter.");
				warn(wbuf);
				}
			}
		    np->vprocclass = PEXTERNAL;
		}
	}

	if(class_ != CLPROC) {
		if (np->vstg == STGCOMMON)
			fatalstr(
			 "Cannot invoke common variable %.50s as a function.",
				np->fvarname);
		fatali("invalid class code %d for function", class_);
		}

/* F77 doesn't allow subscripting of function calls */

	if(p->fcharp || p->lcharp)
	{
		err("no substring of function call");
		goto error;
	}
	impldcl(np);
	np->vimpltype = 0;	/* invoking as function ==> inferred type */
	np->vcalled = 1;
	nargs = fixargs( np->vprocclass!=PINTRINSIC,  p->argsp);

	switch(np->vprocclass)
	{
	case PEXTERNAL:
		if(np->vtype == TYUNKNOWN)
		{
            if(!cxx)
            {
                dclerr("attempt to use untyped function", np);
                np->vtype = dflttype[letter(np->fvarname[0])];
            }
		}
		ap = mkaddr(np);
		if (!extsymtab[np->vardesc.varno].extseen) {
			new_procs = mkchain((char *)np, new_procs);
			extsymtab[np->vardesc.varno].extseen = 1;
			}
call:
		q = mkexpr(OPCALL, (expptr)ap, (expptr)p->argsp);
		q->exprblock.vtype = np->vtype;
		if(np->vleng)
			q->exprblock.vleng = (expptr) cpexpr(np->vleng);
		break;

	case PINTRINSIC:
		q = intrcall(np, p->argsp, nargs);
		break;

	case PSTFUNCT:
		q = stfcall(np, p->argsp);
		break;

	case PTHISPROC:
		warn("recursive call");

/* entries   is the list of multiple entry points */

		for(ep = entries ; ep ; ep = ep->entnextp)
			if(ep->enamep == np)
				break;
		if(ep == NULL)
			Fatal("mkfunct: impossible recursion");

		ap = builtin(np->vtype, ep->entryname->cextname, -2);
		/* the negative last arg prevents adding */
		/* this name to the list of used builtins */
		goto call;

	default:
		fatali("mkfunct: impossible vprocclass %d",
		    (int) (np->vprocclass) );
	}
	free( (charptr) p );
	return(q);

error:
	frexpr((expptr)p);
	return( errnode() );
}



LOCAL expptr stfcall(Namep np, struct Listblock *actlist)
//Namep np;
//struct Listblock *actlist;
{
	register chainp actuals;
	int nargs;
	chainp oactp, formals;
	int type;
	expptr Ln, Lq, q, q1, rhs, ap;
	Namep tnp;
	register struct Rplblock *rp;
	struct Rplblock *tlist;
	static int inv_count;

	if (++inv_count > stfcall_MAX)
		Fatal("Loop invoking recursive statement function?");
	if(actlist)
	{
		actuals = actlist->listp;
		free( (charptr) actlist);
	}
	else
		actuals = NULL;
	oactp = actuals;

	nargs = 0;
	tlist = NULL;
	if( (type = np->vtype) == TYUNKNOWN)
	{
		dclerr("attempt to use untyped statement function", np);
		type = np->vtype = dflttype[letter(np->fvarname[0])];
	}
	formals = (chainp) np->varxptr.vstfdesc->datap;
	rhs = (expptr) (np->varxptr.vstfdesc->nextp);

	/* copy actual arguments into temporaries */
	while(actuals!=NULL && formals!=NULL)
	{
		rp = ALLOC(Rplblock);
		rp->rplnp = tnp = (Namep) formals->datap;
		ap = fixtype((tagptr)actuals->datap);
		if(tnp->vtype==ap->headblock.vtype && tnp->vtype!=TYCHAR
		    && (ap->tag==TCONST || ap->tag==TADDR) )
		{

/* If actuals are constants or variable names, no temporaries are required */
			rp->rplvp = (expptr) ap;
			rp->rplxp = NULL;
			rp->rpltag = ap->tag;
		}
		else	{
			rp->rplvp = (expptr) mktmp(tnp->vtype, tnp->vleng);
			rp -> rplxp = NULL;
			putexpr ( mkexpr(OPASSIGN, cpexpr(rp->rplvp), ap));
			if((rp->rpltag = rp->rplvp->tag) == TERROR)
				err("disagreement of argument types in statement function call");
		}
		rp->rplnextp = tlist;
		tlist = rp;
		actuals = actuals->nextp;
		formals = formals->nextp;
		++nargs;
	}

	if(actuals!=NULL || formals!=NULL)
		err("statement function definition and argument list differ");

	/*
   now push down names involved in formal argument list, then
   evaluate rhs of statement function definition in this environment
*/

	if(tlist)	/* put tlist in front of the rpllist */
	{
		for(rp = tlist; rp->rplnextp; rp = rp->rplnextp)
			;
		rp->rplnextp = rpllist;
		rpllist = tlist;
	}

/* So when the expression finally gets evaled, that evaluator must read
   from the globl   rpllist   14-jun-88 mwm */

	q = (expptr) mkconv(type, fixtype(cpexpr(rhs)) );

	/* get length right of character-valued statement functions... */
	if (type == TYCHAR
	 && (Ln = np->vleng)
	 && q->tag != TERROR
	 && (Lq = q->exprblock.vleng)
	 && (Lq->tag != TCONST
		|| Ln->constblock.Const.ci != Lq->constblock.Const.ci)) {
		q1 = (expptr) mktmp(type, Ln);
		putexpr ( mkexpr(OPASSIGN, cpexpr(q1), q));
		q = q1;
		}

	/* now generate the tree ( t1=a1, (t2=a2,... , f))))) */
	while(--nargs >= 0)
	{
		if(rpllist->rplxp)
			q = mkexpr(OPCOMMA, rpllist->rplxp, q);
		rp = rpllist->rplnextp;
		frexpr(rpllist->rplvp);
		free((char *)rpllist);
		rpllist = rp;
	}
	frchain( &oactp );
	--inv_count;
	return(q);
}


static int replaced;

/* mkplace -- Figure out the proper storage class for the input name and
   return an addrp with the appropriate stuff */

Addrp mkplace(Namep np)
//register Namep np;
{
	register Addrp s;
	register struct Rplblock *rp;
	int regn;

	/* is name on the replace list? */

	for(rp = rpllist ; rp ; rp = rp->rplnextp)
	{
		if(np == rp->rplnp)
		{
			replaced = 1;
			if(rp->rpltag == TNAME)
			{
				np = (Namep) (rp->rplvp);
				break;
			}
			else	return( (Addrp) cpexpr(rp->rplvp) );
		}
	}

	/* is variable a DO index in a register ? */

	if(np->vdovar && ( (regn = inregister(np)) >= 0) )
		if(np->vtype == TYERROR)
			return((Addrp) errnode() );
		else
		{
			s = ALLOC(Addrblock);
			s->tag = TADDR;
			s->vstg = STGREG;
			s->vtype = TYIREG;
			s->memno = regn;
			s->memoffset = ICON(0);
			s -> uname_tag = UNAM_NAME;
			s -> user.name = np;
			return(s);
		}

    if(!cxx)
    {
        if (np->vclass == CLPROC && np->vprocclass != PTHISPROC)
            errstr("external %.60s used as a variable", np->fvarname);
    }
	vardcl(np);
	return(mkaddr(np));
}

 static expptr
subskept(struct Primblock *p, Addrp a)
//struct Primblock *p;
//Addrp a;
{
	expptr ep;
	struct Listblock *Lb;
	chainp cp;

    if(a->uname_tag != UNAM_NAME)
    {
        if(!cxx)
        {
            erri("subskept: uname_tag %d", a->uname_tag);
        }
    }
    else
    {
        if(!cxx)
        {
            a->user.name->vrefused = 1;
        }
        a->user.name->visused = 1;
        a->uname_tag = cxx ? UNAM_REFCXX : UNAM_REF;
    }


    Lb = (struct Listblock *)cpexpr((tagptr)p->argsp);
    if(Lb)
    {
        for(cp = Lb->listp; cp; cp = cp->nextp)
            cp->datap = (char *)putx(fixtype((tagptr)cp->datap));
    }
    if(!cxx)
    {
        if(a->vtype == TYCHAR) {
            ep = p->fcharp ? mkexpr(OPMINUS, cpexpr(p->fcharp), ICON(1))
                : ICON(0);
            Lb->listp = mkchain((char *)ep, Lb->listp);
        }
    }
    else
    {
        if(p->fcharp||
            p->lcharp||
            p->scharp
            )
        {
            if(!Lb)
            {
                int dummy = 42;
                //Lb = mklist(0);

                //Lb->listp = mkchain((char *)putx(fixtype((tagptr)p->fcharp)), Lb->listp);
                //Lb->listp = mkchain((char *)putx(fixtype((tagptr)p->lcharp)), Lb->listp);
                //Lb->listp = mkchain((char *)putx(fixtype((tagptr)p->scharp)), Lb->listp);
            }
        }
    }
	return (expptr)Lb;
	}

 static int doing_vleng;

/* mklhs -- Compute the actual address of the given expression; account
   for array subscripts, stack offset, and substring offsets.  The f -> C
   translator will need this only to worry about the subscript stuff */

expptr mklhs(Primblock *p, int subkeep)
//register struct Primblock *p; int subkeep;
{
	expptr suboffset();
	register Addrp s;
	Namep np;

	if(p->tag != TPRIM)
		return( (expptr) p );
	np = p->namep;

	replaced = 0;
	s = mkplace(np);
	if(s->tag!=TADDR || s->vstg==STGREG)
	{
		free( (charptr) p );
		return( (expptr) s );
	}
	s->parenused = p->parenused;

	/* compute the address modified by subscripts */

    if(!replaced)
        s->memoffset = (/*cxx ||*/ (cxx && np->vdim) || (subkeep && np->vdim
            && (np->vdim->ndim > 1 || np->vtype == TYCHAR
            && (!ISCONST(np->vleng)
            || np->vleng->constblock.Const.ci != 1))))
        ? subskept(p, s)
        : subskept(p, s); // mkexpr(OPPLUS, s->memoffset, suboffset(p));
	frexpr((expptr)p->argsp);
	p->argsp = NULL;

	/* now do substring part */

	if(p->fcharp || p->lcharp || p->scharp)
	{
        if(np->vtype != TYCHAR)
        {
            if(cxx)
            {
                auto chain = mkchain((char*)cpexpr(p->fcharp), 
                                mkchain((char*)cpexpr(p->lcharp), 
                                mkchain((char*)cpexpr(p->scharp), 0)));
                auto l = mklist(chain);
                l->tag = TSPECIALLIST;
                l->vtype = VTSPECIALLIST_SUBARRAY;
                s->memoffset = (expptr)l;
            }
            else
            {
                errstr("substring of noncharacter %s", np->fvarname);
            }
        }
		else	{
			if(p->lcharp == NULL)
				p->lcharp = (expptr) cpexpr(s->vleng);
			if(p->fcharp) {
				doing_vleng = 1;
				s->vleng = fixtype(mkexpr(OPMINUS,
						p->lcharp,
					mkexpr(OPMINUS, p->fcharp, ICON(1) )));
				doing_vleng = 0;
				}
			else	{
				frexpr(s->vleng);
				s->vleng = p->lcharp;
			}
		}
	}

	s->vleng = fixtype( s->vleng );
	s->memoffset = fixtype( s->memoffset );
	free( (charptr) p );
	return( (expptr) s );
}





/* deregister -- remove a register allocation from the list; assumes that
   names are deregistered in stack order (LIFO order - Last In First Out) */

void deregister(Namep np)
//Namep np;
{
	if(nregvar>0 && regnamep[nregvar-1]==np)
	{
		--nregvar;
	}
}




/* memversion -- moves a DO index REGISTER into a memory location; other
   objects are passed through untouched */

Addrp memversion(Namep np)
//register Namep np;
{
	register Addrp s;

	if(np->vdovar==NO || (inregister(np)<0) )
		return(NULL);
	np->vdovar = NO;
	s = mkplace(np);
	np->vdovar = YES;
	return(s);
}



/* inregister -- looks for the input name in the global list   regnamep */

int inregister(Namep np)
//register Namep np;
{
	register int i;

	for(i = 0 ; i < nregvar ; ++i)
		if(regnamep[i] == np)
			return( regnum[i] );
	return(-1);
}



/* suboffset -- Compute the offset from the start of the array, given the
   subscripts as arguments */

expptr suboffset(struct Primblock *p)
//register struct Primblock *p;
{
	int n;
	expptr si, size;
	chainp cp;
	expptr e, e1, offp, prod;
	//expptr subcheck();
	struct Dimblock *dimp;
	expptr sub[MAXDIM+1];
	register Namep np;

	np = p->namep;
	offp = ICON(0);
	n = 0;
	if(p->argsp)
		for(cp = p->argsp->listp ; cp ; cp = cp->nextp)
		{
			si = fixtype(cpexpr((tagptr)cp->datap));
			if (!ISINT(si->headblock.vtype)) {
				NOEXT("non-integer subscript");
				si = mkconv(TYLONG, si);
				}
			sub[n++] = si;
			if(n > maxdim)
			{
				erri("more than %d subscripts", maxdim);
				break;
			}
		}

	dimp = np->vdim;
	//if(n>0 && dimp==NULL)
	//	errstr("subscripts on scalar variable %.68s", np->fvarname);
	//else if(dimp && dimp->ndim!=n)
	//	errstr("wrong number of subscripts on %.68s", np->fvarname);
	//else 
        if(n > 0)
	{
		prod = sub[--n];
		while( --n >= 0)
			prod = mkexpr(OPPLUS, sub[n],
			    mkexpr(OPSTAR, prod, cpexpr(dimp->dims[n].dimsize)) );
		if(checksubs || np->vstg!=STGARG)
			prod = mkexpr(OPMINUS, prod, cpexpr(dimp->baseoffset));

/* Add in the run-time bounds check */

		if(checksubs)
			prod = subcheck(np, prod);
		size = np->vtype == TYCHAR ?
		    (expptr) cpexpr(np->vleng) : ICON(typesize[np->vtype]);
		prod = mkexpr(OPSTAR, prod, size);
		offp = mkexpr(OPPLUS, offp, prod);
	}

/* Check for substring indicator */

	if(p->fcharp && np->vtype==TYCHAR) {
		e = p->fcharp;
		e1 = mkexpr(OPMINUS, cpexpr(e), ICON(1));
		if (!ISCONST(e) && (e->tag != TPRIM || e->primblock.argsp)) {
			e = (expptr)mktmp(TYLONG, ENULL);
			putout(putassign(cpexpr(e), e1));
			p->fcharp = mkexpr(OPPLUS, cpexpr(e), ICON(1));
			e1 = e;
			}
		offp = mkexpr(OPPLUS, offp, e1);
		}
	return(offp);
}




expptr subcheck(Namep np, expptr p)
//Namep np;
//register expptr p;
{
	struct Dimblock *dimp;
	expptr t, checkvar, checkcond, badcall;

	dimp = np->vdim;
	if(dimp->nelt == NULL)
		return(p);	/* don't check arrays with * bounds */
	np->vlastdim = 0;
	if( ISICON(p) )
	{

/* check for negative (constant) offset */

		if(p->constblock.Const.ci < 0)
			goto badsub;
		if( ISICON(dimp->nelt) )

/* see if constant offset exceeds the array declaration */

			if(p->constblock.Const.ci < dimp->nelt->constblock.Const.ci)
				return(p);
			else
				goto badsub;
	}

/* We know that the subscript offset   p   or   dimp -> nelt   is not a constant.
   Now find a register to use for run-time bounds checking */

	if(p->tag==TADDR && p->addrblock.vstg==STGREG)
	{
		checkvar = (expptr) cpexpr(p);
		t = p;
	}
	else	{
		checkvar = (expptr) mktmp(p->headblock.vtype, ENULL);
		t = mkexpr(OPASSIGN, cpexpr(checkvar), p);
	}
	checkcond = mkexpr(OPLT, t, cpexpr(dimp->nelt) );
	if( ! ISICON(p) )
		checkcond = mkexpr(OPAND, checkcond,
		    mkexpr(OPLE, ICON(0), cpexpr(checkvar)) );

/* Construct the actual test */

	badcall = call4(p->headblock.vtype, "s_rnge",
	    mkstrcon(strlen(np->fvarname), np->fvarname),
	    mkconv(TYLONG,  cpexpr(checkvar)),
	    mkstrcon(strlen(procname), procname),
	    ICON(lineno) );
	badcall->exprblock.opcode = OPCCALL;
	p = mkexpr(OPQUEST, checkcond,
	    mkexpr(OPCOLON, checkvar, badcall));

	return(p);

badsub:
	frexpr(p);
	errstr("subscript on variable %s out of range", np->fvarname);
	return ( ICON(0) );
}




Addrp mkaddr(Namep p)
//register Namep p;
{
	Extsym *extp;
	register Addrp t;
	//Addrp intraddr();
	int k;

	switch( p->vstg)
	{
	case STGAUTO:
		if(p->vclass == CLPROC && p->vprocclass == PTHISPROC)
			return (Addrp) cpexpr((expptr)xretslot[p->vtype]);
		goto other;

	case STGUNKNOWN:
		if(!cxx && p->vclass != CLPROC)
			break;	/* Error */

        if(p->vclass == CLPROC)
        {
            extp = mkext(p->fvarname, addunder(p->cvarname, false));
            extp->extstg = STGEXT;
            p->vstg = STGEXT;
            p->vardesc.varno = extp - extsymtab;
            p->vprocclass = PEXTERNAL;
            if((extp->exproto || infertypes)
                && (p->vtype == TYUNKNOWN || p->vimpltype)
                && (k = extp->extype))
                inferdcl(p, k);
        }
        else
        {
            int dummy = 42;
        }

	case STGCOMMON:
	case STGEXT:
	case STGBSS:
	case STGINIT:
	case STGEQUIV:
	case STGARG:
	case STGLENG:
 other:
		t = ALLOC(Addrblock);
		t->tag = TADDR;

		t->vclass = p->vclass;
		t->vtype = p->vtype;
		t->vstg = p->vstg;
		t->memno = p->vardesc.varno;
		t->memoffset = ICON(p->voffset);
		if (p->vdim)
		    t->isarray = 1;
		if(p->vleng)
		{
			t->vleng = (expptr) cpexpr(p->vleng);
			if( ISICON(t->vleng) )
				t->varleng = t->vleng->constblock.Const.ci;
		}

/* Keep the original name around for the C code generation */

		t -> uname_tag = UNAM_NAME;
		t -> user.name = p;
		return(t);

	case STGINTR:

		return ( intraddr (p));
	}
	badstg("mkaddr", p->vstg);
	/* NOT REACHED */ return 0;
}




/* mkarg -- create storage for a new parameter.  This is called when a
   function returns a string (for the return value, which is the first
   parameter), or when a variable-length string is passed to a function. */

Addrp mkarg(int type, int argno)
//int type, argno;
{
	register Addrp p;

	p = ALLOC(Addrblock);
	p->tag = TADDR;
	p->vtype = type;
	p->vclass = CLVAR;

/* TYLENG is the type of the field holding the length of a character string */

	p->vstg = (type==TYLENG ? STGLENG : STGARG);
	p->memno = argno;
	return(p);
}




/* mkprim -- Create a PRIM (primary/primitive) block consisting of a
   Nameblock (or Paramblock), arguments (actual params or array
   subscripts) and substring bounds.  Requires that   v   have lots of
   extra (uninitialized) storage, since it could be a paramblock or
   nameblock */

expptr mkprim(Namep v0, struct Listblock *args, chainp substr)
 //Namep v0;
 //struct Listblock *args;
 //chainp substr;
{
	typedef union {
		/*struct*/ Paramblock paramblock;
		struct Nameblock nameblock;
		struct Headblock headblock;
		} *Primu;
	register Primu v = (Primu)v0;
	register struct Primblock *p;

    if (!v && in_interface_definition) {
        return nullptr;
    }

    if(v->headblock.vclass == CLPARAM)
	{

/* v   is to be a Paramblock */

        if(!cxx)
        {
            if(args || substr)
            {
                errstr("no qualifiers on parameter name %s",
                    v->paramblock.fvarname);
                frexpr((expptr)args);
                if(substr)
                {
                    frexpr((tagptr)substr->datap);
                    frexpr((tagptr)substr->nextp->datap);
                    frchain(&substr);
                }
                frexpr((expptr)v);
                return( errnode() );
            }
        }

        if(symbolic_params)
        {
            p = ALLOC(Primblock);
            p->tag = TPRIM;
            p->vtype = v->paramblock.vtype;

            /* v   is to be a Nameblock */

            p->namep = mkname(v->paramblock.fvarname);
            //p->argsp = args;
            if(substr)
            {
                p->fcharp = (expptr)substr->datap;
                p->lcharp = (expptr)substr->nextp->datap;
                p->scharp = (expptr)substr->nextp->nextp->datap;
                frchain(&substr);
            }
            return((expptr)p);
        }
        else
        {
            if(cxx)
            {
                assert(false);
            }
            return( (expptr) cpexpr((expptr)v->paramblock.paramval->datap) );
        }
	}

	p = ALLOC(Primblock);
	p->tag = TPRIM;
	p->vtype = v->nameblock.vtype;

/* v   is to be a Nameblock */

	p->namep = (Namep) v;
	p->argsp = args;
	if(substr)
	{
		p->fcharp = (expptr) substr->datap;
        p->lcharp = (expptr)substr->nextp->datap;
        p->scharp = (expptr) substr->nextp->nextp->datap;
		frchain(&substr);
	}
	return( (expptr) p);
}



/* vardcl -- attempt to fill out the Name template for variable   v.
   This function is called on identifiers known to be variables or
   recursive references to the same function */

void vardcl(Namep v)
//register Namep v;
{
	struct Dimblock *t;
	expptr neltp;
	extern int doing_stmtfcn;

	if(v->vclass == CLUNKNOWN) {
		v->vclass = CLVAR;
		if (v->vinftype) {
			v->vtype = TYUNKNOWN;
			if (v->vdcldone) {
				v->vdcldone = 0;
				impldcl(v);
				}
			}
		}
	if(v->vdcldone)
		return;
	if(v->vclass == CLNAMELIST)
		return;

	if(v->vtype == TYUNKNOWN)
    {
        if(!cxx)
        {
            impldcl(v);
        }
    }
	else if(v->vclass!=CLVAR && v->vprocclass!=PTHISPROC)
	{
        if(symbolic_params && v->vclass == CLPARAM)
        { }
        else
        {
            if(!cxx)
            {
                dclerr("used as variable", v);
                return;
            }
        }
	}
	if(v->vstg==STGUNKNOWN) {
        if(cxx && v->vtype == TYUNKNOWN)
        {
            int dummy = 42;
        }
        else
        {
            if (doing_stmtfcn) {
                /* neither declare this variable if its only use */
                /* is in defining a stmt function, nor complain  */
                /* that it is never used */
                v->vimpldovar = 1;
                return;
            }
            v->vstg = implstg[ letter(v->fvarname[0]) ];
            v->vimplstg = 1;
        }
	}

/* Compute the actual storage location, i.e. offsets from base addresses,
   possibly the stack pointer */

	switch(v->vstg)
	{
	case STGBSS:
		v->vardesc.varno = ++lastvarno;
		break;
	case STGAUTO:
		if(v->vclass==CLPROC && v->vprocclass==PTHISPROC)
			break;
		if(t = v->vdim)
			if( (neltp = t->nelt) && ISCONST(neltp) ) ;
			else
				dclerr("adjustable automatic array", v);
		break;

	default:
		break;
	}
	v->vdcldone = YES;
}



/* Set the implicit type declaration of parameter   p   based on its first
   letter */

void impldcl(Namep p)
//register Namep p;
{
	register int k;
	int type;
	ftnint leng;

	if(p->vdcldone || (p->vclass==CLPROC && p->vprocclass==PINTRINSIC) )
		return;
	if(p->vtype == TYUNKNOWN)
	{
		k = letter(p->fvarname[0]);
		type = impltype[ k ];
		leng = implleng[ k ];
		if(type == TYUNKNOWN)
		{
			if(p->vclass == CLPROC)
				return;
			dclerr("attempt to use undefined variable", p);
			type = dflttype[k];
			leng = 0;
		}
		settype(p, type, mkintcon(leng), 0);
		p->vimpltype = 1;
	}
}

 void
inferdcl(Namep np,int type)
 //Namep np;
 //int type;
{
	int k = impltype[letter(np->fvarname[0])];
	if (k != type) {
		np->vinftype = 1;
		np->vtype = type;
		frexpr(np->vleng);
		np->vleng = 0;
		}
	np->vimpltype = 0;
	np->vinfproc = 1;
	}


#define ICONEQ(z, c)  (ISICON(z) && z->constblock.Const.ci==c)
#define COMMUTE	{ if(!symbolic_params){ e = lp;  lp = rp;  rp = e; } }


bool is_scalar_variable(expptr v)
{
    switch(v->tag)
    {
    case TADDR:
        {
            int i = 42;
        }
        break;

    case TPRIM:
        {
            if(v->primblock.namep->vdim)
            {
                if(v->primblock.namep->vdim->dims > 0)
                {
                    return false;
                }
            }
        }
        break;

    case TCONST:
        {
            return true;
            //if(v->constblock.Const
            //{
            //    if(v->primblock.namep->vdim->dims > 0)
            //    {
            //        return false;
            //    }
            //}
        }
        break;

    case TSPECIALLIST:
        {
            switch(v->listblock.vtype)
            {
            case VTSPECIALLIST_SUBARRAY:
                {
                    return false;
                }
                break;

            default:
                {
                    int i = 42;
                }
                break;
            }
        }
        break;

    case TEXPR:
        {
            switch(v->exprblock.opcode)
            {
            case OPCALL:
                {
                    switch(v->exprblock.rightp->tag)
                    {
                    case TLIST:
                        {
                            auto lb = v->exprblock.rightp->listblock;
                            for(auto q = lb.listp; q; q = q->nextp)
                            {
                                auto e = (expptr)q->datap;
                                if(!is_scalar_variable(e))
                                {
                                    return false;
                                }
                            }
                        }
                        break;

                    default:
                        {
                            int i = 42;
                        }
                        break;
                    }

                }
                break;

            case OPMEMBER:
            case OPMEMBER_OF_ALLOCATABLE:
                {
                    // hard to tell...
                }
                break;

            default :
                {
                    int i = 42;
                }
                break;
            }
        }
        break;

    default:
        {
            int i = 42;
        }
        break;
    }
    return true;
}

bool is_allocatable(expptr v)
{
    switch (v->tag)
    {
    case TADDR:
    {
        return v->addrblock.user.name->is_allocatable;
    }
    break;

    case TPRIM:
    {
        return v->primblock.namep->is_allocatable;
    }
    break;

    case TCONST:
    {
        return false;
    }
    break;

    case TSPECIALLIST:
    {
        return false;
    }
    break;

    case TEXPR:
    {
        return false;
    }
    break;

    default:
    {
        int i = 42;
    }
    break;
    }
    return false;
}
 
expptr mkopmember(int opcode, expptr lp, expptr rp)
{
    if(!is_scalar_variable(lp))
    {
        //auto e = mkexpr(opcode, lp, rp);
        //return mkexpr(OPCALL_MEMBER, e, nullptr);
        
//        return mkexpr(OPCALL, rp, lp);
        return mkexpr(OPCALL_MEMFUNC, rp, (expptr)mklist(mkchain((char *)lp, CHNULL)));
    }
    else
    {
        assert(opcode == OPMEMBER);
        if (is_allocatable(lp)) {
            opcode = OPMEMBER_OF_ALLOCATABLE;
        }

        return mkexpr(opcode, lp, rp);
    }
}

// replaces TLIST by TSPECIALLIST/VTSPECIALLIST_CONST_PARAM_INIT_BRACE
expptr convert_to_init_list(expptr e) {
    switch (e->tag) {
    case TLIST :
    {
        e->tag = TSPECIALLIST;
        e->listblock.vtype = VTSPECIALLIST_CONST_PARAM_INIT_BRACE;
        if (e->listblock.listp)
        {
            for (auto pp = e->listblock.listp; pp; pp = pp->nextp)
            {
                convert_to_init_list((expptr)pp->datap);
            }
        }
    }
    break;
    }

    return e;
}

     /* mkexpr -- Make expression, and simplify constant subcomponents (tree
   order is not preserved).  Assumes that   lp   is nonempty, and uses
   fold()   to simplify adjacent constants */

expptr mkexpr(int opcode, expptr lp, expptr rp)
//int opcode;
//register expptr lp, rp;
{
	register expptr e, e1;
	int etype;
	int ltype, rtype;
	int ltag, rtag;
	long L;

	ltype = lp->headblock.vtype;
	ltag = lp->tag;
	if(rp && opcode!=OPCALL && opcode != OPCCALL
        && opcode != OPINIT
        && opcode != OPCALL_MEMBER
        && opcode != OPCALL_MEMFUNC
        && opcode != OP_OPT_SOURCE
        && opcode != OP_OPT_STAT
        )
	{
		rtype = rp->headblock.vtype;
		rtag = rp->tag;
	}
	else rtype = 0;

	etype = cktype(opcode, ltype, rtype);
	if(etype == TYERROR)
		goto error;

	switch(opcode)
	{
		/* check for multiplication by 0 and 1 and addition to 0 */

	case OPSTAR:
		if( ISCONST(lp) )
			COMMUTE

			    if( ISICON(rp) )
			{
				if(rp->constblock.Const.ci == 0)
					goto retright;
				goto mulop;
			}
		break;

	case OPSLASH:
	case OPMOD:
		if( ICONEQ(rp, 0) )
		{
			err("attempted division by zero");
			rp = ICON(1);
			break;
		}
		if(opcode == OPMOD)
			break;

/* Handle multiplying or dividing by 1, -1 */

mulop:
		if( ISICON(rp) )
		{
			if(rp->constblock.Const.ci == 1)
				goto retleft;

			if(rp->constblock.Const.ci == -1)
			{
				frexpr(rp);
				return( mkexpr(OPNEG, lp, ENULL) );
			}
		}

/* Group all constants together.  In particular,

	(x * CONST1) * CONST2 ==> x * (CONST1 * CONST2)
	(x * CONST1) / CONST2 ==> x * (CONST1 / CONST2)
*/

		if (lp->tag != TEXPR || !lp->exprblock.rightp
				|| !ISICON(lp->exprblock.rightp))
			break;

		if (lp->exprblock.opcode == OPLSHIFT) {
			L = 1 << lp->exprblock.rightp->constblock.Const.ci;
			if (opcode == OPSTAR || ISICON(rp) &&
					!(L % rp->constblock.Const.ci)) {
				lp->exprblock.opcode = OPSTAR;
				lp->exprblock.rightp->constblock.Const.ci = L;
				}
			}

		if (lp->exprblock.opcode == OPSTAR) {
			if(opcode == OPSTAR)
				e = mkexpr(OPSTAR, lp->exprblock.rightp, rp);
			else if(ISICON(rp) &&
			    (lp->exprblock.rightp->constblock.Const.ci %
			    rp->constblock.Const.ci) == 0)
				e = mkexpr(OPSLASH, lp->exprblock.rightp, rp);
			else	break;

			e1 = lp->exprblock.leftp;
			free( (charptr) lp );
			return( mkexpr(OPSTAR, e1, e) );
			}
		break;


	case OPPLUS:
		if( ISCONST(lp) )
			COMMUTE
			    goto addop;

	case OPMINUS:
		if( ICONEQ(lp, 0) )
		{
			frexpr(lp);
			return( mkexpr(OPNEG, rp, ENULL) );
		}

		if( ISCONST(rp) && is_negatable((Constp)rp))
		{
			opcode = OPPLUS;
			consnegop((Constp)rp);
		}

/* Group constants in an addition expression (also subtraction, since the
   subtracted value was negated above).  In particular,

	(x + CONST1) + CONST2 ==> x + (CONST1 + CONST2)
*/

addop:
		if( ISICON(rp) )
		{
			if(rp->constblock.Const.ci == 0)
				goto retleft;
			if( ISPLUSOP(lp) && ISICON(lp->exprblock.rightp) )
			{
				e = mkexpr(OPPLUS, lp->exprblock.rightp, rp);
				e1 = lp->exprblock.leftp;
				free( (charptr) lp );
				return( mkexpr(OPPLUS, e1, e) );
			}
		}
		if (opcode == OPMINUS && (ISINT(etype) || doing_vleng)) {
			/* check for (i [+const]) - (i [+const]) */
			if (lp->tag == TPRIM)
				e = lp;
			else if (lp->tag == TEXPR && lp->exprblock.opcode == OPPLUS
					&& lp->exprblock.rightp->tag == TCONST) {
				e = lp->exprblock.leftp;
				if (e->tag != TPRIM)
					break;
				}
			else
				break;
			if (e->primblock.argsp)
				break;
			if (rp->tag == TPRIM)
				e1 = rp;
			else if (rp->tag == TEXPR && rp->exprblock.opcode == OPPLUS
					&& rp->exprblock.rightp->tag == TCONST) {
				e1 = rp->exprblock.leftp;
				if (e1->tag != TPRIM)
					break;
				}
			else
				break;
			if (e->primblock.namep != e1->primblock.namep
					|| e1->primblock.argsp)
				break;
			L = e == lp ? 0 : lp->exprblock.rightp->constblock.Const.ci;
			if (e1 != rp)
				L -= rp->exprblock.rightp->constblock.Const.ci;
			frexpr(lp);
			frexpr(rp);
			return ICON(L);
			}

		break;


	case OPPOWER:
		break;

/* Eliminate outermost double negations */

	case OPNEG:
	case OPNEG1:
		if(ltag==TEXPR && lp->exprblock.opcode==OPNEG)
		{
			e = lp->exprblock.leftp;
			free( (charptr) lp );
			return(e);
		}
		break;

/* Eliminate outermost double NOTs */

	case OPNOT:
		if(ltag==TEXPR && lp->exprblock.opcode==OPNOT)
		{
			e = lp->exprblock.leftp;
			free( (charptr) lp );
			return(e);
		}
		break;

    case OP_OPT_STAT:
    case OP_OPT_SOURCE:
        etype = ltype;
        //if(lp != NULL && lp->listblock.listp == NULL)
        //{
        //    free((charptr)lp);
        //    lp = NULL;
        //}
        break;

    case OPCALL_MEMBER:
    case OPCALL_MEMFUNC:
    case OPINIT:
    case OPCALL:
	case OPCCALL:
        if(cxx)
        {
            if(lp && lp->tag == TPRIM &&
                lp->primblock.namep
                )
            {
                lp->primblock.namep->is_invoked = 1;
            }
        }
		etype = ltype;
		if(rp!=NULL && rp->listblock.listp==NULL)
		{
			free( (charptr) rp );
			rp = NULL;
		}
		break;

	case OPAND:
	case OPOR:
		if( ISCONST(lp) )
			COMMUTE

			    if( ISCONST(rp) )
			{
				if(rp->constblock.Const.ci == 0)
					if(opcode == OPOR)
						goto retleft;
					else
						goto retright;
				else if(opcode == OPOR)
					goto retright;
				else
					goto retleft;
			}
	case OPEQV:
	case OPNEQV:

	case OPBITAND:
	case OPBITOR:
	case OPBITXOR:
	case OPBITNOT:
	case OPLSHIFT:
	case OPRSHIFT:

	case OPLT:
	case OPGT:
	case OPLE:
	case OPGE:
	case OPEQ:
	case OPNE:

	case OPCONCAT:
		break;
	case OPMIN:
	case OPMAX:
	case OPMIN2:
	case OPMAX2:
	case OPDMIN:
	case OPDMAX:

	case OPASSIGN:
	case OPASSIGNI:
	case OPPLUSEQ:
	case OPSTAREQ:
	case OPMINUSEQ:
	case OPSLASHEQ:
	case OPMODEQ:
	case OPLSHIFTEQ:
	case OPRSHIFTEQ:
	case OPBITANDEQ:
	case OPBITXOREQ:
	case OPBITOREQ:

	case OPCONV:
	case OPADDR:
	case OPWHATSIN:

	case OPCOMMA:
	case OPCOMMA_ARG:
	case OPQUEST:
	case OPCOLON:
	case OPDOT:
	case OPARROW:
	case OPIDENTITY:
	case OPCHARCAST:
	case OPABS:
	case OPDABS:

    case OPMEMBER:
    case OPMEMBER_OF_ALLOCATABLE:
    case OPSTRCAT:
        break;

	default:
		badop("mkexpr", opcode);
	}

	e = (expptr) ALLOC(Exprblock);
	e->exprblock.tag = TEXPR;
	e->exprblock.opcode = opcode;
	e->exprblock.vtype = etype;
	e->exprblock.leftp = lp;
	e->exprblock.rightp = rp;
    if(!symbolic_params)
    {
        if(ltag==TCONST && (rp==0 || rtag==TCONST) )
            e = fold(e);
    }
	return(e);

retleft:
	frexpr(rp);
	if (lp->tag == TPRIM)
		lp->primblock.parenused = 1;
	return(lp);

retright:
	frexpr(lp);
	if (rp->tag == TPRIM)
		rp->primblock.parenused = 1;
	return(rp);

error:
	frexpr(lp);
	if(rp && opcode!=OPCALL && opcode != OPCCALL 
        && opcode != OPINIT
        && opcode != OPCALL_MEMBER
        && opcode != OPCALL_MEMFUNC
        //&& opcode != OP_OPT_SOURCE
        //&& opcode != OP_OPT_STAT
        )
		frexpr(rp);
	return( errnode() );
}

bool is_const(expptr p){

    auto is_prim_const = [&](){
        if(p->tag == TPRIM)
        {
            if(p->primblock.namep->vclass == CLPARAM)
            {
                Paramblock* pb = (Paramblock*)p->primblock.namep;
                for(auto e_ = pb->paramval; e_; e_ = e_->nextp)
                {
                    auto e = (expptr)e_->datap;
                    if(!is_const(e))
                    {
                        return false;
                    }
                }
                return true;
            }
            if(p->primblock.namep->vclass == CLVAR)
            {
                // vstg == STGUNKNOWN -> assume it's a constant
                return p->primblock.namep->vstg == STGUNKNOWN;
            }

//            auto q = mkname(p->primblock.namep->fvarname);
            //int i = p->primblock.namep;
        }
        return false;
    };

    //auto is_name_const = [&](){
    //    if(p->tag == TNAME)
    //    {
    //        int i = p->primblock.vtype;
    //    }
    //    return false;
    //};


    return !p ||
        p->tag == TCONST ||
        (p->tag == TEXPR && is_const(p->exprblock.leftp) && is_const(p->exprblock.rightp)) ||
        is_prim_const() /*|| is_name_const()*/
            ;
}


#define ERR(s)   { errs = s; goto error; }

/* cktype -- Check and return the type of the expression */

int cktype(int op, int lt, int rt)
//register int op, lt, rt;
{
	char *errs;

	if(lt==TYERROR || rt==TYERROR)
		goto error1;

	if(lt==TYUNKNOWN)
		return(TYUNKNOWN);
	if(rt==TYUNKNOWN)

/* If not unary operation, return UNKNOWN */
    {

        if(!is_unary_op (op) && op != OPCALL && op != OPCCALL 
            && op != OPINIT
            && op != OPCALL_MEMBER
            && op != OPCALL_MEMFUNC
            && op != OP_OPT_SOURCE
            && op != OP_OPT_STAT
            )
            return(TYUNKNOWN);
    }

	switch(op)
	{
	case OPPLUS:
    {
        if (cxx) {  // to support + as string concatination
            return(maxtype(lt, rt));
        }
    }
	case OPMINUS:
	case OPSTAR:
	case OPSLASH:
	case OPPOWER:
	case OPMOD:
		if( ISNUMERIC(lt) && ISNUMERIC(rt) )
			return( maxtype(lt, rt) );
		ERR("nonarithmetic operand of arithmetic operator")

	case OPNEG:
	case OPNEG1:
		if( ISNUMERIC(lt) )
			return(lt);
		ERR("nonarithmetic operand of negation")

	case OPNOT:
		if(ISLOGICAL(lt))
			return(lt);
		ERR("NOT of nonlogical")

	case OPAND:
	case OPOR:
	case OPEQV:
	case OPNEQV:
		if(ISLOGICAL(lt) && ISLOGICAL(rt))
			return( maxtype(lt, rt) );
		ERR("nonlogical operand of logical operator")

	case OPLT:
	case OPGT:
	case OPLE:
	case OPGE:
	case OPEQ:
	case OPNE:
		if(lt==TYCHAR || rt==TYCHAR || ISLOGICAL(lt) || ISLOGICAL(rt))
		{
			if(lt != rt){
				if (htype
					&& (lt == TYCHAR && ISNUMERIC(rt)
					 || rt == TYCHAR && ISNUMERIC(lt)))
						return TYLOGICAL;
				ERR("illegal comparison")
				}
		}

		else if( ISCOMPLEX(lt) || ISCOMPLEX(rt) )
		{
			if(op!=OPEQ && op!=OPNE)
				ERR("order comparison of complex data")
		}

		else if( ! ISNUMERIC(lt) || ! ISNUMERIC(rt) )
			ERR("comparison of nonarithmetic data")
			    return(TYLOGICAL);

    case OPSTRCAT:
    case OPCONCAT:
		if(lt==TYCHAR && rt==TYCHAR)
			return(TYCHAR);
		ERR("concatenation of nonchar data")

    case OP_OPT_SOURCE:
    case OP_OPT_STAT:
        return TYUNKNOWN;

    case OPCALL_MEMBER:
    case OPCALL_MEMFUNC:
    case OPCALL:
    case OPINIT:
	case OPCCALL:
	case OPIDENTITY:
		return(lt);

	case OPADDR:
	case OPCHARCAST:
		return(TYADDR);

	case OPCONV:
		if(rt == 0)
			return(0);
		if(lt==TYCHAR && ISINT(rt) )
			return(TYCHAR);
		if (ISLOGICAL(lt) && ISLOGICAL(rt))
			return lt;
	case OPASSIGN:
	case OPASSIGNI:
	case OPMINUSEQ:
	case OPPLUSEQ:
	case OPSTAREQ:
	case OPSLASHEQ:
	case OPMODEQ:
	case OPLSHIFTEQ:
	case OPRSHIFTEQ:
	case OPBITANDEQ:
	case OPBITXOREQ:
	case OPBITOREQ:
		if( ISINT(lt) && rt==TYCHAR)
			return(lt);
		if (ISLOGICAL(lt) && ISLOGICAL(rt) && op == OPASSIGN)
			return lt;
		if(lt==TYCHAR || rt==TYCHAR || ISLOGICAL(lt) || ISLOGICAL(rt))
			if((op!=OPASSIGN && op != OPPLUSEQ && op != OPMINUSEQ)
			    || (lt!=rt))
			{
				ERR("impossible conversion")
			}
		return(lt);

	case OPMIN:
	case OPMAX:
	case OPDMIN:
	case OPDMAX:
	case OPMIN2:
	case OPMAX2:
	case OPBITOR:
	case OPBITAND:
	case OPBITXOR:
	case OPBITNOT:
	case OPLSHIFT:
	case OPRSHIFT:
	case OPWHATSIN:
	case OPABS:
	case OPDABS:
		return(lt);

	case OPCOMMA:
	case OPCOMMA_ARG:
	case OPQUEST:
	case OPCOLON:		/* Only checks the rightmost type because
				   of C language definition (rightmost
				   comma-expr is the value of the expr) */
		return(rt);

	case OPDOT:
	case OPARROW:
	    return (lt);
	    break;

    case OPMEMBER:
    case OPMEMBER_OF_ALLOCATABLE:
        return TYUNKNOWN;

	default:
		badop("cktype", op);
	}
error:
	err(errs);
error1:
	return(TYERROR);
}

/* fold -- simplifies constant expressions; it assumes that e -> leftp and
   e -> rightp are TCONST or NULL */

 LOCAL expptr
fold(expptr e)
 //register expptr e;
{
	Constp p;
	register expptr lp, rp;
	int etype, mtype, ltype, rtype, opcode;
	int i, bl, ll, lr;
	char *q, *s;
	struct Constblock lcon, rcon;
	long L;
	double d;

	opcode = e->exprblock.opcode;
	etype = e->exprblock.vtype;

	lp = e->exprblock.leftp;
	ltype = lp->headblock.vtype;
	rp = e->exprblock.rightp;

	if(rp == 0)
		switch(opcode)
		{
		case OPNOT:
			lp->constblock.Const.ci = ! lp->constblock.Const.ci;
 retlp:
			e->exprblock.leftp = 0;
			frexpr(e);
			return(lp);

		case OPBITNOT:
			lp->constblock.Const.ci = ~ lp->constblock.Const.ci;
			goto retlp;

		case OPNEG:
		case OPNEG1:
			consnegop((Constp)lp);
			goto retlp;

		case OPCONV:
		case OPADDR:
			return(e);

		case OPABS:
		case OPDABS:
			switch(ltype) {
			    case TYINT1:
			    case TYSHORT:
			    case TYLONG:
#ifdef TYQUAD
			    case TYQUAD:
#endif
				if ((L = lp->constblock.Const.ci) < 0)
					lp->constblock.Const.ci = -L;
				goto retlp;
			    case TYREAL:
			    case TYDREAL:
				if (lp->constblock.vstg) {
				    s = lp->constblock.Const.cds[0];
				    if (*s == '-')
					lp->constblock.Const.cds[0] = s + 1;
				    goto retlp;
				}
				if ((d = lp->constblock.Const.cd[0]) < 0.)
					lp->constblock.Const.cd[0] = -d;
			    case TYCOMPLEX:
			    case TYDCOMPLEX:
				return e;	/* lazy way out */
			    }
		default:
			badop("fold", opcode);
		}

	rtype = rp->headblock.vtype;

	p = ALLOC(Constblock);
	p->tag = TCONST;
	p->vtype = etype;
	p->vleng = e->exprblock.vleng;

	switch(opcode)
	{
	case OPCOMMA:
	case OPCOMMA_ARG:
	case OPQUEST:
	case OPCOLON:
		return(e);

	case OPAND:
		p->Const.ci = lp->constblock.Const.ci &&
		    rp->constblock.Const.ci;
		break;

	case OPOR:
		p->Const.ci = lp->constblock.Const.ci ||
		    rp->constblock.Const.ci;
		break;

	case OPEQV:
		p->Const.ci = lp->constblock.Const.ci ==
		    rp->constblock.Const.ci;
		break;

	case OPNEQV:
		p->Const.ci = lp->constblock.Const.ci !=
		    rp->constblock.Const.ci;
		break;

	case OPBITAND:
		p->Const.ci = lp->constblock.Const.ci &
		    rp->constblock.Const.ci;
		break;

	case OPBITOR:
		p->Const.ci = lp->constblock.Const.ci |
		    rp->constblock.Const.ci;
		break;

	case OPBITXOR:
		p->Const.ci = lp->constblock.Const.ci ^
		    rp->constblock.Const.ci;
		break;

	case OPLSHIFT:
		p->Const.ci = lp->constblock.Const.ci <<
		    rp->constblock.Const.ci;
		break;

	case OPRSHIFT:
		p->Const.ci = lp->constblock.Const.ci >>
		    rp->constblock.Const.ci;
		break;

	case OPCONCAT:
		ll = lp->constblock.vleng->constblock.Const.ci;
		lr = rp->constblock.vleng->constblock.Const.ci;
		bl = lp->constblock.Const.ccp1.blanks;
		p->Const.ccp = q = (char *) ckalloc(ll+lr+bl);
		p->Const.ccp1.blanks = rp->constblock.Const.ccp1.blanks;
		p->vleng = ICON(ll+lr+bl);
		s = lp->constblock.Const.ccp;
		for(i = 0 ; i < ll ; ++i)
			*q++ = *s++;
		for(i = 0 ; i < bl ; i++)
			*q++ = ' ';
		s = rp->constblock.Const.ccp;
		for(i = 0; i < lr; ++i)
			*q++ = *s++;
		break;


	case OPPOWER:
		if( ! ISINT(rtype) )
			return(e);
		conspower(p, (Constp)lp, rp->constblock.Const.ci);
		break;

    case OPSTRCAT:
    case OPMEMBER:
    case OPMEMBER_OF_ALLOCATABLE:
        return e;

	default:
		if(ltype == TYCHAR)
		{
			lcon.Const.ci = cmpstr(lp->constblock.Const.ccp,
			    rp->constblock.Const.ccp,
			    lp->constblock.vleng->constblock.Const.ci,
			    rp->constblock.vleng->constblock.Const.ci);
			rcon.Const.ci = 0;
			mtype = tyint;
		}
		else	{
			mtype = maxtype(ltype, rtype);
			consconv(mtype, &lcon, &lp->constblock);
			consconv(mtype, &rcon, &rp->constblock);
		}
		consbinop(opcode, mtype, p, &lcon, &rcon);
		break;
	}

	frexpr(e);
	return( (expptr) p );
}



/* assign constant l = r , doing coercion */

void consconv(int lt, Constp lc, Constp rc)
 //int lt;
 //register Constp lc, rc;
{
	int rt = rc->vtype;
	register union Constant *lv = &lc->Const, *rv = &rc->Const;

	lc->vtype = lt;
	if (ONEOF(lt, MSKREAL|MSKCOMPLEX) && ONEOF(rt, MSKREAL|MSKCOMPLEX)) {
		memcpy((char *)lv, (char *)rv, sizeof(union Constant));
		lc->vstg = rc->vstg;
		if (ISCOMPLEX(lt) && ISREAL(rt)) {
			if (rc->vstg)
				lv->cds[1] = cds("0",CNULL);
			else
				lv->cd[1] = 0.;
			}
		return;
		}
	lc->vstg = 0;

	switch(lt)
	{

/* Casting to character means just copying the first sizeof (character)
   bytes into a new 1 character string.  This is weird. */

	case TYCHAR:
		*(lv->ccp = (char *) ckalloc(1)) = (char)rv->ci;
		lv->ccp1.blanks = 0;
		break;

	case TYINT1:
	case TYSHORT:
	case TYLONG:
#ifdef TYQUAD
	case TYQUAD:
#endif
		if(rt == TYCHAR)
			lv->ci = rv->ccp[0];
		else if( ISINT(rt) )
			lv->ci = rv->ci;
		else	lv->ci = (ftnint)(rc->vstg ? atof(rv->cds[0]) : rv->cd[0]);

		break;

	case TYCOMPLEX:
	case TYDCOMPLEX:
		lv->cd[1] = 0.;
		lv->cd[0] = rv->ci;
		break;

	case TYREAL:
	case TYDREAL:
		lv->cd[0] = rv->ci;
		break;

	case TYLOGICAL:
	case TYLOGICAL1:
	case TYLOGICAL2:
		lv->ci = rv->ci;
		break;
	}
}



/* Negate constant value -- changes the input node's value */

void consnegop(Constp p)
//register Constp p;
{
	register char *s;

	if (p->vstg) {
		if (ISCOMPLEX(p->vtype)) {
			s = p->Const.cds[1];
			p->Const.cds[1] = *s == '-' ? s+1
					: *s == '0' ? s : s-1;
			}
		s = p->Const.cds[0];
		p->Const.cds[0] = *s == '-' ? s+1
				: *s == '0' ? s : s-1;
		return;
		}
	switch(p->vtype)
	{
	case TYINT1:
	case TYSHORT:
	case TYLONG:
#ifdef TYQUAD
	case TYQUAD:
#endif
		p->Const.ci = - p->Const.ci;
		break;

	case TYCOMPLEX:
	case TYDCOMPLEX:
		p->Const.cd[1] = - p->Const.cd[1];
		/* fall through and do the real parts */
	case TYREAL:
	case TYDREAL:
		p->Const.cd[0] = - p->Const.cd[0];
		break;
	default:
		badtype("consnegop", p->vtype);
	}
}



/* conspower -- Expand out an exponentiation */

 LOCAL void
conspower(Constp p, Constp ap, ftnint n)
 //Constp p, ap;
 //ftnint n;
{
	register union Constant *powp = &p->Const;
	register int type;
	struct Constblock x, x0;

	if (n == 1) {
		memcpy((char *)powp, (char *)&ap->Const, sizeof(ap->Const));
		return;
		}

	switch(type = ap->vtype)	/* pow = 1 */
	{
	case TYINT1:
	case TYSHORT:
	case TYLONG:
#ifdef TYQUAD
	case TYQUAD:
#endif
		powp->ci = 1;
		break;
	case TYCOMPLEX:
	case TYDCOMPLEX:
		powp->cd[1] = 0;
	case TYREAL:
	case TYDREAL:
		powp->cd[0] = 1;
		break;
	default:
		badtype("conspower", type);
	}

	if(n == 0)
		return;
	switch(type)	/* x0 = ap */
	{
	case TYINT1:
	case TYSHORT:
	case TYLONG:
#ifdef TYQUAD
	case TYQUAD:
#endif
		x0.Const.ci = ap->Const.ci;
		break;
	case TYCOMPLEX:
	case TYDCOMPLEX:
		x0.Const.cd[1] =
			ap->vstg ? atof(ap->Const.cds[1]) : ap->Const.cd[1];
	case TYREAL:
	case TYDREAL:
		x0.Const.cd[0] =
			ap->vstg ? atof(ap->Const.cds[0]) : ap->Const.cd[0];
		break;
	}
	x0.vtype = type;
	x0.vstg = 0;
	if(n < 0)
	{
		if( ISINT(type) )
		{
			err("integer ** negative number");
			return;
		}
		else if (!x0.Const.cd[0]
				&& (!ISCOMPLEX(type) || !x0.Const.cd[1])) {
			err("0.0 ** negative number");
			return;
			}
		n = -n;
		consbinop(OPSLASH, type, &x, p, &x0);
	}
	else
		consbinop(OPSTAR, type, &x, p, &x0);

	for( ; ; )
	{
		if(n & 01)
			consbinop(OPSTAR, type, p, p, &x);
		if(n >>= 1)
			consbinop(OPSTAR, type, &x, &x, &x);
		else
			break;
	}
}



/* do constant operation cp = a op b -- assumes that   ap and bp   have data
   matching the input   type */

 LOCAL void
zerodiv()
{ Fatal("division by zero during constant evaluation; cannot recover"); }

 LOCAL void
consbinop(int opcode, int type, Constp cpp, Constp app, Constp bpp)
 //int opcode, type;
 //Constp cpp, app, bpp;
{
	register union Constant *ap = &app->Const,
				*bp = &bpp->Const,
				*cp = &cpp->Const;
	int k;
	double ad[2], bd[2], temp;

	cpp->vstg = 0;

	if (ONEOF(type, MSKREAL|MSKCOMPLEX)) {
		ad[0] = app->vstg ? atof(ap->cds[0]) : ap->cd[0];
		bd[0] = bpp->vstg ? atof(bp->cds[0]) : bp->cd[0];
		if (ISCOMPLEX(type)) {
			ad[1] = app->vstg ? atof(ap->cds[1]) : ap->cd[1];
			bd[1] = bpp->vstg ? atof(bp->cds[1]) : bp->cd[1];
			}
		}
	switch(opcode)
	{
	case OPPLUS:
		switch(type)
		{
		case TYINT1:
		case TYSHORT:
		case TYLONG:
#ifdef TYQUAD
		case TYQUAD:
#endif
			cp->ci = ap->ci + bp->ci;
			break;
		case TYCOMPLEX:
		case TYDCOMPLEX:
			cp->cd[1] = ad[1] + bd[1];
		case TYREAL:
		case TYDREAL:
			cp->cd[0] = ad[0] + bd[0];
			break;
		}
		break;

	case OPMINUS:
		switch(type)
		{
		case TYINT1:
		case TYSHORT:
		case TYLONG:
#ifdef TYQUAD
		case TYQUAD:
#endif
			cp->ci = ap->ci - bp->ci;
			break;
		case TYCOMPLEX:
		case TYDCOMPLEX:
			cp->cd[1] = ad[1] - bd[1];
		case TYREAL:
		case TYDREAL:
			cp->cd[0] = ad[0] - bd[0];
			break;
		}
		break;

	case OPSTAR:
		switch(type)
		{
		case TYINT1:
		case TYSHORT:
		case TYLONG:
#ifdef TYQUAD
		case TYQUAD:
#endif
			cp->ci = ap->ci * bp->ci;
			break;
		case TYREAL:
		case TYDREAL:
			cp->cd[0] = ad[0] * bd[0];
			break;
		case TYCOMPLEX:
		case TYDCOMPLEX:
			temp = ad[0] * bd[0]  -  ad[1] * bd[1] ;
			cp->cd[1] = ad[0] * bd[1]  +  ad[1] * bd[0] ;
			cp->cd[0] = temp;
			break;
		}
		break;
	case OPSLASH:
		switch(type)
		{
		case TYINT1:
		case TYSHORT:
		case TYLONG:
#ifdef TYQUAD
		case TYQUAD:
#endif
			if (!bp->ci)
				zerodiv();
			cp->ci = ap->ci / bp->ci;
			break;
		case TYREAL:
		case TYDREAL:
			if (!bd[0])
				zerodiv();
			cp->cd[0] = ad[0] / bd[0];
			break;
		case TYCOMPLEX:
		case TYDCOMPLEX:
			if (!bd[0] && !bd[1])
				zerodiv();
			zdiv((dcomplex*)cp, (dcomplex*)ad, (dcomplex*)bd);
			break;
		}
		break;

	case OPMOD:
		if( ISINT(type) )
		{
			cp->ci = ap->ci % bp->ci;
			break;
		}
		else
			Fatal("inline mod of noninteger");

	case OPMIN2:
	case OPDMIN:
		switch(type)
		{
		case TYINT1:
		case TYSHORT:
		case TYLONG:
#ifdef TYQUAD
		case TYQUAD:
#endif
			cp->ci = ap->ci <= bp->ci ? ap->ci : bp->ci;
			break;
		case TYREAL:
		case TYDREAL:
			cp->cd[0] = ad[0] <= bd[0] ? ad[0] : bd[0];
			break;
		default:
			Fatal("inline min of exected type");
		}
		break;

	case OPMAX2:
	case OPDMAX:
		switch(type)
		{
		case TYINT1:
		case TYSHORT:
		case TYLONG:
#ifdef TYQUAD
		case TYQUAD:
#endif
			cp->ci = ap->ci >= bp->ci ? ap->ci : bp->ci;
			break;
		case TYREAL:
		case TYDREAL:
			cp->cd[0] = ad[0] >= bd[0] ? ad[0] : bd[0];
			break;
		default:
			Fatal("inline max of exected type");
		}
		break;

	default:	  /* relational ops */
		switch(type)
		{
		case TYINT1:
		case TYSHORT:
		case TYLONG:
#ifdef TYQUAD
		case TYQUAD:
#endif
			if(ap->ci < bp->ci)
				k = -1;
			else if(ap->ci == bp->ci)
				k = 0;
			else	k = 1;
			break;
		case TYREAL:
		case TYDREAL:
			if(ad[0] < bd[0])
				k = -1;
			else if(ad[0] == bd[0])
				k = 0;
			else	k = 1;
			break;
		case TYCOMPLEX:
		case TYDCOMPLEX:
			if(ad[0] == bd[0] &&
			    ad[1] == bd[1] )
				k = 0;
			else	k = 1;
			break;
		}

		switch(opcode)
		{
		case OPEQ:
			cp->ci = (k == 0);
			break;
		case OPNE:
			cp->ci = (k != 0);
			break;
		case OPGT:
			cp->ci = (k == 1);
			break;
		case OPLT:
			cp->ci = (k == -1);
			break;
		case OPGE:
			cp->ci = (k >= 0);
			break;
		case OPLE:
			cp->ci = (k <= 0);
			break;
		}
		break;
	}
}



/* conssgn - returns the sign of a Fortran constant */

int conssgn(expptr p)
//register expptr p;
{
	register char *s;

    if(cxx)
    {
        if(!ISCONST(p))
        {
            return 1;
        }
    }
    else
    {
        if( ! ISCONST(p) )
            Fatal( "sgn(nonconstant)" );
    }

	switch(p->headblock.vtype)
	{
	case TYINT1:
	case TYSHORT:
	case TYLONG:
#ifdef TYQUAD
	case TYQUAD:
#endif
		if(p->constblock.Const.ci > 0) return(1);
		if(p->constblock.Const.ci < 0) return(-1);
		return(0);

	case TYREAL:
	case TYDREAL:
		if (p->constblock.vstg) {
			s = p->constblock.Const.cds[0];
			if (*s == '-')
				return -1;
			if (*s == '0')
				return 0;
			return 1;
			}
		if(p->constblock.Const.cd[0] > 0) return(1);
		if(p->constblock.Const.cd[0] < 0) return(-1);
		return(0);


/* The sign of a complex number is 0 iff the number is 0 + 0i, else it's 1 */

	case TYCOMPLEX:
	case TYDCOMPLEX:
		if (p->constblock.vstg)
			return *p->constblock.Const.cds[0] != '0'
			    && *p->constblock.Const.cds[1] != '0';
		return(p->constblock.Const.cd[0]!=0 || p->constblock.Const.cd[1]!=0);

	default:
		badtype( "conssgn", p->constblock.vtype);
	}
	/* NOT REACHED */ return 0;
}

/* conssgn - returns the sign of a Fortran constant */

conseval_t conseval(expptr p)
{
    auto prim_const_eval = [&]() -> conseval_t {
        if(p->tag == TPRIM)
        {
            if(p->primblock.namep->vclass == CLPARAM)
            {
                Paramblock* pb = (Paramblock*)p->primblock.namep;
                for(auto e_ = pb->paramval; e_; e_ = e_->nextp)
                {
                    auto e = (expptr)e_->datap;
                    return conseval(e);
                }
            }
        }
        return conseval_t{};
    };

    //auto is_name_const = [&](){
    //    if(p->tag == TNAME)
    //    {
    //        int i = p->primblock.vtype;
    //    }
    //    return false;
    //};
    switch(p->tag)
    {
    case TEXPR:
        {
            auto opcode = p->exprblock.opcode;
            auto etype = p->exprblock.vtype;

            auto lp = p->exprblock.leftp;
            auto ltype = lp->headblock.vtype;
            auto rp = p->exprblock.rightp;

            if(rp == 0)
            {
                switch(opcode)
                {
                case OPNOT:
                    return !conseval(lp);

                case OPBITNOT:
                    return ~conseval(lp);

                case OPNEG:
                case OPNEG1:
                    return -conseval(lp);

                case OPCONV:
                case OPADDR:
                    return conseval(lp);

                case OPABS:
                case OPDABS:
                    return conseval(lp).abs();
                default:
                    return conseval_t{};
                }
            }


            switch(opcode)
            {
            case OPCOMMA:
            case OPCOMMA_ARG:
            case OPQUEST:
            case OPCOLON:
                return conseval_t{};

            case OPPLUS: return conseval(lp) + conseval(rp);
            case OPMINUS: return conseval(lp) - conseval(rp);
            case OPSTAR:  return conseval(lp) * conseval(rp);
            case OPSLASH: return conseval(lp) / conseval(rp);


            case OPAND:
                return conseval(lp) && conseval(rp);

            case OPOR:
                return conseval(lp) || conseval(rp);

            case OPEQV:
                return conseval(lp) == conseval(rp);

            case OPNEQV:
                return conseval(lp) != conseval(rp);

            case OPBITAND:
                return conseval(lp) & conseval(rp);

            case OPBITOR:
                return conseval(lp) | conseval(rp);

            case OPBITXOR:
                return conseval(lp) ^ conseval(rp);

            case OPLSHIFT:
                return conseval(lp) << conseval(rp);

            case OPRSHIFT:
                return conseval(lp) >> conseval(rp);

            case OPCONCAT:
                return conseval_t{};


            case OPPOWER:
                return conseval(lp).pow(conseval(rp));


            default:
                return conseval_t{};
            }


        }
        break;

    case TCONST:
        {
            switch(p->headblock.vtype)
            {
            case TYINT1:
            case TYSHORT:
            case TYLONG:
#ifdef TYQUAD
            case TYQUAD:
#endif
                return conseval_t(p->constblock.Const.ci);

            case TYREAL:
            case TYDREAL:
                return conseval_t(p->constblock.Const.cd[0]);


                /* The sign of a complex number is 0 iff the number is 0 + 0i, else it's 1 */

            //case TYCOMPLEX:
            //case TYDCOMPLEX:
            //    if(p->constblock.vstg)
            //        return *p->constblock.Const.cds[0] != '0'
            //        && *p->constblock.Const.cds[1] != '0';
            //    return(p->constblock.Const.cd[0] != 0 || p->constblock.Const.cd[1] != 0);

            default:
                return conseval_t{};
            }
            /* NOT REACHED */ return 0;
        }
        break;
    default:
        return prim_const_eval();
    }

}

char *powint[ ] = {
	"pow_ii",
#ifdef TYQUAD
		  "pow_qi",
#endif
		  "pow_ri", "pow_di", "pow_ci", "pow_zi" };

LOCAL expptr mkpower(expptr p)
//register expptr p;
{
	register expptr q, lp, rp;
	int ltype, rtype, mtype, tyi;

	lp = p->exprblock.leftp;
	rp = p->exprblock.rightp;
	ltype = lp->headblock.vtype;
	rtype = rp->headblock.vtype;

	if (lp->tag == TADDR)
		lp->addrblock.parenused = 0;

	if (rp->tag == TADDR)
		rp->addrblock.parenused = 0;

	if(!cxx && ISICON(rp))
	{
		if(rp->constblock.Const.ci == 0)
		{
			frexpr(p);
			if( ISINT(ltype) )
				return( ICON(1) );
			else if (ISREAL (ltype))
				return mkconv (ltype, ICON (1));
			else
				return( (expptr) putconst((Constp)
					mkconv(ltype, ICON(1))) );
		}
		if(rp->constblock.Const.ci < 0)
		{
			if( ISINT(ltype) )
			{
				frexpr(p);
				err("integer**negative");
				return( errnode() );
			}
			rp->constblock.Const.ci = - rp->constblock.Const.ci;
			p->exprblock.leftp = lp
				= fixexpr((Exprp)mkexpr(OPSLASH, ICON(1), lp));
		}
		if(rp->constblock.Const.ci == 1)
		{
			frexpr(rp);
			free( (charptr) p );
			return(lp);
		}

		if( ONEOF(ltype, MSKINT|MSKREAL) ) {
			p->exprblock.vtype = ltype;
			return(p);
		}
	}

    if(cxx)
    {
        q = call2(ltype, "ft::pow", lp, rp);
    }
    else
    {
        if( ISINT(rtype) )
        {
            if(ltype==TYSHORT && rtype==TYSHORT && (!ISCONST(lp) || tyint==TYSHORT) )
                q = call2(TYSHORT, "pow_hh", lp, rp);
            else	{
                if(ONEOF(ltype,M(TYINT1)|M(TYSHORT)))
                {
                    ltype = TYLONG;
                    lp = mkconv(TYLONG,lp);
                }
#ifdef TYQUAD
                if (ltype == TYQUAD)
                    rp = mkconv(TYQUAD,rp);
                else
#endif
                    rp = mkconv(TYLONG,rp);
                if (ISCONST(rp)) {
                    tyi = tyint;
                    tyint = TYLONG;
                    rp = (expptr)putconst((Constp)rp);
                    tyint = tyi;
                }
                if(cxx)
                {
                    q = call2(ltype, "ft::pow", lp, rp);
                }
                else
                {
                    q = call2(ltype, powint[ltype - TYLONG], lp, rp);
                }
            }
        }
        else if( ISREAL( (mtype = maxtype(ltype,rtype)) )) {
            extern int callk_kludge;
            callk_kludge = TYDREAL;
            if(cxx)
            {
                q = call2(ltype, "ft::pow", lp, rp);
                //            q = call2(mtype, "ft::pow", mkconv(TYDREAL, lp), mkconv(TYDREAL, rp));
            }
            else
            {
                q = call2(mtype, "pow_dd", mkconv(TYDREAL, lp), mkconv(TYDREAL, rp));
            }
            callk_kludge = 0;
        }
        else	{
            if(cxx)
            {
                q = call2(ltype, "ft::pow", lp, rp);
            }
            else
            {
                q  = call2(TYDCOMPLEX, "pow_zz",
                    mkconv(TYDCOMPLEX,lp), mkconv(TYDCOMPLEX,rp));
                if(mtype == TYCOMPLEX)
                    q = mkconv(TYCOMPLEX, q);
            }
        }
    }
	free( (charptr) p );
	return(q);
}

/* Complex Division.  Same code as in Runtime Library
*/


 LOCAL void
zdiv(dcomplex *c, dcomplex *a, dcomplex *b)
 //register dcomplex *a, *b, *c;
{
	double ratio, den;
	double abr, abi;

	if( (abr = b->dreal) < 0.)
		abr = - abr;
	if( (abi = b->dimag) < 0.)
		abi = - abi;
	if( abr <= abi )
	{
		if(abi == 0)
			Fatal("complex division by zero");
		ratio = b->dreal / b->dimag ;
		den = b->dimag * (1 + ratio*ratio);
		c->dreal = (a->dreal*ratio + a->dimag) / den;
		c->dimag = (a->dimag*ratio - a->dreal) / den;
	}

	else
	{
		ratio = b->dimag / b->dreal ;
		den = b->dreal * (1 + ratio*ratio);
		c->dreal = (a->dreal + a->dimag*ratio) / den;
		c->dimag = (a->dimag - a->dreal*ratio) / den;
	}
}
