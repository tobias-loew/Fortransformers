// -*- C++ -*-
/***************************************************************************
 * blitz/array-impl.h    Definition of the Array<P_numtype, N_rank> class
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

/*
 * Wish list for array classes.
 *  - Arrays whose dimensions are unknown at compile time.
 *  - where()/elsewhere()/elsewhere() as in Dan Quinlan's implementation
 *  - block reduction operations
 *  - conversion to/from matrix & vector
 *  - apply(T func(T))
 *  - apply(T func(const T&))
 *  - apply<T func(T)>
 */

#ifndef BZ_ARRAY_H
#define BZ_ARRAY_H

#include <blitz/blitz.h>
#include <blitz/memblock.h>
#include <blitz/range.h>
#include <blitz/tinyvec2.h>
#include <blitz/tvecglobs.h>

#include <blitz/indexexpr.h>

#include <blitz/array/slice.h>     // Subarrays and slicing
#include <blitz/array/map.h>       // Tensor index notation
#include <blitz/array/multi.h>     // Multicomponent arrays
#include <blitz/array/domain.h>    // RectDomain class
#include <blitz/array/storage.h>   // GeneralArrayStorage

#ifdef BZ_HAVE_BOOST_SERIALIZATION
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/base_object.hpp>
#endif


BZ_NAMESPACE(blitz)

/*
 * Forward declarations
 */


template<typename T_numtype, int N_rank, typename Policy>
class ArrayIterator;

template<typename T_numtype, int N_rank, typename Policy>
class ConstArrayIterator;

template<typename T_numtype, int N_rank, typename Policy>
class FastArrayIterator;

template<typename P_expr>
class _bz_ArrayExpr;

template<typename T_array, typename T_index>
class IndirectArray;

template <typename P_numtype,int N_rank, typename Policy>
void swap(Array<P_numtype,N_rank, Policy>&,Array<P_numtype,N_rank, Policy>&);

template <typename P_numtype, int N_rank, typename Policy>
void find(Array<TinyVector<int,N_rank>,1, Policy>&,const Array<P_numtype,N_rank, Policy>&);



/*
 * Policies
 */
namespace array_policy {

    namespace detail {
        template<typename T, int value>
        struct single_value_array {
            template<typename V>
            constexpr T operator[](V&&)const {
                return static_cast<T>(value);
            }
            template<typename V>
            constexpr T operator()(V&&)const {
                return static_cast<T>(value);
            }
        };


        template<int value>
        struct static_zeroOffset__impl {
            static constexpr diffType zeroOffset_ = -value;
        };

        struct dynamic_zeroOffset__impl {
            diffType zeroOffset_;
        };
    }


    struct tag_staticfortranarraypolicy {};
    struct tag_generalfortranarraypolicy {};

    struct GeneralPolicy {};

    struct GeneralFortranArrayPolicy {};

    template<int N_base>
    struct StaticFortranArrayPolicy {};


    template<typename Policy>
    struct policy_traits {
        template<typename P_numtype>
        using memory_block_reference_t = MemoryBlockReference<P_numtype>;


        template<int N_rank>
        using storage_t = GeneralArrayStorage<N_rank>;

        template<int N_rank>
        using length_t = TinyVector<int, N_rank>;

        template<int N_rank>
        using stride_t = TinyVector<diffType, N_rank>;

        using difftype_impl_t = detail::dynamic_zeroOffset__impl;
    };


    template<int N_base >
    struct policy_traits<StaticFortranArrayPolicy<N_base>> {
        template<typename P_numtype>
        using memory_block_reference_t = StaticMemoryBlockReference<P_numtype>;


        template<int N_rank>
        using storage_t = StaticArrayStorage<N_rank>;

        template<int N_rank>
        using length_t = TinyVector<int, N_rank>;

        template<int N_rank>
        using stride_t = detail::single_value_array<diffType, 1>;

        using difftype_impl_t = detail::static_zeroOffset__impl<N_base>;
    };

}

template<typename P_numtype, int N_rank>
using ArrayG = Array<P_numtype, N_rank, array_policy::GeneralPolicy>;



template <typename T>
std::true_type is_indexing_type_impl(const ETBase<T>*);

std::false_type is_indexing_type_impl(const Range*);
std::false_type is_indexing_type_impl(const ETBase<Range>*);
template <int N>
std::false_type is_indexing_type_impl(const IndexPlaceholder<N>*);
template <int N>
std::false_type is_indexing_type_impl(const ETBase<IndexPlaceholder<N>>*);
std::false_type is_indexing_type_impl(...);

template <typename T>
using is_indexing_type = decltype(is_indexing_type_impl(std::declval<T*>()));


/** Declaration of class Array, the "Swiss army knife" of Blitz
    expression template classes. This is an arbitrary (at compile
    time) rank, arbitrary size container.

    \todo Array should inherit protected from MemoryBlockReference.
    To make this work, need to expose
    MemoryBlockReference::numReferences() and make Array<P,N2> a
    friend of Array<P,N> for slicing. (Is this still relevant? Array
    DOES inherit from MemoryBlockReference.)
  */




template<typename P_numtype, int N_rank, typename Policy>
class Array : public array_policy::policy_traits<Policy>::template memory_block_reference_t<P_numtype>
    , public array_policy::policy_traits<Policy>::difftype_impl_t
#ifdef BZ_NEW_EXPRESSION_TEMPLATES
    , public ETBase<Array<P_numtype,N_rank,Policy> >
#endif
{

private:
    using policy_traits = array_policy::policy_traits<Policy>;

    using memory_block_reference_t = typename policy_traits::template memory_block_reference_t<P_numtype>;

    typedef memory_block_reference_t T_base;
    using T_base::data_;

public:
    //////////////////////////////////////////////
    // Public Types
    //////////////////////////////////////////////

    /*
     * T_numtype  is the numeric type stored in the array.
     * T_index    is a vector type which can be used to access elements
     *            of many-dimensional arrays.
     * T_array    is the array type itself -- Array<T_numtype, N_rank>
     * T_iterator is a a fast iterator for the array, used for expression
     *            templates
     * iterator   is a STL-style iterator
     * const_iterator is an STL-style const iterator
     * T_default_storage is the default storage class type for the array
     */
    typedef Policy                      PolicyType;
    typedef P_numtype                T_numtype;
    typedef TinyVector<int, N_rank>  T_index;
    typedef Array<T_numtype, N_rank, Policy> T_array;
    typedef FastArrayIterator<T_numtype, N_rank, Policy> T_iterator;

    typedef ArrayIterator<T_numtype, N_rank, Policy> iterator;
    typedef ConstArrayIterator<T_numtype, N_rank, Policy> const_iterator;

    /**
     * Set default storage order. This is configurable
     * via #defines as it is can be beneficial to set a
     * specific storage for an entire project/file.
     *
     * First check for the Fortan flag and then the column
     * major flag, since Fortran arrays are column major.
     */
#if defined(BZ_FORTRAN_ARRAY)
    typedef FortranArray<N_rank> T_default_storage;
#elif defined(BZ_COLUMN_MAJOR_ARRAY)
    typedef ColumnMajorArray<N_rank> T_default_storage;
#else
    typedef GeneralArrayStorage<N_rank> T_default_storage;
#endif

    static constexpr int rank_ = N_rank;

    //////////////////////////////////////////////
    // Constructors                             //
    //////////////////////////////////////////////


    /** Construct an array from an expression. Because this entails a
    memory allocation, it is explicit so this fact is obvious to
    the user. (There may also be ambiguities in making it
    implicit?) */
    template<typename T_expr>
    explicit Array(_bz_ArrayExpr<T_expr> expr);

    /*
     * Any missing length arguments will have their value taken from the
     * last argument.  For example,
     *   Array<int,3> A(32,64);
     * will create a 32x64x64 array.  This is handled by setupStorage().
     */

    Array(GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        length_ = 0;
        stride_ = 0;
        zeroOffset_ = 0;
    }

    explicit Array(int length0,
        GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        length_[0] = length0;
        setupStorage(0);
    }

    Array(int length0, int length1,
        GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        BZPRECONDITION(N_rank >= 2);
        TAU_TYPE_STRING(p1, "Array<T,N>::Array() [T="
            + CT(T_numtype) + ",N=" + CT(N_rank) + "]");
        TAU_PROFILE(p1, "void (int,int)", TAU_BLITZ);

        length_[0] = length0;
        length_[1] = length1;
        setupStorage(1);
    }

    Array(int length0, int length1, int length2,
        GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        BZPRECONDITION(N_rank >= 3);
        length_[0] = length0;
        length_[1] = length1;
        length_[2] = length2;
        setupStorage(2);
    }

    Array(int length0, int length1, int length2, int length3,
        GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        BZPRECONDITION(N_rank >= 4);
        length_[0] = length0;
        length_[1] = length1;
        length_[2] = length2;
        length_[3] = length3;
        setupStorage(3);
    }

    Array(int length0, int length1, int length2, int length3, int length4,
        GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        BZPRECONDITION(N_rank >= 5);
        length_[0] = length0;
        length_[1] = length1;
        length_[2] = length2;
        length_[3] = length3;
        length_[4] = length4;
        setupStorage(4);
    }

    Array(int length0, int length1, int length2, int length3, int length4,
        int length5,
        GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        BZPRECONDITION(N_rank >= 6);
        length_[0] = length0;
        length_[1] = length1;
        length_[2] = length2;
        length_[3] = length3;
        length_[4] = length4;
        length_[5] = length5;
        setupStorage(5);
    }

    Array(int length0, int length1, int length2, int length3, int length4,
        int length5, int length6,
        GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        BZPRECONDITION(N_rank >= 7);
        length_[0] = length0;
        length_[1] = length1;
        length_[2] = length2;
        length_[3] = length3;
        length_[4] = length4;
        length_[5] = length5;
        length_[6] = length6;
        setupStorage(6);
    }

    Array(int length0, int length1, int length2, int length3, int length4,
        int length5, int length6, int length7,
        GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        BZPRECONDITION(N_rank >= 8);
        length_[0] = length0;
        length_[1] = length1;
        length_[2] = length2;
        length_[3] = length3;
        length_[4] = length4;
        length_[5] = length5;
        length_[6] = length6;
        length_[7] = length7;
        setupStorage(7);
    }

    Array(int length0, int length1, int length2, int length3, int length4,
        int length5, int length6, int length7, int length8,
        GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        BZPRECONDITION(N_rank >= 9);
        length_[0] = length0;
        length_[1] = length1;
        length_[2] = length2;
        length_[3] = length3;
        length_[4] = length4;
        length_[5] = length5;
        length_[6] = length6;
        length_[7] = length7;
        length_[8] = length8;
        setupStorage(8);
    }

    Array(int length0, int length1, int length2, int length3, int length4,
        int length5, int length6, int length7, int length8, int length9,
        GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        BZPRECONDITION(N_rank >= 10);
        length_[0] = length0;
        length_[1] = length1;
        length_[2] = length2;
        length_[3] = length3;
        length_[4] = length4;
        length_[5] = length5;
        length_[6] = length6;
        length_[7] = length7;
        length_[8] = length8;
        length_[9] = length9;
        setupStorage(9);
    }

    Array(int length0, int length1, int length2, int length3, int length4,
        int length5, int length6, int length7, int length8, int length9,
        int length10,
        GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        BZPRECONDITION(N_rank >= 11);
        length_[0] = length0;
        length_[1] = length1;
        length_[2] = length2;
        length_[3] = length3;
        length_[4] = length4;
        length_[5] = length5;
        length_[6] = length6;
        length_[7] = length7;
        length_[8] = length8;
        length_[9] = length9;
        length_[10] = length10;
        setupStorage(10);
    }

    /*
     * Construct an array from an existing block of memory.  Ownership
     * is not acquired (this is provided for backwards compatibility).
     */
    Array(T_numtype* _bz_restrict dataFirst, TinyVector<int, N_rank> shape,
        GeneralArrayStorage<N_rank> storage =
        T_default_storage(contiguousData))
        : memory_block_reference_t(_bz_returntype<sizeType>::product(shape), dataFirst,
            neverDeleteData),
        storage_(storage)
    {
        BZPRECONDITION(dataFirst != 0);

        length_ = shape;
        computeStrides();
        data_ += zeroOffset_;

    }

    /**
        Construct an array from an existing block of memory, with a
        given set of strides.  Ownership is not acquired (i.e. the
        memory block will not be freed by Blitz++).  This constructor is
        used by extractComponent to make a component view of a
        multicomponent array, which is by design noncontiguous. This
        creates an incorrect length in the MemoryBlockReference (though
        that may be of no consequence since we're not freeing the
        memory).
       */
    Array(T_numtype* _bz_restrict dataFirst, TinyVector<int, N_rank> shape,
        TinyVector<diffType, N_rank> stride,
        GeneralArrayStorage<N_rank> storage =
        T_default_storage(contiguousData))
        : memory_block_reference_t(_bz_returntype<sizeType>::product(shape), dataFirst,
            neverDeleteData),
        storage_(storage)
    {
        BZPRECONDITION(dataFirst != 0);

        length_ = shape;
        stride_ = stride;
        calculateZeroOffset();
        data_ += zeroOffset_;
    }

    /**
       Construct an array from an existing block of memory. If the
       storage represents a padded array, the length of the memory block
       will be incorrect, which would lead to a crash if
       "deleteDataWhenDone" is used. For this reason, we check that the
       resulting array is contiguous.
       */
    Array(T_numtype* _bz_restrict dataFirst, TinyVector<int, N_rank> shape,
        preexistingMemoryPolicy deletionPolicy,
        GeneralArrayStorage<N_rank> storage =
        T_default_storage(contiguousData))
        : memory_block_reference_t(_bz_returntype<sizeType>::product(shape), dataFirst,
            deletionPolicy),
        storage_(storage)
    {
        BZPRECONDITION(dataFirst != 0);

        length_ = shape;
        computeStrides();
        data_ += zeroOffset_;

        BZPRECHECK(deletionPolicy != deleteDataWhenDone || isStorageContiguous(), "Non-contiguous storage used with owned pre-existing memory");

        if (deletionPolicy == duplicateData)
            reference(copy());
    }




    Array(array_policy::tag_staticfortranarraypolicy, T_numtype* _bz_restrict dataFirst, TinyVector<int, N_rank> const& shape/*, GeneralArrayStorage<N_rank> storage*/)
        : memory_block_reference_t(dataFirst)
        //, storage_{ storage } // storage not needed since it is only used with StaticFortranArrayPolicy
    {
        BZPRECONDITION(dataFirst != 0);

        length_ = shape;
        //        computeStrides();
        data_ += zeroOffset_;

        //BZPRECHECK(deletionPolicy != deleteDataWhenDone || isStorageContiguous(), "Non-contiguous storage used with owned pre-existing memory");

        //if(deletionPolicy == duplicateData)
        //    reference(copy());
    }



    Array(array_policy::tag_generalfortranarraypolicy, T_numtype* _bz_restrict dataFirst, TinyVector<int, N_rank> const& shape, GeneralArrayStorage<N_rank> storage )
        : memory_block_reference_t(1, dataFirst, neverDeleteData)
        , storage_{ storage }
    {
        BZPRECONDITION(dataFirst != 0);

        storage_.setBase(0, 1);
        length_ = shape;
        computeStrides();
        data_ += zeroOffset_;

        //BZPRECHECK(deletionPolicy != deleteDataWhenDone || isStorageContiguous(), "Non-contiguous storage used with owned pre-existing memory");

        //if(deletionPolicy == duplicateData)
        //    reference(copy());
    }



  /**
     Construct an array from an existing block of memory, with a given
     set of strides. If the strides represent a noncontiguous array,
     the calculated length of the memory block will be wrong, which
     will lead to a crash if "deleteDataWhenDone" is specified. For
     this reason, we check that the resulting array is contiguous.
     */
    Array(T_numtype* _bz_restrict dataFirst, TinyVector<int, N_rank> shape,
        TinyVector<diffType, N_rank> stride,
        preexistingMemoryPolicy deletionPolicy,
        GeneralArrayStorage<N_rank> storage = 
	  T_default_storage(contiguousData))
      : memory_block_reference_t(_bz_returntype<sizeType>::product(shape), dataFirst, 
          deletionPolicy),
        storage_(storage)
    {
      BZPRECONDITION(dataFirst != 0);

        length_ = shape;
        stride_ = stride;
        calculateZeroOffset();
        data_ += zeroOffset_;

	BZPRECHECK(deletionPolicy!=deleteDataWhenDone || isStorageContiguous(), "Non-contiguous storage used with owned pre-existing memory");

        if (deletionPolicy == duplicateData)
            reference(copy());
    }

    /*
     * This constructor takes an extent (length) vector and storage format.
     */

    Array(const TinyVector<int, N_rank>& extent, 
        GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        length_ = extent;
        setupStorage(N_rank - 1);
    }

    /*
     * This construct takes a vector of bases (lbounds) and a vector of
     * extents.
     */

    Array(const TinyVector<int, N_rank>& lbounds,
        const TinyVector<int, N_rank>& extent,
        const GeneralArrayStorage<N_rank>& storage 
           = T_default_storage());

    /*
     * These constructors allow arbitrary bases (starting indices) to be set.
     * e.g. Array<int,2> A(Range(10,20), Range(20,30))
     * will create an 11x11 array whose indices are 10..20 and 20..30
     */
    Array(Range r0, 
        GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        BZPRECONDITION(r0.isAscendingContiguous());

        length_[0] = r0.length();
        storage_.setBase(0, r0.first());
        setupStorage(0);
    }

    Array(Range r0, Range r1,
        GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        BZPRECONDITION(r0.isAscendingContiguous() && 
            r1.isAscendingContiguous());

        length_[0] = r0.length();
        storage_.setBase(0, r0.first());
        length_[1] = r1.length();
        storage_.setBase(1, r1.first());

        setupStorage(1);
    }

    Array(Range r0, Range r1, Range r2,
        GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        BZPRECONDITION(r0.isAscendingContiguous() &&
            r1.isAscendingContiguous() && r2.isAscendingContiguous());

        length_[0] = r0.length();
        storage_.setBase(0, r0.first());
        length_[1] = r1.length();
        storage_.setBase(1, r1.first());
        length_[2] = r2.length();
        storage_.setBase(2, r2.first());

        setupStorage(2);
    }

    Array(Range r0, Range r1, Range r2, Range r3,
        GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        BZPRECONDITION(r0.isAscendingContiguous() &&
            r1.isAscendingContiguous() && r2.isAscendingContiguous()
            && r3.isAscendingContiguous());

        length_[0] = r0.length();
        storage_.setBase(0, r0.first());
        length_[1] = r1.length();
        storage_.setBase(1, r1.first());
        length_[2] = r2.length();
        storage_.setBase(2, r2.first());
        length_[3] = r3.length();
        storage_.setBase(3, r3.first());

        setupStorage(3);
    }

    Array(Range r0, Range r1, Range r2, Range r3, Range r4,
        GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        BZPRECONDITION(r0.isAscendingContiguous() &&
            r1.isAscendingContiguous() && r2.isAscendingContiguous()
            && r3.isAscendingContiguous() && r4.isAscendingContiguous());

        length_[0] = r0.length();
        storage_.setBase(0, r0.first());
        length_[1] = r1.length();
        storage_.setBase(1, r1.first());
        length_[2] = r2.length();
        storage_.setBase(2, r2.first());
        length_[3] = r3.length();
        storage_.setBase(3, r3.first());
        length_[4] = r4.length();
        storage_.setBase(4, r4.first());

        setupStorage(4);
    }

    Array(Range r0, Range r1, Range r2, Range r3, Range r4, Range r5,
        GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        BZPRECONDITION(r0.isAscendingContiguous() &&
            r1.isAscendingContiguous() && r2.isAscendingContiguous()
            && r3.isAscendingContiguous() && r4.isAscendingContiguous()
            && r5.isAscendingContiguous());

        length_[0] = r0.length();
        storage_.setBase(0, r0.first());
        length_[1] = r1.length();
        storage_.setBase(1, r1.first());
        length_[2] = r2.length();
        storage_.setBase(2, r2.first());
        length_[3] = r3.length();
        storage_.setBase(3, r3.first());
        length_[4] = r4.length();
        storage_.setBase(4, r4.first());
        length_[5] = r5.length();
        storage_.setBase(5, r5.first());

        setupStorage(5);
    }

    Array(Range r0, Range r1, Range r2, Range r3, Range r4, Range r5,
        Range r6,
        GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        BZPRECONDITION(r0.isAscendingContiguous() &&
            r1.isAscendingContiguous() && r2.isAscendingContiguous()
            && r3.isAscendingContiguous() && r4.isAscendingContiguous()
            && r5.isAscendingContiguous() && r6.isAscendingContiguous());

        length_[0] = r0.length();
        storage_.setBase(0, r0.first());
        length_[1] = r1.length();
        storage_.setBase(1, r1.first());
        length_[2] = r2.length();
        storage_.setBase(2, r2.first());
        length_[3] = r3.length();
        storage_.setBase(3, r3.first());
        length_[4] = r4.length();
        storage_.setBase(4, r4.first());
        length_[5] = r5.length();
        storage_.setBase(5, r5.first());
        length_[6] = r6.length();
        storage_.setBase(6, r6.first());

        setupStorage(6);
    }

    Array(Range r0, Range r1, Range r2, Range r3, Range r4, Range r5,
        Range r6, Range r7,
        GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        BZPRECONDITION(r0.isAscendingContiguous() &&
            r1.isAscendingContiguous() && r2.isAscendingContiguous()
            && r3.isAscendingContiguous() && r4.isAscendingContiguous()
            && r5.isAscendingContiguous() && r6.isAscendingContiguous()
            && r7.isAscendingContiguous());

        length_[0] = r0.length();
        storage_.setBase(0, r0.first());
        length_[1] = r1.length();
        storage_.setBase(1, r1.first());
        length_[2] = r2.length();
        storage_.setBase(2, r2.first());
        length_[3] = r3.length();
        storage_.setBase(3, r3.first());
        length_[4] = r4.length();
        storage_.setBase(4, r4.first());
        length_[5] = r5.length();
        storage_.setBase(5, r5.first());
        length_[6] = r6.length();
        storage_.setBase(6, r6.first());
        length_[7] = r7.length();
        storage_.setBase(7, r7.first());

        setupStorage(7);
    }

    Array(Range r0, Range r1, Range r2, Range r3, Range r4, Range r5,
        Range r6, Range r7, Range r8,
        GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        BZPRECONDITION(r0.isAscendingContiguous() &&
            r1.isAscendingContiguous() && r2.isAscendingContiguous()
            && r3.isAscendingContiguous() && r4.isAscendingContiguous()
            && r5.isAscendingContiguous() && r6.isAscendingContiguous()
            && r7.isAscendingContiguous() && r8.isAscendingContiguous());

        length_[0] = r0.length();
        storage_.setBase(0, r0.first());
        length_[1] = r1.length();
        storage_.setBase(1, r1.first());
        length_[2] = r2.length();
        storage_.setBase(2, r2.first());
        length_[3] = r3.length();
        storage_.setBase(3, r3.first());
        length_[4] = r4.length();
        storage_.setBase(4, r4.first());
        length_[5] = r5.length();
        storage_.setBase(5, r5.first());
        length_[6] = r6.length();
        storage_.setBase(6, r6.first());
        length_[7] = r7.length();
        storage_.setBase(7, r7.first());
        length_[8] = r8.length();
        storage_.setBase(8, r8.first());

        setupStorage(8);
    }

    Array(Range r0, Range r1, Range r2, Range r3, Range r4, Range r5,
        Range r6, Range r7, Range r8, Range r9,
        GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        BZPRECONDITION(r0.isAscendingContiguous() &&
            r1.isAscendingContiguous() && r2.isAscendingContiguous()
            && r3.isAscendingContiguous() && r4.isAscendingContiguous()
            && r5.isAscendingContiguous() && r6.isAscendingContiguous()
            && r7.isAscendingContiguous() && r8.isAscendingContiguous()
            && r9.isAscendingContiguous());

        length_[0] = r0.length();
        storage_.setBase(0, r0.first());
        length_[1] = r1.length();
        storage_.setBase(1, r1.first());
        length_[2] = r2.length();
        storage_.setBase(2, r2.first());
        length_[3] = r3.length();
        storage_.setBase(3, r3.first());
        length_[4] = r4.length();
        storage_.setBase(4, r4.first());
        length_[5] = r5.length();
        storage_.setBase(5, r5.first());
        length_[6] = r6.length();
        storage_.setBase(6, r6.first());
        length_[7] = r7.length();
        storage_.setBase(7, r7.first());
        length_[8] = r8.length();
        storage_.setBase(8, r8.first());
        length_[9] = r9.length();
        storage_.setBase(9, r9.first());

        setupStorage(9);
    }

    Array(Range r0, Range r1, Range r2, Range r3, Range r4, Range r5,
        Range r6, Range r7, Range r8, Range r9, Range r10,
        GeneralArrayStorage<N_rank> storage = T_default_storage())
        : storage_(storage)
    {
        BZPRECONDITION(r0.isAscendingContiguous() &&
            r1.isAscendingContiguous() && r2.isAscendingContiguous()
            && r3.isAscendingContiguous() && r4.isAscendingContiguous()
            && r5.isAscendingContiguous() && r6.isAscendingContiguous()
            && r7.isAscendingContiguous() && r8.isAscendingContiguous()
            && r9.isAscendingContiguous() && r10.isAscendingContiguous());

        length_[0] = r0.length();
        storage_.setBase(0, r0.first());
        length_[1] = r1.length();
        storage_.setBase(1, r1.first());
        length_[2] = r2.length();
        storage_.setBase(2, r2.first());
        length_[3] = r3.length();
        storage_.setBase(3, r3.first());
        length_[4] = r4.length();
        storage_.setBase(4, r4.first());
        length_[5] = r5.length();
        storage_.setBase(5, r5.first());
        length_[6] = r6.length();
        storage_.setBase(6, r6.first());
        length_[7] = r7.length();
        storage_.setBase(7, r7.first());
        length_[8] = r8.length();
        storage_.setBase(8, r8.first());
        length_[9] = r9.length();
        storage_.setBase(9, r9.first());
        length_[10] = r10.length();
        storage_.setBase(10, r10.first());

        setupStorage(10);
    }

    /*
     * Create a reference of another array
     */
    Array(const Array<T_numtype, N_rank, Policy>& array)
#ifdef BZ_NEW_EXPRESSION_TEMPLATES
        : memory_block_reference_t(),
          ETBase< Array<T_numtype, N_rank, Policy> >(array)
#else
        : memory_block_reference_t()
#endif
    {
        // NEEDS_WORK: this const_cast is a tad ugly.
        reference(const_cast<T_array&>(array));
    }

    /*
     * These constructors are used for creating interlaced arrays (see
     * <blitz/arrayshape.h>
     */
    Array(const TinyVector<int,N_rank-1>& shape,
        int lastExtent, const GeneralArrayStorage<N_rank>& storage);
    //Array(const TinyVector<Range,N_rank-1>& shape,
    //    int lastExtent, const GeneralArrayStorage<N_rank>& storage);

    /*
     * These constructors make the array a view of a subportion of another
     * array.  If there fewer than N_rank Range arguments provided, no
     * slicing is performed in the unspecified ranks.
     * e.g. Array<int,3> A(20,20,20);
     *      Array<int,3> B(A, Range(5,15));
     * is equivalent to:
     *      Array<int,3> B(A, Range(5,15), Range::all(), Range::all());
     */
    Array(Array<T_numtype, N_rank, Policy>& array, Range r0)
    {
        constructSubarray(Policy{}, array, r0);
    }

    Array(Array<T_numtype, N_rank, Policy>& array, Range r0, Range r1)
    {
        constructSubarray(Policy{}, array, r0, r1);
    }

    Array(Array<T_numtype, N_rank, Policy>& array, Range r0, Range r1, Range r2)
    {
        constructSubarray(Policy{}, array, r0, r1, r2);
    }

    Array(Array<T_numtype, N_rank, Policy>& array, Range r0, Range r1, Range r2,
        Range r3)
    {
        constructSubarray(Policy{}, array, r0, r1, r2, r3);
    }

    Array(Array<T_numtype, N_rank, Policy>& array, Range r0, Range r1, Range r2,
        Range r3, Range r4)
    {
        constructSubarray(Policy{}, array, r0, r1, r2, r3, r4);
    }

    Array(Array<T_numtype, N_rank, Policy>& array, Range r0, Range r1, Range r2,
        Range r3, Range r4, Range r5)
    {
        constructSubarray(Policy{}, array, r0, r1, r2, r3, r4, r5);
    }

    Array(Array<T_numtype, N_rank, Policy>& array, Range r0, Range r1, Range r2,
        Range r3, Range r4, Range r5, Range r6)
    {
        constructSubarray(Policy{}, array, r0, r1, r2, r3, r4, r5, r6);
    }

    Array(Array<T_numtype, N_rank, Policy>& array, Range r0, Range r1, Range r2,
        Range r3, Range r4, Range r5, Range r6, Range r7)
    {
        constructSubarray(Policy{}, array, r0, r1, r2, r3, r4, r5, r6, r7);
    }

    Array(Array<T_numtype, N_rank, Policy>& array, Range r0, Range r1, Range r2,
        Range r3, Range r4, Range r5, Range r6, Range r7, Range r8)
    {
        constructSubarray(Policy{}, array, r0, r1, r2, r3, r4, r5, r6, r7, r8);
    }

    Array(Array<T_numtype, N_rank, Policy>& array, Range r0, Range r1, Range r2,
        Range r3, Range r4, Range r5, Range r6, Range r7, Range r8, Range r9)
    {
        constructSubarray(Policy{}, array, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9);
    }

    Array(Array<T_numtype, N_rank, Policy>& array, Range r0, Range r1, Range r2,
        Range r3, Range r4, Range r5, Range r6, Range r7, Range r8, Range r9,
        Range r10)
    {
        constructSubarray(Policy{}, array, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10);
    }

    Array(Array<T_numtype, N_rank, Policy>& array, 
        const RectDomain<N_rank>& subdomain)
    {
        constructSubarray(Policy{}, array, subdomain);
    }

    /* Constructor added by Julian Cummings */
    Array(Array<T_numtype, N_rank, Policy>& array, 
        const StridedDomain<N_rank>& subdomain)
    {
        constructSubarray(Policy{}, array, subdomain);
    }

    /*
     * This constructor is invoked by the operator()'s which take
     * a combination of integer and Range arguments.  It's not intended
     * for end-user use.
     */
    template<int N_rank2, typename R0, typename R1, typename R2, typename R3, typename R4,
        typename R5, typename R6, typename R7, typename R8, typename R9, typename R10>
    Array(Array<T_numtype,N_rank2, Policy>& array, R0 r0, R1 r1, R2 r2,
        R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9, R10 r10)
    {
        constructSlice(array, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10);
    }

    //////////////////////////////////////////////
    // Member functions
    //////////////////////////////////////////////

    constexpr decltype(auto)    base() const
    { return storage_.base(); }

    constexpr int                               base(int rank) const
    { return storage_.base(rank); }

    iterator                          begin() 
    { return iterator(*this); }

    const_iterator                    begin() const
    { return const_iterator(*this); }

    T_iterator                        beginFast() const
    { return T_iterator(*this); }

    // Deprecated: now extractComponent(...)
    template<typename P_numtype2>
    Array<P_numtype2,N_rank, Policy>          chopComponent(P_numtype2 a, int compNum,
                                                    int numComponents) const
    { return extractComponent(a, compNum, numComponents); }

    int                               cols() const
    { return length_[1]; }

    int                               columns() const
    { return length_[1]; }

    T_array                           copy() const;

    // data_ always refers to the point (0,0,...,0) which may
    // not be in the array if the base is not zero in each rank.
    // These data() routines return a pointer to the first
    // element in the array (but note that it may not be
    // stored first in memory if some ranks are stored descending).

    template<typename Policy>
    diffType dataOffset_impl(Policy) const {
        return dot(storage_.base(), stride_);
    }

    template<int N_base>
    constexpr diffType dataOffset_impl(array_policy::StaticFortranArrayPolicy<N_base>) const {
        return N_base;
    }


    diffType dataOffset() const {
        return dataOffset_impl(Policy{});
    }

  //  diffType                               dataOffset() const
  //{ return dot(storage_.base(), stride_); }

    const T_numtype* _bz_restrict         data() const
    { return data_ + dataOffset(); }

    T_numtype* _bz_restrict               data() 
    { return data_ + dataOffset(); }

    // These dataZero() routines refer to the point (0,0,...,0)
    // which may not be in the array if the bases are nonzero.
    
    const T_numtype* _bz_restrict         dataZero() const
    { return data_; }

    T_numtype* _bz_restrict               dataZero()
    { return data_; }

    // These dataFirst() routines refer to the element in the
    // array which falls first in memory.

    diffType                               dataFirstOffset() const
    {
      diffType pos = 0;

        // Used to use tinyvector expressions:
        // return data_ + dot(storage_.base()
        //     + (1 - storage_.ascendingFlag()) * (length_ - 1), stride_);

        for (int i=0; i < N_rank; ++i)
           pos += (storage_.base(i) + (1-storage_.isRankStoredAscending(i)) *
		   (length_(i)-1)) * stride_(i);

        return pos;
    }
    
    const T_numtype* _bz_restrict         dataFirst() const
    { return data_ + dataFirstOffset(); }

    T_numtype* _bz_restrict               dataFirst()
    { return data_ + dataFirstOffset(); }

    int                               depth() const
    { return length_[2]; }

    int                               dimensions() const
    { return N_rank; }

    RectDomain<N_rank>                domain() const
    { return RectDomain<N_rank>(lbound(), ubound()); }

    void dumpStructureInformation(ostream& os = cout) const;

    iterator                          end()
    { return iterator(*this,0); }

    const_iterator                    end() const
    { return const_iterator(*this,0); }

    int                               extent(int rank) const
    { return length_[rank]; }

    const TinyVector<int,N_rank>&     extent() const
    { return length_; }

    template<typename P_numtype2>
    Array<P_numtype2,N_rank, Policy>          extractComponent(P_numtype2, int compNum,
                                                       int numComponents) const;

    void                              free() 
    {
        T_base::changeToNullBlock();
        length_ = 0;
    }
 
    bool                              isMajorRank(int rank) const 
    { return storage_.ordering(rank) == N_rank-1; }
    bool                              isMinorRank(int rank) const 
    { return storage_.ordering(rank) != N_rank-1; }
    bool                              isRankStoredAscending(int rank) const 
    { return storage_.isRankStoredAscending(rank); }

    bool                              isStorageContiguous() const;

    constexpr int                               lbound(int rank) const 
    { return base(rank); }
    constexpr TinyVector<int,N_rank>            lbound() const
    { return base(); }

    constexpr int                               length(int rank) const 
    { return length_[rank]; }
    constexpr const TinyVector<int, N_rank>&    length() const
    { return length_; }

    void                              makeUnique();

    sizeType                               numElements() const 
    { return _bz_returntype<sizeType>::product(length_); }

    // NEEDS_WORK -- Expose the numReferences() method
    // memory_block_reference_t::numReferences;

    // The storage_.ordering_ array is a list of dimensions from
    // the most minor (stride 1) to major dimension.  Generally,
    // ordering(0) will return the dimension which has the smallest
    // stride, and ordering(N_rank-1) will return the dimension with
    // the largest stride.
    int                               ordering(int storageRankIndex) const
    { return storage_.ordering(storageRankIndex); }

    constexpr decltype(auto)    ordering() const
    { return storage_.ordering(); }

    void                              transposeSelf(int r0, int r1, int r2=0, 
        int r3=0, int r4=0, int r5=0, int r6=0, int r7=0, int r8=0, int 
        r9=0, int r10=0);
    T_array                           transpose(int r0, int r1, int r2=0,
        int r3=0, int r4=0, int r5=0, int r6=0, int r7=0, int r8=0, int
        r9=0, int r10=0) const;

  static int                               rank()
    { return rank_; }

    void                              reference(const T_array&);
    void                              weakReference(const T_array&);

    // Added by Derrick Bass
    T_array                           reindex(const TinyVector<int,N_rank>&);
    void                              reindexSelf(
                                        const TinyVector<int,N_rank>&);


//    constexpr TinyBasedVector(std::initializer_list<T_numtype> list) : TinyBasedVector(list, std::make_index_sequence<n_length>())
//    {
//    }
//
//
//private:
//    template<size_t...Is>
//    void assignconstexpr TinyBasedVector(std::initializer_list<T_numtype> list, std::integer_sequence<size_t, Is...>)
//        : data_{
//        (
//#if _MSVC_LANG > 201403
//            Is >= list.size()
//            ?
//            T_numtype()
//            :
//            std::move(list.begin()[Is])
//            //std::move(*(std::next(list.begin(), Is)))
//#else
//            std::move(list.begin()[Is])
//#endif
//            )...
//    } {}

    template<typename ...Args, typename Indices = std::make_index_sequence<sizeof...(Args)>>
    void                              resize_static(int extend_0, Args&&... extents) {
        static_assert(sizeof...(extents) + 1 == N_rank, "rank mismatch");

        auto zeroOffset_old = zeroOffset_;
        length_ = { extend_0, extents ... };

        // Compute strides
        computeStrides();

        // Adjust the base of the array to account for non-zero base
        // indices and reversals
        data_ += zeroOffset_- zeroOffset_old;
    }

    void                              resize_static(const TinyVector<int, N_rank>& length) {

        auto zeroOffset_old = zeroOffset_;
        length_ = length;

        // Compute strides
        computeStrides();

        // Adjust the base of the array to account for non-zero base
        // indices and reversals
        data_ += zeroOffset_ - zeroOffset_old;
    }

    void                              resize(int extent);
    void                              resize(int extent1, int extent2);
    void                              resize(int extent1, int extent2,
                                        int extent3);
    void                              resize(int extent1, int extent2,
                                        int extent3, int extent4);
    void                              resize(int extent1, int extent2,
                                        int extent3, int extent4, int extent5);
    void                              resize(int extent1, int extent2,
                                        int extent3, int extent4, int extent5,
                                        int extent6);
    void                              resize(int extent1, int extent2,
                                        int extent3, int extent4, int extent5,
                                        int extent6, int extent7);
    void                              resize(int extent1, int extent2,
                                        int extent3, int extent4, int extent5,
                                        int extent6, int extent7, int extent8);
    void                              resize(int extent1, int extent2,
                                        int extent3, int extent4, int extent5,
                                        int extent6, int extent7, int extent8,
                                        int extent9);
    void                              resize(int extent1, int extent2,
                                        int extent3, int extent4, int extent5,
                                        int extent6, int extent7, int extent8,
                                        int extent9, int extent10);
    void                              resize(int extent1, int extent2,
                                        int extent3, int extent4, int extent5,
                                        int extent6, int extent7, int extent8,
                                        int extent9, int extent10, 
                                        int extent11);


    void                              resize(Range r1);
    void                              resize(Range r1, Range r2);
    void                              resize(Range r1, Range r2, Range r3);
    void                              resize(Range r1, Range r2, Range r3,
                                        Range r4);
    void                              resize(Range r1, Range r2, Range r3,
                                        Range r4, Range r5);
    void                              resize(Range r1, Range r2, Range r3,
                                        Range r4, Range r5, Range r6);
    void                              resize(Range r1, Range r2, Range r3,
                                        Range r4, Range r5, Range r6,
                                        Range r7);
    void                              resize(Range r1, Range r2, Range r3,
                                        Range r4, Range r5, Range r6,
                                        Range r7, Range r8);
    void                              resize(Range r1, Range r2, Range r3,
                                        Range r4, Range r5, Range r6,
                                        Range r7, Range r8, Range r9);
    void                              resize(Range r1, Range r2, Range r3,
                                        Range r4, Range r5, Range r6,
                                        Range r7, Range r8, Range r9, 
                                        Range r10);
    void                              resize(Range r1, Range r2, Range r3,
                                        Range r4, Range r5, Range r6,
                                        Range r7, Range r8, Range r9, 
                                        Range r10, Range r11);

    void                              resize(const TinyVector<int,N_rank>&);
 

    void                              resizeAndPreserve(const TinyVector<int,
                                                                   N_rank>&);
    void                              resizeAndPreserve(int extent);
    void                              resizeAndPreserve(int extent1, 
                                        int extent2);
    void                              resizeAndPreserve(int extent1, 
                                        int extent2, int extent3);
    void                              resizeAndPreserve(int extent1,
                                        int extent2, int extent3, int extent4);
    void                              resizeAndPreserve(int extent1,
                                        int extent2, int extent3, int extent4,
                                        int extent5);
    void                              resizeAndPreserve(int extent1,
                                        int extent2, int extent3, int extent4,
                                        int extent5, int extent6);
    void                              resizeAndPreserve(int extent1,
                                        int extent2, int extent3, int extent4,
                                        int extent5, int extent6, int extent7);
    void                              resizeAndPreserve(int extent1,
                                        int extent2, int extent3, int extent4,
                                        int extent5, int extent6, int extent7,
                                        int extent8);
    void                              resizeAndPreserve(int extent1,
                                        int extent2, int extent3, int extent4,
                                        int extent5, int extent6, int extent7,
                                        int extent8, int extent9);
    void                              resizeAndPreserve(int extent1,
                                        int extent2, int extent3, int extent4,
                                        int extent5, int extent6, int extent7,
                                        int extent8, int extent9, 
                                        int extent10);
    void                              resizeAndPreserve(int extent1,
                                        int extent2, int extent3, int extent4,
                                        int extent5, int extent6, int extent7,
                                        int extent8, int extent9, int extent10,
                                        int extent11);

    // NEEDS_WORK -- resizeAndPreserve(Range,...)
    // NEEDS_WORK -- resizeAndPreserve(const Domain<N_rank>&);

    T_array                           reverse(int rank);
    void                              reverseSelf(int rank);

    int                               rows() const
    { return length_[0]; }
    
    void                              setStorage(GeneralArrayStorage<N_rank>);
    
    void                              slice(int rank, Range r);

    const TinyVector<int, N_rank>&    shape() const
    { return length_; }

    sizeType                               size() const
    { return numElements(); }

  /** Returns the length of the array storage. This can be larger than
      the number of elements due to padding to meet alignment
      requirements. If you want to extract the array data to, for
      example, write it to disk, this is the size of the block
      needed. \todo Is this safe if there is no block? */
    sizeType                               storageSize() const
  { return T_base::blockLength(); }

    constexpr const TinyVector<diffType, N_rank>&    stride() const
    { return stride_; }

  constexpr diffType                               stride(int rank) const
    { return stride_[rank]; }

    bool                              threadLocal(bool disableLock = true) const
        { return T_base::lockReferenceCount(!disableLock); }

    constexpr int                               ubound(int rank) const
    { return base(rank) + length_(rank) - 1; }

    /*constexpr*/ TinyVector<int, N_rank>           ubound() const
    { 
        TinyVector<int, N_rank> ub;
        for (int i=0; i < N_rank; ++i)
          ub(i) = base(i) + extent(i) - 1;
        // WAS: ub = base() + extent() - 1;
        return ub;
    }

    int                               zeroOffset() const
    { return zeroOffset_; }

  /** Returns true if the array is aligned on a simd vector width. */
  bool isVectorAligned(diffType offset) const
  { return simdTypes<T_numtype>::isVectorAligned(dataFirst()+offset); };

    //////////////////////////////////////////////
    // Debugging routines
    //////////////////////////////////////////////

    bool isInRangeForDim(int i, int d) const {
        return i >= base(d) && (ordering(d) == N_rank - 1 && length_[d] == -1 || (i - base(d)) < length_[d]);
    }

    //bool isInRange(int i0) const {
    //    return i0 >= base(0) && (ordering(0) == N_rank-1 && length_[0] == -1 || (i0 - base(0)) < length_[0]);
    //}

    //bool isInRange(int i0, int i1) const {
    //    return i0 >= base(0) && (i0 - base(0)) < length_[0]
    //        && i1 >= base(1) && (i1 - base(1)) < length_[1];
    //}

    //bool isInRange(int i0, int i1, int i2) const {
    //    return i0 >= base(0) && (i0 - base(0)) < length_[0]
    //        && i1 >= base(1) && (i1 - base(1)) < length_[1]
    //        && i2 >= base(2) && (i2 - base(2)) < length_[2];
    //}

    //bool isInRange(int i0, int i1, int i2, int i3) const {
    //    return i0 >= base(0) && (i0 - base(0)) < length_[0]
    //        && i1 >= base(1) && (i1 - base(1)) < length_[1]
    //        && i2 >= base(2) && (i2 - base(2)) < length_[2]
    //        && i3 >= base(3) && (i3 - base(3)) < length_[3];
    //}

    //bool isInRange(int i0, int i1, int i2, int i3, int i4) const {
    //    return i0 >= base(0) && (i0 - base(0)) < length_[0]
    //        && i1 >= base(1) && (i1 - base(1)) < length_[1]
    //        && i2 >= base(2) && (i2 - base(2)) < length_[2]
    //        && i3 >= base(3) && (i3 - base(3)) < length_[3]
    //        && i4 >= base(4) && (i4 - base(4)) < length_[4];
    //}

    //bool isInRange(int i0, int i1, int i2, int i3, int i4, int i5) const {
    //    return i0 >= base(0) && (i0 - base(0)) < length_[0]
    //        && i1 >= base(1) && (i1 - base(1)) < length_[1]
    //        && i2 >= base(2) && (i2 - base(2)) < length_[2]
    //        && i3 >= base(3) && (i3 - base(3)) < length_[3]
    //        && i4 >= base(4) && (i4 - base(4)) < length_[4]
    //        && i5 >= base(5) && (i5 - base(5)) < length_[5];
    //}

    //bool isInRange(int i0, int i1, int i2, int i3, int i4, int i5, int i6) const {
    //    return i0 >= base(0) && (i0 - base(0)) < length_[0]
    //        && i1 >= base(1) && (i1 - base(1)) < length_[1]
    //        && i2 >= base(2) && (i2 - base(2)) < length_[2]
    //        && i3 >= base(3) && (i3 - base(3)) < length_[3]
    //        && i4 >= base(4) && (i4 - base(4)) < length_[4]
    //        && i5 >= base(5) && (i5 - base(5)) < length_[5]
    //        && i6 >= base(6) && (i6 - base(6)) < length_[6];
    //}

    //bool isInRange(int i0, int i1, int i2, int i3, int i4,
    //    int i5, int i6, int i7) const {
    //    return i0 >= base(0) && (i0 - base(0)) < length_[0]
    //        && i1 >= base(1) && (i1 - base(1)) < length_[1]
    //        && i2 >= base(2) && (i2 - base(2)) < length_[2]
    //        && i3 >= base(3) && (i3 - base(3)) < length_[3]
    //        && i4 >= base(4) && (i4 - base(4)) < length_[4]
    //        && i5 >= base(5) && (i5 - base(5)) < length_[5]
    //        && i6 >= base(6) && (i6 - base(6)) < length_[6]
    //        && i7 >= base(7) && (i7 - base(7)) < length_[7];
    //}

    //bool isInRange(int i0, int i1, int i2, int i3, int i4,
    //    int i5, int i6, int i7, int i8) const {
    //    return i0 >= base(0) && (i0 - base(0)) < length_[0]
    //        && i1 >= base(1) && (i1 - base(1)) < length_[1]
    //        && i2 >= base(2) && (i2 - base(2)) < length_[2]
    //        && i3 >= base(3) && (i3 - base(3)) < length_[3]
    //        && i4 >= base(4) && (i4 - base(4)) < length_[4]
    //        && i5 >= base(5) && (i5 - base(5)) < length_[5]
    //        && i6 >= base(6) && (i6 - base(6)) < length_[6]
    //        && i7 >= base(7) && (i7 - base(7)) < length_[7]
    //        && i8 >= base(8) && (i8 - base(8)) < length_[8];
    //}

    //bool isInRange(int i0, int i1, int i2, int i3, int i4,
    //    int i5, int i6, int i7, int i8, int i9) const {
    //    return i0 >= base(0) && (i0 - base(0)) < length_[0]
    //        && i1 >= base(1) && (i1 - base(1)) < length_[1]
    //        && i2 >= base(2) && (i2 - base(2)) < length_[2]
    //        && i3 >= base(3) && (i3 - base(3)) < length_[3]
    //        && i4 >= base(4) && (i4 - base(4)) < length_[4]
    //        && i5 >= base(5) && (i5 - base(5)) < length_[5]
    //        && i6 >= base(6) && (i6 - base(6)) < length_[6]
    //        && i7 >= base(7) && (i7 - base(7)) < length_[7]
    //        && i8 >= base(8) && (i8 - base(8)) < length_[8]
    //        && i9 >= base(9) && (i9 - base(9)) < length_[9];
    //}

    //bool isInRange(int i0, int i1, int i2, int i3, int i4,
    //    int i5, int i6, int i7, int i8, int i9, int i10) const {
    //    return i0 >= base(0) && (i0 - base(0)) < length_[0]
    //        && i1 >= base(1) && (i1 - base(1)) < length_[1]
    //        && i2 >= base(2) && (i2 - base(2)) < length_[2]
    //        && i3 >= base(3) && (i3 - base(3)) < length_[3]
    //        && i4 >= base(4) && (i4 - base(4)) < length_[4]
    //        && i5 >= base(5) && (i5 - base(5)) < length_[5]
    //        && i6 >= base(6) && (i6 - base(6)) < length_[6]
    //        && i7 >= base(7) && (i7 - base(7)) < length_[7]
    //        && i8 >= base(8) && (i8 - base(8)) < length_[8]
    //        && i9 >= base(9) && (i9 - base(9)) < length_[9]
    //        && i10 >= base(10) && (i10 - base(10)) < length_[10];
    //}

template<typename... Ns>
struct all_int;
template<typename N, typename... Ns>
struct all_int<N, Ns...> :
    std::conditional <
    std::is_same<int, std::remove_cv_t<std::remove_reference_t<N>>>::value,
    all_int<Ns...>,
    std::false_type
    >::type
{};
template<>
struct all_int<> :
    std::true_type
{};

    template<typename... Ns>
    typename std::enable_if<
        all_int<Ns...>::value,
        bool 
    >::type
        isInRange(Ns... ns) const {
        return isInRange(T_index(ns...));
    }

    bool isInRange(const T_index& index) const {
        for (int i=0; i < N_rank; ++i)
            if (index[i] < base(i) || 
               ((ordering(i) != N_rank - 1 || length_[i] != -1) && (index[i] - base(i)) >= length_[i]))
                return false;

        return true;
    }

    bool assertInRange(const T_index& BZ_DEBUG_PARAM(index)) const {
        BZPRECHECK(isInRange(index), "Array index out of range: " << index
            << endl << "Lower bounds: " << storage_.base() << endl
            <<         "Length:       " << length_ << endl);
        return true;
    }

    bool assertInRange(int BZ_DEBUG_PARAM(i0)) const {
        BZPRECHECK(isInRange(i0), "Array index out of range: " << i0
            << endl << "Lower bounds: " << storage_.base() << endl
            <<         "Length:       " << length_ << endl);
        return true;
    }

    bool assertInRange(int BZ_DEBUG_PARAM(i0), int BZ_DEBUG_PARAM(i1)) const {
        BZPRECHECK(isInRange(i0,i1), "Array index out of range: (" 
            << i0 << ", " << i1 << ")"
            << endl << "Lower bounds: " << storage_.base() << endl
            <<         "Length:       " << length_ << endl);
        return true;
    }

    bool assertInRange(int BZ_DEBUG_PARAM(i0), int BZ_DEBUG_PARAM(i1),
        int BZ_DEBUG_PARAM(i2)) const
    {
        BZPRECHECK(isInRange(i0,i1,i2), "Array index out of range: ("
            << i0 << ", " << i1 << ", " << i2 << ")"
            << endl << "Lower bounds: " << storage_.base() << endl
            <<         "Length:       " << length_ << endl);
        return true;
    }

    bool assertInRange(int BZ_DEBUG_PARAM(i0), int BZ_DEBUG_PARAM(i1),
        int BZ_DEBUG_PARAM(i2), int BZ_DEBUG_PARAM(i3)) const
    {
        BZPRECHECK(isInRange(i0,i1,i2,i3), "Array index out of range: ("
            << i0 << ", " << i1 << ", " << i2 << ", " << i3 << ")"
            << endl << "Lower bounds: " << storage_.base() << endl
            <<         "Length:       " << length_ << endl);
        return true;
    }

    bool assertInRange(int BZ_DEBUG_PARAM(i0), int BZ_DEBUG_PARAM(i1),
        int BZ_DEBUG_PARAM(i2), int BZ_DEBUG_PARAM(i3),
        int BZ_DEBUG_PARAM(i4)) const
    {
        BZPRECHECK(isInRange(i0,i1,i2,i3,i4), "Array index out of range: ("
            << i0 << ", " << i1 << ", " << i2 << ", " << i3 
            << ", " << i4 << ")"
            << endl << "Lower bounds: " << storage_.base() << endl
            <<         "Length:       " << length_ << endl);
        return true;
    }

    bool assertInRange(int BZ_DEBUG_PARAM(i0), int BZ_DEBUG_PARAM(i1),
        int BZ_DEBUG_PARAM(i2), int BZ_DEBUG_PARAM(i3), int BZ_DEBUG_PARAM(i4),
        int BZ_DEBUG_PARAM(i5)) const
    {
        BZPRECHECK(isInRange(i0,i1,i2,i3,i4,i5), "Array index out of range: ("
            << i0 << ", " << i1 << ", " << i2 << ", " << i3
            << ", " << i4 << ", " << i5 << ")"
            << endl << "Lower bounds: " << storage_.base() << endl
            <<         "Length:       " << length_ << endl);
        return true;
    }

    bool assertInRange(int BZ_DEBUG_PARAM(i0), int BZ_DEBUG_PARAM(i1),
        int BZ_DEBUG_PARAM(i2), int BZ_DEBUG_PARAM(i3), int BZ_DEBUG_PARAM(i4),
        int BZ_DEBUG_PARAM(i5), int BZ_DEBUG_PARAM(i6)) const
    {
        BZPRECHECK(isInRange(i0,i1,i2,i3,i4,i5,i6), 
            "Array index out of range: ("
            << i0 << ", " << i1 << ", " << i2 << ", " << i3
            << ", " << i4 << ", " << i5 << ", " << i6 << ")"
            << endl << "Lower bounds: " << storage_.base() << endl
            <<         "Length:       " << length_ << endl);
        return true;
    }

    bool assertInRange(int BZ_DEBUG_PARAM(i0), int BZ_DEBUG_PARAM(i1),
        int BZ_DEBUG_PARAM(i2), int BZ_DEBUG_PARAM(i3), int BZ_DEBUG_PARAM(i4),
        int BZ_DEBUG_PARAM(i5), int BZ_DEBUG_PARAM(i6),
        int BZ_DEBUG_PARAM(i7)) const
    {
        BZPRECHECK(isInRange(i0,i1,i2,i3,i4,i5,i6,i7),
            "Array index out of range: ("
            << i0 << ", " << i1 << ", " << i2 << ", " << i3
            << ", " << i4 << ", " << i5 << ", " << i6 << ", " << i7 << ")"
            << endl << "Lower bounds: " << storage_.base() << endl
            <<         "Length:       " << length_ << endl);
        return true;
    }

    bool assertInRange(int BZ_DEBUG_PARAM(i0), int BZ_DEBUG_PARAM(i1),
        int BZ_DEBUG_PARAM(i2), int BZ_DEBUG_PARAM(i3), int BZ_DEBUG_PARAM(i4),
        int BZ_DEBUG_PARAM(i5), int BZ_DEBUG_PARAM(i6), int BZ_DEBUG_PARAM(i7),
        int BZ_DEBUG_PARAM(i8)) const
    {
        BZPRECHECK(isInRange(i0,i1,i2,i3,i4,i5,i6,i7,i8),
            "Array index out of range: ("
            << i0 << ", " << i1 << ", " << i2 << ", " << i3
            << ", " << i4 << ", " << i5 << ", " << i6 << ", " << i7 
            << ", " << i8 << ")"
            << endl << "Lower bounds: " << storage_.base() << endl
            <<         "Length:       " << length_ << endl);
        return true;
    }

    bool assertInRange(int BZ_DEBUG_PARAM(i0), int BZ_DEBUG_PARAM(i1),
        int BZ_DEBUG_PARAM(i2), int BZ_DEBUG_PARAM(i3), int BZ_DEBUG_PARAM(i4),
        int BZ_DEBUG_PARAM(i5), int BZ_DEBUG_PARAM(i6), int BZ_DEBUG_PARAM(i7),
        int BZ_DEBUG_PARAM(i8), int BZ_DEBUG_PARAM(i9)) const
    {
        BZPRECHECK(isInRange(i0,i1,i2,i3,i4,i5,i6,i7,i8,i9),
            "Array index out of range: ("
            << i0 << ", " << i1 << ", " << i2 << ", " << i3
            << ", " << i4 << ", " << i5 << ", " << i6 << ", " << i7
            << ", " << i8 << ", " << i9 << ")"
            << endl << "Lower bounds: " << storage_.base() << endl
            <<         "Length:       " << length_ << endl);
        return true;
    }

    bool assertInRange(int BZ_DEBUG_PARAM(i0), int BZ_DEBUG_PARAM(i1),
        int BZ_DEBUG_PARAM(i2), int BZ_DEBUG_PARAM(i3), int BZ_DEBUG_PARAM(i4),
        int BZ_DEBUG_PARAM(i5), int BZ_DEBUG_PARAM(i6), int BZ_DEBUG_PARAM(i7),
        int BZ_DEBUG_PARAM(i8), int BZ_DEBUG_PARAM(i9),
        int BZ_DEBUG_PARAM(i10)) const
    {
        BZPRECHECK(isInRange(i0,i1,i2,i3,i4,i5,i6,i7,i8,i9,i10),
            "Array index out of range: ("
            << i0 << ", " << i1 << ", " << i2 << ", " << i3
            << ", " << i4 << ", " << i5 << ", " << i6 << ", " << i7
            << ", " << i8 << ", " << i9 << ", " << i10 << ")"
            << endl << "Lower bounds: " << storage_.base() << endl
            <<         "Length:       " << length_ << endl);
        return true;
    }

    //////////////////////////////////////////////
    // Subscripting operators
    //////////////////////////////////////////////
//#ifdef BZ_HAVE_SUBSCRIPTING_OPERATORS
    template<int N_rank2>
    const T_numtype& _bz_restrict operator()(const TinyVector<int,N_rank2>& index) const
    {
        assertInRange(index);
        return data_[dot(index, stride_)];
    }

    template<int N_rank2>
    T_numtype& _bz_restrict operator()(const TinyVector<int,N_rank2>& index) 
    {
        assertInRange(index);
        return data_[dot(index, stride_)];
    }

    const T_numtype& _bz_restrict operator()(TinyVector<int,1> index) const
    {
        assertInRange(index[0]);
        return data_[index[0] * stride_[0]];
    }

    T_numtype& operator()(TinyVector<int,1> index)
    {
        assertInRange(index[0]);
        return data_[index[0] * stride_[0]];
    }

    const T_numtype& _bz_restrict operator()(TinyVector<int,2> index) const
    {
        assertInRange(index[0], index[1]);
        return data_[index[0] * stride_[0] 
		     + index[1] * stride_[1]];
    }

    T_numtype& operator()(TinyVector<int,2> index)
    {
        assertInRange(index[0], index[1]);
        return data_[index[0] * stride_[0] 
		     + index[1] * stride_[1]];
    }

    const T_numtype& _bz_restrict operator()(TinyVector<int,3> index) const
    {
        assertInRange(index[0], index[1], index[2]);
        return data_[(index[0]) * stride_[0] 
		     + index[1] * stride_[1]
		     + index[2] * stride_[2]];
    }

    T_numtype& operator()(TinyVector<int,3> index)
    {
        assertInRange(index[0], index[1], index[2]);
        return data_[(index[0]) * stride_[0]
		     + index[1] * stride_[1]
		     + index[2] * stride_[2]];
    }

    const T_numtype& _bz_restrict operator()(const TinyVector<int,4>& index) const
    {
        assertInRange(index[0], index[1], index[2], index[3]);
        return data_[(index[0]) * stride_[0] 
		     + index[1] * stride_[1]
		     + index[2] * stride_[2] + index[3] * stride_[3]];
    }

    T_numtype& operator()(const TinyVector<int,4>& index)
    {
        assertInRange(index[0], index[1], index[2], index[3]);
        return data_[(index[0]) * stride_[0] 
		     + index[1] * stride_[1]
		     + index[2] * stride_[2] + index[3] * stride_[3]];
    }

    const T_numtype& _bz_restrict operator()(const TinyVector<int,5>& index) const
    {
        assertInRange(index[0], index[1], index[2], index[3],
            index[4]);
        return data_[(index[0]) * stride_[0] 
		     + index[1] * stride_[1]
		     + index[2] * stride_[2] + index[3] * stride_[3]
		     + index[4] * stride_[4]];
    }

    T_numtype& operator()(const TinyVector<int,5>& index)
    {
        assertInRange(index[0], index[1], index[2], index[3],
            index[4]);
        return data_[(index[0]) * stride_[0]
		     + index[1] * stride_[1]
		     + index[2] * stride_[2] + index[3] * stride_[3]
		     + index[4] * stride_[4]];
    }

    const T_numtype& _bz_restrict operator()(const TinyVector<int,6>& index) const
    {
        assertInRange(index[0], index[1], index[2], index[3],
            index[4], index[5]);
        return data_[(index[0]) * stride_[0]
		     + index[1] * stride_[1]
		     + index[2] * stride_[2] + index[3] * stride_[3]
		     + index[4] * stride_[4] + index[5] * stride_[5]];
    }

    T_numtype& operator()(const TinyVector<int,6>& index)
    {
        assertInRange(index[0], index[1], index[2], index[3],
            index[4], index[5]);
        return data_[(index[0]) * stride_[0]
		     + index[1] * stride_[1]
		     + index[2] * stride_[2] + index[3] * stride_[3]
		     + index[4] * stride_[4] + index[5] * stride_[5]];
    }

    const T_numtype& _bz_restrict operator()(const TinyVector<int,7>& index) const
    {
        assertInRange(index[0], index[1], index[2], index[3],
            index[4], index[5], index[6]);
        return data_[(index[0]) * stride_[0] 
		     + index[1] * stride_[1]
		     + index[2] * stride_[2] + index[3] * stride_[3]
		     + index[4] * stride_[4] + index[5] * stride_[5]
		     + index[6] * stride_[6]];
    }

    T_numtype& operator()(const TinyVector<int,7>& index)
    {
        assertInRange(index[0], index[1], index[2], index[3],
            index[4], index[5], index[6]);
        return data_[(index[0]) * stride_[0] 
		     + index[1] * stride_[1]
		     + index[2] * stride_[2] + index[3] * stride_[3]
		     + index[4] * stride_[4] + index[5] * stride_[5]
		     + index[6] * stride_[6]];
    }

    const T_numtype& _bz_restrict operator()(const TinyVector<int,8>& index) const
    {
        assertInRange(index[0], index[1], index[2], index[3],
            index[4], index[5], index[6], index[7]);
        return data_[(index[0]) * stride_[0]
		     + index[1] * stride_[1]
		     + index[2] * stride_[2] + index[3] * stride_[3]
		     + index[4] * stride_[4] + index[5] * stride_[5]
		     + index[6] * stride_[6] + index[7] * stride_[7]];
    }

    T_numtype& operator()(const TinyVector<int,8>& index)
    {
        assertInRange(index[0], index[1], index[2], index[3],
            index[4], index[5], index[6], index[7]);
        return data_[(index[0]) * stride_[0] 
		     + index[1] * stride_[1]
		     + index[2] * stride_[2] + index[3] * stride_[3]
		     + index[4] * stride_[4] + index[5] * stride_[5]
		     + index[6] * stride_[6] + index[7] * stride_[7]];
    }

    const T_numtype& _bz_restrict operator()(const TinyVector<int,9>& index) const
    {
        assertInRange(index[0], index[1], index[2], index[3],
            index[4], index[5], index[6], index[7], index[8]);
        return data_[(index[0]) * stride_[0]
		     + index[1] * stride_[1]
		     + index[2] * stride_[2] + index[3] * stride_[3]
		     + index[4] * stride_[4] + index[5] * stride_[5]
		     + index[6] * stride_[6] + index[7] * stride_[7]
		     + index[8] * stride_[8]];
    }

    T_numtype& operator()(const TinyVector<int,9>& index)
    {
        assertInRange(index[0], index[1], index[2], index[3],
            index[4], index[5], index[6], index[7], index[8]);
        return data_[(index[0]) * stride_[0]
		     + index[1] * stride_[1]
		     + index[2] * stride_[2] + index[3] * stride_[3]
		     + index[4] * stride_[4] + index[5] * stride_[5]
		     + index[6] * stride_[6] + index[7] * stride_[7]
		     + index[8] * stride_[8]];
    }

    const T_numtype& _bz_restrict operator()(const TinyVector<int,10>& index) const
    {
        assertInRange(index[0], index[1], index[2], index[3],
            index[4], index[5], index[6], index[7], index[8], index[9]);
        return data_[(index[0]) * stride_[0]
		     + index[1] * stride_[1]
		     + index[2] * stride_[2] + index[3] * stride_[3]
		     + index[4] * stride_[4] + index[5] * stride_[5]
		     + index[6] * stride_[6] + index[7] * stride_[7]
		     + index[8] * stride_[8] + index[9] * stride_[9]];
    }

    T_numtype& operator()(const TinyVector<int,10>& index)
    {
        assertInRange(index[0], index[1], index[2], index[3],
            index[4], index[5], index[6], index[7], index[8], index[9]);
        return data_[(index[0]) * stride_[0]
		     + index[1] * stride_[1]
		     + index[2] * stride_[2] + index[3] * stride_[3]
		     + index[4] * stride_[4] + index[5] * stride_[5]
		     + index[6] * stride_[6] + index[7] * stride_[7]
		     + index[8] * stride_[8] + index[9] * stride_[9]];
    }

    const T_numtype& _bz_restrict operator()(const TinyVector<int,11>& index) const
    {
        assertInRange(index[0], index[1], index[2], index[3],
            index[4], index[5], index[6], index[7], index[8], index[9],
            index[10]);
        return data_[(index[0]) * stride_[0] 
		     + index[1] * stride_[1]
		     + index[2] * stride_[2] + index[3] * stride_[3]
		     + index[4] * stride_[4] + index[5] * stride_[5]
		     + index[6] * stride_[6] + index[7] * stride_[7]
		     + index[8] * stride_[8] + index[9] * stride_[9]
		     + index[10] * stride_[10]];
    }

    T_numtype& operator()(const TinyVector<int,11>& index)
    {
        assertInRange(index[0], index[1], index[2], index[3],
            index[4], index[5], index[6], index[7], index[8], index[9],
            index[10]);
        return data_[(index[0]) * stride_[0]
		     + index[1] * stride_[1]
		     + index[2] * stride_[2] + index[3] * stride_[3]
		     + index[4] * stride_[4] + index[5] * stride_[5]
		     + index[6] * stride_[6] + index[7] * stride_[7]
		     + index[8] * stride_[8] + index[9] * stride_[9]
		     + index[10] * stride_[10]];
    }
//#endif

    const T_numtype& _bz_restrict operator()(int i0) const
    { 
        assertInRange(i0);
        return data_[(i0) * stride_[0]]; 
    }

    T_numtype& _bz_restrict operator()(int i0) 
    {
        assertInRange(i0);
        return data_[(i0) * stride_[0]];
    }

    const T_numtype& _bz_restrict operator()(int i0, int i1) const
    { 
        assertInRange(i0, i1);
        return data_[(i0) * stride_[0] + i1 * stride_[1]];
    }

    T_numtype& _bz_restrict operator()(int i0, int i1)
    {
        assertInRange(i0, i1);
        return data_[(i0) * stride_[0] + i1 * stride_[1]];
    }

    const T_numtype& _bz_restrict operator()(int i0, int i1, int i2) const
    {
        assertInRange(i0, i1, i2);
        return data_[(i0) * stride_[0] + i1 * stride_[1]
		     + i2 * stride_[2]];
    }

    T_numtype& _bz_restrict operator()(int i0, int i1, int i2) 
    {
        assertInRange(i0, i1, i2);
        return data_[(i0) * stride_[0] + i1 * stride_[1]
		     + i2 * stride_[2]];
    }

    const T_numtype& _bz_restrict operator()(int i0, int i1, int i2, int i3) const
    {
        assertInRange(i0, i1, i2, i3);
        return data_[(i0) * stride_[0] + i1 * stride_[1]
		     + i2 * stride_[2] + i3 * stride_[3]];
    }

    T_numtype& _bz_restrict operator()(int i0, int i1, int i2, int i3)
    {
        assertInRange(i0, i1, i2, i3);
        return data_[(i0) * stride_[0] + i1 * stride_[1]
		     + i2 * stride_[2] + i3 * stride_[3]];
    }

    const T_numtype& _bz_restrict operator()(int i0, int i1, int i2, int i3,
        int i4) const
    {
        assertInRange(i0, i1, i2, i3, i4);
        return data_[(i0) * stride_[0] + i1 * stride_[1]
		     + i2 * stride_[2] + i3 * stride_[3] + i4 * stride_[4]];
    }

    T_numtype& _bz_restrict operator()(int i0, int i1, int i2, int i3,
        int i4)
    {
        assertInRange(i0, i1, i2, i3, i4);
        return data_[(i0) * stride_[0] + i1 * stride_[1]
		     + i2 * stride_[2] + i3 * stride_[3] + i4 * stride_[4]];
    }

    const T_numtype& _bz_restrict operator()(int i0, int i1, int i2, int i3,
        int i4, int i5) const
    {
        assertInRange(i0, i1, i2, i3, i4, i5);
        return data_[(i0) * stride_[0] + i1 * stride_[1]
		     + i2 * stride_[2] + i3 * stride_[3] + i4 * stride_[4]
		     + i5 * stride_[5]];
    }

    T_numtype& _bz_restrict operator()(int i0, int i1, int i2, int i3,
        int i4, int i5)
    {
        assertInRange(i0, i1, i2, i3, i4, i5);
        return data_[(i0) * stride_[0] + i1 * stride_[1]
            + i2 * stride_[2] + i3 * stride_[3] + i4 * stride_[4]
            + i5 * stride_[5]];
    }

    const T_numtype& _bz_restrict operator()(int i0, int i1, int i2, int i3,
        int i4, int i5, int i6) const
    {
        assertInRange(i0, i1, i2, i3, i4, i5, i6);
        return data_[(i0) * stride_[0] + i1 * stride_[1]
            + i2 * stride_[2] + i3 * stride_[3] + i4 * stride_[4]
            + i5 * stride_[5] + i6 * stride_[6]];
    }

    T_numtype& _bz_restrict operator()(int i0, int i1, int i2, int i3,
        int i4, int i5, int i6)
    {
        assertInRange(i0, i1, i2, i3, i4, i5, i6);
        return data_[(i0) * stride_[0] + i1 * stride_[1]
            + i2 * stride_[2] + i3 * stride_[3] + i4 * stride_[4]
            + i5 * stride_[5] + i6 * stride_[6]];
    }

    const T_numtype& _bz_restrict operator()(int i0, int i1, int i2, int i3,
        int i4, int i5, int i6, int i7) const
    {
        assertInRange(i0, i1, i2, i3, i4, i5, i6, i7);
        return data_[(i0) * stride_[0] + i1 * stride_[1]
            + i2 * stride_[2] + i3 * stride_[3] + i4 * stride_[4]
            + i5 * stride_[5] + i6 * stride_[6] + i7 * stride_[7]];
    }

    T_numtype& _bz_restrict operator()(int i0, int i1, int i2, int i3,
        int i4, int i5, int i6, int i7)
    {
        assertInRange(i0, i1, i2, i3, i4, i5, i6, i7);
        return data_[(i0) * stride_[0] + i1 * stride_[1]
            + i2 * stride_[2] + i3 * stride_[3] + i4 * stride_[4]
            + i5 * stride_[5] + i6 * stride_[6] + i7 * stride_[7]];
    }

    const T_numtype& _bz_restrict operator()(int i0, int i1, int i2, int i3,
        int i4, int i5, int i6, int i7, int i8) const
    {
        assertInRange(i0, i1, i2, i3, i4, i5, i6, i7, i8);
        return data_[(i0) * stride_[0] + i1 * stride_[1]
            + i2 * stride_[2] + i3 * stride_[3] + i4 * stride_[4]
            + i5 * stride_[5] + i6 * stride_[6] + i7 * stride_[7]
            + i8 * stride_[8]];
    }

    T_numtype& _bz_restrict operator()(int i0, int i1, int i2, int i3,
        int i4, int i5, int i6, int i7, int i8)
    {
        assertInRange(i0, i1, i2, i3, i4, i5, i6, i7, i8);
        return data_[(i0) * stride_[0] + i1 * stride_[1]
            + i2 * stride_[2] + i3 * stride_[3] + i4 * stride_[4]
            + i5 * stride_[5] + i6 * stride_[6] + i7 * stride_[7]
            + i8 * stride_[8]];
    }

    const T_numtype& _bz_restrict operator()(int i0, int i1, int i2, int i3,
        int i4, int i5, int i6, int i7, int i8, int i9) const
    {
        assertInRange(i0, i1, i2, i3, i4, i5, i6, i7, i8, i9);
        return data_[(i0) * stride_[0] + i1 * stride_[1]
            + i2 * stride_[2] + i3 * stride_[3] + i4 * stride_[4]
            + i5 * stride_[5] + i6 * stride_[6] + i7 * stride_[7]
            + i8 * stride_[8] + i9 * stride_[9]];
    }

    T_numtype& _bz_restrict operator()(int i0, int i1, int i2, int i3,
        int i4, int i5, int i6, int i7, int i8, int i9)
    {
        assertInRange(i0, i1, i2, i3, i4, i5, i6, i7, i8, i9);
        return data_[(i0) * stride_[0] + i1 * stride_[1]
            + i2 * stride_[2] + i3 * stride_[3] + i4 * stride_[4]
            + i5 * stride_[5] + i6 * stride_[6] + i7 * stride_[7]
            + i8 * stride_[8] + i9 * stride_[9]];
    }

    const T_numtype& _bz_restrict operator()(int i0, int i1, int i2, int i3,
        int i4, int i5, int i6, int i7, int i8, int i9, int i10) const
    {
        assertInRange(i0, i1, i2, i3, i4, i5, i6, i7, i8, 
            i9, i10);
        return data_[(i0) * stride_[0] + i1 * stride_[1]
            + i2 * stride_[2] + i3 * stride_[3] + i4 * stride_[4]
            + i5 * stride_[5] + i6 * stride_[6] + i7 * stride_[7]
            + i8 * stride_[8] + i9 * stride_[9] + i10 * stride_[10]];
    }

    T_numtype& _bz_restrict operator()(int i0, int i1, int i2, int i3,
        int i4, int i5, int i6, int i7, int i8, int i9, int i10)
    {
        assertInRange(i0, i1, i2, i3, i4, i5, i6, i7, i8, 
            i9, i10);
        return data_[(i0) * stride_[0] + i1 * stride_[1]
            + i2 * stride_[2] + i3 * stride_[3] + i4 * stride_[4]
            + i5 * stride_[5] + i6 * stride_[6] + i7 * stride_[7]
            + i8 * stride_[8] + i9 * stride_[9] + i10 * stride_[10]];
    }



    /* Operators added by Tobias Loew */
    template<typename I>
    auto operator()(const ETBase<I>& indexer) const
    {
        return make_indexing_functor(const_cast<ETBase<I>&>(indexer).unwrap())(*this);
    }

    //template<typename I>
    //auto operator()(ETBase<I>& indexer) const
    //{
    //    return make_indexing_functor(indexer.unwrap())(*this);
    //}

    //template<typename I>
    //auto operator()(const ETBase<I>& indexer)
    //{
    //    return make_indexing_functor(const_cast<ETBase<I>&>(indexer).unwrap())(*this);
    //}

    //template<typename I>
    //auto operator()(ETBase<I>& indexer)
    //{
    //    return make_indexing_functor(indexer.unwrap())(*this);
    //}

    template<typename I>
    auto operator()(const _bz_ArrayExpr<I>& indexer) const
    {
        return make_indexing_functor(const_cast<_bz_ArrayExpr<I>&>(indexer).unwrap())(*this);
    }

    template<typename I>
    auto operator()(const _bz_ArrayExpr<I>& indexer) 
    {
        return make_indexing_functor(const_cast<_bz_ArrayExpr<I>&>(indexer).unwrap())(*this);
    }

    template<typename I, typename T>
    auto operator()(const ETBase<I>& indexer, T&& t) const
    {
        auto slice = (*this)(Range(), std::forward<T>(t));
        return make_indexing_functor_copy(const_cast<ETBase<I>&>(indexer).unwrap())(slice);
    }
//    template<typename I, typename T>
//    auto ind_helper(const ETBase<I>& indexer, T&& t) const
//    {
//        auto slice = (*this)(Range(), std::forward<T>(t));
//        return make_indexing_functor_copy(const_cast<ETBase<I>&>(indexer).unwrap())(slice);
////        return slice(indexer);
//    }

    template<typename T, typename I>
    auto operator()(T&& t, const ETBase<I>& indexer) const
    {
        auto slice = (*this)(std::forward<T>(t), Range());
        return make_indexing_functor_copy(const_cast<ETBase<I>&>(indexer).unwrap())(slice);
    }
    //template<typename I, typename T>
    //auto ind_helper(T&& t, const ETBase<I>& indexer) const
    //{
    //        auto&& slice = (*this)(Range(), std::forward<T>(t));
    //    return slice(indexer);
    //}


    /*
     * Slicing to produce subarrays.  If the number of Range arguments is
     * fewer than N_rank, then missing arguments are treated like Range::all().
     */

    T_array& noConst() const
    { return const_cast<T_array&>(*this); }

    T_array operator()(const RectDomain<N_rank>& subdomain) const
    {
        return T_array(noConst(), subdomain);
    }

    /* Operator added by Julian Cummings */
    T_array operator()(const StridedDomain<N_rank>& subdomain) const
    {
        return T_array(noConst(), subdomain);
    }

    T_array operator()(Range r0) const
    {
        return T_array(noConst(), r0);
    }

    T_array operator()(Range r0, Range r1) const
    {
        return T_array(noConst(), r0, r1);
    }

    T_array operator()(Range r0, Range r1, Range r2) const
    {
        return T_array(noConst(), r0, r1, r2);
    }

    T_array operator()(Range r0, Range r1, Range r2, Range r3) const
    {
        return T_array(noConst(), r0, r1, r2, r3);
    }

    T_array operator()(Range r0, Range r1, Range r2, Range r3, Range r4) const
    {
        return T_array(noConst(), r0, r1, r2, r3, r4);
    }

    T_array operator()(Range r0, Range r1, Range r2, Range r3, Range r4,
        Range r5) const
    {
        return T_array(noConst(), r0, r1, r2, r3, r4, r5);
    }

    T_array operator()(Range r0, Range r1, Range r2, Range r3, Range r4,
        Range r5, Range r6) const
    {
        return T_array(noConst(), r0, r1, r2, r3, r4, r5, r6);
    }

    T_array operator()(Range r0, Range r1, Range r2, Range r3, Range r4,
        Range r5, Range r6, Range r7) const
    {
        return T_array(noConst(), r0, r1, r2, r3, r4, r5, r6, r7);
    }

    T_array operator()(Range r0, Range r1, Range r2, Range r3, Range r4,
        Range r5, Range r6, Range r7, Range r8) const
    {
        return T_array(noConst(), r0, r1, r2, r3, r4, r5, r6, r7, r8);
    }

    T_array operator()(Range r0, Range r1, Range r2, Range r3, Range r4,
        Range r5, Range r6, Range r7, Range r8, Range r9) const
    {
        return T_array(noConst(), r0, r1, r2, r3, r4, r5, r6, r7, r8, r9);
    }

    T_array operator()(Range r0, Range r1, Range r2, Range r3, Range r4,
        Range r5, Range r6, Range r7, Range r8, Range r9, Range r10) const
    {
        return T_array(noConst(), r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10);
    }

    // Allow any mixture of Range, int and Vector<int> objects as
    // operands for operator():   A(Range(3,7), 5, Range(2,4))

    /*
     * These versions of operator() allow any combination of int
     * and Range operands to be used.  Each int operand reduces
     * the rank of the resulting array by one.  
     *
     * e.g.  Array<int,4> A(20,20,20,20);
     *       Array<int,2> B = A(Range(5,15), 3, 5, Range(8,9));
     *
     * SliceInfo is a helper class defined in <blitz/arrayslice.h>.
     * It counts the number of Range vs. int arguments and does some
     * other helpful things.
     *
     * Once partial specialization becomes widely implemented, these
     * operators may be expanded to accept Vector<int> arguments
     * and produce ArrayPick<T,N> objects.
     *
     * This operator() is not provided with a single argument because
     * the appropriate cases exist above.
     */

#ifdef BZ_HAVE_PARTIAL_ORDERING


    template<typename T1, typename T2
        //, typename std::enable_if<
        //    !is_indexing_type<std::remove_cv_t<std::remove_reference_t<T1>>>::value
        //    &&
        //!is_indexing_type<std::remove_cv_t<std::remove_reference_t<T2>>>::value
        //>::type* = nullptr     //not working with VS 2015
    >
    typename std::enable_if<
        !is_indexing_type<std::remove_cv_t<std::remove_reference_t<T1>>>::value
        &&
        !is_indexing_type<std::remove_cv_t<std::remove_reference_t<T2>>>::value
        , typename SliceInfo<T_numtype,Policy, T1, T2>::T_slice
        >::type
    operator()(T1 r1, T2 r2) const
    {
        typedef typename SliceInfo<T_numtype,Policy,T1,T2>::T_slice slice;
        return slice(noConst(), r1, r2, nilArraySection(), nilArraySection(), nilArraySection(),
            nilArraySection(), nilArraySection(), nilArraySection(),
            nilArraySection(), nilArraySection(), nilArraySection());
    }

    template<typename T1, typename T2, typename T3>
    typename SliceInfo<T_numtype,Policy,T1,T2,T3>::T_slice 
    operator()(T1 r1, T2 r2, T3 r3) const
    {
        typedef typename SliceInfo<T_numtype,Policy,T1,T2,T3>::T_slice slice;
        return slice(noConst(), r1, r2, r3, nilArraySection(), nilArraySection(), nilArraySection(),
            nilArraySection(), nilArraySection(), nilArraySection(),
            nilArraySection(), nilArraySection());
    }

    template<typename T1, typename T2, typename T3, typename T4>
    typename SliceInfo<T_numtype,Policy,T1,T2,T3,T4>::T_slice
    operator()(T1 r1, T2 r2, T3 r3, T4 r4) const
    {
        typedef typename SliceInfo<T_numtype,Policy,T1,T2,T3,T4>::T_slice slice;
        return slice(noConst(), r1, r2, r3, r4, nilArraySection(), nilArraySection(),
            nilArraySection(), nilArraySection(), nilArraySection(),
            nilArraySection(), nilArraySection());
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    typename SliceInfo<T_numtype,Policy,T1,T2,T3,T4,T5>::T_slice
    operator()(T1 r1, T2 r2, T3 r3, T4 r4, T5 r5) const
    {
        typedef typename SliceInfo<T_numtype,Policy,T1,T2,T3,T4,T5>::T_slice slice;
        return slice(noConst(), r1, r2, r3, r4, r5, nilArraySection(),
            nilArraySection(), nilArraySection(), nilArraySection(),
            nilArraySection(), nilArraySection());
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    typename SliceInfo<T_numtype,Policy,T1,T2,T3,T4,T5,T6>::T_slice
    operator()(T1 r1, T2 r2, T3 r3, T4 r4, T5 r5, T6 r6) const
    {
        typedef typename SliceInfo<T_numtype,Policy,T1,T2,T3,T4,T5,T6>::T_slice slice;
        return slice(noConst(), r1, r2, r3, r4, r5, r6, nilArraySection(), nilArraySection(), nilArraySection(),
            nilArraySection(), nilArraySection());
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6,
        typename T7>
    typename SliceInfo<T_numtype,Policy,T1,T2,T3,T4,T5,T6,T7>::T_slice
    operator()(T1 r1, T2 r2, T3 r3, T4 r4, T5 r5, T6 r6, T7 r7) const
    {
        typedef typename SliceInfo<T_numtype,Policy,T1,T2,T3,T4,T5,T6,T7>::T_slice slice;
        return slice(noConst(), r1, r2, r3, r4, r5, r6, r7, nilArraySection(), nilArraySection(),
            nilArraySection(), nilArraySection());
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6,
        typename T7, typename T8>
    typename SliceInfo<T_numtype,Policy,T1,T2,T3,T4,T5,T6,T7,T8>::T_slice
    operator()(T1 r1, T2 r2, T3 r3, T4 r4, T5 r5, T6 r6, T7 r7, T8 r8) const
    {
        typedef typename SliceInfo<T_numtype,Policy,T1,T2,T3,T4,T5,T6,T7,T8>::T_slice slice;
        return slice(noConst(), r1, r2, r3, r4, r5, r6, r7, r8,
            nilArraySection(), nilArraySection(), nilArraySection());
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6,
        typename T7, typename T8, typename T9>
    typename SliceInfo<T_numtype,Policy,T1,T2,T3,T4,T5,T6,T7,T8,T9>::T_slice
    operator()(T1 r1, T2 r2, T3 r3, T4 r4, T5 r5, T6 r6, T7 r7, T8 r8, T9 r9) const
    {
        typedef typename SliceInfo<T_numtype,Policy,T1,T2,T3,T4,T5,T6,T7,T8,T9>::T_slice slice;
        return slice(noConst(), r1, r2, r3, r4, r5, r6, r7, r8, r9, nilArraySection(), nilArraySection());
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6,
        typename T7, typename T8, typename T9, typename T10>
    typename SliceInfo<T_numtype,Policy,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10>::T_slice
    operator()(T1 r1, T2 r2, T3 r3, T4 r4, T5 r5, T6 r6, T7 r7, T8 r8, T9 r9, T10 r10) const
    {
        typedef typename SliceInfo<T_numtype,Policy,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10>::T_slice slice;
        return slice(noConst(), r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, nilArraySection());
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6,
        typename T7, typename T8, typename T9, typename T10, typename T11>
    typename SliceInfo<T_numtype,Policy,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11>::T_slice
    operator()(T1 r1, T2 r2, T3 r3, T4 r4, T5 r5, T6 r6, T7 r7, T8 r8, T9 r9, T10 r10, T11 r11) const
    {
        typedef typename SliceInfo<T_numtype,Policy,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11>::T_slice slice;
        return slice(noConst(), r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11);
    }

#endif // BZ_HAVE_PARTIAL_ORDERING

    /*
     * These versions of operator() are provided to support tensor-style
     * array notation, e.g.
     *
     * Array<float, 2> A, B;
     * firstIndex i;
     * secondIndex j;
     * thirdIndex k;
     * Array<float, 3> C = A(i,j) * B(j,k);
     */

    template<int N0>
    _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_array >::T_expr, N0> >
    operator()(IndexPlaceholder<N0>) const
    { 
        return _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_array >::T_expr, N0> >
            (noConst());
    }

    template<int N0, int N1>
    _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_array >::T_expr, N0, N1> >
    operator()(IndexPlaceholder<N0>, IndexPlaceholder<N1>) const
    {
        return _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_array >::T_expr, N0,
            N1> >(noConst());
    } 

    template<int N0, int N1, int N2>
    _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_array >::T_expr, N0, N1, N2> >
    operator()(IndexPlaceholder<N0>, IndexPlaceholder<N1>,
        IndexPlaceholder<N2>) const
    {
        return _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_array >::T_expr, N0,
            N1, N2> >(noConst());
    }

    template<int N0, int N1, int N2, int N3>
    _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_array >::T_expr, N0, N1, N2, N3> >
    operator()(IndexPlaceholder<N0>, IndexPlaceholder<N1>,
        IndexPlaceholder<N2>, IndexPlaceholder<N3>) const
    {
        return _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_array >::T_expr, N0,
            N1, N2, N3> >(noConst());
    }

    template<int N0, int N1, int N2, int N3, int N4>
    _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_array >::T_expr, N0, N1, N2, N3, N4> >
    operator()(IndexPlaceholder<N0>, IndexPlaceholder<N1>,
        IndexPlaceholder<N2>, IndexPlaceholder<N3>, 
        IndexPlaceholder<N4>) const
    {
        return _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_array >::T_expr, N0,
            N1, N2, N3, N4> >(noConst());
    }

    template<int N0, int N1, int N2, int N3, int N4, int N5>
    _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_array >::T_expr, N0, N1, N2, N3, 
        N4, N5> >
    operator()(IndexPlaceholder<N0>, IndexPlaceholder<N1>,
        IndexPlaceholder<N2>, IndexPlaceholder<N3>, IndexPlaceholder<N4>,
        IndexPlaceholder<N5>) const
    {
        return _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_array >::T_expr, N0,
            N1, N2, N3, N4, N5> >(noConst());
    }

    template<int N0, int N1, int N2, int N3, int N4, int N5, int N6>
    _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_array >::T_expr, N0, N1, N2, N3,
        N4, N5, N6> >
    operator()(IndexPlaceholder<N0>, IndexPlaceholder<N1>,
        IndexPlaceholder<N2>, IndexPlaceholder<N3>, IndexPlaceholder<N4>,
        IndexPlaceholder<N5>, IndexPlaceholder<N6>) const
    {
        return _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_array >::T_expr, N0,
            N1, N2, N3, N4, N5, N6> >(noConst());
    }

    template<int N0, int N1, int N2, int N3, int N4, int N5, int N6,
        int N7>
    _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_array >::T_expr, N0, N1, N2, N3,
        N4, N5, N6, N7> >
    operator()(IndexPlaceholder<N0>, IndexPlaceholder<N1>,
        IndexPlaceholder<N2>, IndexPlaceholder<N3>, IndexPlaceholder<N4>,
        IndexPlaceholder<N5>, IndexPlaceholder<N6>, 
        IndexPlaceholder<N7>) const
    {
        return _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_array >::T_expr, N0,
            N1, N2, N3, N4, N5, N6, N7> >(noConst());
    }

    template<int N0, int N1, int N2, int N3, int N4, int N5, int N6,
        int N7, int N8>
    _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_array >::T_expr, N0, N1, N2, N3,
        N4, N5, N6, N7, N8> >
    operator()(IndexPlaceholder<N0>, IndexPlaceholder<N1>,
        IndexPlaceholder<N2>, IndexPlaceholder<N3>, IndexPlaceholder<N4>,
        IndexPlaceholder<N5>, IndexPlaceholder<N6>, IndexPlaceholder<N7>,
        IndexPlaceholder<N8>) const
    {
        return _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_array >::T_expr, N0,
            N1, N2, N3, N4, N5, N6, N7, N8> >(noConst());
    }

    template<int N0, int N1, int N2, int N3, int N4, int N5, int N6,
        int N7, int N8, int N9>
    _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_array >::T_expr, N0, N1, N2, N3,
        N4, N5, N6, N7, N8, N9> >
    operator()(IndexPlaceholder<N0>, IndexPlaceholder<N1>,
        IndexPlaceholder<N2>, IndexPlaceholder<N3>, IndexPlaceholder<N4>,
        IndexPlaceholder<N5>, IndexPlaceholder<N6>, IndexPlaceholder<N7>,
        IndexPlaceholder<N8>, IndexPlaceholder<N9>) const
    {
        return _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_array >::T_expr, N0,
            N1, N2, N3, N4, N5, N6, N7, N8, N9> >(noConst());
    }

    template<int N0, int N1, int N2, int N3, int N4, int N5, int N6,
        int N7, int N8, int N9, int N10>
    _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_array >::T_expr, N0, N1, N2, N3,
        N4, N5, N6, N7, N8, N9, N10> >
    operator()(IndexPlaceholder<N0>, IndexPlaceholder<N1>,
        IndexPlaceholder<N2>, IndexPlaceholder<N3>, IndexPlaceholder<N4>,
        IndexPlaceholder<N5>, IndexPlaceholder<N6>, IndexPlaceholder<N7>,
        IndexPlaceholder<N8>, IndexPlaceholder<N9>, 
        IndexPlaceholder<N10>) const
    {
        return _bz_ArrayExpr<ArrayIndexMapping<typename asExpr<T_array >::T_expr, N0,
            N1, N2, N3, N4, N5, N6, N7, N8, N9, N10> >(noConst());
    }

    //////////////////////////////////////////////
    // Support for multicomponent arrays
    //////////////////////////////////////////////

    /*
     * See <blitz/array/multi.h> for an explanation of the traits class
     * multicomponent_traits.
     */

    Array<typename multicomponent_traits<T_numtype>::T_element,N_rank, Policy>
    operator[](const unsigned component) {
        typedef typename multicomponent_traits<T_numtype>::T_element T_compType;

        return extractComponent(T_compType(),component,
                                multicomponent_traits<T_numtype>::numComponents);
    }

    const Array<typename multicomponent_traits<T_numtype>::T_element,N_rank, Policy>
    operator[](const unsigned component) const {
        typedef typename multicomponent_traits<T_numtype>::T_element T_compType;

        return extractComponent(T_compType(),component,
                                multicomponent_traits<T_numtype>::numComponents);
    }

    Array<typename multicomponent_traits<T_numtype>::T_element,N_rank, Policy>
    operator[](const int component) {
        return operator[](static_cast<unsigned>(component));
    }

    const Array<typename multicomponent_traits<T_numtype>::T_element,N_rank, Policy>
    operator[](const int component) const {
        return operator[](static_cast<unsigned>(component));
    }

    //////////////////////////////////////////////
    // Indirection
    //////////////////////////////////////////////
 
    template<typename T_indexContainer>
    IndirectArray<T_array, T_indexContainer>
    operator[](const T_indexContainer& index)
    {
        return IndirectArray<T_array, T_indexContainer>(*this,
            const_cast<T_indexContainer&>(index));
    }
 
    //////////////////////////////////////////////
    // Assignment Operators
    //////////////////////////////////////////////

  /** \name Assignment operators.  \todo Index placeholder
      operand. \todo Random operand.  @{ */

  /** 
      Scalar operand assignment.  \todo Need a precondition check on
      isStorageContiguous when operator, is used. \todo We should do
      bounds checking, right now we will buffer overrun if the number
      of initializers in the list is larger than numElements. */
#ifdef BLITZ_USE_LISTINITIALIZATION
    ListInitializationSwitch<T_array> operator=(T_numtype x) {
        return ListInitializationSwitch<T_array>(*this, x);
    }
#else
    void
        operator=(T_numtype x) {
        initialize(x);
    }
#endif


    //template<typename PolicyT>
    //T_array& initialize_impl(T_numtype x);

    //template<>
    //T_array& initialize_impl<array_policy::StaticFortranArrayPolicy<1>>(T_numtype x);


    template<typename PolicyT>
        T_array& initialize_impl(PolicyT, T_numtype x) {
        // we can't use asExpr here, because if we are initializing an array
        // whose components are also ETBase, it would parse as an array
        // expression, not as an initialization with a scalar.
        (*this) = _bz_ArrayExpr<_bz_ArrayExprConstant<T_numtype> >(x);
        return *this;
    }

    template<int N_base>
    T_array& initialize_impl(array_policy::StaticFortranArrayPolicy<N_base>, T_numtype x) {
    // we can't use asExpr here, because if we are initializing an array
    // whose components are also ETBase, it would parse as an array
    // expression, not as an initialization with a scalar.
        //const int ub = N_base + length_[0];
        for(int index = N_base; index < N_base + length_[0]; ++index) {
            data_[index] = x;
        }
        //(*this) = _bz_ArrayExpr<_bz_ArrayExprConstant<T_numtype> >(x);
        return *this;
    }



    T_array& initialize(T_numtype x){
        return initialize_impl(Policy{}, x);
    }

    // Was:
    // T_array& operator=(T_numtype);

#ifdef BZ_NEW_EXPRESSION_TEMPLATES
  
  // we need this because we can't use default assignment op so it
  // must be overridden
    T_array& operator=(const Array<T_numtype, N_rank, Policy>& x) {
        return assign_impl(Policy{}, x);
    }

    template<typename PolicyT>
    T_array& assign_impl(PolicyT, const Array<T_numtype, N_rank, Policy>&);

    template<int N_base>
    T_array& assign_impl(array_policy::StaticFortranArrayPolicy<N_base>, const Array<T_numtype, N_rank, Policy>&);


// handle case with different policies
    template<typename Policy2>
    T_array& operator=(const Array<T_numtype, N_rank, Policy2>& x);


  // we can't define a generic template for the assignment operator
  // because it will cause the list initialization assignment above to
  // not work when implict conversions to T_numtype are necessary.

  //template<typename T> T_array& operator=(const T&);
  template<typename T_expr> T_array& operator=(const ETBase<T_expr>&);

  T_array& operator+=(const T_array&);
  T_array& operator-=(const T_array&);
  T_array& operator*=(const T_array&);
  T_array& operator/=(const T_array&);
  T_array& operator%=(const T_array&);
  T_array& operator^=(const T_array&);
  T_array& operator&=(const T_array&);
  T_array& operator|=(const T_array&);
  T_array& operator>>=(const T_array&);
  T_array& operator<<=(const T_array&);

  T_array& operator+=(const T_numtype&);
  T_array& operator-=(const T_numtype&);
  T_array& operator*=(const T_numtype&);
  T_array& operator/=(const T_numtype&);
  T_array& operator%=(const T_numtype&);
  T_array& operator^=(const T_numtype&);
  T_array& operator&=(const T_numtype&);
  T_array& operator|=(const T_numtype&);
  T_array& operator>>=(const T_numtype&);
  T_array& operator<<=(const T_numtype&);

  template<typename T_expr> T_array& operator+=(const ETBase<T_expr>&);
  template<typename T_expr> T_array& operator-=(const ETBase<T_expr>&);
  template<typename T_expr> T_array& operator*=(const ETBase<T_expr>&);
  template<typename T_expr> T_array& operator/=(const ETBase<T_expr>&);
  template<typename T_expr> T_array& operator%=(const ETBase<T_expr>&);
  template<typename T_expr> T_array& operator^=(const ETBase<T_expr>&);
  template<typename T_expr> T_array& operator&=(const ETBase<T_expr>&);
  template<typename T_expr> T_array& operator|=(const ETBase<T_expr>&);
  template<typename T_expr> T_array& operator>>=(const ETBase<T_expr>&);
  template<typename T_expr> T_array& operator<<=(const ETBase<T_expr>&);

#else
    T_array& operator+=(T_numtype);
    T_array& operator-=(T_numtype);
    T_array& operator*=(T_numtype);
    T_array& operator/=(T_numtype);
    T_array& operator%=(T_numtype);
    T_array& operator^=(T_numtype);
    T_array& operator&=(T_numtype);
    T_array& operator|=(T_numtype);
    T_array& operator>>=(T_numtype);
    T_array& operator<<=(T_numtype);

    // Array operands
    T_array& operator=(const Array<T_numtype,N_rank, Policy>&);

    template<typename P_numtype2> 
    T_array& operator=(const Array<P_numtype2,N_rank, Policy>&);
    template<typename P_numtype2>
    T_array& operator+=(const Array<P_numtype2,N_rank, Policy>&);
    template<typename P_numtype2>
    T_array& operator-=(const Array<P_numtype2,N_rank, Policy>&);
    template<typename P_numtype2>
    T_array& operator*=(const Array<P_numtype2, N_rank, Policy>&);
    template<typename P_numtype2>
    T_array& operator/=(const Array<P_numtype2, N_rank, Policy>&);
    template<typename P_numtype2>
    T_array& operator%=(const Array<P_numtype2, N_rank, Policy>&);
    template<typename P_numtype2>
    T_array& operator^=(const Array<P_numtype2, N_rank, Policy>&);
    template<typename P_numtype2>
    T_array& operator&=(const Array<P_numtype2, N_rank, Policy>&);
    template<typename P_numtype2>
    T_array& operator|=(const Array<P_numtype2, N_rank, Policy>&);
    template<typename P_numtype2>
    T_array& operator>>=(const Array<P_numtype2, N_rank, Policy>&);
    template<typename P_numtype2>
    T_array& operator<<=(const Array<P_numtype2, N_rank, Policy>&);

    // Array expression operands
    template<typename T_expr>
    inline T_array& operator=(BZ_ETPARM(_bz_ArrayExpr<T_expr>) expr);
    template<typename T_expr>
    inline T_array& operator+=(BZ_ETPARM(_bz_ArrayExpr<T_expr>) expr);
    template<typename T_expr>
    inline T_array& operator-=(BZ_ETPARM(_bz_ArrayExpr<T_expr>) expr);
    template<typename T_expr>
    inline T_array& operator*=(BZ_ETPARM(_bz_ArrayExpr<T_expr>) expr);
    template<typename T_expr>
    inline T_array& operator/=(BZ_ETPARM(_bz_ArrayExpr<T_expr>) expr);
    template<typename T_expr>
    inline T_array& operator%=(BZ_ETPARM(_bz_ArrayExpr<T_expr>) expr);
    template<typename T_expr>
    inline T_array& operator^=(BZ_ETPARM(_bz_ArrayExpr<T_expr>) expr);
    template<typename T_expr>
    inline T_array& operator&=(BZ_ETPARM(_bz_ArrayExpr<T_expr>) expr);
    template<typename T_expr>
    inline T_array& operator|=(BZ_ETPARM(_bz_ArrayExpr<T_expr>) expr);
    template<typename T_expr>
    inline T_array& operator>>=(BZ_ETPARM(_bz_ArrayExpr<T_expr>) expr);
    template<typename T_expr>
    inline T_array& operator<<=(BZ_ETPARM(_bz_ArrayExpr<T_expr>) expr);

  /// @}

#endif

public:

  T_numtype* _bz_restrict getInitializationIterator() { return dataFirst(); }
//iterator getInitializationIterator() { return begin(); }

    bool canCollapse(int outerRank, int innerRank) const { 
#ifdef BZ_DEBUG_TRAVERSE
        BZ_DEBUG_MESSAGE("stride(" << innerRank << ")=" << stride(innerRank)
          << ", extent()=" << extent(innerRank) << ", stride(outerRank)="
          << stride(outerRank));
#endif
        return (stride(innerRank) * extent(innerRank) == stride(outerRank)); 
    }

protected:
    //////////////////////////////////////////////
    // Implementation routines
    //////////////////////////////////////////////

    _bz_inline2 void computeStrides();
    _bz_inline2 void setupStorage(int rank);
    template<typename PolicyT>
    void constructSubarray(PolicyT, Array<T_numtype, N_rank, Policy>& array,
        const RectDomain<N_rank>&);
    template<typename PolicyT>
    void constructSubarray(PolicyT, Array<T_numtype, N_rank, Policy>& array,
        const StridedDomain<N_rank>&);
    template<typename PolicyT>
    void constructSubarray(PolicyT, Array<T_numtype, N_rank, Policy>& array, Range r0);
    template<typename PolicyT>
    void constructSubarray(PolicyT, Array<T_numtype, N_rank, Policy>& array, Range r0, Range r1);
    template<typename PolicyT>
    void constructSubarray(PolicyT, Array<T_numtype, N_rank, Policy>& array, Range r0,
        Range r1, Range r2);
    template<typename PolicyT>
    void constructSubarray(PolicyT, Array<T_numtype, N_rank, Policy>& array, Range r0,
        Range r1, Range r2, Range r3);
    template<typename PolicyT>
    void constructSubarray(PolicyT, Array<T_numtype, N_rank, Policy>& array, Range r0,
        Range r1, Range r2, Range r3, Range r4);
    template<typename PolicyT>
    void constructSubarray(PolicyT, Array<T_numtype, N_rank, Policy>& array, Range r0,
        Range r1, Range r2, Range r3, Range r4, Range r5);
    template<typename PolicyT>
    void constructSubarray(PolicyT, Array<T_numtype, N_rank, Policy>& array, Range r0,
        Range r1, Range r2, Range r3, Range r4, Range r5, Range r6);
    template<typename PolicyT>
    void constructSubarray(PolicyT, Array<T_numtype, N_rank, Policy>& array, Range r0,
        Range r1, Range r2, Range r3, Range r4, Range r5, Range r6,
        Range r7);
    template<typename PolicyT>
    void constructSubarray(PolicyT, Array<T_numtype, N_rank, Policy>& array, Range r0,
        Range r1, Range r2, Range r3, Range r4, Range r5, Range r6,
        Range r7, Range r8);
    template<typename PolicyT>
    void constructSubarray(PolicyT, Array<T_numtype, N_rank, Policy>& array, Range r0,
        Range r1, Range r2, Range r3, Range r4, Range r5, Range r6,
        Range r7, Range r8, Range r9);
    template<typename PolicyT>
    void constructSubarray(PolicyT, Array<T_numtype, N_rank, Policy>& array, Range r0,
        Range r1, Range r2, Range r3, Range r4, Range r5, Range r6,
        Range r7, Range r8, Range r9, Range r10);

    template<int N_base>
    void constructSubarray(array_policy::StaticFortranArrayPolicy<N_base>, Array<T_numtype, N_rank, Policy>& array, Range r0);

    void calculateZeroOffset();

    template<int N_rank2, typename R0, typename R1, typename R2, typename R3, typename R4, 
        typename R5, typename R6, typename R7, typename R8, typename R9, typename R10>
    void constructSlice(Array<T_numtype, N_rank2, Policy>& array, R0 r0, R1 r1, R2 r2,
        R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9, R10 r10);

    template<int N_rank2>
    void slice(int& setRank, Range r, Array<T_numtype,N_rank2, Policy>& array,
        TinyVector<int,N_rank2>& rankMap, int sourceRank);

    template<int N_rank2>
    void slice(int& setRank, int i, Array<T_numtype,N_rank2, Policy>& array,
        TinyVector<int,N_rank2>& rankMap, int sourceRank);

    template<int N_rank2>
    void slice(int&, nilArraySection, Array<T_numtype,N_rank2, Policy>&,
        TinyVector<int,N_rank2>&, int)
    { }

    void doTranspose(int destRank, int sourceRank, T_array& array);

private:
  // serialization support
#ifdef BZ_HAVE_BOOST_SERIALIZATION
  friend class boost::serialization::access;

    template<class T_arch>
    void serialize(T_arch& ar, const unsigned int version) {
      ar & boost::serialization::base_object<memory_block_reference_t >(*this);
      ar & length_;
      ar & storage_;
      ar & stride_;
      ar & zeroOffset_;
    };
#endif

protected:
    //////////////////////////////////////////////
    // Data members
    //////////////////////////////////////////////

    // NB: adding new data members may require changes to ctors, reference()

    /*
     * For a description of the storage_ members, see the comments for class 
     * GeneralArrayStorage<N_rank> above.
     *
     * length_[] contains the extent of each rank.  E.g. a 10x20x30 array
     *           would have length_ = { 10, 20, 30}.
     * stride_[] contains the stride to move to the next element along each
     *           rank.
     * zeroOffset_ is the distance from the first element in the array 
     *           to the point (0,0,...,0).  If base_ is zero and all ranks are 
     *           stored ascending, then zeroOffset_ is zero.  This value
     *           is needed because to speed up indexing, the data_ member
     *           (inherited from MemoryBlockReference) always refers to
     *           (0,0,...,0).
     */
    typename policy_traits::template storage_t<N_rank> storage_;
    typename policy_traits::template length_t<N_rank> length_;
    typename policy_traits::template stride_t<N_rank> stride_;
//    typename policy_traits::difftype_t zeroOffset_;
};



/*
 * Global Functions
 */

template<typename T_numtype, typename Policy>
ostream& operator<<(ostream&, const Array<T_numtype,1, Policy>&);

template<typename T_numtype, int N_rank, typename Policy>
ostream& operator<<(ostream&, const Array<T_numtype,N_rank, Policy>&);

template<typename T_numtype, int N_rank, typename Policy>
istream& operator>>(istream& is, Array<T_numtype,N_rank, Policy>& x);

template <typename P_numtype,int N_rank, typename Policy>
void swap(Array<P_numtype,N_rank, Policy>& a,Array<P_numtype,N_rank, Policy>& b) {
    Array<P_numtype,N_rank> c(a);
    a.reference(b);
    b.reference(c);
}

template <typename P_expr, typename Policy>
void find(Array<TinyVector<int,P_expr::rank>,1, Policy>& indices,
          const _bz_ArrayExpr<P_expr>& expr) {
    find(indices,
         static_cast< Array<typename P_expr::T_numtype,P_expr::rank, Policy> >(expr));
}

template <typename P_numtype, int N_rank, typename Policy>
void find(Array<TinyVector<int,N_rank>,1, Policy>& indices,
          const Array<P_numtype,N_rank, Policy>& exprVals) {
    indices.resize(exprVals.size());
    typename Array<P_numtype,N_rank, Policy>::const_iterator it, end = exprVals.end();
    int j=0; 
    for (it = exprVals.begin(); it != end; ++it)
        if (*it) 
            indices(j++) = it.position();
    if (j) 
        indices.resizeAndPreserve(j);
    else 
        indices.free();
    return;
}


BZ_NAMESPACE_END

/*
 * Removed the "kitchen-sink inclusion" here because it made
 * dependencies very difficult to figure out.
 */
#include <blitz/array.cc>
#include <blitz/tinyvec2.cc>


#endif // BZ_ARRAY_H
