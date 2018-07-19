// -*- C++ -*-
/***************************************************************************
 * blitz/tinymat.h       Declaration of TinyBasedMatrix<T, N, M>
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

#ifndef BZ_TINYBASEDMAT_H
#define BZ_TINYBASEDMAT_H

#include <blitz/blitz.h>
#include <blitz/tinyvec2.h>
#include <blitz/tinyvec2io.cc> // fix this - for some reason it doesn't get included through tinyvec2.h
#include <blitz/listinit.h>
#include <blitz/etbase.h>
#include <blitz/array/slice.h>
#include <blitz/indexmap-forward.h>
//#include <blitz/meta/vecassign.h>
//#include <blitz/update.h>

#ifdef BZ_HAVE_BOOST_SERIALIZATION
#include <boost/serialization/serialization.hpp>
#endif


BZ_NAMESPACE(blitz)

/*****************************************************************************
 * Forward declarations
 */

template<typename P_numtype, int N_rows, int N_columns, int N_rows_base, int N_columns_base, bool B_column_major>
class FastTBM2Iterator;
template<typename P_numtype, int N_rows, int N_columns, int N_rows_base, int N_columns_base, bool B_column_major>
class FastTBM2CopyIterator;

template<typename P_expr>
class _bz_ArrayExpr;

template<int N0>
class IndexPlaceholder;

/*****************************************************************************
 * Declaration of class TinyBasedMatrix
 */

template<typename P_numtype, int N_rows, int N_columns, int N_rows_base, int N_columns_base, bool B_column_major>
class TinyBasedMatrix : public ETBase<TinyBasedMatrix<P_numtype, N_rows, N_columns, N_rows_base, N_columns_base, B_column_major> >
{
public:
    typedef P_numtype T_numtype;
    // typedef _bz_tinyMatrixRef<T_numtype, N_rows, N_columns, N_columns, 1> 
    //     T_reference;
    typedef TinyVector<int, 2>  T_index;
    typedef TinyBasedMatrix<T_numtype, N_rows, N_columns, N_rows_base, N_columns_base, B_column_major> T_matrix;
  typedef FastTBM2Iterator<T_numtype,N_rows, N_columns, N_rows_base, N_columns_base, B_column_major> T_iterator;
    typedef T_numtype*                                   iterator;
    typedef const T_numtype*                             const_iterator;
  typedef FastTBM2CopyIterator<P_numtype, N_rows, N_columns, N_rows_base, N_columns_base, B_column_major> T_range_result;


  static constexpr int n_rank_0_order_index = B_column_major ? 0 : 1;
  static constexpr int n_rank_1_order_index = B_column_major ? 1 : 0;

  static constexpr int n_rank_0_stride = B_column_major ? 1 : N_columns;
  static constexpr int n_rank_1_stride = B_column_major ? N_rows : 1;
  static constexpr int zero_offset = -n_rank_0_stride*N_rows_base - n_rank_1_stride*N_columns_base;

  static constexpr int
    //numArrayOperands = 1, 
    //numIndexPlaceholders = 0,
        rank_ = 2;


  static constexpr int n_length_major = B_column_major ? N_rows : N_columns; 
  static constexpr int n_length_minor = B_column_major ? N_columns : N_rows;

  static constexpr int n_base_major = B_column_major ? N_rows_base : N_columns_base;
  static constexpr int n_base_minor = B_column_major ? N_columns_base : N_rows_base;


  TinyBasedMatrix() = default; // defaulting enables zero-initialization of aggregates
  //TinyBasedMatrix() { }

  inline TinyBasedMatrix(const T_matrix&);

  template <typename T_numtype2>
  inline TinyBasedMatrix(const TinyBasedMatrix<T_numtype2, N_rows, N_columns, N_rows_base, N_columns_base, B_column_major>& x);
  
  inline TinyBasedMatrix(T_numtype initValue);
  

  // we need a const AND non-const version to avoid template ctors from jumping in
  inline TinyBasedMatrix(const T_numtype(&x)[N_rows * N_columns]) {
      memcpy(data_, x, N_rows * N_columns * sizeof(T_numtype));
  }

  inline TinyBasedMatrix(T_numtype(&x)[N_rows * N_columns]) {
      memcpy(data_, x, N_rows * N_columns * sizeof(T_numtype));
  }



  static TinyVector<int, 2>    base() 
  {
      return{N_rows_base, N_columns_base};
  }

  static int                               base(int rank) 
  { 
      BZPRECONDITION(rank<2); 
      return rank == 0 
             ? N_rows_base
             : N_columns_base
             ;
  }

    iterator                          begin() 
    { return iterator(*this); }

    const_iterator                    begin() const
    { return const_iterator(*this); }

    T_iterator                        beginFast() const
    { return T_iterator(*this); }

  static int                               cols() 
  { return N_columns; }

  static int                               columns() 
  { return N_columns; }

    const T_numtype* _bz_restrict         data() const
    { return data_; }

    T_numtype* _bz_restrict               data() 
    { return data_; }
    
    const T_numtype* _bz_restrict         dataFirst() const
    { return data_; }

    T_numtype* _bz_restrict               dataFirst()
    { return data_; }

  static int                               dimensions() 
    { return 2; }

  static RectDomain<2>                domain();

    iterator                          end()
  { return data_+size(); }

    const_iterator                    end() const
  { return data_+size(); }

  static int                               extent(int rank) 
  { BZPRECONDITION(rank<2); return rank==0 ? N_rows : N_columns; }

  static const TinyVector<int,2>     extent() 
  { return TinyVector<int,2>(N_rows, N_columns); }

  static bool                              isMajorRank(int rank) 
  { BZPRECONDITION(rank<2); return rank == n_rank_1_order_index; }
  static bool                              isMinorRank(int rank)
  { BZPRECONDITION(rank<2); return rank == n_rank_0_order_index;; }
  static bool                              isRankStoredAscending(int rank) 
  { BZPRECONDITION(rank<2); return true; }

  static int                               lbound(int rank) 
  { BZPRECONDITION(rank<2); return base(rank); }
  static TinyVector<int,2>            lbound() 
  { return base(); }

  static int                               length(int rank) 
  { BZPRECONDITION(rank<2); return rank==0 ? N_rows : N_columns; }
  static TinyVector<int, 2>    length() 
  { return TinyVector<int,2>(N_rows, N_columns); }

  static sizeType                               numElements() 
  { return N_rows*N_columns; }

  static int                               ordering(int rank) 
  { BZPRECONDITION(rank<2); return B_column_major ? rank : 1-rank; }

  static TinyVector<int, 2>    ordering() 
  {
      return TinyVector<int, 2>(n_rank_0_order_index, n_rank_1_order_index);
  }

  static int                               rank()
    { return rank_; }

  static int                               rows() 
    { return N_rows; }

  static TinyVector<int, 2>    shape() 
  { return length(); }

  static sizeType                               size() 
    { return numElements(); }

  static TinyVector<diffType, 2>    stride() 
  { 
      return TinyVector<int, 2>(n_rank_0_stride, n_rank_1_stride);
  }

  static diffType                               stride(int rank) 
  { BZPRECONDITION(rank<2); 
    return rank == 0 ? n_rank_0_stride : n_rank_1_stride;
  }

  static int                               ubound(int rank) 
    { return length(rank) - 1 + lbound(rank); }

  static TinyVector<int, 2>           ubound() 
  { return return TinyVector<int, 2>(
      N_rows-1 + N_rows_base, 
      N_columns-1 + N_columns_base
      ); 
  }

  static int                               zeroOffset() 
  { 
      return -n_rank_0_stride*N_rows_base - n_rank_1_stride*N_columns_base;
  }

    //////////////////////////////////////////////
    // Debugging routines
    //////////////////////////////////////////////

    bool isInRange(const T_index& index) const {
        for (int i=0; i < rank_; ++i)
	  if (index[i] < base(i) || (index[i] - base(i)) >= length(i))
                return false;

        return true;
    }

    bool isInRange(int i0, int i1) const {
      return i0 >= base(0) && (i0 - base(0)) < length(0)
	&& i1 >= base(1) && (i1 - base(1)) < length(1);
    }

    bool assertInRange(const T_index& BZ_DEBUG_PARAM(index)) const {
        BZPRECHECK(isInRange(index), "TinyBasedMatrix index out of range: " << index
            << endl << "Lower bounds: " << base() << endl
		   <<         "Length:       " << length() << endl);
        return true;
    }

    bool assertInRange(int BZ_DEBUG_PARAM(i0), int BZ_DEBUG_PARAM(i1)) const {
        BZPRECHECK(isInRange(i0,i1), "TinyBasedMatrix index out of range: (" 
            << i0 << ", " << i1 << ")"
            << endl << "Lower bounds: " << base() << endl
		   <<         "Length:       " << length() << endl);
        return true;
    }

    //////////////////////////////////////////////
    // Subscripting operators
    //////////////////////////////////////////////


    T_matrix& noConst() const
    { return const_cast<T_matrix&>(*this); }

  const T_numtype& _bz_restrict operator()(unsigned i, unsigned j) const
    {
      assertInRange(i,j);
      return data_[zero_offset + i*n_rank_0_stride + j*n_rank_1_stride];
    }

  T_numtype& _bz_restrict operator()(unsigned i, unsigned j)
    {
      assertInRange(i,j);
      return data_[zero_offset + i*n_rank_0_stride + j*n_rank_1_stride];
    }

  const T_numtype& _bz_restrict operator()(T_index i) const
    {
      assertInRange(i);
      return data_[zero_offset + i[0]* n_rank_0_stride + i[1] * n_rank_1_stride];
    }

  T_numtype& _bz_restrict operator()(T_index i)
    {
      assertInRange(i);
      return data_[zero_offset + i[0] * n_rank_0_stride + i[1] * n_rank_1_stride];
  }

    template<int N0, int N1>
    inline 
    _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_matrix>::T_expr, N0, N1> >
    operator()(IndexPlaceholder<N0>, IndexPlaceholder<N1>) const;


    Array<T_numtype, 2, array_policy::GeneralPolicy> operator()(Range r0, Range r1) const
    {
        GeneralArrayStorage<2> storage;
        if(B_column_major)
        {
            storage.ordering() = {0,1};
        }
        storage.base() = TinyVector<int, 2>{N_rows_base, N_columns_base};
        auto A = Array<T_numtype, 2, array_policy::GeneralPolicy>{(T_numtype*)&(data_),
            blitz::shape(N_rows, N_columns),
            neverDeleteData,
            storage
        };
        A.reindexSelf({N_rows_base, N_columns_base});

        return A(r0, r1);
        //return Array<T_numtype, 2>{(T_numtype*)&(data_[zero_offset + r0.first(N_rows_base) * n_rank_0_stride + r1.first(N_columns_base) * n_rank_1_stride]),
        //    blitz::shape(
        //        r0.last(N_rows + N_rows_base - 1) - r0.first(N_rows_base) + 1,
        //        r1.last(N_columns + N_columns_base - 1) - r1.first(N_columns_base) + 1
        //    
        //    ),
        //    neverDeleteData};
        ////      return Array<T_numtype,1>(noConst(), r0);
    }

    Array<T_numtype, 1, array_policy::GeneralPolicy> operator()(unsigned i, Range r1) const
    {
        GeneralArrayStorage<2> storage;
        if(B_column_major)
        {
            storage.ordering() = {0,1};
        }
        storage.base() = TinyVector<int, 2>{N_rows_base, N_columns_base};
        auto A = Array<T_numtype, 2, array_policy::GeneralPolicy>{(T_numtype*)&(data_),
            blitz::shape(N_rows, N_columns),
            neverDeleteData, 
            storage
        };
        A.reindexSelf({N_rows_base, N_columns_base});

        return A(i, r1);
    }

    Array<T_numtype, 1, array_policy::GeneralPolicy> operator()(Range r0, unsigned j) const
    {
        GeneralArrayStorage<2> storage;
        if(B_column_major)
        {
            storage.ordering() = {0,1};
        }
        storage.base() = TinyVector<int, 2>{N_rows_base, N_columns_base};
        auto A = Array<T_numtype, 2, array_policy::GeneralPolicy>{(T_numtype*)&(data_),
            blitz::shape(N_rows, N_columns),
            neverDeleteData,
            storage
        };
        A.reindexSelf({N_rows_base, N_columns_base});

        return A(r0, j);
    }

    static constexpr int make_lower_mjor(int n){
        return n == INT_MIN ? n_base_major : n;
    }
    static constexpr int make_upper_mjor(int n){
        return n == INT_MAX ? n_base_major + n_length_major - 1 : n;
    }

    template<int Lower, int Upper>
    TinyBasedVector<T_numtype, make_upper_mjor(Upper) - make_lower_mjor(Lower) + 1, n_base_major>&
        static_major_range(unsigned j) const
    {
        return reinterpret_cast<TinyBasedVector<T_numtype, make_upper_mjor(Upper) - make_lower_mjor(Lower) + 1, n_base_major>&>(
            const_cast<T_numtype&>(data_[make_lower_mjor(Lower) - n_base_major + n_length_major * (j - n_base_minor)])
            );
    }

    TinyBasedVector<T_numtype, n_length_major, n_base_major>&
        static_major_slice(unsigned j) const
    {
        return reinterpret_cast<TinyBasedVector<T_numtype, n_length_major, n_base_major>&>(
            const_cast<T_numtype&>(data_[n_length_major * (j - n_base_minor)])
            );
    }



    // 0-based access
    typedef T_numtype T_array_major[n_length_major];
    const T_array_major& operator[](diffType i) const
    {
    //    BZPRECONDITION(lengthCheck(static_cast<int>(i + N_base)));
        return (const T_array_major&)*(&data_[i*n_length_major]);
    }

    // 0-based access
    T_array_major & _bz_restrict operator[](diffType i)
    {
    //    BZPRECONDITION(lengthCheck(static_cast<int>(i + N_base)));
        return (T_array_major&)*(&data_[i*n_length_major]);
    }



  /** Fastread must return reference so the iterator can turn it into an
      iterator for the contained in case we have a multicomponent. */
  const T_numtype& fastRead(diffType i) const
  { return data_[i]; }
  
  /** Since data_ is simd aligned by construction, we just have
      to check the offest. */
  bool isVectorAligned(diffType offset) const 
  { return (offset%simdTypes<T_numtype>::vecWidth)==0; }

    // T_reference getRef()
    // { return T_reference((T_numtype*)data_); }

    // const T_reference getRef() const
    // { return T_reference((T_numtype*)data_); }

    //////////////////////////////////////////////
    // Assignment Operators
    //////////////////////////////////////////////

    // Scalar operand
#ifdef BLITZ_USE_LISTINITIALIZATION
    ListInitializationSwitch<T_matrix,T_numtype*>
    operator=(T_numtype x)
    {
        return ListInitializationSwitch<T_matrix,T_numtype*>(*this, x);
    }
#else
  void
      operator=(T_numtype x) {
      initialize(x);
  }
#endif

  T_matrix& initialize(T_numtype x);

    T_numtype* _bz_restrict getInitializationIterator()
    { return dataFirst(); }

    template<typename T_expr>
    T_matrix& operator=(const ETBase<T_expr>&);

    template<typename T> T_matrix& operator+=(const T&);
    template<typename T> T_matrix& operator-=(const T&);
    template<typename T> T_matrix& operator*=(const T&);
    template<typename T> T_matrix& operator/=(const T&);
    template<typename T> T_matrix& operator%=(const T&);
    template<typename T> T_matrix& operator^=(const T&);
    template<typename T> T_matrix& operator&=(const T&);
    template<typename T> T_matrix& operator|=(const T&);
    template<typename T> T_matrix& operator>>=(const T&);
    template<typename T> T_matrix& operator<<=(const T&);

  static bool canCollapse(int outerRank, int innerRank) { 
#ifdef BZ_DEBUG_TRAVERSE
        BZ_DEBUG_MESSAGE("stride(" << innerRank << ")=" << stride(innerRank)
          << ", extent()=" << extent(innerRank) << ", stride(outerRank)="
          << stride(outerRank));
#endif
        return (stride(innerRank) * extent(innerRank) == stride(outerRank)); 
    }


private:
  template<typename T_expr, typename T_update>
  void _tbm_evaluate(const T_expr& expr, T_update);

#ifdef BZ_HAVE_BOOST_SERIALIZATION
  friend class boost::serialization::access;

  template<class T_arch>
  void serialize(T_arch& ar, const unsigned int version) {
    ar & data_;
  };
#endif
  
  BZ_ALIGN_VARIABLE(T_numtype, data_[N_rows * N_columns], BZ_SIMD_WIDTH)
};

BZ_NAMESPACE_END

#ifdef BZ_HAVE_BOOST_SERIALIZATION
namespace boost {
  namespace mpi {
    template<typename T> struct is_mpi_datatype;
    template <typename T, int N, int M>
    struct is_mpi_datatype<blitz::TinyBasedMatrix<T, N, M> > 
      : public is_mpi_datatype<T> { };
  } };
#endif

#include <blitz/extension/tinybasedmat2.cc>

#endif // BZ_TINYBASEDMAT_H

