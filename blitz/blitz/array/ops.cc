/***************************************************************************
 * blitz/array/ops.cc  Basic operators for arrays.
 *
 * $Id$
 *
 * Copyright (C) 1997-2011 Todd Veldhuizen <tveldhui@acm.org>
 * Copyright (C) 2017-2018 Tobias Loew <tobi@die-loews.de>
 *
 * This file is a part of Blitz.
 *
 * Blitz is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Blitz is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public 
 * License along with Blitz.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Suggestions:          tobi@die-loews.de
 * Bugs:                 tobi@die-loews.de    
 *
 * For more information, please see the Blitz++ Home Page:
 *    https://sourceforge.net/projects/blitz/
 *
 ****************************************************************************/
#ifndef BZ_ARRAYOPS_CC
#define BZ_ARRAYOPS_CC

#ifndef BZ_ARRAY_H
 #error <blitz/array/ops.cc> must be included via <blitz/array.h>
#endif

#include <blitz/update.h>
#include <blitz/globeval.cc>

BZ_NAMESPACE(blitz)

/*
 * Constant operands
 */

//template<typename P_numtype, int N_rank, typename Policy>
//template<typename PolicyT>
//_bz_forceinline
//Array<P_numtype, N_rank, Policy>& Array<P_numtype, N_rank, Policy>::initialize_impl(T_numtype x)
//{
//  // we can't use asExpr here, because if we are initializing an array
//  // whose components are also ETBase, it would parse as an array
//  // expression, not as an initialization with a scalar.
//  (*this) = _bz_ArrayExpr<_bz_ArrayExprConstant<T_numtype> >(x);
//  return *this;
//}
//
//
//template<typename P_numtype, int N_rank, typename Policy>
//template<>
//_bz_forceinline
//Array<P_numtype, N_rank, Policy>& Array<P_numtype, N_rank, Policy>::initialize_impl<array_policy::StaticFortranArrayPolicy<1>>(T_numtype x) {
//    // we can't use asExpr here, because if we are initializing an array
//    // whose components are also ETBase, it would parse as an array
//    // expression, not as an initialization with a scalar.
//    (*this) = _bz_ArrayExpr<_bz_ArrayExprConstant<T_numtype> >(x);
//    return *this;
//}


#ifdef BZ_NEW_EXPRESSION_TEMPLATES

// need to do operator= separately from the generic templates below so
// that operator=(T_numtype) will be the best match for list
// initializations.
template<typename P_numtype, int N_rank, typename Policy> template<typename T_expr>
_bz_forceinline 
Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator=(const ETBase<T_expr>& expr)
{
  _bz_evaluate(*this, _bz_typename asExpr<T_expr>::T_expr(expr.unwrap()), 
	       _bz_update<T_numtype, 
	       _bz_typename asExpr<T_expr>::T_expr::T_result>());
    return *this;
}

// do NOT remove this operator. it won't work without it, trust me...
// handle case with different policies
template<typename P_numtype, int N_rank, typename Policy>
template<typename Policy2>
_bz_forceinline
Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator=(const Array<T_numtype, N_rank, Policy2>& x)
{
  typedef typename asExpr<Array<T_numtype, N_rank, Policy2> >::T_expr T_expr;
  _bz_evaluate(*this, asExpr<Array<T_numtype, N_rank, Policy2> >::getExpr(x),
	       _bz_update<T_numtype, 
	       _bz_typename T_expr::T_result>());
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy>
template<typename PolicyT>
_bz_forceinline
Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::assign_impl(PolicyT, const Array<T_numtype, N_rank, Policy>& x) {
    typedef typename asExpr<Array<T_numtype, N_rank, Policy> >::T_expr T_expr;
    _bz_evaluate(*this, asExpr<Array<T_numtype, N_rank, Policy> >::getExpr(x),
        _bz_update<T_numtype,
        _bz_typename T_expr::T_result>());
    return *this;
}


template<typename P_numtype, int N_rank, typename Policy>
template<int N_base>
_bz_forceinline
Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::assign_impl(array_policy::StaticFortranArrayPolicy<N_base>, const Array<T_numtype, N_rank, Policy>& x) {

    //const int ub = N_base + length_[0];
    for(int index = N_base; index < N_base + length_[0]; ++index) {
        data_[index] = x.data_[index];
    }

    return *this;
}



#define BZ_ARRAY_UPDATE(op,name)					\
template<typename P_numtype, int N_rank, typename Policy>				\
template<typename T_expr>						\
_bz_forceinline							\
Array<P_numtype, N_rank, Policy>&						\
Array<P_numtype, N_rank, Policy>::operator op(const ETBase<T_expr>& expr)	\
{									\
_bz_evaluate(*this, _bz_typename asExpr<T_expr>::T_expr(expr.unwrap()), \
name<T_numtype, _bz_typename asExpr<T_expr>::T_expr::T_result>()); \
return *this;							\
}									\
template<typename P_numtype, int N_rank, typename Policy>				\
_bz_forceinline							\
Array<P_numtype, N_rank, Policy>&						\
Array<P_numtype, N_rank, Policy>::operator op(const Array<T_numtype, N_rank, Policy>& x) \
{									\
typedef typename asExpr<Array<T_numtype, N_rank, Policy> >::T_expr T_expr;	\
_bz_evaluate(*this, asExpr<Array<T_numtype, N_rank, Policy> >::getExpr(x),	\
name<T_numtype, _bz_typename T_expr::T_result>());	\
return *this;							\
}									\
template<typename P_numtype, int N_rank, typename Policy>				\
_bz_forceinline							\
Array<P_numtype, N_rank, Policy>&						\
Array<P_numtype, N_rank, Policy>::operator op(const T_numtype& x)		\
{									\
_bz_evaluate_value(*this, x,			\
name<T_numtype, T_numtype>());	\
return *this;							\
} 
 


#define BZ_ARRAY_UPDATE_POLICY_DISPATCH(op,name)					\
/* original version - no longer used */ \
/*  /
  template<typename P_numtype, int N_rank, typename Policy>				\
  template<typename T_expr>						\
  _bz_forceinline							\
  Array<P_numtype, N_rank, Policy>&						\
  Array<P_numtype, N_rank, Policy>::operator op(const ETBase<T_expr>& expr)	\
  {									\
    _bz_evaluate(*this, _bz_typename asExpr<T_expr>::T_expr(expr.unwrap()), \
         name<T_numtype, _bz_typename asExpr<T_expr>::T_expr::T_result>()); \
    return *this;							\
  }									\
  template<typename P_numtype, int N_rank, typename Policy>				\
  _bz_forceinline							\
  Array<P_numtype, N_rank, Policy>&						\
  Array<P_numtype, N_rank, Policy>::operator op(const Array<T_numtype, N_rank, Policy>& x) \
  {									\
    typedef typename asExpr<Array<T_numtype, N_rank, Policy> >::T_expr T_expr;	\
    _bz_evaluate(*this, asExpr<Array<T_numtype, N_rank, Policy> >::getExpr(x),	\
         name<T_numtype, _bz_typename T_expr::T_result>());	\
    return *this;							\
  }									\
  template<typename P_numtype, int N_rank, typename Policy>				\
  _bz_forceinline							\
  Array<P_numtype, N_rank, Policy>&						\
  Array<P_numtype, N_rank, Policy>::operator op(const T_numtype& x)		\
  {									\
    typedef typename asExpr<T_numtype>::T_expr T_expr;			\
    _bz_evaluate(*this, asExpr<T_numtype>::getExpr(x),			\
         name<T_numtype, _bz_typename T_expr::T_result>());	\
    return *this;							\
  } \
*/ \
\
\
/* new Policy based overloaded version */ \
\
template<typename Policy_Dispatch, typename P_numtype, int N_rank, typename Policy, typename T_expr>				\
_bz_forceinline							\
Array<P_numtype, N_rank, Policy>&						\
name##_expr(Policy_Dispatch, Array<P_numtype, N_rank, Policy>& self, const ETBase<T_expr>& expr)	\
{									\
    using T_Array = Array<P_numtype, N_rank, Policy>; \
_bz_evaluate(self, _bz_typename asExpr<T_expr>::T_expr(expr.unwrap()), \
    name<typename T_Array::T_numtype, _bz_typename asExpr<T_expr>::T_expr::T_result>()); \
    return self;							\
}									\
template<typename Policy_Dispatch, typename P_numtype, int N_rank, typename Policy>				\
_bz_forceinline							\
Array<P_numtype, N_rank, Policy>&						\
name##_array(Policy_Dispatch, Array<P_numtype, N_rank, Policy>& self, const Array<typename Array<P_numtype, N_rank, Policy>::T_numtype, N_rank, Policy>& x) \
{									\
    using T_Array = Array<P_numtype, N_rank, Policy>; \
typedef typename asExpr<Array<typename T_Array::T_numtype, N_rank, Policy> >::T_expr T_expr;	\
_bz_evaluate(self, asExpr<Array<typename T_Array::T_numtype, N_rank, Policy> >::getExpr(x), \
    name<typename T_Array::T_numtype, _bz_typename T_expr::T_result>());	\
    return self;							\
}									\
template<typename Policy_Dispatch, typename P_numtype, int N_rank, typename Policy>				\
_bz_forceinline							\
Array<P_numtype, N_rank, Policy>&						\
name##_value(Policy_Dispatch, Array<P_numtype, N_rank, Policy>& self, const typename Array<P_numtype, N_rank, Policy>::T_numtype& x)		\
{									\
    using T_Array = Array<P_numtype, N_rank, Policy>; \
typedef typename asExpr<typename T_Array::T_numtype>::T_expr T_expr;			\
_bz_evaluate(self, asExpr<typename T_Array::T_numtype>::getExpr(x), \
    name<typename T_Array::T_numtype, _bz_typename T_expr::T_result>());	\
    return self;							\
} \
\
\
/* external entry points */\
template<typename P_numtype, int N_rank, typename Policy>				\
template<typename T_expr>						\
_bz_forceinline							\
Array<P_numtype, N_rank, Policy>&						\
Array<P_numtype, N_rank, Policy>::operator op(const ETBase<T_expr>& expr)	\
{									\
    return name##_expr(Policy{}, *this, expr); \
}									\
template<typename P_numtype, int N_rank, typename Policy>				\
_bz_forceinline							\
Array<P_numtype, N_rank, Policy>&						\
Array<P_numtype, N_rank, Policy>::operator op(const Array<T_numtype, N_rank, Policy>& x) \
{									\
    return name##_array(Policy{}, *this, x); \
}									\
template<typename P_numtype, int N_rank, typename Policy>				\
_bz_forceinline							\
Array<P_numtype, N_rank, Policy>&						\
Array<P_numtype, N_rank, Policy>::operator op(const T_numtype& x)		\
{									\
    return name##_value(Policy{}, *this, x); \
} \
\


BZ_ARRAY_UPDATE(+=, _bz_plus_update)
BZ_ARRAY_UPDATE(-=, _bz_minus_update)
BZ_ARRAY_UPDATE(*=, _bz_multiply_update)
BZ_ARRAY_UPDATE(/=, _bz_divide_update)
BZ_ARRAY_UPDATE(%=, _bz_mod_update)
BZ_ARRAY_UPDATE(^=, _bz_xor_update)
BZ_ARRAY_UPDATE(&=, _bz_bitand_update)
BZ_ARRAY_UPDATE(|=, _bz_bitor_update)
BZ_ARRAY_UPDATE(<<=, _bz_shiftl_update)
BZ_ARRAY_UPDATE(>>=, _bz_shiftr_update)

#else

template<typename P_numtype, int N_rank, typename Policy>
inline Array<P_numtype, N_rank, Policy>& 
Array<P_numtype, N_rank, Policy>::operator+=(T_numtype x)
{
    (*this) += _bz_ArrayExpr<_bz_ArrayExprConstant<T_numtype> >(x);
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator-=(T_numtype x)
{
    (*this) -= _bz_ArrayExpr<_bz_ArrayExprConstant<T_numtype> >(x);
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator*=(T_numtype x)
{
    (*this) *= _bz_ArrayExpr<_bz_ArrayExprConstant<T_numtype> >(x);
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator/=(T_numtype x)
{
    (*this) /= _bz_ArrayExpr<_bz_ArrayExprConstant<T_numtype> >(x);
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator%=(T_numtype x)
{
    (*this) %= _bz_ArrayExpr<_bz_ArrayExprConstant<T_numtype> >(x);
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator^=(T_numtype x)
{
    (*this) ^= _bz_ArrayExpr<_bz_ArrayExprConstant<T_numtype> >(x);
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator&=(T_numtype x)
{
    (*this) &= _bz_ArrayExpr<_bz_ArrayExprConstant<T_numtype> >(x);
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator|=(T_numtype x)
{
    (*this) |= _bz_ArrayExpr<_bz_ArrayExprConstant<T_numtype> >(x);
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator>>=(T_numtype x)
{
    (*this) <<= _bz_ArrayExpr<_bz_ArrayExprConstant<T_numtype> >(x);
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator<<=(T_numtype x)
{
    (*this) <<= _bz_ArrayExpr<_bz_ArrayExprConstant<T_numtype> >(x);
    return *this;
}

/*
 * Array operands
 */

template<typename P_numtype, int N_rank, typename Policy>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator=(const Array<T_numtype, N_rank, Policy>& x)
{
    (*this) = _bz_ArrayExpr<FastArrayIterator<T_numtype, N_rank> >(x.beginFast());
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy> template<typename P_numtype2>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator=(const Array<P_numtype2,N_rank>& x)
{
    (*this) = _bz_ArrayExpr<FastArrayIterator<P_numtype2, N_rank> >(x.beginFast());
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy> template<typename P_numtype2>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator+=(const Array<P_numtype2,N_rank>& x)
{
    (*this) += _bz_ArrayExpr<FastArrayIterator<P_numtype2, N_rank> >(x.beginFast());
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy> template<typename P_numtype2>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator-=(const Array<P_numtype2,N_rank>& x)
{
    (*this) -= _bz_ArrayExpr<FastArrayIterator<P_numtype2, N_rank> >(x.beginFast());
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy> template<typename P_numtype2>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator*=(const Array<P_numtype2,N_rank>& x)
{
    (*this) *= _bz_ArrayExpr<FastArrayIterator<P_numtype2, N_rank> >(x.beginFast());
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy> template<typename P_numtype2>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator/=(const Array<P_numtype2,N_rank>& x)
{
    (*this) /= _bz_ArrayExpr<FastArrayIterator<P_numtype2, N_rank> >(x.beginFast());
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy> template<typename P_numtype2>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator%=(const Array<P_numtype2,N_rank>& x)
{
    (*this) %= _bz_ArrayExpr<FastArrayIterator<P_numtype2, N_rank> >(x.beginFast());
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy> template<typename P_numtype2>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator^=(const Array<P_numtype2,N_rank>& x)
{
    (*this) ^= _bz_ArrayExpr<FastArrayIterator<P_numtype2, N_rank> >(x.beginFast());
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy> template<typename P_numtype2>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator&=(const Array<P_numtype2,N_rank>& x)
{
    (*this) &= _bz_ArrayExpr<FastArrayIterator<P_numtype2, N_rank> >(x.beginFast());
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy> template<typename P_numtype2>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator|=(const Array<P_numtype2,N_rank>& x)
{
    (*this) |= _bz_ArrayExpr<FastArrayIterator<P_numtype2, N_rank> >(x.beginFast());
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy> template<typename P_numtype2>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator>>=(const Array<P_numtype2,N_rank>& x)
{
    (*this) >>= _bz_ArrayExpr<FastArrayIterator<P_numtype2, N_rank> >(x.beginFast());
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy> template<typename P_numtype2>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator<<=(const Array<P_numtype2,N_rank>& x)
{
    (*this) <<= _bz_ArrayExpr<FastArrayIterator<P_numtype2, N_rank> >(x.beginFast());
    return *this;
}

/*
 * Array expression operands
 */

template<typename P_numtype, int N_rank, typename Policy> template<typename T_expr>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator=(BZ_ETPARM(_bz_ArrayExpr<T_expr>) expr)
{
    evaluate(expr, _bz_update<T_numtype, _bz_typename T_expr::T_numtype>());
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy> template<typename T_expr>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator+=(BZ_ETPARM(_bz_ArrayExpr<T_expr>) expr)
{
    evaluate(expr, _bz_plus_update<T_numtype, _bz_typename T_expr::T_numtype>());
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy> template<typename T_expr>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator-=(BZ_ETPARM(_bz_ArrayExpr<T_expr>) expr)
{
    evaluate(expr, _bz_minus_update<T_numtype, 
        _bz_typename T_expr::T_numtype>());
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy> template<typename T_expr>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator*=(BZ_ETPARM(_bz_ArrayExpr<T_expr>) expr)
{
    evaluate(expr, _bz_multiply_update<T_numtype,
        _bz_typename T_expr::T_numtype>());
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy> template<typename T_expr>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator/=(BZ_ETPARM(_bz_ArrayExpr<T_expr>) expr)
{
    evaluate(expr, _bz_divide_update<T_numtype,
        _bz_typename T_expr::T_numtype>());
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy> template<typename T_expr>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator%=(BZ_ETPARM(_bz_ArrayExpr<T_expr>) expr)
{
    evaluate(expr, _bz_mod_update<T_numtype,
        _bz_typename T_expr::T_numtype>());
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy> template<typename T_expr>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator^=(BZ_ETPARM(_bz_ArrayExpr<T_expr>) expr)
{
    evaluate(expr, _bz_xor_update<T_numtype,
        _bz_typename T_expr::T_numtype>());
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy> template<typename T_expr>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator&=(BZ_ETPARM(_bz_ArrayExpr<T_expr>) expr)
{
    evaluate(expr, _bz_bitand_update<T_numtype,
        _bz_typename T_expr::T_numtype>());
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy> template<typename T_expr>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator|=(BZ_ETPARM(_bz_ArrayExpr<T_expr>) expr)
{
    evaluate(expr, _bz_bitor_update<T_numtype,
        _bz_typename T_expr::T_numtype>());
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy> template<typename T_expr>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator>>=(BZ_ETPARM(_bz_ArrayExpr<T_expr>) expr)
{
    evaluate(expr, _bz_shiftr_update<T_numtype,
        _bz_typename T_expr::T_numtype>());
    return *this;
}

template<typename P_numtype, int N_rank, typename Policy> template<typename T_expr>
inline Array<P_numtype, N_rank, Policy>&
Array<P_numtype, N_rank, Policy>::operator<<=(BZ_ETPARM(_bz_ArrayExpr<T_expr>) expr)
{
    evaluate(expr, _bz_shiftl_update<T_numtype,
        _bz_typename T_expr::T_numtype>());
    return *this;
}

#endif // BZ_NEW_EXPRESSION_TEMPLATES

BZ_NAMESPACE_END

#endif // BZ_ARRAYOPS_CC
