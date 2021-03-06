#pragma once

/*
 * Usage is granted as specified in thefollowing "Boost Software License - Version 1.0", but usage and/or distribution and/or selling as a or within a commercial Fortran to C/C++ translation-tool is prohibited.
 * 
 * 
 * Boost Software License - Version 1.0 - August 17th, 2003
 * 
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */


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

