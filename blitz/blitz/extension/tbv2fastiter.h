// -*- C++ -*-
/***************************************************************************
 * blitz/array/fastiter.h  Declaration of FastArrayIterator<P_numtype,N_rank>
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
#ifndef BZ_ARRAY_TBV2FASTITER_H
#define BZ_ARRAY_TBV2FASTITER_H

#include <blitz/blitz.h>

#ifdef BZ_HAVE_STD
 #include <sstream>
#else
 #include <strstream.h>
#endif

#include <blitz/constpointerstack.h>
#include <blitz/bzdebug.h>
#include <blitz/shapecheck.h>
#include <blitz/prettyprint.h>
#include <blitz/extension/tinybasedvec2.h>

BZ_NAMESPACE(blitz)

// forward declaration
template<typename, int, int> class FastTBV2Iterator;
template<typename, int, int> class FastTBV2CopyIterator;


template<typename P_numtype, int N_length, int N_base, typename P_arraytype>
class FastTBV2IteratorBase {
public:
    typedef P_numtype                T_numtype;
  typedef typename opType<T_numtype>::T_optype T_optype;
  // if T_numtype is POD, then T_result is T_numtype, but if T_numtype
  // is an ET class, T_result will be the array class for that class.
  typedef typename asET<T_numtype>::T_wrapped T_typeprop;
  typedef typename unwrapET<T_typeprop>::T_unwrapped T_result;

  /** Result type for fastRead_tv is a FastTVIterator. This is only
      used for mixed TV/Array expressions. */
  typedef ETBase<FastTBV2Iterator<T_numtype, 
				 simdTypes<T_numtype>::vecWidth, N_base> > T_tvtypeprop;
  typedef typename unwrapET<T_tvtypeprop>::T_unwrapped T_tvresult;

  typedef TinyBasedVector<T_numtype, N_length, N_base> T_vector;
  typedef FastTBV2IteratorBase<P_numtype, N_length, N_base, P_arraytype> T_iterator;
    typedef const T_vector& T_ctorArg1;
    typedef int            T_ctorArg2;    // dummy
  typedef FastTBV2CopyIterator<P_numtype, N_length, N_base> T_range_result;

    static constexpr int 
        numArrayOperands = 0, 
        numTVOperands = 1, 
        numTMOperands = 0, 
        numIndexPlaceholders = 0,
      minWidth = simdTypes<T_numtype>::vecWidth,
      maxWidth = simdTypes<T_numtype>::vecWidth,
        rank_ = 1;

  /** For an iterator, the vectorized result for width N is always a
      TinyBasedVector<T_numtype, N>. */
  template<int N> struct tvresult {
    typedef FastTV2Iterator<T_numtype, N> Type;
  };

  FastTBV2IteratorBase(const T_iterator& x)
        : data_(x.data_), array_(x.array_)
    { }

    void operator=(const T_iterator& x)
    {
      BZPRECONDITION(0);
      // doesn't this copy the data in x.array_ and then make data_
      // point to x's array? doesn't seem right
        array_ = x.array_;
        data_ = x.data_;
        stack_ = x.stack_;
        //stride_ = x.stride_;
    }

    FastTBV2IteratorBase(const T_vector& array)
        : array_(array)
    {
        data_   = array_.data();
    }

    ~FastTBV2IteratorBase()
    { }

    T_numtype operator()(int i) const
    { return array_(i); }

  T_result operator()(TinyVector<int,1> i) const
    {
        BZPRECONDITION(array_.lengthCheck(i[0]));
        return array_(i[0]);
    }

    int ascending(const int r) const
    {
      BZPRECONDITION(r==0);
      return true;
    }

    int ordering(const int r) const
    {
      BZPRECONDITION(r==0);
      return 0;
    }

    int lbound(const int r) const
    { 
      BZPRECONDITION(r==0);
      return array_.lbound();
    }

    int ubound(const int r) const
    { 
      BZPRECONDITION(r==0);
      return array_.ubound();
    }
    
  //RectDomain<rank_> domain() const { return array_.domain(); };

    T_result first_value() const { return *data_; }

    T_result operator*() const
    { return *data_; }

    T_result operator[](int i) const
    { return data_[i * stride_]; }

    T_result fastRead(diffType i) const
  { return array_.fastRead(i); }

  template<int N>
  typename tvresult<N>::Type fastRead_tv(diffType i) const
  { 
    return typename tvresult<N>::Type(*reinterpret_cast<const TinyVector<T_numtype,N>*>(&data_[i]));
  }

  /** Since data_ is simd aligned by construction, we just have
      to check the offest. */
  bool isVectorAligned(diffType offset) const 
  { return (offset%simdTypes<T_numtype>::vecWidth==0) ? true : false; }

  diffType suggestStride(int r) const
  { BZPRECONDITION(r==0); return stride_; }

    bool isStride(int r, diffType stride) const
  { BZPRECONDITION(r==0); return stride==stride_; }

    void push(int position)
    {
      BZPRECONDITION(position==0); stack_[position] = data_;
    }
  
    void pop(int position)
    { 
      BZPRECONDITION(position==0); data_ = stack_[position];
    }

    void advance()
    {
      data_ += stride_;
    }

    void advance(int n)
    {
      data_ += n * stride_;
    }

    void loadStride(int r)
    {
      BZPRECONDITION(r==0); //stride_ = 1;
    }

  // This is used as lvalue, so it should return the actual data
    const T_numtype * _bz_restrict data() const
    { return data_; }

    // returns the lvalue, ie a pointer to the data
    T_numtype * _bz_restrict data_addr()
    {
        return const_cast<T_numtype *>(data_);
    }


  const T_vector& array() const 
  {return array_; }

  void _bz_setData(const T_numtype* ptr)
  { BZPRECONDITION(0); //data_ = ptr; 
  }

    // this is needed for the stencil expression fastRead to work
    void _bz_offsetData(sizeType i)
  { BZPRECONDITION(0); //data_ += i;
  }

    // and these are needed for stencil expression shift to work
    void _bz_offsetData(sizeType offset, int dim)
  { BZPRECONDITION(0); //data_ += offset*array_.stride(dim); 
  }
  
    void _bz_offsetData(sizeType offset1, int dim1, sizeType offset2, int dim2)
  { BZPRECONDITION(0); //data_ += offset1*array_.stride(dim1); 
    //data_ += offset2*array_.stride(dim2); 
  }

    int stride() const
    { return stride_; }

    constexpr bool isUnitStride(int r) const
    { BZPRECONDITION(r==0); return stride_ == 1; }

    constexpr bool isUnitStride() const
    { return stride_ == 1; }

    void advanceUnitStride()
    { ++data_; }

  bool canCollapse(int outerLoopRank, int innerLoopRank) const
  {
    BZPRECONDITION(outerLoopRank==0);
    BZPRECONDITION(innerLoopRank==0);
    return true;
  }

    template<typename T_shape>
    bool shapeCheck(const T_shape& s) const
  { return areShapesConformable(s, TinyVector<int, rank_>(N_length)); }

  /*
    // Experimental
    T_numtype& operator()(int i) const
    {
      return (T_numtype&)data_[i*array_.stride(0)];
    }

    // Experimental
    T_numtype& operator()(int i, int j) const
    {
      return (T_numtype&)data_[i*array_.stride(0) + j*array_.stride(1)];
    }

    // Experimental
    T_numtype& operator()(int i, int j, int k) const
    {
      return (T_numtype&)data_[i*array_.stride(0)
			       + j*array_.stride(1)
			       + k*array_.stride(2)];
    }

    // Experimental

    void moveTo(int i)
    {
        data_ = &const_cast<T_vector&>(array_)(i);
    }

    void moveTo(int i, int j)
    {
        data_ = &const_cast<T_vector&>(array_)(i,j);
    }

    void moveTo(int i, int j, int k)
    {
        data_ = &const_cast<T_vector&>(array_)(i,j,k);
    }

    template<int N_rank2>
    void moveTo(const TinyVector<int,N_rank2>& i)
    {
        data_ = &const_cast<T_vector&>(array_)(i);
    }

    // Experimental
    void operator=(T_numtype x)
    {   *const_cast<T_numtype*>(data_) = x; }

    // Experimental
    template<typename T_value>
    void operator=(T_value x)
    {   *const_cast<T_numtype*>(data_) = x; }

    // Experimental
    template<typename T_value>
    void operator+=(T_value x)
    { *const_cast<T_numtype*>(data_) += x; }

    // NEEDS_WORK: other operators
  
    // Experimental
    operator T_numtype() const
    { return *data_; }
  */

  //   Experimental
    T_result shift(int offset, int dim) const
    {
      return data_[offset*array_.stride(dim)];
    }

    // Experimental
    T_result shift(int offset1, int dim1, int offset2, int dim2) const
    {
      return data_[offset1*array_.stride(dim1) 
    		   + offset2*array_.stride(dim2)];
    }

    void prettyPrint(BZ_STD_SCOPE(string) &str, 
        prettyPrintFormat& format) const
    {
        if (format.tersePrintingSelected())
            str += format.nextArrayOperandSymbol();
        else if (format.dumpArrayShapesMode())
        {
#ifdef BZ_HAVE_STD
	    BZ_STD_SCOPE(ostringstream) ostr;
#else
            ostrstream ostr;
#endif
            ostr << array_.shape();
            str += ostr.str();
        }
        else {
            str += "TinyBasedVector<";
            str += BZ_DEBUG_TEMPLATE_AS_STRING_LITERAL(T_numtype);
            str += ",";

            char tmpBuf[10];
            sprintf(tmpBuf, "%d", N_length);

            str += tmpBuf;
            str += ">";
        }
    }

  // vectors can't be sliced
  template<typename T1, typename T2 = nilArraySection, 
	   class T3 = nilArraySection, typename T4 = nilArraySection, 
	   class T5 = nilArraySection, typename T6 = nilArraySection, 
	   class T7 = nilArraySection, typename T8 = nilArraySection, 
	   class T9 = nilArraySection, typename T10 = nilArraySection, 
	   class T11 = nilArraySection>
  class SliceInfo {
  public:    
    typedef void T_slice;
};

protected:
  const T_numtype * _bz_restrict           data_;
  P_arraytype                          array_;
  ConstPointerStack<T_numtype,rank_>  stack_;
  static constexpr diffType                             stride_=1;
};


template<typename P_numtype, int N_length, int N_base> class FastTBV2CopyIterator;

template<typename P_numtype, int N_length, int N_base>
class FastTBV2Iterator : 
  public FastTBV2IteratorBase<P_numtype, N_length, N_base, const TinyBasedVector<P_numtype, N_length, N_base>&> 
{
public:
  typedef FastTBV2IteratorBase<P_numtype, N_length, N_base, 
				const TinyBasedVector<P_numtype, N_length, N_base>&> T_base;
  typedef typename T_base::T_numtype T_numtype;
  typedef typename T_base::T_vector T_vector;
  typedef typename T_base::T_iterator T_iterator;
  typedef typename T_base::T_ctorArg1 T_ctorArg1;
  typedef typename T_base::T_ctorArg2 T_ctorArg2;
  typedef typename T_base::T_range_result T_range_result;
  
  using T_base::rank_;
  using T_base::numArrayOperands;
  using T_base::numTVOperands;
  using T_base::numTMOperands;
  using T_base::numIndexPlaceholders;

  // NB: this ctor does NOT preserve stack and stride
  // parameters.  This is for speed purposes.
  FastTBV2Iterator(const FastTBV2Iterator<P_numtype, N_length, N_base>& x) 
    : T_base(x)
  { }

  FastTBV2Iterator(const T_vector& array) : T_base(array) {}
  
  using T_base::operator=;
  void operator=(const FastTBV2Iterator<P_numtype, N_length, N_base>& x)
  {
    T_base::operator=(x);
  }
  
  using T_base::operator();

  // template<int N>
  // T_range_result operator()(const RectDomain<N>& d) const
  // {
  //   return T_range_result(T_base::array_(d));
  // }

  // template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6,
  // 	   typename T7, typename T8, typename T9, typename T10, typename T11>
  // FastTBV2CopyIterator<T_numtype, T_base::template SliceInfo<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11>::T_slice::rank>
  // operator()(T1 r1, T2 r2, T3 r3, T4 r4, T5 r5, T6 r6, T7 r7, T8 r8, T9 r9, T10 r10, T11 r11) const
  // {
  //   typedef FastTBV2CopyIterator<T_numtype, T_base::template SliceInfo<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11>::T_slice::rank> slice;

  //   return slice(array_(r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11));
  // }
  
};

/* This version of the FastTBV2Iterator makes a COPY of the array
   it's pointing to. This makes it possible to return expressions of
   arrays that have gone out of scope, or to slice expressions. */
template<typename P_numtype, int N_length, int N_base>
class FastTBV2CopyIterator : 
  public FastTBV2IteratorBase<P_numtype, N_length, N_base, const TinyBasedVector<P_numtype, N_length, N_base> >
{
public:
  typedef FastTBV2IteratorBase<P_numtype, N_length, N_base, 
				const TinyBasedVector<P_numtype, N_length, N_base> > T_base;
  typedef typename T_base::T_numtype T_numtype;
  typedef typename T_base::T_vector T_vector;
  typedef typename T_base::T_iterator T_iterator;
  typedef typename T_base::T_ctorArg1 T_ctorArg1;
  typedef typename T_base::T_ctorArg2 T_ctorArg2;
  typedef typename T_base::T_range_result T_range_result;

  using T_base::rank_;
  using T_base::numArrayOperands;
  using T_base::numTVOperands;
  using T_base::numTMOperands;
  using T_base::numIndexPlaceholders;


  // NB: this ctor does NOT preserve stack and stride
  // parameters.  This is for speed purposes.
  FastTBV2CopyIterator(const FastTBV2CopyIterator<P_numtype, N_length, N_base>& x) 
    : T_base(x)
  { }

  FastTBV2CopyIterator(const T_vector& array) : T_base(array) { }
  
  using T_base::operator=;
  void operator=(const FastTBV2CopyIterator& x)
  {
    T_base::operator=(x);
  }
  
  using T_base::operator();

  // template<int N>
  // T_range_result operator()(const RectDomain<N>& d) const
  // {
  //   return T_range_result(T_base::array_(d));
  // }

  // template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6,
  // 	   typename T7, typename T8, typename T9, typename T10, typename T11>
  // FastTBV2CopyIterator<T_numtype, T_base::template SliceInfo<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11>::T_slice::rank>
  // operator()(T1 r1, T2 r2, T3 r3, T4 r4, T5 r5, T6 r6, T7 r7, T8 r8, T9 r9, T10 r10, T11 r11) const
  // {
  //   typedef FastTBV2CopyIterator<T_numtype, T_base::template SliceInfo<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11>::T_slice::rank> slice;

  //   return slice(array_(r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11));
  // }
};


BZ_NAMESPACE_END

#endif // BZ_ARRAY_TBV2FASTITER_H
