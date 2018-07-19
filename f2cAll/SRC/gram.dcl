spec:	  dcl { /* reset type global dimension */ ndim_type = 0; } 
	| common
	| external
	| intrinsic
	| equivalence
	| data
    | use
	| implicit
	| namelist
    | SPRIVATE
    | SPRIVATE SNAME
    | SPUBLIC
    | SPUBLIC SNAME
	| SSAVE
		{ NO66("SAVE statement");
		  saveall = YES; }
	| SSAVE savelist
		{ NO66("SAVE statement"); }
	| SFORMAT
		{ fmtstmt(thislabel); setfmt(thislabel); }
	| SPARAM in_dcl SLPAR paramlist SRPAR
		{ NO66("PARAMETER statement"); }
	;

dcl:	  type name in_dcl new_dcl dims lengspec_or_dims
		{ 
          if(!in_interface_definition){
              settype($2, $1, $6, 0);
		      if(ndim>0) {
                setbound($2,ndim,dims, $6);
              } else if(ndim_type>0) {
                setbound($2,ndim_type,dims_type, $6);
              }
          }
		}
    |	  type SCOLON SCOLON name in_dcl new_dcl dims lengspec_or_dims opt_init_list
		{ 
          if(!in_interface_definition){
              settype($4, $1, $8, 0);
		      if(ndim>0) {
                setbound($4,ndim,dims, $8);
              } else if(ndim_type>0) {
                setbound($4,ndim_type,dims_type, $8);
              }
              if($9)
                {
                          make_param((Paramblock *)$4, 0, $9);
                }
            }
		}
    | type SCOMMA { ndim_type = 0; needkwd = 1; }  attributes  SCOLON SCOLON name in_dcl new_dcl dims lengspec_or_dims opt_init_list
		{ 
          if(!in_interface_definition){
              $$.first = $1;
              $$.second = $4;
              settype($7, $1, $11, $4);
		      if(ndim>0) {
                setbound($7,ndim,dims, $11);
              } else if(ndim_type>0) {
                setbound($7,ndim_type,dims_type, $11);
              }
              if(/*($4 & ATTRIBUTE_PARAMETER) &&*/ $12)
                {
                          make_param((Paramblock *)$7, $4, $12);
                }
                else if(($4 & ATTRIBUTE_PARAMETER) /*|| $12*/)
                {
			        err("parameter requires initialization2");
                }
              if($4 & ATTRIBUTE_SAVE)
                {
		            $7->vsave = YES;
                }
            }
		}
    | STYPE SNAME 
    {newtype(token);}
    | STYPE opt_type_attrs SCOLON SCOLON SNAME 
    {newtype(token);}
	| dcl SCOMMA name dims lengspec_or_dims opt_init_list
		{ 
          if(!in_interface_definition){
              $$ = $1;
              settype2($3, $1, $5);
		      if(ndim>0) {
                setbound($3,ndim,dims, $5);
              } else if(ndim_type>0) {
                setbound($3,ndim_type,dims_type, $5);
              }
              if(/*($1.second & ATTRIBUTE_PARAMETER) &&*/ $6)
                {
                          make_param((Paramblock *)$3, $1.second, $6);
                }
                else if(($1.second & ATTRIBUTE_PARAMETER) /*|| $6*/)
                {
			        err("parameter requires initialization1");
                }
              if($1.second & ATTRIBUTE_SAVE)
                {
		            $3->vsave = YES;
                }
            }
		}
	| dcl SSLASHD datainit vallist SSLASHD
		{
          if(!in_interface_definition){
             if (new_dcl == 2) {
			    err("attempt to give DATA in type-declaration");
			    new_dcl = 1;
			    }
            }
		}
	;

type_attr:
    SBIND SLPAR SNAME SRPAR
    |
    SEXTENDS SLPAR SNAME SRPAR
        {var_derived_type_extends_name = token;}

type_attrs:
         SCOMMA {needkwd=1;} type_attr
    |
    type_attrs SCOMMA {needkwd=1;} type_attr

opt_type_attrs:
         {needkwd=0;}/*empty*/
    |
    type_attrs

opt_init_list:
        /*empty*/
        {$$= 0;}
//    | SEQUALS SLPARSLASH {needwkey_if_seen_type_dcolon=1;} opt_type paraminits SSLASHRPAR
//        {$$ = $5;}

    | SEQUALS expr
        {$$ = $2;}
    | SEQUALS SRESHAPE SLPAR expr SCOMMA const_array SCOMMA {needkwd  =  1;} SORDER SEQUALS const_array SRPAR
		{ 
         $$ = call1(TYUNKNOWN, "ft::transpose", $4);
        }


inout: SIN {$$= ATTRIBUTE_IN;}
    | SOUT {$$= ATTRIBUTE_OUT;}
    | SINOUT {$$= ATTRIBUTE_INOUT;}


attribute: 
        SINTENT  SLPAR { needkwd = 1; } inout SRPAR 
        {
           $$ = $4;
        }
        |
        SVALUE
        {
           $$ = ATTRIBUTE_VALUE;
        }
        |
        SPARAM
        {
           $$ = ATTRIBUTE_PARAMETER;
        }
        |
        SSAVE
        {
           $$ = ATTRIBUTE_SAVE;
        }
        |
        SPRIVATE
        {
           $$ = ATTRIBUTE_PRIVATE;
        }
        |
        SPUBLIC
        {
           $$ = ATTRIBUTE_PUBLIC;
        }
        |
        SALLOCATABLE
        {
           $$ = ATTRIBUTE_ALLOCATABLE;
        }
        |
        SOPTIONAL
        {
           $$ = ATTRIBUTE_OPTIONAL;
        }
        |
        SEXTERNAL
        {
           $$ = ATTRIBUTE_EXTERNAL;
        }
        |
        SDIMENSION  dims_type
        {
           $$ = 0;
        }
        ;


attributes: 
        { needkwd = 1; }  attribute
         {
           $$ = $2;
        }
       |
        attributes SCOMMA { needkwd = 1; }  attribute
        {
           $$ = $1 | $4;
        }
        ;


new_dcl:	{ new_dcl = 2; }

type:	  typespec type_lengspec
		{ varleng = $2; }
	;

typespec:  typename
		{ varleng = ($1<0 || ONEOF($1,M(TYLOGICAL)|M(TYLONG))
				? 0 : typesize[$1]);
		  vartype = $1; }
	;

typename:    SINTEGER	{ $$ = TYLONG; }
	| SREAL		{ $$ = tyreal; }
	| SCOMPLEX	{ ++complex_seen; $$ = tycomplex; }
	| SDOUBLE	{ $$ = TYDREAL; /*dp*/ }
	| SDCOMPLEX	{ ++dcomplex_seen; NOEXT("DOUBLE COMPLEX statement"); $$ = TYDCOMPLEX; }
	| SLOGICAL	{ $$ = TYLOGICAL; }
	| SCHARACTER	{ NO66("CHARACTER statement"); $$ = TYCHAR; }
	| SUNDEFINED	{ $$ = TYUNKNOWN; }
	| SDIMENSION	{ $$ = TYUNKNOWN; }
	| SAUTOMATIC	{ NOEXT("AUTOMATIC statement"); $$ = - STGAUTO; }
	| SSTATIC	{ NOEXT("STATIC statement"); $$ = - STGBSS; }
	| STYPE SLPAR SNAME SRPAR	{ $$ = TY_DERIVED; var_derived_type_name = token;}  
	| SCLASS SLPAR SNAME SRPAR	{ $$ = TY_DERIVED; var_derived_type_name = token;}  
	;

type_lengspec:
    lengspec {$$ = $1;}
    |
    SLPAR {trykwd=1;} type_lengspec_helper { $$ = $3;} 
//    |
//    SLPAR {trykwd=1;} intonlyon expr intonlyoff SRPAR {$$ = $4->constblock.Const.ci;}
//    |
//	SLPAR {trykwd=1;} type_lengspec_name SRPAR	{ $$ = $3;}   
    ;

type_lengspec_helper:
    intonlyon expr intonlyoff type_lengspec_opt_kind SRPAR {$$ = $2->constblock.Const.ci;}
    |
	type_lengspec_name SRPAR	{ $$ = $1;}   
    ;

type_lengspec_name_kwd:
        SC_BOOL                         { $$ = length_from_name("c_bool");}  
      | SC_CHAR                         { $$ = length_from_name("c_char");}  
      | SC_DOUBLE                       { $$ = length_from_name("c_double");}  
      | SC_DOUBLE_COMPLEX               { $$ = length_from_name("c_double_complex");}  
      | SC_FLOAT                        { $$ = length_from_name("c_float");}  
      | SC_FLOAT_COMPLEX                { $$ = length_from_name("c_float_complex");}  
      | SC_FLOAT128                     { $$ = length_from_name("c_float128");}  
      | SC_FLOAT128_COMPLEX             { $$ = length_from_name("c_float128_complex");}  
      | SC_INT                          { $$ = length_from_name("c_int");}  
      | SC_INT_FAST128_T                { $$ = length_from_name("c_int_fast128_t");}  
      | SC_INT_FAST16_T                 { $$ = length_from_name("c_int_fast16_t");}  
      | SC_INT_FAST32_T                 { $$ = length_from_name("c_int_fast32_t");}  
      | SC_INT_FAST64_T                 { $$ = length_from_name("c_int_fast64_t");}  
      | SC_INT_FAST8_T                  { $$ = length_from_name("c_int_fast8_t");}  
      | SC_INT_LEAST128_T               { $$ = length_from_name("c_int_least128_t");}  
      | SC_INT_LEAST16_T                { $$ = length_from_name("c_int_least16_t");}  
      | SC_INT_LEAST32_T                { $$ = length_from_name("c_int_least32_t");}  
      | SC_INT_LEAST64_T                { $$ = length_from_name("c_int_least64_t");}  
      | SC_INT_LEAST8_T                 { $$ = length_from_name("c_int_least8_t");}  
      | SC_INT128_T                     { $$ = length_from_name("c_int128_t");}  
      | SC_INT16_T                      { $$ = length_from_name("c_int16_t");}  
      | SC_INT32_T                      { $$ = length_from_name("c_int32_t");}  
      | SC_INT64_T                      { $$ = length_from_name("c_int64_t");}  
      | SC_INT8_T                       { $$ = length_from_name("c_int8_t");}  
      | SC_INTMAX_T                     { $$ = length_from_name("c_intmax_t");}  
      | SC_INTPTR_T                     { $$ = length_from_name("c_intptr_t");}  
      | SC_LONG                         { $$ = length_from_name("c_long");}  
      | SC_LONG_DOUBLE                  { $$ = length_from_name("c_long_double");}  
      | SC_LONG_DOUBLE_COMPLEX          { $$ = length_from_name("c_long_double_complex");}  
      | SC_LONG_LONG                    { $$ = length_from_name("c_long_long");}  
      | SC_PTRDIFF_T                    { $$ = length_from_name("c_ptrdiff_t");}  
      | SC_SHORT                        { $$ = length_from_name("c_short");}  
      | SC_SIGNED_CHAR                  { $$ = length_from_name("c_signed_char");}  
      | SC_SIZE_T                       { $$ = length_from_name("c_size_t");}  
      | SINT64                          { $$ = length_from_name("int64");}  
      | SREAL128                        { $$ = length_from_name("real128");}  


//name_as_string : 
//    SNAME {$$ = strdup(token);}
    ;

type_lengspec_name:
    type_lengspec_name_kwd { $$ = $1;}   
    |
	SKIND SEQUALS {trykwd=1; /* jdslgjks*/} type_lengspec_name_kwd type_lengspec_opt_len  { $$ = $4;}   
    |
    // "len" only used with characters -> len and kind ignored (set to 1)
	SLEN SEQUALS intonlyon expr intonlyoff type_lengspec_opt_kind { $$ = 1;}
    ;

type_lengspec_opt_len:
    /* empty */
    |
    SCOMMA {trykwd=1;}  SLEN SEQUALS expr
    ;

type_lengspec_opt_kind:
    /* empty */
    |
    SCOMMA {trykwd=1;}  type_lengspec_opt_kind_helper
    ;

type_lengspec_opt_kind_helper:
    SKIND SEQUALS {trykwd=1;} type_lengspec_name_kwd
    |
    type_lengspec_name_kwd
    ;


lengspec:
		{ $$ = varleng; }
	| SSTAR intonlyon expr intonlyoff
		{
		expptr p;
		p = $3;
		NO66("length specification *n");
		if( ! ISICON(p) || p->constblock.Const.ci <= 0 )
			{
			$$ = 0;
            if(!cxx)
            {
			    dclerr("length must be a positive integer constant",
				    NPNULL);
            }
        }
		else {
			if (vartype == TYCHAR)
				$$ = p->constblock.Const.ci;
			else switch((int)p->constblock.Const.ci) {
				case 1:	$$ = 1; break;
				case 2: $$ = typesize[TYSHORT];	break;
				case 4: $$ = typesize[TYLONG];	break;
				case 8: $$ = typesize[TYDREAL];	break;
				case 16: $$ = typesize[TYDCOMPLEX]; break;
				default:
					dclerr("invalid length",NPNULL);
					$$ = varleng;
				}
			}
		}
	| SSTAR intonlyon SLPAR SSTAR SRPAR intonlyoff
		{ NO66("length specification *(*)"); $$ = -1; }
	;

lengspec_or_dims:
		{ $$ = mkintcon(varleng); }
	| SSTAR intonlyon expr intonlyoff
		{
            if(cxx){
                $$ = $3;
            } else {
		    expptr p;
		    p = $3;
		    NO66("length specification *n");
		    if( ! ISICON(p) || p->constblock.Const.ci <= 0 )
			    {
			    $$ = mkintcon(0);
                if(!cxx)
                {
			        dclerr("length must be a positive integer constant",
				        NPNULL);
                }
            }
		    else {
			    if (vartype == TYCHAR)
				    $$ = mkintcon(p->constblock.Const.ci);
			    else switch((int)p->constblock.Const.ci) {
				    case 1:	$$ = mkintcon(1); break;
				    case 2: $$ = mkintcon(typesize[TYSHORT]);	break;
				    case 4: $$ = mkintcon(typesize[TYLONG]);	break;
				    case 8: $$ = mkintcon(typesize[TYDREAL]);	break;
				    case 16: $$ = mkintcon(typesize[TYDCOMPLEX]); break;
				    default:
					    dclerr("invalid length",NPNULL);
					    $$ = mkintcon(varleng);
				    }
			    }
		    }
        }
    
	| SSTAR intonlyon SLPAR SSTAR SRPAR intonlyoff
		{ NO66("length specification *(*)"); $$ = mkintcon(-1); }
	;

common:	  SCOMMON in_dcl var
		{ incomm( $$ = comblock("") , $3 ); }
	| SCOMMON in_dcl comblock var
		{ $$ = $3;  incomm($3, $4); }
	| common opt_comma comblock opt_comma var
		{ $$ = $3;  incomm($3, $5); }
	| common SCOMMA var
		{ incomm($1, $3); }
	;

comblock:  SCONCAT
		{ $$ = comblock(""); }
	| SSLASH SNAME SSLASH
		{ $$ = comblock(token); }
	;

external: SEXTERNAL in_dcl name
		{ setext($3); }
	| external SCOMMA name
		{ setext($3); }
	;

intrinsic:  SINTRINSIC in_dcl name
		{ NO66("INTRINSIC statement"); setintr($3); }
	| intrinsic SCOMMA name
		{ setintr($3); }
	;

equivalence:  SEQUIV in_dcl equivset
	| equivalence SCOMMA equivset
	;

equivset:  SLPAR equivlist SRPAR
		{
		struct Equivblock *p;
		if(nequiv >= maxequiv)
			many("equivalences", 'q', maxequiv);
		p  =  & eqvclass[nequiv++];
		p->eqvinit = NO;
		p->eqvbottom = 0;
		p->eqvtop = 0;
		p->equivs = $2;
		}
	;

equivlist:  lhs
		{ $$=ALLOC(Eqvchain);
		  $$->eqvitem.eqvlhs = (struct Primblock *)$1;
		}
	| equivlist SCOMMA lhs
		{ $$=ALLOC(Eqvchain);
		  $$->eqvitem.eqvlhs = (struct Primblock *) $3;
		  $$->eqvnextp = $1;
		}
	;

data:	  SDATA in_data datalist
	| data opt_comma datalist
	;

in_data:
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
		}
	;

datalist:  datainit datavarlist SSLASH datapop vallist SSLASH
		{ if(!cxx){
          ftnint junk;
		  if(nextdata(&junk) != NULL)
			err("too few initializers");
            }
		  frdata($2);
		  frrpl();
		}
	;

datainit: /* nothing */ { frchain(&datastack); curdtp = 0; }

datapop: /* nothing */ { pop_datastack(); }

vallist:  { toomanyinit = NO; }  val
	| vallist SCOMMA val
	;

val:	  value
		{ dataval(ENULL, $1); }
	| simple SSTAR value
		{ dataval($1, $3); }
	;

value:	
    name SLPAR funarglist SRPAR            //       %prec SOPCALL
		{ 
		  $$ = mkexpr(OPINIT, (expptr)$1, convert_to_init_list((expptr)mklist($3))); 
        }
    |
  simple
	| addop simple
		{ if( $1==OPMINUS && ISCONST($2) )
			consnegop((Constp)$2);
		  $$ = $2;
		}
	/* | complex_const */
	;

savelist: saveitem
	| savelist SCOMMA saveitem
	;

saveitem: name
		{ int k;
		  $1->vsave = YES;
		  k = $1->vstg;
		if( ! ONEOF(k, M(STGUNKNOWN)|M(STGBSS)|M(STGINIT)) )
			dclerr("can only save static variables", $1);
		}
	| comblock
	;

paramlist:  paramitem
	| paramlist SCOMMA paramitem
	;

paramitem:  name SEQUALS expr
		{ if($1->vclass == CLUNKNOWN)
			make_param((Paramblock *)$1, ATTRIBUTE_PARAMETER, $3);
		  else dclerr("cannot make into parameter", $1);
		}
//    | name SEQUALS SLPARSLASH {needwkey_if_seen_type_dcolon=1;/*skjdhg*/} opt_type paraminits SSLASHRPAR
//		{ if($1->vclass ==  CLUNKNOWN)
//			make_param((Paramblock *)$1, ATTRIBUTE_PARAMETER, $6);
//		  else dclerr("cannot make into parameter", $1);
//		}
    | name SEQUALS SRESHAPE {int kasjhgjkasdgkj = 42;} SLPAR expr SCOMMA const_array SCOMMA {needkwd  =  1;} SORDER SEQUALS const_array SRPAR
		{ 
        if($1->vclass ==  CLUNKNOWN)
			make_param((Paramblock *)$1, ATTRIBUTE_PARAMETER, $6);
		  else dclerr("cannot make into parameter", $1);
        }
//    |    
//    SLPARSLASH {needwkey_if_seen_type_dcolon=1;/*skjdhg*/} opt_type paraminits SSLASHRPAR
//		{ if($1->vclass ==  CLUNKNOWN)
//			make_param((Paramblock *)$1, ATTRIBUTE_PARAMETER, $6);
//		  else dclerr("cannot make into parameter", $1);
//		}
	;
    

const_array_items:
    const_array_items SCOMMA expr
    |
    expr
    ;

const_array:
    SLPARSLASH const_array_items SSLASHRPAR
    ;


paraminit:  
//    SLPAR expr SCOMMA name SEQUALS expr SCOMMA expr SRPAR
//    {
//        chainp p = mkchain((char *)$8, CHNULL);
//        p = mkchain((char *)$6, p);
//        p = mkchain((char *)$2, p);
//        Listblock * l = mklist(p);
//        l->tag = TSPECIALLIST;
//        l->vtype = VTSPECIALLIST_IMPLIED_DO;
//        $$ = mkchain((char *)l, CHNULL);
//    }
//    |
        expr_do_list
		{ $$ = mkchain((char *)$1, CHNULL); }
      |
        expr
		{ $$ = mkchain((char *)$1, CHNULL); }
;


opt_type:
        /* empty */  {needwkey_if_seen_type_dcolon=0;}   
        |
        type SCOLON SCOLON


paraminits:  
        paraminit
		{ $$ = $1; }
	| paraminits SCOMMA paraminit
		{ $$ = hookup($3, $1); }
	;



var:	  name dims
		{ if(ndim>0) setbound($1, ndim, dims, 0); }
	;

datavar:	  lhs
		{ Namep np;
		  np = ( (struct Primblock *) $1) -> namep;
		  vardcl(np);
		  if(np->vstg == STGCOMMON)
			extsymtab[np->vardesc.varno].extinit = YES;
		  else if(np->vstg==STGEQUIV)
			eqvclass[np->vardesc.varno].eqvinit = YES;
		  else if(np->vstg!=STGINIT && np->vstg!=STGBSS)
			dclerr("inconsistent storage classes", np);
		  $$ = mkchain((char *)$1, CHNULL);
		}
	| SLPAR datavarlist SCOMMA dospec SRPAR
		{ chainp p; struct Impldoblock *q;
		pop_datastack();
		q = ALLOC(Impldoblock);
		q->tag = TIMPLDO;
		(q->varnp = (Namep) ($4->datap))->vimpldovar = 1;
		p = $4->nextp;
		if(p)  { q->implb = (expptr)(p->datap); p = p->nextp; }
		if(p)  { q->impub = (expptr)(p->datap); p = p->nextp; }
		if(p)  { q->impstep = (expptr)(p->datap); }
		frchain2( $4 );
		$$ = mkchain((char *)q, CHNULL);
		q->datalist = hookup($2, $$);
		}
	;

datavarlist: datavar
		{ if (!datastack)
			curdtp = 0;
		  datastack = mkchain((char *)curdtp, datastack);
		  curdtp = $1; curdtelt = 0;
		  }
	| datavarlist SCOMMA datavar
		{ $$ = hookup($1, $3); }
	;


dims_type:
    {in_dims_type = true; ndim_type = 0;}    // init
    dims                // scan
    {in_dims_type = false; ndim_type = ndim; ndim = 0;} // remember
    ;

dims:
		{ ndim = 0; }
	| SLPAR dimlist SRPAR
	;

dimlist:   { ndim = 0; }   dim
	| dimlist SCOMMA dim
	;

dim:	  ubound
		{
          Dims* cur_dims = in_dims_type ? dims_type : dims;
		  if(ndim == maxdim)
			err("too many dimensions");
		  else if(ndim < maxdim)
			{ cur_dims[ndim].lb = 0;
			  cur_dims[ndim].ub = $1;
			}
		  ++ndim;
		}
	| expr SCOLON ubound
		{
          Dims* cur_dims = in_dims_type ? dims_type : dims;
		  if(ndim == maxdim)
			err("too many dimensions");
		  else if(ndim < maxdim)
			{ cur_dims[ndim].lb = $1;
			  cur_dims[ndim].ub = $3;
			}
		  ++ndim;
		}
	| SCOLON
		{
          Dims* cur_dims = in_dims_type ? dims_type : dims;
		  if(ndim == maxdim)
			err("too many dimensions");
		  else if(ndim < maxdim)
			{ cur_dims[ndim].lb = nullptr;
			  cur_dims[ndim].ub = nullptr;
			}
		  ++ndim;
		}
	;

ubound:	  SSTAR
		{ $$ = 0; }
	| expr
	;

labellist: label
		{ nstars = 1; labarray[0] = $1; }
	| labellist SCOMMA label
		{ if(nstars < maxlablist)  labarray[nstars++] = $3; }
	;

label:	  SICON
		{ $$ = execlab( convci(toklen, token) ); }
	;

implicit:  SIMPLICIT in_dcl implist
		{ NO66("IMPLICIT statement"); }
	| implicit SCOMMA implist
	;

opt_intrinsic:
    /*empty*/
    |
    SCOMMA {needkwd = 1; /*oiuqwe*/} SINTRINSIC
    ;

opt_dcolon:
    /*empty*/
    |
    SCOLON SCOLON
    ;


only_names:  
        SNAME
	| only_names SCOMMA SNAME
	;

opt_onlys:
    /*empty*/
    |
    SCOMMA {needkwd=1;} SONLY SCOLON only_names
    ;

use:  SUSE opt_intrinsic opt_dcolon SNAME { exuse(token); } opt_onlys
	;

implist:  imptype SLPAR letgroups SRPAR
	| imptype
		{ if (vartype != TYUNKNOWN)
			dclerr("-- expected letter range",NPNULL);
		  setimpl(vartype, varleng, 'a', 'z'); }
	;

imptype:   { needkwd = 1; } type
		/* { vartype = $2; } */
	;

letgroups: letgroup
	| letgroups SCOMMA letgroup
	;

letgroup:  letter
		{ setimpl(vartype, varleng, $1, $1); }
	| letter SMINUS letter
		{ setimpl(vartype, varleng, $1, $3); }
	;

letter:  SNAME
		{ if(toklen!=1 || token[0]<'a' || token[0]>'z')
			{
			dclerr("implicit item must be single letter", NPNULL);
			$$ = 0;
			}
		  else $$ = token[0];
		}
	;

namelist:	SNAMELIST
	| namelist namelistentry
	;

namelistentry:  SSLASH name SSLASH namelistlist
		{
		if($2->vclass == CLUNKNOWN)
			{
			$2->vclass = CLNAMELIST;
			$2->vtype = TYINT;
			$2->vstg = STGBSS;
			$2->varxptr.namelist = $4;
			$2->vardesc.varno = ++lastvarno;
			}
		else dclerr("cannot be a namelist name", $2);
		}
	;

namelistlist:  name
		{ $$ = mkchain((char *)$1, CHNULL); }
	| namelistlist SCOMMA name
		{ $$ = hookup($1, mkchain((char *)$3, CHNULL)); }
	;

in_dcl:
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
		}
	;
