/***************************************************************************
 * blitz/tinyvec.cc  Declaration of TinyVector methods
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
 ***************************************************************************/

#ifndef BZ_TINYBASEDMAT2_CC
#define BZ_TINYBASEDMAT2_CC

#include <blitz/extension/tbmevaluate.h>
//#include <blitz/tinymat2io.cc>
#include <blitz/array/domain.h>

BZ_NAMESPACE(blitz)

template<typename P_numtype, int N_rows, int N_columns, int N_rows_base, int N_columns_base, bool B_column_major>
inline TinyBasedMatrix<P_numtype, N_rows, N_columns, N_rows_base, N_columns_base, B_column_major>::TinyBasedMatrix(T_numtype initValue) 
{
  for (sizeType i=0; i < numElements(); ++i)
    data_[i] = initValue;
}

template<typename P_numtype, int N_rows, int N_columns, int N_rows_base, int N_columns_base, bool B_column_major>
inline TinyBasedMatrix<P_numtype, N_rows, N_columns, N_rows_base, N_columns_base, B_column_major>::TinyBasedMatrix(const T_matrix& x) 
{
  for (sizeType i=0; i < numElements(); ++i)
    data_[i] = x.data_[i];
}

template<typename P_numtype, int N_rows, int N_columns, int N_rows_base, int N_columns_base, bool B_column_major>
template<typename P_numtype2>
inline
TinyBasedMatrix<P_numtype, N_rows, N_columns, N_rows_base, N_columns_base, B_column_major>::TinyBasedMatrix(const TinyBasedMatrix<P_numtype2, N_rows, N_columns, N_rows_base, N_columns_base, B_column_major>& x)
{
  for (sizeType i=0; i < numElements(); ++i)
    data_[i] = static_cast<P_numtype>(x.data_[i]);
}

/*
 * Assignment-type operators
 */

template<typename P_numtype, int N_rows, int N_columns, int N_rows_base, int N_columns_base, bool B_column_major>
TinyBasedMatrix<P_numtype, N_rows, N_columns, N_rows_base, N_columns_base, B_column_major>&
TinyBasedMatrix<P_numtype, N_rows, N_columns, N_rows_base, N_columns_base, B_column_major>::initialize(T_numtype x)
{
    for(sizeType i = 0; i < numElements(); ++i){
        data_[i] = static_cast<P_numtype>(x);
    }
//    (*this) = _bz_ArrayExpr<_bz_ArrayExprConstant<T_numtype> >(x);
    return *this;
}

template<typename P_numtype, int N_rows, int N_columns, int N_rows_base, int N_columns_base, bool B_column_major>
template<typename T_expr>
inline
TinyBasedMatrix<P_numtype, N_rows, N_columns, N_rows_base, N_columns_base, B_column_major>&
TinyBasedMatrix<P_numtype, N_rows, N_columns, N_rows_base, N_columns_base, B_column_major>::operator=(const ETBase<T_expr>& expr)
{
  _tbm_evaluate(_bz_typename asExpr<T_expr>::T_expr(expr.unwrap()), 
	       _bz_update<T_numtype, _bz_typename asExpr<T_expr>::T_expr::T_result>());
    return *this;
}

#define BZ_TBM2_UPDATE(op,name)						\
  template<typename P_numtype, int N_rows, int N_columns, int N_rows_base, int N_columns_base, bool B_column_major>		\
  template<typename T>							\
  inline TinyBasedMatrix<P_numtype, N_rows, N_columns, N_rows_base, N_columns_base, B_column_major>&			\
  TinyBasedMatrix<P_numtype, N_rows, N_columns, N_rows_base, N_columns_base, B_column_major>::operator op(const T& expr)	\
  {									\
    _tbm_evaluate(_bz_typename asExpr<T>::T_expr(expr),			\
		 name<T_numtype, _bz_typename asExpr<T>::T_expr::T_result>()); \
  return *this;								\
  }

BZ_TBM2_UPDATE(+=, _bz_plus_update)
BZ_TBM2_UPDATE(-=, _bz_minus_update)
BZ_TBM2_UPDATE(*=, _bz_multiply_update)
BZ_TBM2_UPDATE(/=, _bz_divide_update)
BZ_TBM2_UPDATE(%=, _bz_mod_update)
BZ_TBM2_UPDATE(^=, _bz_xor_update)
BZ_TBM2_UPDATE(&=, _bz_bitand_update)
BZ_TBM2_UPDATE(|=, _bz_bitor_update)
BZ_TBM2_UPDATE(<<=, _bz_shiftl_update)
BZ_TBM2_UPDATE(>>=, _bz_shiftr_update)

/*
 * Other member functions
 */


template<typename P_numtype, int N_rows, int N_columns, int N_rows_base, int N_columns_base, bool B_column_major>
inline RectDomain<2> 
TinyBasedMatrix<P_numtype, N_rows, N_columns, N_rows_base, N_columns_base, B_column_major>::domain()
{
  return RectDomain<2>(lbound(), ubound()); 
}


template<typename P_numtype, int N_rows, int N_columns, int N_rows_base, int N_columns_base, bool B_column_major>
template<int N0, int N1>
inline _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<TinyBasedMatrix<P_numtype, N_rows, N_columns, N_rows_base, N_columns_base, B_column_major> >::T_expr, N0, N1> >
TinyBasedMatrix<P_numtype, N_rows, N_columns, N_rows_base, N_columns_base, B_column_major>::operator()(IndexPlaceholder<N0>, IndexPlaceholder<N1>) const
{
  return _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_matrix>::T_expr, 
					 N0, N1> >(noConst());
} 


BZ_NAMESPACE_END

#endif // BZ_TINYBASEDMAT2_CC
