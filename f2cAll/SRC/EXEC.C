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

#include <string>
#include <algorithm>
#include <assert.h>
#include "defs.h"
#include "func_def.h"
#include "p1defs.h"
#include "names.h"

LOCAL void popctl();
LOCAL void pushctl(int code);
LOCAL void exar2(int op, expptr e, struct Labelblock *l1, struct Labelblock *l2);
void mkstfunct(struct Primblock *lp, expptr rp);
extern std::vector<derived_type> new_types;
extern bool redirect_to_proto;


/*   Logical IF codes
*/


void exif(expptr p)
//expptr p;
{
    pushctl(CTLIF);
    putif(p, 0);	/* 0 => if, not elseif */
}



void exelif(expptr p)
//expptr p;
{
    if (ctlstack->ctltype == CTLIF || ctlstack->ctltype == CTLIFX)
    putif(p, 1);	/* 1 ==> elseif */
    else
    execerr("elseif out of place", CNULL);
}





void exelse()
{
    register struct Ctlframe *c;

    for(c = ctlstack; c->ctltype == CTLIFX; --c);
    if(c->ctltype == CTLIF) {
        p1_else ();
        c->ctltype = CTLELSE;
        }
    else
        execerr("else out of place", CNULL);
    }


void exendif()
{
    while(ctlstack->ctltype == CTLIFX) {
        popctl();
        p1else_end();
        }
    if(ctlstack->ctltype == CTLIF) {
        popctl();
        p1_endif ();
        }
    else if(ctlstack->ctltype == CTLELSE) {
        popctl();
        p1else_end ();
        }
    else
        execerr("endif out of place", CNULL);
    }


void new_endif()
{
    if (ctlstack->ctltype == CTLIF || ctlstack->ctltype == CTLIFX)
        pushctl(CTLIFX);
    else
        err("new_endif bug");
    }

/* pushctl -- Start a new control construct, initialize the labels (to
   zero) */

 LOCAL void
pushctl(int code)
 //int code;
{
    register int i;

    if(++ctlstack >= lastctl)
        many("loops or if-then-elses", 'c', maxctl);
    ctlstack->ctltype = code;
    for(i = 0 ; i < 4 ; ++i)
        ctlstack->ctlabels[i] = 0;
    ctlstack->dowhile = 0;
    ctlstack->select = false;
    ctlstack->first_case = false;
    ctlstack->where_count = 0;
    ctlstack->loop_ctrls.clear();
    ctlstack->needs_breakout_for = false;
    ctlstack->case_ = false;
    ++blklevel;
}


 LOCAL void
popctl()
{
    if( ctlstack-- < ctls )
        Fatal("control stack empty");
    --blklevel;
}



/* poplab -- update the flags in   labeltab   */

LOCAL void poplab()
{
    register struct Labelblock  *lp;

    for(lp = labeltab ; lp < highlabtab ; ++lp)
        if(lp->labdefined)
        {
            /* mark all labels in inner blocks unreachable */
            if(lp->blklevel > (unsigned)blklevel)
                lp->labinacc = YES;
        }
        else if(lp->blklevel > (unsigned)blklevel)
        {
            /* move all labels referred to in inner blocks out a level */
            lp->blklevel = blklevel;
        }
}


/*  BRANCHING CODE
*/

void exgoto(struct Labelblock *lab)
//struct Labelblock *lab;
{
    lab->labused = 1;
    p1_goto (lab -> stateno);
}







void exequals(struct Primblock *lp, expptr rp)
//register struct Primblock *lp;
//register expptr rp;
{
    if(cxx)
    {
        // we don't do statement functions
        //assert(parstate >= INEXEC);

        expptr new_lp, new_rp;

        if(parstate < INDATA)
            enddcl();
        new_lp = mklhs(lp, keepsubs);
        new_rp = fixtype(rp);
        puteq(new_lp, new_rp);
        return;
    }
    if(lp->tag != TPRIM)
    {
        err("assignment to a non-variable");
        frexpr((expptr)lp);
        frexpr(rp);
    }
    else if(lp->namep->vclass!=CLVAR && lp->argsp)
    {
        if(cxx)
        {
            // we don't do statement functions
            assert(parstate >= INEXEC);

            expptr new_lp, new_rp;

            if(parstate < INDATA)
                enddcl();
            new_lp = mklhs(lp, keepsubs);
            new_rp = fixtype(rp);
            puteq(new_lp, new_rp);
        }
        else
        {
            if(parstate >= INEXEC)
                err("statement function amid executables");
            mkstfunct(lp, rp);
        }
    }
    else
    {
        expptr new_lp, new_rp;

        if(parstate < INDATA)
            enddcl();
        new_lp = mklhs (lp, keepsubs);
        new_rp = fixtype (rp);
        puteq(new_lp, new_rp);
    }
}



/* Make Statement Function */

long laststfcn = -1, thisstno;
int doing_stmtfcn;

void mkstfunct(struct Primblock *lp, expptr rp)
//struct Primblock *lp;
//expptr rp;
{
    register struct Primblock *p;
    register Namep np;
    chainp args;

    laststfcn = thisstno;
    np = lp->namep;
    if(np->vclass == CLUNKNOWN)
        np->vclass = CLPROC;
    else
    {
        dclerr("redeclaration of statement function", np);
        return;
    }
    np->vprocclass = PSTFUNCT;
    np->vstg = STGSTFUNCT;

/* Set the type of the function */

    impldcl(np);
    if (np->vtype == TYCHAR && !np->vleng)
        err("character statement function with length (*)");
    args = (lp->argsp ? lp->argsp->listp : CHNULL);
    np->varxptr.vstfdesc = mkchain((char *)args, (chainp)rp);

    for(doing_stmtfcn = 1 ; args ; args = args->nextp)

/* It is an error for the formal parameters to have arguments or
   subscripts */

        if( ((tagptr)(args->datap))->tag!=TPRIM ||
            (p = (struct Primblock *)(args->datap) )->argsp ||
            p->fcharp || p->lcharp )
            err("non-variable argument in statement function definition");
        else
        {

/* Replace the name on the left-hand side */

            args->datap = (char *)p->namep;
            vardcl(p -> namep);
            free((char *)p);
        }
    doing_stmtfcn = 0;
}

 static void
mixed_type(Namep np)
// Namep np;
{
    char buf[128];
    sprintf(buf, "%s function %.90s invoked as subroutine",
        ftn_types[np->vtype], np->fvarname);
    warn(buf);
    }


void excall(Namep name, struct Listblock *args, int nstars, struct Labelblock *labels[])
//Namep name;
//struct Listblock *args;
//int nstars;
//struct Labelblock *labels[ ];
{
    register expptr p;

    if (name->vtype != TYSUBR) {
        if (name->vinfproc && !name->vcalled) {
            name->vtype = TYSUBR;
            frexpr(name->vleng);
            name->vleng = 0;
            }
        else if (!name->vimpltype && name->vtype != TYUNKNOWN)
            mixed_type(name);
        else
            settype(name, TYSUBR, (ftnint)0, 0);
        }
    p = mkfunct( mkprim(name, args, CHNULL) );

/* Subroutines and their identifiers acquire the type INT */

    p->exprblock.vtype = p->exprblock.leftp->headblock.vtype = TYINT;

/* Handle the alternate return mechanism */

    if(nstars > 0)
        putcmgo(putx(fixtype(p)), nstars, labels);
    else
        putexpr(p);
}



void exstop(int stop, expptr p)
//int stop;
//register expptr p;
{
    char *str;
    int n;
    //expptr mkstrcon();

    if(p)
    {
        if( ! ISCONST(p) )
        {
            execerr("pause/stop argument must be constant", CNULL);
            frexpr(p);
            p = mkstrcon(0, CNULL);
        }
        else if( ISINT(p->constblock.vtype) )
        {
            str = convic(p->constblock.Const.ci);
            n = strlen(str);
            if(n > 0)
            {
                p->constblock.Const.ccp = copyn(n, str);
                p->constblock.Const.ccp1.blanks = 0;
                p->constblock.vtype = TYCHAR;
                p->constblock.vleng = (expptr) ICON(n);
            }
            else
                p = (expptr) mkstrcon(0, CNULL);
        }
        else if(p->constblock.vtype != TYCHAR)
        {
            execerr("pause/stop argument must be integer or string", CNULL);
            p = (expptr) mkstrcon(0, CNULL);
        }
    }
    else	p = (expptr) mkstrcon(0, CNULL);

    {
    expptr subr_call;

    subr_call = call1(TYSUBR, (stop ? "s_stop" : "s_paus"), p);
    putexpr( subr_call );
    }
}

/* DO LOOP CODE */

#define DOINIT	par[0]
#define DOLIMIT	par[1]
#define DOINCR	par[2]


/* Macros for   ctlstack -> dostepsign   */

#define VARSTEP	0
#define POSSTEP	1
#define NEGSTEP	2


/* exdo -- generate DO loop code.  In the case of a variable increment,
   positive increment tests are placed above the body, negative increment
   tests are placed below (see   enddo()   ) */

void exdo(int range, Namep loopname, chainp spec)
//int range;			/* end label */
//Namep loopname;
//chainp spec;			/* input spec must have at least 2 exprs */
{
    register expptr p;
    register Namep np;
    chainp cp;		/* loops over the fields in   spec */
    register int i;
    int dotype;		/* type of the index variable */
    int incsign = 0;		/* sign of the increment, if it's constant
                   */
    Addrp dovarp;		/* loop index variable */
    expptr doinit;		/* constant or register for init param */
    expptr par[3];		/* local specification parameters */

    expptr init, test, inc, limit;	/* Expressions in the resulting FOR loop */


    test = ENULL;
    limit = ENULL;

    pushctl(CTLDO);
    dorange = ctlstack->dolabel = range;
    ctlstack->loopname = loopname;

    ctlstack->loop_ctrls.resize(1);

    /* Declare the loop index */

    np = (Namep)spec->datap;
    ctlstack->loop_ctrls.back().donamep = NULL;
    if (!np) { /* do while */
        ctlstack->dowhile = 1;
#if 0
        if (loopname) {
            if (loopname->vtype == TYUNKNOWN) {
                loopname->vdcldone = 1;
                loopname->vclass = CLLABEL;
                loopname->vprocclass = PLABEL;
                loopname->vtype = TYLABEL;
                }
            if (loopname->vtype == TYLABEL)
                if (loopname->vdovar)
                    dclerr("already in use as a loop name",
                        loopname);
                else
                    loopname->vdovar = 1;
            else
                dclerr("already declared; cannot be a loop name",
                    loopname);
            }
#endif
        putwhile((expptr)spec->nextp);
        NOEXT("do while");
        spec->nextp = 0;
        frchain(&spec);
        return;
        }
    if(np->vdovar)
    {
        errstr("nested loops with variable %s", np->fvarname);
        ctlstack->loop_ctrls.back().donamep = NULL;
        return;
    }

/* Create a memory-resident version of the index variable */

    dovarp = mkplace(np);
    if( ! ONEOF(dovarp->vtype, MSKINT|MSKREAL) )
    {
        err("bad type on do variable");
        return;
    }
    ctlstack->loop_ctrls.back().donamep = np;

    np->vdovar = YES;

/* Now   dovarp   points to the index to be used within the loop,   dostgp
   points to the one which may need to be stored */

    dotype = dovarp->vtype;

/* Count the input specifications and type-check each one independently;
   this just eliminates non-numeric values from the specification */

    for(i=0 , cp = spec->nextp ; cp!=NULL && i<3 ; cp = cp->nextp)
    {
        p = par[i++] = fixtype((tagptr)cp->datap);
        if( ! ONEOF(p->headblock.vtype, MSKINT|MSKREAL) )
        {
            if(!cxx)
            {
                err("bad type on DO parameter");
                return;
            }
        }
    }

    frchain(&spec);
    switch(i)
    {
    case 0:
    case 1:
        err("too few DO parameters");
        return;

    default:
        err("too many DO parameters");
        return;

    case 2:
        DOINCR = (expptr) ICON(1);

    case 3:
        break;
    }


/* Now all of the local specification fields are set, but their types are
   not yet consistent */

/* Declare the loop initialization value, casting it properly and declaring a
   register if need be */

    if (ISCONST (DOINIT) || !onetripflag)
/* putx added 6-29-89 (mwm), not sure if fixtype is required, but I doubt it
   since mkconv is called just before */
        doinit = putx (mkconv (dotype, DOINIT));
    else {
        doinit = (expptr) mktmp(dotype, ENULL);
        puteq (cpexpr (doinit), DOINIT);
    } /* else */

/* Declare the loop ending value, casting it to the type of the index
   variable */

    if( ISCONST(DOLIMIT) )
        ctlstack->loop_ctrls.back().domax = mkconv(dotype, DOLIMIT);
    else {
        if(dolimitconstflag) { 
            ctlstack->loop_ctrls.back().domax = mkconv(dotype, DOLIMIT);
            limit = cpexpr(ctlstack->loop_ctrls.back().domax);
        }
        else {
            ctlstack->loop_ctrls.back().domax = (expptr) mktmp0(dotype, ENULL);
            puteq (cpexpr (ctlstack ->loop_ctrls.back().domax), DOLIMIT);
        }
    } /* else */

/* Declare the loop increment value, casting it to the type of the index
   variable */

    if( ISCONST(DOINCR) || is_const(DOINCR))
    {
        ctlstack->loop_ctrls.back().dostep = mkconv(dotype, DOINCR);
        auto val = conseval(ctlstack->loop_ctrls.back().dostep);
        switch(val.state)
        {
        case conseval_t::state_::int_:
            {
                if((incsign = val.n) == 0)
                    err("zero DO increment");
                ctlstack->loop_ctrls.back().dostepsign = (val.n > 0 ? POSSTEP : NEGSTEP);
            }
            break;

        case conseval_t::state_::double_:
            {
                if((incsign = (int)val.d) == 0)
                    err("zero DO increment");
                ctlstack->loop_ctrls.back().dostepsign = (val.d > 0 ? POSSTEP : NEGSTEP);
            }
            break;

        default:
            {
                ctlstack->loop_ctrls.back().dostepsign = VARSTEP;
            }
            break;
        }

        if( (incsign = conssgn(ctlstack->loop_ctrls.back().dostep)) == 0)
            err("zero DO increment");
        //ctlstack->dostepsign = (incsign > 0 ? POSSTEP : NEGSTEP);
    }
    else
    {
        ctlstack->loop_ctrls.back().dostep = (expptr) mktmp0(dotype, ENULL);
        ctlstack->loop_ctrls.back().dostepsign = VARSTEP;
        puteq (cpexpr (ctlstack ->loop_ctrls.back().dostep), DOINCR);
    }

/* All data is now properly typed and in the   ctlstack,   except for the
   initial value.  Assignments of temps have been generated already */

    switch (ctlstack ->loop_ctrls.back().dostepsign) {
        case VARSTEP:
        test = mkexpr (OPQUEST, mkexpr (OPLT,
            cpexpr (ctlstack ->loop_ctrls.back().dostep), ICON(0)),
            mkexpr (OPCOLON,
                mkexpr (OPGE, cpexpr((expptr)dovarp),
                    cpexpr (ctlstack ->loop_ctrls.back().domax)),
                mkexpr (OPLE, cpexpr((expptr)dovarp),
                    cpexpr (ctlstack ->loop_ctrls.back().domax))));
        break;
        case POSSTEP:
            test = mkexpr (OPLE, cpexpr((expptr)dovarp),
            cpexpr (ctlstack ->loop_ctrls.back().domax));
            break;
        case NEGSTEP:
            test = mkexpr (OPGE, cpexpr((expptr)dovarp),
            cpexpr (ctlstack ->loop_ctrls.back().domax));
            break;
        default:
            erri ("exdo:  bad dostepsign '%d'", ctlstack ->loop_ctrls.back().dostepsign);
            break;
    } /* switch (ctlstack -> dostepsign) */

    if (onetripflag)
        test = mkexpr (OPOR, test,
            mkexpr (OPEQ, cpexpr((expptr)dovarp), cpexpr (doinit)));
    init = mkexpr (OPASSIGN, cpexpr((expptr)dovarp), doinit);
    inc = mkexpr (OPPLUSEQ, (expptr)dovarp, cpexpr (ctlstack ->loop_ctrls.back().dostep));

    if(!cxx)
    {
        if (!onetripflag && ISCONST (ctlstack ->loop_ctrls.back().domax) && ISCONST (doinit)
            && ctlstack ->loop_ctrls.back().dostepsign != VARSTEP) {
            expptr tester;

            tester = mkexpr (OPMINUS, cpexpr (doinit),
                cpexpr (ctlstack ->loop_ctrls.back().domax));
            if (incsign == conssgn (tester))
                warn ("DO range never executed");
            frexpr (tester);
        } /* if !onetripflag && */
    }

    p1_for (init, test, inc, limit);
}



void exdoconc(int range, chainp full_spec)
//int range;			/* end label */
//Namep loopname;
//chainp spec;			/* input spec must have at least 2 exprs */
{
    register expptr p;
    register Namep np;
    chainp cp;		/* loops over the fields in   spec */
    register int i;
    int dotype;		/* type of the index variable */
    int incsign = 0;		/* sign of the increment, if it's constant
                   */
    Addrp dovarp;		/* loop index variable */
    expptr doinit;		/* constant or register for init param */
    expptr par[3];		/* local specification parameters */

    expptr init, test, inc, limit;	/* Expressions in the resulting FOR loop */



    chainp loop_start = (chainp)full_spec->datap;
    chainp loop = loop_start;


    pushctl(CTLDOCONC);
    dorange = ctlstack->dolabel = range;
    //ctlstack->loopname = loopname;

    for(; loop; loop = loop->nextp)
    {
        test = ENULL;
        limit = ENULL;

        ctlstack->loop_ctrls.emplace_back();
        /* Declare the loop index */
        auto loop_spec = (chainp)loop->datap;

        np = (Namep)loop_spec->datap;
        ctlstack->loop_ctrls.back().donamep = NULL;

        /* Create a memory-resident version of the index variable */

        dovarp = mkplace(np);
        if( ! ONEOF(dovarp->vtype, MSKINT|MSKREAL) )
        {
            err("bad type on do variable");
            return;
        }
        ctlstack->loop_ctrls.back().donamep = np;

        np->vdovar = YES;

        /* Now   dovarp   points to the index to be used within the loop,   dostgp
        points to the one which may need to be stored */

        dotype = dovarp->vtype;

        /* Count the input specifications and type-check each one independently;
        this just eliminates non-numeric values from the specification */

        for(i=0 , cp = loop_spec->nextp ; i<3 ; ++i, cp = cp->nextp)
        {
            if(i == 2 && !cp->datap)
            {
                DOINCR = (expptr)ICON(1);
            }
            else
            {
                p = par[i] = fixtype((tagptr)cp->datap);
            }
            //if( ! ONEOF(p->headblock.vtype, MSKINT|MSKREAL) )
            //{
            //	err("bad type on DO parameter");
            //	return;
            //}
        }



        /* Now all of the local specification fields are set, but their types are
        not yet consistent */

        /* Declare the loop initialization value, casting it properly and declaring a
        register if need be */

        if (ISCONST (DOINIT) || !onetripflag)
            /* putx added 6-29-89 (mwm), not sure if fixtype is required, but I doubt it
            since mkconv is called just before */
            doinit = putx (mkconv (dotype, DOINIT));
        else {
            doinit = (expptr) mktmp(dotype, ENULL);
            puteq (cpexpr (doinit), DOINIT);
        } /* else */

          /* Declare the loop ending value, casting it to the type of the index
          variable */

        if( ISCONST(DOLIMIT) )
            ctlstack->loop_ctrls.back().domax = mkconv(dotype, DOLIMIT);
        else {
            if(dolimitconstflag) { 
                ctlstack->loop_ctrls.back().domax = mkconv(dotype, DOLIMIT);
                limit = cpexpr(ctlstack->loop_ctrls.back().domax);
            }
            else {
                ctlstack->loop_ctrls.back().domax = (expptr) mktmp0(dotype, ENULL);
                puteq (cpexpr (ctlstack ->loop_ctrls.back().domax), DOLIMIT);
            }
        } /* else */

          /* Declare the loop increment value, casting it to the type of the index
          variable */

        if( ISCONST(DOINCR) || is_const(DOINCR))
        {
            ctlstack->loop_ctrls.back().dostep = mkconv(dotype, DOINCR);
            auto val = conseval(ctlstack->loop_ctrls.back().dostep);
            switch(val.state)
            {
            case conseval_t::state_::int_:
                {
                    if((incsign = val.n) == 0)
                        err("zero DO increment");
                    ctlstack->loop_ctrls.back().dostepsign = (val.n > 0 ? POSSTEP : NEGSTEP);
                }
                break;

            case conseval_t::state_::double_:
                {
                    if((incsign = (int)val.d) == 0)
                        err("zero DO increment");
                    ctlstack->loop_ctrls.back().dostepsign = (val.d > 0 ? POSSTEP : NEGSTEP);
                }
                break;

            default:
                {
                    ctlstack->loop_ctrls.back().dostepsign = VARSTEP;
                }
                break;
            }

            if( (incsign = conssgn(ctlstack->loop_ctrls.back().dostep)) == 0)
                err("zero DO increment");
            //ctlstack->dostepsign = (incsign > 0 ? POSSTEP : NEGSTEP);
        }
        else
        {
            ctlstack->loop_ctrls.back().dostep = (expptr) mktmp0(dotype, ENULL);
            ctlstack->loop_ctrls.back().dostepsign = VARSTEP;
            puteq (cpexpr (ctlstack ->loop_ctrls.back().dostep), DOINCR);
        }

        /* All data is now properly typed and in the   ctlstack,   except for the
        initial value.  Assignments of temps have been generated already */

        switch (ctlstack ->loop_ctrls.back().dostepsign) {
        case VARSTEP:
            test = mkexpr (OPQUEST, mkexpr (OPLT,
                cpexpr (ctlstack ->loop_ctrls.back().dostep), ICON(0)),
                mkexpr (OPCOLON,
                mkexpr (OPGE, cpexpr((expptr)dovarp),
                cpexpr (ctlstack ->loop_ctrls.back().domax)),
                mkexpr (OPLE, cpexpr((expptr)dovarp),
                cpexpr (ctlstack ->loop_ctrls.back().domax))));
            break;
        case POSSTEP:
            test = mkexpr (OPLE, cpexpr((expptr)dovarp),
                cpexpr (ctlstack ->loop_ctrls.back().domax));
            break;
        case NEGSTEP:
            test = mkexpr (OPGE, cpexpr((expptr)dovarp),
                cpexpr (ctlstack ->loop_ctrls.back().domax));
            break;
        default:
            erri ("exdo:  bad dostepsign '%d'", ctlstack ->loop_ctrls.back().dostepsign);
            break;
        } /* switch (ctlstack -> dostepsign) */

        if (onetripflag)
            test = mkexpr (OPOR, test,
                mkexpr (OPEQ, cpexpr((expptr)dovarp), cpexpr (doinit)));
        init = mkexpr (OPASSIGN, cpexpr((expptr)dovarp), doinit);
        inc = mkexpr (OPPLUSEQ, (expptr)dovarp, cpexpr (ctlstack ->loop_ctrls.back().dostep));

        if(!cxx)
        {
            if (!onetripflag && ISCONST (ctlstack ->loop_ctrls.back().domax) && ISCONST (doinit)
                && ctlstack ->loop_ctrls.back().dostepsign != VARSTEP) {
                expptr tester;

                tester = mkexpr (OPMINUS, cpexpr (doinit),
                    cpexpr (ctlstack ->loop_ctrls.back().domax));
                if (incsign == conssgn (tester))
                    warn ("DO range never executed");
                frexpr (tester);
            } /* if !onetripflag && */
        }

        char buf[COMMENT_BUFFER_SIZE + 1] = "!!!DO CONCURRENT!!!";

        p1_comment(buf);
        p1_for (init, test, inc, limit);


        frchain(&loop_spec);
    }

    if(full_spec->nextp->datap)
    {
        chainp where_start = (chainp)full_spec->nextp->datap;
        chainp where_ = where_start;
        for(; where_; where_ = where_->nextp)
        {
            ++ctlstack->where_count;
            p1_if(putx(fixtype((expptr)where_->datap)));
        }
        frchain(&where_start);
    }

    frchain(&loop_start);
    frchain(&full_spec);

}

void exenddo(Namep np)
 //Namep np;
{
    Namep np1;
    int here;
    struct Ctlframe *cf;

    if(ctlstack < ctls)
        Fatal("control stack empty");
    here = ctlstack->dolabel;
    if(ctlstack->ctltype != CTLDOCONC)
    {
        if(ctlstack->ctltype != CTLDO
            || here >= 0 && (!thislabel || thislabel->labelno != here)) {
            err("misplaced ENDDO");
            return;
        }
    }
    else
    {
        for(unsigned int i = 0; i < ctlstack->where_count; ++i)
        {
            p1_endif();
        }
    }

    if(np != ctlstack->loopname) {
        if(np1 = ctlstack->loopname)
            errstr("expected \"enddo %s\"", np1->fvarname);
        else
            err("expected unnamed ENDDO");
        for(cf = ctls; cf < ctlstack; cf++)
            if(cf->ctltype == CTLDO && cf->loopname == np) {
                here = cf->dolabel;
                break;
            }
    }
    enddo(here, ctlstack->needs_breakout_for, ctlstack->loop_ctrls.size());
}


void enddo(int here, bool needs_breakout_for, int loops_count)
//int here;
{
    register struct Ctlframe *q;
    Namep np;			/* name of the current DO index */
    Addrp ap;
    register int i;
    register expptr e;

/* Many DO's can end at the same statement, so keep looping over all
   nested indicies */

    while(here == dorange)
    {
        if(ctlstack->loop_ctrls.empty())
        {
            err("ctlstack->loop_ctrls.empty()");
        }

        if(np = ctlstack->loop_ctrls.back().donamep)
        {
            p1for_end(needs_breakout_for, loops_count);

/* Now we're done with all of the tests, and the loop has terminated.
   Store the index value back in long-term memory */

            if(ap = memversion(np))
                puteq((expptr)ap, (expptr)mkplace(np));
            for(i = 0; i < 4; ++i)
                ctlstack->ctlabels[i] = 0;

            for(auto&& lc : ctlstack->loop_ctrls)
            {
                deregister(lc.donamep);
                lc.donamep->vdovar = NO;
                e = lc.dostep;
                if(e->tag == TADDR && e->addrblock.istemp)
                    frtemp((Addrp)e);
                else
                    frexpr(e);
                e = lc.domax;
                if(e->tag == TADDR && e->addrblock.istemp)
                    frtemp((Addrp)e);
                else
                    frexpr(e);
            }
        }
        else if(ctlstack->dowhile)
            p1for_end(needs_breakout_for, loops_count);

/* Set   dorange   to the closing label of the next most enclosing DO loop
   */

        popctl();
        poplab();
        dorange = 0;
        for(q = ctlstack; q >= ctls; --q)
            if(q->ctltype == CTLDO || q->ctltype == CTLDOCONC)
            {
                dorange = q->dolabel;
                break;
            }
    }
}


void exwrite(chainp p0, chainp p1)
{
//    TSPECIALLIST
    p1put(P1_WRITE_CXX);
  
    Listblock * l0 = mklist(p0);
    l0->tag = TSPECIALLIST;
    l0->vtype = VTSPECIALLIST_ARGS_LIST;
    putexpr((expptr)l0);

    Listblock * l1 = mklist(p1);
    l1->tag = TSPECIALLIST;
    l1->vtype = VTSPECIALLIST_ARGS_LIST;
    putexpr((expptr)l1);
}

void exallocate(chainp p, bool deallocate)
{
//    TSPECIALLIST
    p1put(deallocate ? P1_DEALLOCATE : P1_ALLOCATE);
  
    Listblock * l0 = mklist(p);
    l0->tag = TSPECIALLIST;
    l0->vtype = VTSPECIALLIST_ARGS_LIST;
    putexpr((expptr)l0);
}



void exassign(Namep vname, struct Labelblock *labelval)
// register Namep vname;
//struct Labelblock *labelval;
{
    Addrp p;
    expptr mkaddcon();
    register Addrp q;
    char *fs;
    register chainp cp, cpprev;
    register ftnint k, stno;

    p = mkplace(vname);
    if( ! ONEOF(p->vtype, MSKINT|MSKADDR) ) {
        err("noninteger assign variable");
        return;
        }

    /* If the label hasn't been defined, then we do things twice:
     * once for an executable stmt label, once for a format
     */

    /* code for executable label... */

/* Now store the assigned value in a list associated with this variable.
   This will be used later to generate a switch() statement in the C output */

    fs = labelval->fmtstring;
    if (!labelval->labdefined || !fs) {

        if (vname -> vis_assigned == 0) {
            vname -> varxptr.assigned_values = CHNULL;
            vname -> vis_assigned = 1;
            }

        /* don't duplicate labels... */

        stno = labelval->stateno;
        cpprev = 0;
        for(k = 0, cp = vname->varxptr.assigned_values;
                cp; cpprev = cp, cp = cp->nextp, k++)
            if ((ftnint)cp->datap == stno)
                break;
        if (!cp) {
            cp = mkchain((char *)stno, CHNULL);
            if (cpprev)
                cpprev->nextp = cp;
            else
                vname->varxptr.assigned_values = cp;
            labelval->labused = 1;
            }
        putout(mkexpr(OPASSIGN, (expptr)p, mkintcon(k)));
        }

    /* Code for FORMAT label... */

    if (!labelval->labdefined || fs) {
        extern void fmtname();

        labelval->fmtlabused = 1;
        p = ALLOC(Addrblock);
        p->tag = TADDR;
        p->vtype = TYCHAR;
        p->vstg = STGAUTO;
        p->memoffset = ICON(0);
        fmtname(vname, p);
        q = ALLOC(Addrblock);
        q->tag = TADDR;
        q->vtype = TYCHAR;
        q->vstg = STGAUTO;
        q->ntempelt = 1;
        q->memoffset = ICON(0);
        q->uname_tag = UNAM_IDENT;
        sprintf(q->user.ident, "fmt_%ld", labelval->stateno);
        putout(mkexpr(OPASSIGN, (expptr)p, (expptr)q));
        }

} /* exassign */



void exarif(expptr expr, struct Labelblock *neglab, struct Labelblock *zerlab, struct Labelblock *poslab)
//expptr expr;
//struct Labelblock *neglab, *zerlab, *poslab;
{
    register int lm, lz, lp;

    lm = neglab->stateno;
    lz = zerlab->stateno;
    lp = poslab->stateno;
    expr = fixtype(expr);

    if( ! ONEOF(expr->headblock.vtype, MSKINT|MSKREAL) )
    {
        err("invalid type of arithmetic if expression");
        frexpr(expr);
    }
    else
    {
        if (lm == lz && lz == lp)
            exgoto (neglab);
        else if(lm == lz)
            exar2(OPLE, expr, neglab, poslab);
        else if(lm == lp)
            exar2(OPNE, expr, neglab, zerlab);
        else if(lz == lp)
            exar2(OPGE, expr, zerlab, neglab);
        else {
            expptr t;

        if (!addressable (expr)) {
        t = (expptr) mktmp(expr -> headblock.vtype, ENULL);
        expr = mkexpr (OPASSIGN, cpexpr (t), expr);
        } else
        t = (expptr) cpexpr (expr);

        p1_if(putx(fixtype(mkexpr (OPLT, expr, ICON (0)))));
        exgoto(neglab);
        p1_elif (mkexpr (OPEQ, t, ICON (0)));
        exgoto(zerlab);
        p1_else ();
        exgoto(poslab);
        p1else_end ();
        } /* else */
    }
}



/* exar2 -- Do arithmetic IF for only 2 distinct labels;   if !(e.op.0)
   goto l2 else goto l1.  If this seems backwards, that's because it is,
   in order to make the 1 pass algorithm work. */

 LOCAL void
exar2(int op, expptr e, struct Labelblock *l1, struct Labelblock *l2)
 //int op;
 //expptr e;
 //struct Labelblock *l1, *l2;
{
    expptr comp;

    comp = mkexpr (op, e, ICON (0));
    p1_if(putx(fixtype(comp)));
    exgoto(l1);
    p1_else ();
    exgoto(l2);
    p1else_end ();
}


/* exreturn -- return the value in   p  from a SUBROUTINE call -- used to
   implement the alternate return mechanism */

void exreturn(expptr p)
//register expptr p;
{
    if(procclass != CLPROC)
        warn("RETURN statement in main or block data");
    if(p && (std::get<2>(proctype)!=TYSUBR || procclass!=CLPROC) )
    {
        err("alternate return in nonsubroutine");
        p = 0;
    }

    if (p || std::get<2>(proctype) == TYSUBR) {
        if(std::get<2>(proctype) == TYSUBR && cxx)
        {}
        else
        {
            if (p == ENULL) p = ICON (0);
            p = mkconv (TYLONG, fixtype (p));
        }
        p1_subr_ret (p);
    } /* if p || proctype == TYSUBR */
    else
        p1_subr_ret((expptr)retslot);
}


void exasgoto(Namep labvar)
//Namep labvar;
{
    register Addrp p;
    void p1_asgoto();

    p = mkplace(labvar);
    if( ! ISINT(p->vtype) )
        err("assigned goto variable must be integer");
    else {
        p1_asgoto (p);
    } /* else */
}





void exselect(expptr p)
{
    // push select
    pushctl(CTLSELECT);
    ctlstack->select = true;
    ctlstack->first_case = true;

    expptr value = fixtype(p);
    p1_switch(value);
}

void exendselect()
{
    if(ctlstack->ctltype == CTLCASE){
        // close final case
        p1case_end();

        popctl();
    }

    if(ctlstack->ctltype != CTLSELECT){
        err("misplaced ENDSELECT");
        return;
    }
    p1select_end();
    popctl();

}

void excase(chainp p)
{
    switch(ctlstack->ctltype)
    {
    case CTLSELECT:
        {
            // should be first case


            // push case
            pushctl(CTLCASE);
        }
        break;

    case CTLCASE:
        {
            p1case_end();

        }
        break;

    default:
        {
            err("misplaced CASE");
            return;
        }
        break;
    }

    if(!p)
    {
        // default case
        p1_case(nullptr, nullptr, nullptr);
    }
    else
    {
        chainp act = p;
        while(act){
            expptr case_ = fixtype((expptr)act->datap);
            p1_case(case_, nullptr, nullptr);

            act = act->nextp;
        }
    }

    p1put(P1_CASE_OPEN);


    //expptr value = fixtype(p);
    //p1_switch(value);

    //if(ctlstack->ctltype != CTLSELECT){
    //    err("misplaced CASE");
    //    return;
    //}
    //ctlstack->first_case = true;
    //expptr value = cpexpr(p);

}

void excycle()
{
    //for(auto q = ctlstack; q >= ctls; --q)
    //{
    //    if(q->ctltype == CTLDO)
    //    {
    //        // ok
    //        break;
    //    }
    //    if(q->ctltype == CTLCASE)
    //    {
    //        --q;
    //        assert(false);
    //        err("implement goto end of loop");
    //        //// oops, break would not leave the loop
    //        //for(; q >= ctls; --q)
    //        //{
    //        //    if(q->ctltype == CTLDO)
    //        //    {
    //        //        // goto this 
    //        //        break;
    //        //    }
    //        //}
    //        break;
    //    }
    //}

    p1put(P1_CONTINUE);
}

void exexit()
{
    bool breakout_for = false;
    for(auto q = ctlstack; q >= ctls; --q)
    {
        if(q->ctltype == CTLDO)
        {
            // ok
            break;
        }
        if(q->ctltype == CTLCASE)
        {
            --q;
            // oops, break would not leave the loop
            for(; q >= ctls; --q)
            {
                if(q->ctltype == CTLDO)
                {
                    q->needs_breakout_for = true;
                    breakout_for = true;
                    break;
                }
            }
            break;
        }
    }

    p1put(breakout_for ? P1_GOTO_BREAKOUT_FOR : P1_BREAK);
}

void exmodule(const char* name){

    p1_namespace(name);
    putmodulehead();
    module_name = name;
}

void excontains(){
    redirect_to_proto = false;
//    p1put(P1_REDIRECT_TO_PROT_END);

    seen_contains = true;

    //for(auto&& t : new_types)
    //{
    //    p1putds(P1_STRUCT, t.members.size(), t.name.c_str());


    //    for(auto&& n : t.members)
    //    {
    //        p1_name(n);
    //    }
    //}

    new_types.clear();

    start_formatting(true);

    procinit();

}



void exendmodule(){
    if(!seen_contains)
    {
        enddcl(); endproc(); excontains();
    }

    p1namespace_end(module_name.c_str());
    module_name.empty();
    seen_contains = false;
    start_formatting(true);

}

void exuse(const char* name){
    if(in_interface_definition) {
        return;
    }


    static std::vector<std::string> global_ingnore = {
        "iso_c_binding",
        "interface_module",
        "error_type_module",
//        "fluid_type_module",
    };

    // check for ignores
    {
        std::string n = name;
        std::transform(n.begin(), n.end(), n.begin(), ::tolower);
        if(std::find(global_ingnore.begin(), global_ingnore.end(), n) != global_ingnore.end())
        {
            return;
        }
    }



    add_global_include(name, true);
    p1_using_namespace(name);
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}
void add_proto_include(const char* name, bool is_use)
{
    std::string n = name;
    std::transform(n.begin(), n.end(), n.begin(), ::tolower);
    if(is_use){
        n += ".h";
    }
    else {
        replaceAll(n, ".inc", ".h");
        replaceAll(n, ".for", ".h");
    }

    if(std::find(proto_includes.begin(), proto_includes.end(), n) == proto_includes.end())
    {
        proto_includes.push_back(n);
    }
}

void add_global_include(const char* name_, bool is_use)
{
    std::string name = name_;

    // add includes to ignore here
    static std::vector<std::string> global_ingnore = {
        "iso_c_binding.h"
//        ...
    };

    // check for ignores
    {
        std::string n = name;
        std::transform(n.begin(), n.end(), n.begin(), ::tolower);
        if(is_use){
            n += ".h";
        }
        else {
            replaceAll(n, ".inc", ".h");
            replaceAll(n, ".for", ".h");
        }
        if(std::find(global_ingnore.begin(), global_ingnore.end(), n) != global_ingnore.end())
        {
            return;
        }

    }

    // add cpp include to change name here
    static std::vector<std::pair<std::string, std::string>> global_replace = {
//        {"include_name.h", "new_include_name"},
  //           with ".h"        without ".h"      
    };

    // check for ignores
    {
        std::string n = name;
        std::transform(n.begin(), n.end(), n.begin(), ::tolower);
        if(is_use){
            n += ".h";
        }
        else {
            replaceAll(n, ".inc", ".h");
            replaceAll(n, ".for", ".h");
        }
        auto it = std::find_if(global_replace.begin(), global_replace.end(), 
            [&](auto&& v){ return v.first == n; }
        );
        
        if(it != global_replace.end())
        {
            name = it->second;
            if(!is_use){
                name += ".h";
            }
        }

    }



    if(procclass == CLMODULEHEAD)
    {
        add_proto_include(name.c_str(), is_use);
    }
    std::string n = name;
    std::transform(n.begin(), n.end(), n.begin(), ::tolower);
    if(is_use){
        n += ".h";
    }
    else {
        replaceAll(n, ".inc", ".h");
        replaceAll(n, ".for", ".h");
    }

    if(std::find(global_includes.begin(), global_includes.end(), n) == global_includes.end())
    {
        global_includes.push_back(n);
    }
}