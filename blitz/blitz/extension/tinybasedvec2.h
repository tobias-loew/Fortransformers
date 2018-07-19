// -*- C++ -*-
/***************************************************************************
 * blitz/tinyvec.h      Declaration of the TinyBasedVector<T, N> class
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

#ifndef BZ_TINYBASEDVEC_H
#define BZ_TINYBASEDVEC_H

#include <blitz/blitz.h>
#include <blitz/listinit.h>
#include <blitz/et-forward.h>
#include <blitz/etbase.h>
#include <blitz/simdtypes.h>
#include <blitz/array/slice.h>

#ifdef BZ_HAVE_BOOST_SERIALIZATION
#include <boost/serialization/serialization.hpp>
#endif
#ifdef BZ_HAVE_BOOST_MPI
#include <boost/mpi/datatype.hpp>
#endif

#ifdef BZ_HAVE_CSTRING
#include <cstring> // For memcpy
#endif

BZ_NAMESPACE(blitz)

/*****************************************************************************
 * Forward declarations
 */

template<typename P_numtype, int N_length, int N_base>
class FastTBV2Iterator;
template<typename P_numtype, int N_length, int N_base>
class FastTBV2CopyIterator;

struct tag_zero_init {};

/** The TinyBasedVector class is a one-dimensional, fixed length vector
    that implements the blitz expression template
    machinery. TinyBasedVector-only expressions are very fast because they
    usually get reduced to just the unrolled (and vectorized, if
    enabled) assembly instructions. TinyVectors can also be used in
    mixed expressions with other ET classes. */
template<typename P_numtype, int N_length, int N_base>
class TinyBasedVector : public ETBase<TinyBasedVector<P_numtype, N_length, N_base> >
{
public:

    //////////////////////////////////////////////
    // Public Types
    //////////////////////////////////////////////

    typedef P_numtype                                    T_numtype;
    typedef TinyBasedVector<T_numtype,N_length, N_base>               T_vector;
    typedef FastTBV2Iterator<T_numtype,N_length, N_base>         T_iterator;
    typedef T_numtype*                                   iterator;
    typedef const T_numtype*                             const_iterator;
  typedef FastTBV2CopyIterator<P_numtype, N_length, N_base> T_range_result;

  static constexpr int n_length = N_length;
  static constexpr int n_base = N_base;

    static constexpr int 
    //numArrayOperands = 1, 
    //numIndexPlaceholders = 0,
        rank_ = 1;

    // do not mark constexpr as it forces ru-time arrays to be zero-initialized
    TinyBasedVector() = default; // defaulting enables zero-initialization of aggregates
                                           //    TinyBasedVector()  { }
//    ~TinyBasedVector() { }
    constexpr TinyBasedVector(tag_zero_init) : data_{}
    {}

#if _MSC_VER > 1900
//#error check
//    template<typename ...Args>
//    constexpr TinyBasedVector(Args&& ...args) : data_{std::forward<Args>(args)...} {}
#endif
    constexpr TinyBasedVector(std::initializer_list<T_numtype> list) : TinyBasedVector(list, std::make_index_sequence<n_length>())
    {
    }


private:
    template<size_t...Is>
    constexpr TinyBasedVector(std::initializer_list<T_numtype> list, std::integer_sequence<size_t, Is...>)
        : data_{
        (
#if _MSVC_LANG > 201403
            Is >= list.size()
            ?
            T_numtype()
            :
			std::move(list.begin()[Is])
			//std::move(*(std::next(list.begin(), Is)))
#else
            std::move(list.begin()[Is])
#endif
            )...
    } {}

public:

  TinyBasedVector(const TinyBasedVector<T_numtype,N_length, N_base>& x);

    template <typename T_numtype2>
    TinyBasedVector(const TinyBasedVector<T_numtype2,N_length, N_base>& x);

  /** This constructor creates a TinyBasedVector from another ETBase
      object. It needs to be explicit to avoid all kinds of
      ambiguities. */
    template <typename T_expr>
    inline explicit TinyBasedVector(const ETBase<T_expr>& expr) {
      *this = expr; }

  /** This constructor creates a TinyBasedVector specifically from an
      expression. This one we do NOT want to be explicit because that
      breaks simple construction assignments like "TinyBasedVector<double,
      1> v = a+b;", forcing the user to explicitly write it like a
      construction. */
    template <typename T_expr>
    inline TinyBasedVector(const _bz_ArrayExpr<T_expr>& expr) {
      *this = expr; }

    inline TinyBasedVector(const T_numtype initValue);

    // we need a const AND non-const version to avoid template ctors from jumping in
    inline TinyBasedVector(const T_numtype(&x)[N_length]) {
        memcpy(data_, x, N_length * sizeof(T_numtype));
    }

    inline TinyBasedVector(T_numtype(&x)[N_length]) {
        memcpy(data_, x, N_length * sizeof(T_numtype));
    }


    TinyBasedVector(T_numtype x0, T_numtype x1)
    {
        data_[0] = x0;
        data_[1] = x1;
    }

    TinyBasedVector(T_numtype x0, T_numtype x1, T_numtype x2)
    {
        data_[0] = x0;
        data_[1] = x1;
        data_[2] = x2;
    }

    TinyBasedVector(T_numtype x0, T_numtype x1, T_numtype x2,
        T_numtype x3)
    {
        data_[0] = x0;
        data_[1] = x1;
        data_[2] = x2;
        data_[3] = x3;
    }

    TinyBasedVector(T_numtype x0, T_numtype x1, T_numtype x2,
        T_numtype x3, T_numtype x4)
    {
        data_[0] = x0;
        data_[1] = x1;
        data_[2] = x2;
        data_[3] = x3;
        data_[4] = x4;
    }

    TinyBasedVector(T_numtype x0, T_numtype x1, T_numtype x2,
        T_numtype x3, T_numtype x4, T_numtype x5)
    {
        data_[0] = x0;
        data_[1] = x1;
        data_[2] = x2;
        data_[3] = x3;
        data_[4] = x4;
        data_[5] = x5;
    }

    TinyBasedVector(T_numtype x0, T_numtype x1, T_numtype x2,
        T_numtype x3, T_numtype x4, T_numtype x5, T_numtype x6)
    {
        data_[0] = x0;
        data_[1] = x1;
        data_[2] = x2;
        data_[3] = x3;
        data_[4] = x4;
        data_[5] = x5;
        data_[6] = x6;
    }

    TinyBasedVector(T_numtype x0, T_numtype x1, T_numtype x2,
        T_numtype x3, T_numtype x4, T_numtype x5, T_numtype x6,
        T_numtype x7)
    {
        data_[0] = x0;
        data_[1] = x1;
        data_[2] = x2;
        data_[3] = x3;
        data_[4] = x4;
        data_[5] = x5;
        data_[6] = x6;
        data_[7] = x7;
    }

    TinyBasedVector(T_numtype x0, T_numtype x1, T_numtype x2,
        T_numtype x3, T_numtype x4, T_numtype x5, T_numtype x6,
        T_numtype x7, T_numtype x8)
    {
        data_[0] = x0;
        data_[1] = x1;
        data_[2] = x2;
        data_[3] = x3;
        data_[4] = x4;
        data_[5] = x5;
        data_[6] = x6;
        data_[7] = x7;
        data_[8] = x8;
    }

    TinyBasedVector(T_numtype x0, T_numtype x1, T_numtype x2,
        T_numtype x3, T_numtype x4, T_numtype x5, T_numtype x6,
        T_numtype x7, T_numtype x8, T_numtype x9)
    {
        data_[0] = x0;
        data_[1] = x1;
        data_[2] = x2;
        data_[3] = x3;
        data_[4] = x4;
        data_[5] = x5;
        data_[6] = x6;
        data_[7] = x7;
        data_[8] = x8;
        data_[9] = x9;
    }

    TinyBasedVector(T_numtype x0, T_numtype x1, T_numtype x2,
        T_numtype x3, T_numtype x4, T_numtype x5, T_numtype x6,
        T_numtype x7, T_numtype x8, T_numtype x9, T_numtype x10)
    {
        data_[0] = x0;
        data_[1] = x1;
        data_[2] = x2;
        data_[3] = x3;
        data_[4] = x4;
        data_[5] = x5;
        data_[6] = x6;
        data_[7] = x7;
        data_[8] = x8;
        data_[9] = x9;
        data_[10] = x10;
    }

    constexpr static int base()
  { return N_base; }

    constexpr static int                               base(int rank)
  { BZPRECONDITION(rank==0); return N_base; }


    constexpr T_iterator      beginFast() const       { return T_iterator(*this);      }

    iterator       begin()       { return data_; }
    constexpr const_iterator begin() const { return data_; }

    constexpr static int                               dimensions()
    { return 1; }

  iterator       end()       { return data_ + N_length; }
  constexpr const_iterator end() const { return data_ + N_length; }

  T_numtype * _bz_restrict data()
    { return data_; }

  constexpr const T_numtype * _bz_restrict data() const
    { return data_; }

  T_numtype * _bz_restrict dataFirst()
    { return data_; }

  constexpr const T_numtype * _bz_restrict dataFirst() const
    { return data_; }

  constexpr const TinyVector<int, rank_>    shape() const
    { return N_length; }

  constexpr static int                               lbound(int rank)
  { BZPRECONDITION(rank==0); return N_base; }
  constexpr static int            lbound()
  { return N_base; }

  constexpr static int                               length(int rank)
  { BZPRECONDITION(rank==0); return N_length; }
  constexpr static int    length()
  { return N_length; }

  constexpr static int                               extent(int rank)
  { BZPRECONDITION(rank==0); return N_length; }

  constexpr static int                               ordering(int storageRankIndex)
  { return 0; }

  constexpr static int    ordering()
  { return 0; }

  constexpr static  int                               rank()
    { return rank_; }

  constexpr static sizeType                               numElements()
  { return length(); }

  constexpr static diffType    stride()
    { return 1; }

  constexpr static diffType                               stride(int rank)
    { BZPRECONDITION(rank==0); return 1; }

  constexpr static int                               ubound(int rank)
  { BZPRECONDITION(rank==0); return length()-1 + N_base; }

  constexpr static int           ubound()
  { return length()-1 + N_base; }

     template<typename P_expr, typename P_updater>
     void _bz_assign(P_expr, P_updater);

     constexpr T_numtype operator*() const
    { return *data_; }

    //////////////////////////////////////////////
    // Subscripting operators
    //////////////////////////////////////////////

     constexpr T_vector& noConst() const
    { return const_cast<T_vector&>(*this); }

     constexpr static bool lengthCheck(int i)
    {
         BZ_CONSTEXPR_PRECHECK(i >= N_base && i < N_length + N_base,
            "TinyBasedVector<" << BZ_DEBUG_TEMPLATE_AS_STRING_LITERAL(T_numtype) 
            << "," << N_length << "," << N_base << "> index out of bounds: " << i);

         return true;
    }

  constexpr const T_numtype& operator()(int i) const
    {
      BZPRECONDITION(lengthCheck(i));
      return data_[i - N_base];
    }

  T_numtype& _bz_restrict operator()(int i)
    { 
      BZPRECONDITION(lengthCheck(i));
        return data_[i - N_base];
    }

  constexpr T_numtype operator()(TinyVector<int,1> i) const
    {
      BZPRECONDITION(lengthCheck(i[0]));
        return data_[i[0] - N_base];
    }

  Array<T_numtype, 1, array_policy::GeneralPolicy> op_range_orig(Range r0) const {
      GeneralArrayStorage<1> storage;
      storage.base() = TinyVector<int, 1>{N_base};
      return Array<T_numtype, 1, array_policy::GeneralPolicy>{(T_numtype*)&(data_[r0.first(N_base) - N_base]),
          blitz::shape(r0.last(N_length + N_base - 1) - r0.first(N_base) + 1),
          neverDeleteData,
          storage
      };
      //      return Array<T_numtype,1>(noConst(), r0);
  }

  Array<T_numtype, 1, array_policy::GeneralPolicy> operator()(Stride s0) const {
      auto ar = op_range_orig(Range());
      auto&& r0 = s0.get_range();
      return Array<T_numtype, 1, array_policy::GeneralPolicy>(ar.noConst(), r0);
      //GeneralArrayStorage<1> storage;
      //storage.base() = TinyVector<int, 1>{N_base};
      //return Array<T_numtype, 1, array_policy::GeneralPolicy>{(T_numtype*)&(data_[r0.first(N_base) - N_base]),
      //    blitz::shape(r0.last(N_length + N_base - 1) - r0.first(N_base) + 1),
      //    neverDeleteData,
      //    storage
      //};
  }

  Array<T_numtype, 1, array_policy::StaticFortranArrayPolicy<N_base>> operator()(Range r0) const
  {
      return Array<T_numtype, 1, array_policy::StaticFortranArrayPolicy<N_base>>{
          array_policy::tag_staticfortranarraypolicy{},
              (T_numtype*)&(data_[r0.first(N_base) - N_base]),
              blitz::shape(r0.last(N_length + N_base - 1) - r0.first(N_base) + 1)
      };
  }

  static constexpr int make_lower(int n){
      return n == INT_MIN ? N_base : n;
  }
  static constexpr int make_upper(int n){
      return n == INT_MAX ? N_base + N_length - 1 : n;
  }
  template<int Lower = INT_MIN, int Upper = INT_MAX>
  constexpr TinyBasedVector<T_numtype, make_upper(Upper) - make_lower(Lower) + 1, N_base>& static_range() const
  {
      return reinterpret_cast<TinyBasedVector<T_numtype, make_upper(Upper) - make_lower(Lower) + 1, N_base>&>(
          const_cast<T_numtype&>(data_[make_lower(Lower) - N_base])
          );
  }



    template<int N0>
    _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_vector>::T_expr, N0> >
    operator()(IndexPlaceholder<N0>) const;


    /* Operator added by Tobias Loew */
    template<typename T>
    constexpr auto operator()(const ETBase<T>& indexer) const
    {
        return make_indexing_functor(const_cast<ETBase<T>&>(indexer).unwrap())(*this);
    }


    // 0-based access
    constexpr const T_numtype& operator[](diffType i) const
    {
        BZPRECONDITION(lengthCheck(static_cast<int>(i + N_base)));
        return data_[i];
    }

    // 0-based access
    T_numtype& _bz_restrict operator[](diffType i)
    {
        BZPRECONDITION(lengthCheck(static_cast<int>(i + N_base)));
        return data_[i];
    }

  // must return reference so the iterator can turn it into an
  // iterator for the contained in case we have a multicomponent.
    constexpr const T_numtype& fastRead(diffType i) const
    { return data_[i]; }

  /** Since data_ is simd aligned by construction, we just have
      to check the offest. */
    constexpr bool isVectorAligned(diffType offset) const
  { return (offset%simdTypes<T_numtype>::vecWidth)==0; }

    constexpr bool canCollapse(int outerLoopRank, int innerLoopRank) const
  {
        BZPRECONDITION(outerLoopRank==0);
    BZPRECONDITION(innerLoopRank==0);
    return true;
  }

    //////////////////////////////////////////////
    // Assignment operators
    //////////////////////////////////////////////

    // Scalar operand
#ifdef BLITZ_USE_LISTINITIALIZATION
    ListInitializationSwitch<T_vector, T_numtype*> operator=(T_numtype x) {
        return ListInitializationSwitch<T_vector, T_numtype*>(*this, x);
    }
#else
    void
        operator=(T_numtype x) {
        initialize(x);
    }
#endif

  T_vector& initialize(T_numtype);

    template<typename T_expr>
    T_vector& operator=(const ETBase<T_expr>&);

    template<typename T> T_vector& operator+=(const T&);
    template<typename T> T_vector& operator-=(const T&);
    template<typename T> T_vector& operator*=(const T&);
    template<typename T> T_vector& operator/=(const T&);
    template<typename T> T_vector& operator%=(const T&);
    template<typename T> T_vector& operator^=(const T&);
    template<typename T> T_vector& operator&=(const T&);
    template<typename T> T_vector& operator|=(const T&);
    template<typename T> T_vector& operator>>=(const T&);
    template<typename T> T_vector& operator<<=(const T&);

    T_numtype* _bz_restrict getInitializationIterator()
    { return dataFirst(); }

  // // vectors can't be sliced
  // template<typename T1, typename T2 = nilArraySection, 
  // 	   class T3 = nilArraySection, typename T4 = nilArraySection, 
  // 	   class T5 = nilArraySection, typename T6 = nilArraySection, 
  // 	   class T7 = nilArraySection, typename T8 = nilArraySection, 
  // 	   class T9 = nilArraySection, typename T10 = nilArraySection, 
  // 	   class T11 = nilArraySection>
  // class SliceInfo {
  // public:    
  //   typedef void T_slice;
  // };

private:
  template<typename T_expr, typename T_update>
  void _tbv_evaluate(const T_expr& expr, T_update);

#ifdef BZ_HAVE_BOOST_SERIALIZATION
  friend class boost::serialization::access;
  
  template<class T_arch>
  void serialize(T_arch& ar, const unsigned int version) {
    ar & data_;
  };
#endif


  BZ_ALIGN_VARIABLE(T_numtype, data_[N_length], BZ_SIMD_WIDTH)
};

// Specialization for N = 0: KCC is giving some
// peculiar errors, perhaps this will fix.
template<typename P_numtype, int N_base>
class TinyBasedVector<P_numtype, 0, N_base> : public ETBase<TinyBasedVector<P_numtype, 0, N_base> >
{
public:
    static constexpr int N_length = 0;

    //////////////////////////////////////////////
    // Public Types
    //////////////////////////////////////////////

    typedef P_numtype                                    T_numtype;
    typedef TinyBasedVector<T_numtype,N_length, N_base>               T_vector;
    typedef FastTBV2Iterator<T_numtype,N_length, N_base>         T_iterator;
    typedef T_numtype*                                   iterator;
    typedef const T_numtype*                             const_iterator;
  typedef FastTBV2CopyIterator<P_numtype, N_length, N_base> T_range_result;

  static constexpr int n_length = N_length;
  static constexpr int n_base = N_base;

    static constexpr int 
    //numArrayOperands = 1, 
    //numIndexPlaceholders = 0,
        rank_ = 1;

    constexpr TinyBasedVector() = default; // defaulting enables zero-length constexprs
                                           //    TinyBasedVector()  { }
//    ~TinyBasedVector() { }

#if _MSC_VER > 1900
//#error check
//    template<typename ...Args>
//    constexpr TinyBasedVector(Args&& ...args) : data_{std::forward<Args>(args)...} {}
#endif
    constexpr TinyBasedVector(std::initializer_list<T_numtype> list) : TinyBasedVector(list, std::make_index_sequence<n_length>())
    {
    }


private:
    template<size_t...Is>
    constexpr TinyBasedVector(std::initializer_list<T_numtype> list, std::integer_sequence<size_t, Is...>)
//        : data_{
//        (
//#if _MSVC_LANG > 201403
//            Is >= list.size()
//            ?
//            T_numtype()
//            :
//            std::move(*(std::next(list.begin(), Is)))
//#else
//            std::move(list.begin()[Is])
//#endif
//            )...
//    } 
    {}

public:

    TinyBasedVector(const TinyBasedVector<T_numtype, N_length, N_base>& x) {}

    template <typename T_numtype2>
    TinyBasedVector(const TinyBasedVector<T_numtype2, N_length, N_base>& x) {}

  /** This constructor creates a TinyBasedVector from another ETBase
      object. It needs to be explicit to avoid all kinds of
      ambiguities. */
    template <typename T_expr>
    inline explicit TinyBasedVector(const ETBase<T_expr>& expr) {
    //  *this = expr;
    }

  /** This constructor creates a TinyBasedVector specifically from an
      expression. This one we do NOT want to be explicit because that
      breaks simple construction assignments like "TinyBasedVector<double,
      1> v = a+b;", forcing the user to explicitly write it like a
      construction. */
    template <typename T_expr>
    inline TinyBasedVector(const _bz_ArrayExpr<T_expr>& expr) {
    //  *this = expr; 
    }

    inline TinyBasedVector(const T_numtype initValue) {}

    // we need a const AND non-const version to avoid template ctors from jumping in
    inline TinyBasedVector(const T_numtype(&x)[N_length]) {
        //memcpy(data_, x, N_length * sizeof(T_numtype));
    }

    inline TinyBasedVector(T_numtype(&x)[N_length]) {
        //memcpy(data_, x, N_length * sizeof(T_numtype));
    }


    //TinyBasedVector(T_numtype x0, T_numtype x1)
    //{
    //    data_[0] = x0;
    //    data_[1] = x1;
    //}

    //TinyBasedVector(T_numtype x0, T_numtype x1, T_numtype x2)
    //{
    //    data_[0] = x0;
    //    data_[1] = x1;
    //    data_[2] = x2;
    //}

    //TinyBasedVector(T_numtype x0, T_numtype x1, T_numtype x2,
    //    T_numtype x3)
    //{
    //    data_[0] = x0;
    //    data_[1] = x1;
    //    data_[2] = x2;
    //    data_[3] = x3;
    //}

    //TinyBasedVector(T_numtype x0, T_numtype x1, T_numtype x2,
    //    T_numtype x3, T_numtype x4)
    //{
    //    data_[0] = x0;
    //    data_[1] = x1;
    //    data_[2] = x2;
    //    data_[3] = x3;
    //    data_[4] = x4;
    //}

    //TinyBasedVector(T_numtype x0, T_numtype x1, T_numtype x2,
    //    T_numtype x3, T_numtype x4, T_numtype x5)
    //{
    //    data_[0] = x0;
    //    data_[1] = x1;
    //    data_[2] = x2;
    //    data_[3] = x3;
    //    data_[4] = x4;
    //    data_[5] = x5;
    //}

    //TinyBasedVector(T_numtype x0, T_numtype x1, T_numtype x2,
    //    T_numtype x3, T_numtype x4, T_numtype x5, T_numtype x6)
    //{
    //    data_[0] = x0;
    //    data_[1] = x1;
    //    data_[2] = x2;
    //    data_[3] = x3;
    //    data_[4] = x4;
    //    data_[5] = x5;
    //    data_[6] = x6;
    //}

    //TinyBasedVector(T_numtype x0, T_numtype x1, T_numtype x2,
    //    T_numtype x3, T_numtype x4, T_numtype x5, T_numtype x6,
    //    T_numtype x7)
    //{
    //    data_[0] = x0;
    //    data_[1] = x1;
    //    data_[2] = x2;
    //    data_[3] = x3;
    //    data_[4] = x4;
    //    data_[5] = x5;
    //    data_[6] = x6;
    //    data_[7] = x7;
    //}

    //TinyBasedVector(T_numtype x0, T_numtype x1, T_numtype x2,
    //    T_numtype x3, T_numtype x4, T_numtype x5, T_numtype x6,
    //    T_numtype x7, T_numtype x8)
    //{
    //    data_[0] = x0;
    //    data_[1] = x1;
    //    data_[2] = x2;
    //    data_[3] = x3;
    //    data_[4] = x4;
    //    data_[5] = x5;
    //    data_[6] = x6;
    //    data_[7] = x7;
    //    data_[8] = x8;
    //}

    //TinyBasedVector(T_numtype x0, T_numtype x1, T_numtype x2,
    //    T_numtype x3, T_numtype x4, T_numtype x5, T_numtype x6,
    //    T_numtype x7, T_numtype x8, T_numtype x9)
    //{
    //    data_[0] = x0;
    //    data_[1] = x1;
    //    data_[2] = x2;
    //    data_[3] = x3;
    //    data_[4] = x4;
    //    data_[5] = x5;
    //    data_[6] = x6;
    //    data_[7] = x7;
    //    data_[8] = x8;
    //    data_[9] = x9;
    //}

    //TinyBasedVector(T_numtype x0, T_numtype x1, T_numtype x2,
    //    T_numtype x3, T_numtype x4, T_numtype x5, T_numtype x6,
    //    T_numtype x7, T_numtype x8, T_numtype x9, T_numtype x10)
    //{
    //    data_[0] = x0;
    //    data_[1] = x1;
    //    data_[2] = x2;
    //    data_[3] = x3;
    //    data_[4] = x4;
    //    data_[5] = x5;
    //    data_[6] = x6;
    //    data_[7] = x7;
    //    data_[8] = x8;
    //    data_[9] = x9;
    //    data_[10] = x10;
    //}

    constexpr static int base()
  { return N_base; }

    constexpr static int                               base(int rank)
  { BZPRECONDITION(rank==0); return N_base; }


    constexpr T_iterator      beginFast() const       { return T_iterator(*this);      }

    iterator       begin()       { return nullptr; }
    constexpr const_iterator begin() const { return nullptr; }

    constexpr static int                               dimensions()
    { return 1; }

  iterator       end()       { return nullptr; }
  constexpr const_iterator end() const { return nullptr; }

  T_numtype * _bz_restrict data()
    { return nullptr; }

  constexpr const T_numtype * _bz_restrict data() const
    { return nullptr; }

  T_numtype * _bz_restrict dataFirst()
    { return nullptr; }

  constexpr const T_numtype * _bz_restrict dataFirst() const
    { return nullptr; }

  constexpr const TinyVector<int, rank_>    shape() const
    { return N_length; }

  constexpr static int                               lbound(int rank)
  { BZPRECONDITION(rank==0); return N_base; }
  constexpr static int            lbound()
  { return N_base; }

  constexpr static int                               length(int rank)
  { BZPRECONDITION(rank==0); return N_length; }
  constexpr static int    length()
  { return N_length; }

  constexpr static int                               extent(int rank)
  { BZPRECONDITION(rank==0); return N_length; }

  constexpr static int                               ordering(int storageRankIndex)
  { return 0; }

  constexpr static int    ordering()
  { return 0; }

  constexpr static  int                               rank()
    { return rank_; }

  constexpr static sizeType                               numElements()
  { return length(); }

  constexpr static diffType    stride()
    { return 1; }

  constexpr static diffType                               stride(int rank)
    { BZPRECONDITION(rank==0); return 1; }

  constexpr static int                               ubound(int rank)
  { BZPRECONDITION(rank==0); return length()-1 + N_base; }

  constexpr static int           ubound()
  { return length()-1 + N_base; }

     template<typename P_expr, typename P_updater>
     void _bz_assign(P_expr, P_updater) {}

     constexpr T_numtype operator*() const
     {
         return T_numtype{};
     }

    //////////////////////////////////////////////
    // Subscripting operators
    //////////////////////////////////////////////

     constexpr T_vector& noConst() const
    { return const_cast<T_vector&>(*this); }

     constexpr static bool lengthCheck(int i)
    {
         BZ_CONSTEXPR_PRECHECK(i >= N_base && i < N_length + N_base,
            "TinyBasedVector<" << BZ_DEBUG_TEMPLATE_AS_STRING_LITERAL(T_numtype) 
            << "," << N_length << "," << N_base << "> index out of bounds: " << i);
        return true;
    }

     static constexpr T_numtype sdata{};
     constexpr const T_numtype& operator()(int i) const
    {
      BZPRECONDITION(lengthCheck(i));
      return sdata;
  }

  T_numtype& _bz_restrict operator()(int i)
    { 
      BZPRECONDITION(lengthCheck(i));
      static T_numtype data{};
        return data;
    }

  constexpr T_numtype operator()(TinyVector<int,1> i) const
    {
      BZPRECONDITION(lengthCheck(i[0]));
      return T_numtype{};
    }


  Array<T_numtype, 1, array_policy::GeneralPolicy> operator()(Stride s0) const {
      auto&& r0 = s0.get_range();
      return Array<T_numtype, 1, array_policy::GeneralPolicy>(noConst(), r0);
      //GeneralArrayStorage<1> storage;
      //storage.base() = TinyVector<int, 1>{ N_base };
      //return Array<T_numtype, 1, array_policy::GeneralPolicy>{(T_numtype*)&(data_[r0.first(N_base) - N_base]),
      //    blitz::shape(r0.last(N_length + N_base - 1) - r0.first(N_base) + 1),
      //    neverDeleteData,
      //    storage
      //};
  }

  Array<T_numtype, 1, array_policy::StaticFortranArrayPolicy<N_base>> operator()(Range r0) const
  {
      return Array<T_numtype, 1, array_policy::StaticFortranArrayPolicy<N_base>>{
          array_policy::tag_staticfortranarraypolicy{},
              (T_numtype*)&(*this),
              blitz::shape(r0.last(N_length + N_base - 1) - r0.first(N_base) + 1)
      };
  }

  static constexpr int make_lower(int n){
      return n == INT_MIN ? N_base : n;
  }
  static constexpr int make_upper(int n){
      return n == INT_MAX ? N_base + N_length - 1 : n;
  }
  template<int Lower = INT_MIN, int Upper = INT_MAX>
  constexpr TinyBasedVector<T_numtype, make_upper(Upper) - make_lower(Lower) + 1, N_base>& static_range() const
  {
      return reinterpret_cast<TinyBasedVector<T_numtype, make_upper(Upper) - make_lower(Lower) + 1, N_base>&>(
          const_cast<T_numtype&>(*this)
          );
  }



    template<int N0>
    _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_vector>::T_expr, N0> >
        operator()(IndexPlaceholder<N0>) const {
        return _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_vector>::T_expr, N0> >
            (noConst());
    }


    /* Operator added by Tobias Loew */
    template<typename T>
    constexpr auto operator()(const ETBase<T>& indexer) const
    {
        return make_indexing_functor(const_cast<ETBase<T>&>(indexer).unwrap())(*this);
    }


    // 0-based access
    constexpr const T_numtype& operator[](diffType i) const
    {
        BZPRECONDITION(lengthCheck(static_cast<int>(i + N_base)));
        return sdata;
    }

    // 0-based access
    T_numtype& _bz_restrict operator[](diffType i)
    {
        BZPRECONDITION(lengthCheck(static_cast<int>(i + N_base)));
        static T_numtype data{};
        return data;
    }

  // must return reference so the iterator can turn it into an
  // iterator for the contained in case we have a multicomponent.
    constexpr const T_numtype& fastRead(diffType i) const
    {
        return sdata;
    }

  /** Since data_ is simd aligned by construction, we just have
      to check the offest. */
    constexpr bool isVectorAligned(diffType offset) const
  { return (offset%simdTypes<T_numtype>::vecWidth)==0; }

    constexpr bool canCollapse(int outerLoopRank, int innerLoopRank) const
  {
        BZPRECONDITION(outerLoopRank==0);
    BZPRECONDITION(innerLoopRank==0);
    return true;
  }

    //////////////////////////////////////////////
    // Assignment operators
    //////////////////////////////////////////////

    // Scalar operand
#ifdef BLITZ_USE_LISTINITIALIZATION
    ListInitializationSwitch<T_vector, T_numtype*> operator=(T_numtype x) {
        return ListInitializationSwitch<T_vector, T_numtype*>(*this, x);
    }
#else
    void
        operator=(T_numtype x) {
        initialize(x);
    }
#endif

  T_vector& initialize(T_numtype x) {
      //(*this) = _bz_ArrayExpr<_bz_ArrayExprConstant<T_numtype> >(x);
      return *this;
  }

    template<typename T_expr>
    T_vector& operator=(const ETBase<T_expr>& expr) {
        //_tbv_evaluate(_bz_typename asExpr<T_expr>::T_expr(expr.unwrap()),
        //    _bz_update<
        //    T_numtype,
        //    _bz_typename asExpr<T_expr>::T_expr::T_result>());
        return *this;
    }

    template<typename T> T_vector& operator+=(const T&);
    template<typename T> T_vector& operator-=(const T&);
    template<typename T> T_vector& operator*=(const T&);
    template<typename T> T_vector& operator/=(const T&);
    template<typename T> T_vector& operator%=(const T&);
    template<typename T> T_vector& operator^=(const T&);
    template<typename T> T_vector& operator&=(const T&);
    template<typename T> T_vector& operator|=(const T&);
    template<typename T> T_vector& operator>>=(const T&);
    template<typename T> T_vector& operator<<=(const T&);

    T_numtype* _bz_restrict getInitializationIterator()
    { return nullptr; }

  // // vectors can't be sliced
  // template<typename T1, typename T2 = nilArraySection, 
  // 	   class T3 = nilArraySection, typename T4 = nilArraySection, 
  // 	   class T5 = nilArraySection, typename T6 = nilArraySection, 
  // 	   class T7 = nilArraySection, typename T8 = nilArraySection, 
  // 	   class T9 = nilArraySection, typename T10 = nilArraySection, 
  // 	   class T11 = nilArraySection>
  // class SliceInfo {
  // public:    
  //   typedef void T_slice;
  // };

private:
  template<typename T_expr, typename T_update>
  void _tbv_evaluate(const T_expr& expr, T_update) {}

#ifdef BZ_HAVE_BOOST_SERIALIZATION
  friend class boost::serialization::access;
  
  template<class T_arch>
  void serialize(T_arch& ar, const unsigned int version) {
//    ar & data_;
  };
#endif

  ///*constexpr*/ T_numtype data_[1] = {};
//  BZ_ALIGN_VARIABLE(T_numtype, data_[N_length>0? N_length : 1], BZ_SIMD_WIDTH)
};


BZ_NAMESPACE_END

#ifdef BZ_HAVE_BOOST_SERIALIZATION
namespace boost {
  namespace mpi {
    template<typename T> struct is_mpi_datatype;
    template <typename T, int N, int B>
    struct is_mpi_datatype<blitz::TinyBasedVector<T, N, B> > 
      : public is_mpi_datatype<T> { };
  } };
#endif


#include "tinybasedvec2.cc"

#endif // BZ_TINYBASEDVEC_H

