#pragma once

#include <boost/preprocessor.hpp>

#define TOKENPASTE(x, y, z, a) x ## y ## z ## a
#define TOKENPASTE4(x, y, z, a) TOKENPASTE(x, y, z, a)
#define UNIQUE static void TOKENPASTE2(Unique_, __LINE__)(void) {}

// macro to make a variable constant inside a scope
// used to ensure upper limit is constant during do loop
//#pragma message ("CONSTIFY not enabled !!!\n")

// everything is tested
#define CONSTIFY(v) 
//#define CONSTIFY(v) const auto & TOKENPASTE4(___,v,__,__LINE__) = v; const auto & v = TOKENPASTE4(___,v,__,__LINE__); 



#define PP_TEXT_OUT_MACRO(z, n, text) BOOST_PP_EXPAND(text)

#define PP_MULT_INIT(out, lower, upper) BOOST_PP_ENUM(BOOST_PP_SUB(BOOST_PP_INC(upper), lower), PP_TEXT_OUT_MACRO, out)

