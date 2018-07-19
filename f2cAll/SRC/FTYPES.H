
/* variable types (stored in the   vtype  field of   expptr)
 * numeric assumptions:
 *	int < reals < complexes
 *	TYDREAL-TYREAL = TYDCOMPLEX-TYCOMPLEX
 */

#ifdef NO_TYQUAD
#undef TYQUAD
#define TYQUAD_inc 0
#else
#define TYQUAD 5
#define TYQUAD_inc 1
#endif

#define TYUNKNOWN 0
#define TYADDR 1
#define TYINT1 2
#define TYSHORT 3
#define TYLONG 4
/* #define TYQUAD 5 */
#define TYREAL (5+TYQUAD_inc)
#define TYDREAL (6+TYQUAD_inc)
#define TYCOMPLEX (7+TYQUAD_inc)
#define TYDCOMPLEX (8+TYQUAD_inc)
#define TYLOGICAL1 (9+TYQUAD_inc)
#define TYLOGICAL2 (10+TYQUAD_inc)
#define TYLOGICAL (11+TYQUAD_inc)
#define TYCHAR (12+TYQUAD_inc)
#define TYSUBR (13+TYQUAD_inc)
#define TYERROR (14+TYQUAD_inc)
#define TYCILIST (15+TYQUAD_inc)
#define TYICILIST (16+TYQUAD_inc)
#define TYOLIST (17+TYQUAD_inc)
#define TYCLLIST (18+TYQUAD_inc)
#define TYALIST (19+TYQUAD_inc)
#define TYINLIST (20+TYQUAD_inc)
#define TYVOID (21+TYQUAD_inc)
#define TYLABEL (22+TYQUAD_inc)
#define TYFTNLEN (23+TYQUAD_inc)

#define TYINTPTR (24+TYQUAD_inc)
#define TY_DERIVED (25+TYQUAD_inc)
#define TY_DATA_STRING (26+TYQUAD_inc)  // used for temporary storing strings in data statements

// use the two most significant bits for pure/elemental
#define TY_PREFIX_ELEMENTAL 0x80000000
#define TY_PREFIX_PURE 0x40000000

#define TY_TYPE_MASK 0x3fffffff

#define GET_TYPE(T) (T & TY_TYPE_MASK)



/* TYVOID is not in any tables. */

/* NTYPES, NTYPES0 -- Total number of types, used to allocate tables indexed by
   type.  Such tables can include the size (in bytes) of objects of a given
   type, or labels for returning objects of different types from procedures
   (see array   rtvlabels)   */

#define NTYPES TY_DERIVED
#define NTYPES0 (NTYPES+1)
#define TYBLANK TYSUBR		/* Huh? */

