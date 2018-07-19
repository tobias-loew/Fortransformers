/* f2c.h  --  Standard Fortran to C header file */

/**  barf  [ba:rf]  2.  "He suggested using FORTRAN, and everybody barfed."

	- From The Shogakukan DICTIONARY OF NEW ENGLISH (Second edition) */

#ifndef F2C_EXTENSIONS_INCLUDE
#define F2C_EXTENSIONS_INCLUDE


#define TOKENPASTE(x, y, z, a) x ## y ## z ## a
#define TOKENPASTE4(x, y, z, a) TOKENPASTE(x, y, z, a)
#define UNIQUE static void TOKENPASTE2(Unique_, __LINE__)(void) {}

// macro to make a variable constant inside a scope
// used to ensure upper limit is constant during do loop
#define CONSTIFY(v) const auto & TOKENPASTE4(___,v,__,__LINE__) = v; const auto & v = TOKENPASTE4(___,v,__,__LINE__); 



#endif
