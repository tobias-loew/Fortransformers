/****************************************************************
Copyright 1990, 1993 by AT&T Bell Laboratories and Bellcore.

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

#include "func_def.h"
#include "defs.h"

/* ROUTINES CALLED DURING DATA AND PARAMETER STATEMENT PROCESSING */

static char datafmt[] = "%s\t%09ld\t%d";
static char *cur_varname;

extern bool in_type_definition;

extern lambda_init_map_t lambda_init_map;

Addrp nextdata(ftnint *elenp);
void setdata(Addrp varp, expptr valp, ftnint elen);
char *dataname(int stg, long memno);
void dataline(char *varname, ftnint offset, int type);


void dataval_exp(expptr datap) {

}

/* another initializer, called from parser */
void dataval(expptr repp, expptr valp)
{
	int i, nrep;
	ftnint elen;
	register Addrp p;
	//Addrp nextdata();

	if (parstate < INDATA) {
		frexpr(repp);
		goto ret;
		}
	if(repp == NULL)
		nrep = 1;
	else if (ISICON(repp) && repp->constblock.Const.ci >= 0)
		nrep = repp->constblock.Const.ci;
	else
	{
		err("invalid repetition count in DATA statement");
		frexpr(repp);
		goto ret;
	}
	frexpr(repp);

	if( ! ISCONST(valp) )
	{
//		err("non-constant initializer");
//		goto ret;
	}

	if(toomanyinit) goto ret;
	for(i = 0 ; i < nrep ; ++i)
	{
		p = nextdata(&elen);
		if(p == NULL)
		{
			err("too many initializers");
			toomanyinit = YES;
			goto ret;
		}
		setdata((Addrp)p, valp, elen);
		frexpr((expptr)p);
	}

ret:
	frexpr(valp);
}


Addrp nextdata(ftnint *elenp)
{
	register struct Impldoblock *ip;
	struct Primblock *pp;
	register Namep np;
	register struct Rplblock *rp;
	tagptr p;
	expptr neltp;
	register expptr q;
	int skip;
	ftnint off, vlen;

	while(curdtp)
	{
		p = (tagptr)curdtp->datap;
		if(p->tag == TIMPLDO)
		{
			ip = &(p->impldoblock);
			if(ip->implb==NULL || ip->impub==NULL || ip->varnp==NULL)
				fatali("bad impldoblock 0%o", (int) ip);
			if(ip->isactive)
				ip->varvp->Const.ci += ip->impdiff;
			else
			{
				q = fixtype(cpexpr(ip->implb));
				if( ! ISICON(q) )
					goto doerr;
				ip->varvp = (Constp) q;

				if(ip->impstep)
				{
					q = fixtype(cpexpr(ip->impstep));
					if( ! ISICON(q) )
						goto doerr;
					ip->impdiff = q->constblock.Const.ci;
					frexpr(q);
				}
				else
					ip->impdiff = 1;

				q = fixtype(cpexpr(ip->impub));
				if(! ISICON(q))
					goto doerr;
				ip->implim = q->constblock.Const.ci;
				frexpr(q);

				ip->isactive = YES;
				rp = ALLOC(Rplblock);
				rp->rplnextp = rpllist;
				rpllist = rp;
				rp->rplnp = ip->varnp;
				rp->rplvp = (expptr) (ip->varvp);
				rp->rpltag = TCONST;
			}

			if( (ip->impdiff>0 && (ip->varvp->Const.ci <= ip->implim))
			    || (ip->impdiff<0 && (ip->varvp->Const.ci >= ip->implim)) )
			{ /* start new loop */
				curdtp = ip->datalist;
				goto next;
			}

			/* clean up loop */

			if(rpllist)
			{
				rp = rpllist;
				rpllist = rpllist->rplnextp;
				free( (charptr) rp);
			}
			else
				Fatal("rpllist empty");

			frexpr((expptr)ip->varvp);
			ip->isactive = NO;
			curdtp = curdtp->nextp;
			goto next;
		}

		pp = (struct Primblock *) p;
		np = pp->namep;
		cur_varname = np->fvarname;
		skip = YES;

		if(p->primblock.argsp==NULL && np->vdim!=NULL)
		{   /* array initialization */
			q = (expptr) mkaddr(np);
			off = typesize[np->vtype] * curdtelt;
			if(np->vtype == TYCHAR)
				off *= np->vleng->constblock.Const.ci;
			q->addrblock.memoffset =
			    mkexpr(OPPLUS, q->addrblock.memoffset, mkintcon(off) );
			if( (neltp = np->vdim->nelt) && ISCONST(neltp))
			{
				if(++curdtelt < neltp->constblock.Const.ci)
					skip = NO;
			}
            else {
                if (!cxx) {
                    err("attempt to initialize adjustable array");
                }
                else {
                    // assume it fits
                    ++curdtelt;
                    skip = NO;
                }
            }
		}
		else
			q = mklhs((struct Primblock *)cpexpr((expptr)pp), 0);
		if(skip)
		{
			curdtp = curdtp->nextp;
			curdtelt = 0;
		}
		if(q->headblock.vtype == TYCHAR)
			if(ISICON(q->headblock.vleng))
				*elenp = q->headblock.vleng->constblock.Const.ci;
			else	{
				err("initialization of string of nonconstant length");
				continue;
			}
		else	*elenp = typesize[q->headblock.vtype];

		if (np->vstg == STGBSS) {
			vlen = np->vtype==TYCHAR
				? np->vleng->constblock.Const.ci
				: typesize[np->vtype];
			if(vlen > 0)
				np->vstg = STGINIT;
			}
		return( (Addrp) q );

doerr:
		err("nonconstant implied DO parameter");
		frexpr(q);
		curdtp = curdtp->nextp;

next:
		curdtelt = 0;
	}

	return(NULL);
}



LOCAL FILEP dfile;


void setdata(Addrp varp, expptr valp, ftnint elen)
//register Addrp varp;
//ftnint elen;
//register Constp valp;
{
	struct Constblock con;
	register int type;
	int i, k, valtype;
	ftnint offset;
    char *varname;
    //char *dataname(), *varname;
	static Addrp badvar;
	register unsigned char *s;
	static int last_lineno;
	static char *last_varname;

	if (varp->vstg == STGCOMMON) {
		if (!(dfile = blkdfile))
			dfile = blkdfile = opf(blkdfname, textwrite);
		}
	else {
		if (procclass == CLBLOCK) {
			if (varp != badvar) {
				badvar = varp;
				warn1("%s is not in a COMMON block",
					varp->uname_tag == UNAM_NAME
					? varp->user.name->fvarname
					: "???");
				}
			return;
			}
		if (!(dfile = initfile))
			dfile = initfile = opf(initfname, textwrite);
		}


//    assert(false);
	varname = dataname(varp->vstg, varp->memno);

    auto&& lambda_init = lambda_init_map[std::next(varname)];
    lambda_init.items.emplace_back();
    auto&& lambda_item = lambda_init.items.back();
    lambda_item.type = varp->vtype;
    if (varp->vtype == TY_DERIVED) {
        lambda_item.derived_type_name = varp->user.name->derived_type_name;
    }

    // check for none one-based arrays
    if (!lambda_init.lambda_required) {
        for (int i = 0; i < varp->user.name->vdim->ndim; ++i) {
            auto&& d = varp->user.name->vdim->dims[i];
            if (d.lb) {
                if (d.lb->tag != TCONST || d.lb->constblock.Const.ci != 1) {
                    lambda_init.lambda_required = true;
                    break;
                }
            }
        }
    }

    auto tsize = varp->vtype != TY_DERIVED ? typesize[varp->vtype] : 1;

    if (varp->memoffset) {
        switch (varp->memoffset->tag) {
        case TEXPR: // strange but seems to work
            {
                auto val = conseval(varp->memoffset);
                switch (val.state)
                {
                case conseval_t::state_::int_:
                    {
                        offset = val.n;
                    }
                    break;

                case conseval_t::state_::double_:
                    {
                        assert(false);
                    }
                    break;

                default:
                    {
                        assert(false);
                    }
                    break;
                }

            }break;
        case TCONST:
            {
                offset = varp->memoffset->constblock.Const.ci;
            }break;

        case TLIST:
            {
                expptr e = (expptr)(varp->memoffset->listblock.listp->datap);
                auto val = conseval(e);
                switch (val.state)
                {
                case conseval_t::state_::int_:
                    {
                        offset = val.n - 1;
                        offset *= tsize;
                    }
                    break;

                case conseval_t::state_::double_:
                    {
                        assert(false);
                    }
                    break;

                default:
                    {
                        if(e->tag == TADDR) {
                            // needs lambda-init
//                            auto&& data = lambda_init[varname];
                            offset = 0;
                            lambda_init.lambda_required = true;
                            lambda_item.index_expr = cpexpr(e);
                        } else {
                            assert(false);
                        }
                    }
                    break;
                }

                if(e && e->tag == TCONST) {
                }
            }break;

        default: 
            {
                assert(false);
            }
                 break;
        }
    }
    else {
        offset = 0;
    }

    lambda_item.offset = offset;
    lambda_item.index = (offset / tsize) + 1;


	type = varp->vtype;

    if (type == TY_DERIVED || valp->tag != TCONST) {
        assert(type != TY_DERIVED || valp->tag != TCONST);
        lambda_init.map_required = true;
        lambda_item.data_expr = cpexpr(valp);

        // dummy output to trigger generation
        dataline(varname, offset, TYLONG);
        prconi(dfile, 0);
    }
    else {
        auto valpc = (Constp)valp;
        valtype = valpc->vtype;
        if (type != TYCHAR && valtype == TYCHAR)
        {
            if (!ftn66flag
                && (last_varname != cur_varname || last_lineno != lineno)) {
                /* prevent multiple warnings */
                last_lineno = lineno;
                warn1(
                    "non-character datum %.42s initialized with character string",
                    last_varname = cur_varname);
            }
            varp->vleng = ICON(tsize);
            varp->vtype = type = TYCHAR;
        }
        else if ((type == TYCHAR && valtype != TYCHAR) ||
            (cktype(OPASSIGN, type, valtype) == TYERROR))
        {
            err("incompatible types in initialization");
            return;
        }
        if (type == TYADDR)
            con.Const.ci = valpc->Const.ci;
        else if (type != TYCHAR)
        {
            if (valtype == TYUNKNOWN)
                con.Const.ci = valpc->Const.ci;
            else	consconv(type, &con, valpc);
        }

        k = 1;


        switch (type)
        {
        case TYLOGICAL:
            if (tylogical != TYLONG)
                type = tylogical;
        case TYINT1:
        case TYLOGICAL1:
        case TYLOGICAL2:
        case TYSHORT:
        case TYLONG:
#ifdef TYQUAD
        case TYQUAD:
#endif
            dataline(varname, offset, type);
            prconi(dfile, con.Const.ci);
            lambda_item.data = to_string(con.Const.ci);
            break;

        case TYADDR:
            dataline(varname, offset, type);
            prcona(dfile, con.Const.ci);
            lambda_item.data = to_string(con.Const.ci);
            break;

        case TYCOMPLEX:
        case TYDCOMPLEX:
            k = 2;
        case TYREAL:
        case TYDREAL:
            dataline(varname, offset, type);
            prconr(dfile, &con, k);

            if (k > 1) {
                lambda_item.data = "{";
            }
            if (con.vstg) {
                lambda_item.data += con.Const.cds[0];
                if (k > 1) {
                    lambda_item.data += ", ";
                    lambda_item.data += con.Const.cds[1];
                }
            }
            else {
                lambda_item.data += to_string(con.Const.cd[0]);
                if (k > 1) {
                    lambda_item.data += ", ";
                    lambda_item.data += to_string(con.Const.cd[1]);
                }
            }
            if (k > 1) {
                lambda_item.data = "}";
            }

            break;

        case TYCHAR:
            {
                if (!cxx) {
                    k = valpc->vleng->constblock.Const.ci;
                    if (elen < k)
                        k = elen;
                    s = (unsigned char *)valpc->Const.ccp;
                    for (i = 0; i < k; ++i) {
                        dataline(varname, offset++, TYCHAR);
                        fprintf(dfile, "\t%d\n", *s++);
                    }
                    k = elen - valpc->vleng->constblock.Const.ci;
                    if (k > 0) {
                        dataline(varname, offset, TYBLANK);
                        fprintf(dfile, "\t%d\n", k);
                    }
                }
                else {
                    k = valpc->vleng->constblock.Const.ci;
                    if (elen < k) {
                        k = elen;
                    }

                    dataline(varname, offset, TY_DATA_STRING);
                    fprintf(dfile, "\t%d\t%d", elen, k);
                    for (int i = 0; i < k; ++i) {
                        fprintf(dfile, "\t%02x", ((unsigned char *)valpc->Const.ccp)[i]);
                    }
                    fprintf(dfile, "\n");

                    lambda_item.data = "\"";
                    lambda_item.data += valpc->Const.ccp;
                    lambda_item.data += "\"";
                }
            }
            break;

        default:
            badtype("setdata", type);
        }
    }
}



/*
   output form of name is padded with blanks and preceded
   with a storage class digit
*/
char *dataname(int stg,long memno)
 //int stg;
 //long memno;
{
	static char varname[64];
	register char *s, *t;
	char buf[16]/*, *memname()*/;

	if (stg == STGCOMMON) {
		varname[0] = '2';
		sprintf(s = buf, "Q.%ld", memno);
		}
	else {
		varname[0] = stg==STGEQUIV ? '1' : '0';
		s = memname(stg, memno);
		}
	t = varname + 1;
	while(*t++ = *s++);
	*t = 0;
	return(varname);
}





void frdata(chainp p0)
//chainp p0;
{
	register struct Chain *p;
	register tagptr q;

	for(p = p0 ; p ; p = p->nextp)
	{
		q = (tagptr)p->datap;
		if(q->tag == TIMPLDO)
		{
			if(q->impldoblock.isbusy)
				return;	/* circular chain completed */
			q->impldoblock.isbusy = YES;
			frdata(q->impldoblock.datalist);
			free( (charptr) q);
		}
		else
			frexpr(q);
	}

	frchain( &p0);
}



void dataline(char *varname, ftnint offset, int type)
//char *varname;
//ftnint offset;
//int type;
{
	fprintf(dfile, datafmt, varname, offset, type);
}


void make_param(Paramblock *p, int attribute, expptr expr)
 //register struct Paramblock *p;
 //expptr e;
{
	register expptr q;

    if(/*in_type_definition &&*/ !(attribute & ATTRIBUTE_PARAMETER))
    {
        // it's a variable initialisation
    }
    else
    {
        // put it to the end of the hash_index
        // to avoid constant being initialized in the wrong order
        p->hash_index = hash_counter++;
        p->vclass = CLPARAM;
        impldcl((Namep)p);
    }

    //for(auto e_ = ch; e_; e_ = e_->nextp)
    {
        auto e = expr;

        if(cxx){

            if (e->tag == TLIST) {
                // change to TSPECIALLIST
                e->tag = TSPECIALLIST;
                e->listblock.vtype = VTSPECIALLIST_CONST_PARAM_INIT;
            }

            q = e;
        }
        else
        {
            q = mkconv(p->vtype, e);
        }

        p->paramval = mkchain((char*)q, p->paramval);

        if(p->vtype == TYCHAR) {
            if(q->tag == TEXPR)
            {
                q = fixexpr(&q->exprblock);
                p->paramval->datap = (char*)q;
            }
            if(!ISCONST(q) || q->constblock.vtype != TYCHAR) {
                if (!cxx) {
                    errstr("invalid value for character parameter %s",
                        p->fvarname);
                    return;
                }
            }
            if(!(e = p->vleng))
                p->vleng = ICON(q->constblock.vleng->constblock.Const.ci
                    + q->constblock.Const.ccp1.blanks);
            else if(q->constblock.vleng->constblock.Const.ci
        > e->constblock.Const.ci) {
                q->constblock.vleng->constblock.Const.ci
                    = e->constblock.Const.ci;
                q->constblock.Const.ccp1.blanks = 0;
            }
            else
                q->constblock.Const.ccp1.blanks
                = e->constblock.Const.ci
                - q->constblock.vleng->constblock.Const.ci;
        }
    }
	
    //frchain(&ch);
 }
