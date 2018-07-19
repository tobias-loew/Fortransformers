/* f2c.h  --  Standard Fortran to C header file */

/**  barf  [ba:rf]  2.  "He suggested using FORTRAN, and everybody barfed."

	- From The Shogakukan DICTIONARY OF NEW ENGLISH (Second edition) */

#pragma once

#include <algorithm>
#include <array>
#include <type_traits>
#include <boost/preprocessor.hpp>
#include <boost/optional.hpp>
#include <boost/array.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/implicit_cast.hpp>
#include <boost/algorithm/string/trim.hpp>
#include "f2cpp_extensions.h"
#include <blitz/array.h>
#include <blitz/extension/tbv2fastiter.h>
#include <blitz/extension/tinybasedvec2.h>
#include <blitz/extension/tbm2fastiter.h>
#include <blitz/extension/tinybasedmat2.h>
#include <blitz/extension/indexingvec2.h>
#include <blitz/extension/member_projection.h>


// FT_USE_TINYBASEDVECTOR_AS_BASE uses blitz::TinyBasedVector as standard base (instead of blitz::Array)
#define FT_USE_TINYBASEDVECTOR_AS_BASE
#define FT_NATIVE_SUPPORT_STRING_SUBTRINGS

#define FT_CONST        const
#define FT_SELECT_ANY   __declspec(selectany)
#define FT_STORAGE      extern


static constexpr int c_size_t = 4;


namespace boost_ext
{

    template<class T,
        class S
    >
        struct numeric_converter
    {
        typedef boost::numeric::conversion_traits<T, S> Traits;

        typedef typename Traits::source_type   source_type;
        typedef typename Traits::argument_type argument_type;
        typedef typename Traits::result_type   result_type;

        static result_type convert(argument_type s)
        {
            try
            {
                return boost::numeric_cast<T, S>(s);
            }
            catch (const boost::numeric::negative_overflow&) {
                return boost::numeric::bounds<T>::lowest();
            }
            catch (const boost::numeric::positive_overflow&) {
                return boost::numeric::bounds<T>::highest();
            }

        }
    };

} // namespace boost


namespace ft {
    using boost::optional;

    //template<typename T, typename R1>
    //using nat_fort_array = blitz::Array<T, 1, blitz::array_policy::GeneralFortranArrayPolicy>;

    //template<typename T, typename R1, typename R2>
    //using nat_fort_array_2 = blitz::Array<T, 2, blitz::array_policy::GeneralFortranArrayPolicy>;

    //template<typename T, typename R>
    //class const_expr_fort_array;


#ifdef FT_USE_TINYBASEDVECTOR_AS_BASE

    struct tag_tinybasedvector_ctor{};
    struct tag_tinybasedmatrix_ctor{};

//    template<typename T, int I1>
//    using fixed_fort_array = fort_array<T, ft::bs<1, I1>>;
//#else
//    template<typename T, int I1>
//    using fixed_fort_array = blitz::TinyBasedVector<T, I1, 1>;
#endif
    template<typename T, int I1>
    using fixed_fort_array = blitz::TinyBasedVector<T, I1, 1>;


    template<int Lower, int Upper>
    struct bs
    {
        static constexpr int lower = Lower;
        static constexpr int upper = Upper;
    };

    struct dyn{};   // tagging dynamic dimensions
    using dyn_dim = dyn;
//    struct dyn_dim{};   // tagging dynamic dimensions

    using rng_t = blitz::Range;

    static constexpr int dyn_unbounded = blitz::toEnd - 1;
    struct dyn_bounds : blitz::Range {
        dyn_bounds(T_numtype first)
            :blitz::Range(first, dyn_unbounded)
        {}

        dyn_bounds(T_numtype first, T_numtype last)
            :blitz::Range(first, last)
        {}

    };

    struct dyn_alloc_tag{};
    struct dyn_alloc{
        int length;
        dyn_alloc(int length)
            :length(length)
        {}
    };


    template<int Lower>
    struct fixed_lb_dyn_ub {
        static constexpr int lower = Lower;
    };

    using dyn_ub = fixed_lb_dyn_ub<1>;

    template<typename T, typename R1, typename R2>
    class fort_array_2_base;

    template<int Lower>
    struct allocating_fixed_lb_dyn_ub {
        //rng_t r;    // cannot use blitz;;Range as it asserts on
        static constexpr int lower = Lower;
        int upper;
        explicit allocating_fixed_lb_dyn_ub(int upper)
            :upper(upper) {}
        //explicit allocating_rng(rng_t const& r)
        //    :r(r)
        //{}

        int length() const {
            return upper - lower + 1;
        }
    };

    using allocating_length = allocating_fixed_lb_dyn_ub<1>;

    struct allocating_rng {
        //rng_t r;    // cannot use blitz;;Range as it asserts on
        int lower;
        int upper;
        explicit allocating_rng(int lower, int upper)
            :lower(lower)
            , upper(upper)
        {}
        explicit allocating_rng(int upper)
            :lower(1)
            , upper(upper)
        {}
        //explicit allocating_rng(rng_t const& r)
        //    :r(r)
        //{}

        int length() const{
            return upper - lower + 1;
        }
    };


    template<typename T>
    struct allocator_wrap{
        using is_allocator_wrap = void;

        T&& t;
        allocating_rng const& r;
        allocator_wrap(T&& t, allocating_rng const& r)
            :t(t)
            , r(r)
        {}
    };

    template<typename T>
    decltype(auto) make_allocator_wrap(T&& t, allocating_rng const& r) {
        return allocator_wrap<T>(std::forward<T>(t), r);
    }

    template<typename T, int Lower>
    struct allocator_wrap_fixed_lb_dyn_ub {
        using is_allocator_wrap = void;

        T&& t;
        allocating_fixed_lb_dyn_ub<Lower> const& r;
        allocator_wrap_fixed_lb_dyn_ub(T&& t, allocating_fixed_lb_dyn_ub<Lower> const& r)
            :t(t)
            , r(r)
        {}
    };

    template<typename T, int Lower>
    decltype(auto) make_allocator_wrap(T&& t, allocating_fixed_lb_dyn_ub<Lower> const& r) {
        return allocator_wrap_fixed_lb_dyn_ub<T, Lower>(std::forward<T>(t), r);
    }






    template<typename T>
    struct allocator_2_wrap{
        using is_allocator_2_wrap = void;

        T&& t;
        allocating_rng const& r1;
        allocating_rng const& r2;
        allocator_2_wrap(T&& t, allocating_rng const& r1, allocating_rng const& r2)
            :t(t)
            , r1(r1)
            , r2(r2)
        {}
    };

    template<typename T>
    decltype(auto) make_allocator_2_wrap(T&& t, allocating_rng const& r1, allocating_rng const& r2){
        return AllocatorWrap&&(std::forward<T>(t), r1, r2);
    }



    template<typename T, int Lower1, int Lower2>
    struct allocator_2_wrap_fixed_lb_dyn_ub {
        using is_allocator_2_wrap = void;

        T&& t;
        allocating_fixed_lb_dyn_ub<Lower1> const& r1;
        allocating_fixed_lb_dyn_ub<Lower2> const& r2;
        allocator_2_wrap_fixed_lb_dyn_ub(T&& t, allocating_fixed_lb_dyn_ub<Lower1> const& r1, allocating_fixed_lb_dyn_ub<Lower2> const& r2)
            :t(t)
            , r1(r1)
            , r2(r2) {}
    };

    template<typename T, int Lower1, int Lower2>
    decltype(auto) make_allocator_2_wrap(T&& t, allocating_fixed_lb_dyn_ub<Lower1> const& r1, allocating_fixed_lb_dyn_ub<Lower2> const& r2) {
        return allocator_2_wrap_fixed_lb_dyn_ub<T, Lower1, Lower2>(std::forward<T>(t), r1, r2);
    }






    template<typename T>
    struct MULT_INIT_wrap{
        T const& t;
        int lower;
        int upper;

        MULT_INIT_wrap(T&& t, int lower, int upper)
            : t(t)
            , lower(lower)
            , upper(upper)
        {}
    };

    template<typename T>
    decltype(auto) MULT_INIT(T&& t, int lower, int upper){
        return MULT_INIT_wrap<T>(std::forward<T>(t), lower, upper);
    }






#ifdef FT_USE_TINYBASEDVECTOR_AS_BASE
    template<typename T, typename R>
    struct make_blitz_base_array{
        using type = blitz::TinyBasedVector<T, R::upper - R::lower + 1, R::lower>;
    };
#else
    template<typename T, typename R>
    struct make_blitz_base_array{
        using type = blitz::Array<T, 1, blitz::array_policy::GeneralFortranArrayPolicy>;
    };
#endif

    template<typename T, typename R>
    using blitz_base_array = typename make_blitz_base_array<T, R>::type;

                    // fortran array
    template<typename T, typename R>
    class fort_array_base : public blitz_base_array<T, R>
    {
    public:
        using blitz_type = blitz_base_array<T, R>;
        using base_type = blitz_base_array<T, R>;
        using base_type::base_type;
        using base_type::operator =;

#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
        template<typename T_expr>
        fort_array_base(blitz::_bz_ArrayExpr<T_expr> expr)
            : base_type(expr)
        {}
#endif
        //template<typename T, typename S>
        //fort_array_base(fort_array_base<T, S> const & other)
        //    : base_type(other)
        //{}


#ifdef FT_USE_TINYBASEDVECTOR_AS_BASE
        //fort_array_base(blitz::Range r0, blitz::GeneralArrayStorage<1> storage)
        //{
        //}
        constexpr fort_array_base(tag_tinybasedvector_ctor)
        {
        }
#endif

        static constexpr int const_length_ = R::upper - R::lower + 1;
        static constexpr int size() {
            return const_length_;
        }
        typedef T T_array[const_length_];
        operator const T_array&() const {
            return reinterpret_cast<T_array const&>(*data());
        }
        operator T_array&() {
            return reinterpret_cast<T_array&>(*data());
        }

        fort_array_base& operator=(std::initializer_list<T> list) {
            T* data_ = data();
            for (auto&& item : list) {
                *data_ = item;
                ++data_;
            }
            return *this;
        }
    };

    template<typename T>
    class fort_array_base<T,dyn> : public blitz::Array<T, 1, blitz::array_policy::GeneralFortranArrayPolicy>
    {
    public:
        using blitz_type = blitz::Array<T, 1, blitz::array_policy::GeneralFortranArrayPolicy>;
        using base_type = blitz::Array<T, 1, blitz::array_policy::GeneralFortranArrayPolicy>;
        using base_type::base_type;
        using base_type::operator =;

        template<typename T, typename S>
        fort_array_base(fort_array_base<T, S> const & other)
            : base_type(other)
        {}

        // copy-ctor is not inherited
        fort_array_base(blitz_type const & other)
            : base_type(other)
        {}

        fort_array_base()
            : base_type(blitz::fortranArray)
        {}


        typedef T T_array[];
        operator const T_array&() const {
            return reinterpret_cast<T_array const&>(*data());
        }
        operator T_array&() {
            return reinterpret_cast<T_array&>(*data());
        }
        fort_array_base& operator=(std::initializer_list<T> list) {
            T* data_ = data();
            for (auto&& item : list) {
                *data_ = item;
                ++data_;
            }
            return *this;
        }
    };




    template<typename T, int Lower>
    class fort_array_base<T, fixed_lb_dyn_ub<Lower>> : public blitz::Array<T, 1, blitz::array_policy::GeneralFortranArrayPolicy> {
    public:
        using blitz_type = blitz::Array<T, 1, blitz::array_policy::GeneralFortranArrayPolicy>;
        using base_type = blitz::Array<T, 1, blitz::array_policy::GeneralFortranArrayPolicy>;
        using base_type::base_type;
        using base_type::operator =;

//        template<typename T, typename S>
//        fort_array_base(fort_array_base<T, S> const & other)
//            : base_type(other) {}
//
//        // copy-ctor is not inherited
//        fort_array_base(blitz_type const & other)
//            : base_type(other) {}

        fort_array_base()
            : base_type(blitz::fortranArray) {}


        typedef T T_array[];
        operator const T_array&() const {
            return reinterpret_cast<T_array const&>(*data());
        }
        operator T_array&() {
            return reinterpret_cast<T_array&>(*data());
        }
        fort_array_base& operator=(std::initializer_list<T> list) {
            T* data_ = data();
            for (auto&& item : list) {
                *data_ = item;
                ++data_;
            }
            return *this;
        }
    };


    // fortran array
    template<typename T, typename R>
    class fort_array : public fort_array_base<T,R>
    {
    public:
        using base_type = fort_array_base<T, R>;

        using base_type::operator =;
        using base_type::base_type;

#if _MSC_VER > 1900
//#error check
//        template<typename ...Args>
//        constexpr fort_array(Args&& ...args) : base_type{std::forward<Args>(args)...}
//        {
//        }
#endif
        constexpr fort_array()
#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
            : base_type(blitz::Range(R::lower, R::upper), blitz::fortranArray)
#else
            : base_type(tag_tinybasedvector_ctor())
#endif
        {}

        //        fort_array(std::initializer_list<T> const & init)
//#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
//            : base_type(blitz::Range(R::lower, R::upper), blitz::fortranArray)
//#else
//            : base_type(tag_tinybasedvector_ctor())
//#endif
//        {
//            auto i = R::lower;
//            for(auto&& v : init)
//            {
//                (*this)(i) = v;
//                ++i;
//            }
//        }

        template<int I1>
        explicit fort_array(fixed_fort_array<T, I1> const& other)
            : base_type(other.noConst().data(), blitz::TinyVector<int, 1>(I1), blitz::fortranArray)
        {
        }

#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
        fort_array(T_array const& data)
            : base_type(const_cast<T_array&>(data), blitz::TinyVector<int, 1>(const_length_), blitz::fortranArray)
        {
        }
        fort_array(T_array& data)
            : base_type(data, blitz::TinyVector<int, 1>(const_length_), blitz::fortranArray)
        {
        }
#endif

        template<class T>
        explicit fort_array(T&& expr)
#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
            : base_type(blitz::Range(R::lower, R::upper), blitz::fortranArray)
#else
            : base_type(tag_tinybasedvector_ctor())
#endif
        {
            *this = expr;
        }


        //fort_array( typename base_type::T_numtype(&x)[base_type::n_length]) : base_type(x){
        //}

        //operator const T*() const {
        //    return data();
        //}
        //operator T*() {
        //    return data();
        //}

        //operator base_type&(){
        //    return *this
        //}
        //operator base_type const &() const {
        //    return *this
        //}


        template<
            typename T1,
            typename T2,
            typename ...Ts
        >
        fort_array(
            T1&& t1,
            T2&& t2,
            Ts&&... ts
            )
            : base_type(tag_tinybasedvector_ctor())
        {
            int index = 0;
            values_init(
                index,
                std::forward<T1>(t1),
                std::forward<T2>(t2),
                std::forward<Ts>(ts)...
            );
        }

    private:
        void values_init(
            int index
        )
        {}

        template<
            typename T1,
            typename ...Ts
        >
            void values_init(
                int index,
                T1&& t1,
                Ts&&... ts
            )
        {
            value_init(
                index,
                std::forward<T1>(t1)
            );
            values_init(
                index,
                std::forward<Ts>(ts)...
            );
        }


        void value_init(
            int& index,
            std::initializer_list<T> const & init
        )
        {
            for(auto&& v : init)
            {
                (*this)(++index) = v;
            }
        }

        template<typename S>
        void value_init(
            int& index,
            fort_array<T,S> const & init
        )
        {
            auto i = S::lower;
            while(i <= S::upper)
            {
                (*this)(++index) = init(i);
                ++i;
            }
        }

        //template<typename S>
        //void value_init(
        //    int& index,
        //    const_expr_fort_array<T,S> const & init
        //)
        //{
        //    auto i = S::lower;
        //    while(i <= S::upper)
        //    {
        //        (*this)(++index) = init(i);
        //        ++i;
        //    }
        //}

        void value_init(
            int& index,
            T const & init
        )
        {
            (*this)(++index) = init;
        }


        void value_init(
            int& index,
            MULT_INIT_wrap<T> const & init
        )
        {
            auto i = init.lower;
            while(i <= init.upper)
            {
                (*this)(++index) = init.t;
                ++i;
            }
        }


    public:
        template<int Lower, int Upper>
        ft::fort_array<T, ft::bs<Lower, Upper>>& subrange() {
            return reinterpret_cast<ft::fort_array<T, ft::bs<Lower, Upper>>&>((*this)(Lower));
        }


    };

    // fortran array reference
    template<typename T, typename R>
    class fort_array_r_ : public fort_array_base<T, R>
    {
    public:
        using base_type = fort_array_base<T, R>;
        using base_type::operator =;


        fort_array_r_(fort_array_base<T, R>& other)
            : base_type(other)
        {
        }

        //fort_array_r_& operator = (fort_array_base<T, R> const& other)
        //{

        //}

        //operator base_type&(){
        //    return *this
        //}
        //operator base_type const &() const {
        //    return *this
        //}
    };

    // fortran array reference
    template<typename T>
    class fort_array_r_<T, dyn> : public fort_array_base<T, dyn>
    {
    public:
        using base_type = fort_array_base<T, dyn>;
        using base_type::operator =;

        fort_array_r_(base_type const& other)
            : base_type(other)
        {
        }

        //template<typename S>
        //fort_array_r_(fort_array_base<T, S> const& other)
        //    : base_type(other)
        //{
        //}

        fort_array_r_(base_type::base_type const& other)
            : base_type(other)
        {
        }

        // checked ok!
        fort_array_r_(T& data)
            : base_type(blitz::array_policy::tag_generalfortranarraypolicy{}, &data, blitz::TinyVector<int, 1>(-1), blitz::fortranArray)
        {
        }
        // checked ok!
        template<typename S1, typename S2>
        fort_array_r_(fort_array_2_base<T, S1, S2>& other)
            : base_type(blitz::array_policy::tag_generalfortranarraypolicy{}, other.data(), blitz::TinyVector<int, 1>(-1), blitz::fortranArray)
        {
        }


        fort_array_r_(T_array const& data)
            : base_type(const_cast<T_array&>(data), blitz::TinyVector<int, 1>(const_length_), blitz::fortranArray)
        {
        }
        fort_array_r_(T_array& data)
            : base_type(data, blitz::TinyVector<int, 1>(const_length_), blitz::fortranArray)
        {
        }

        template<int I1>
        fort_array_r_(fixed_fort_array<T, I1> & other)
            : base_type(other.data(), blitz::TinyVector<int, 1>(I1), blitz::fortranArray)
        {
        }
        operator base_type&() = delete;
    };


    // fortran array const-reference
    template<typename T, typename R>
    class fort_array_cr_ : public fort_array_base<T, R>
    {
    public:
        using base_type = fort_array_base<T, R>;
        using base_type::base_type;
        fort_array_cr_(base_type const& other)
            : base_type(other)
        {
        }

        template<int I1>
        fort_array_cr_(fixed_fort_array<T, I1> const& other)
            : base_type(other.noConst().data(), blitz::TinyVector<int, 1>(I1), blitz::fortranArray)
        {
            static_assert(I1 == R::upper - R::lower + 1, "I1 == R::upper - R::lower + 1");
        }

        fort_array_cr_(T_array const& data)
            : base_type(const_cast<T_array&>(data), blitz::TinyVector<int, 1>(const_length_), blitz::fortranArray)
        {
        }

        operator base_type&() = delete;
    };

    // fortran array const-reference
    template<typename T>
    class fort_array_cr_<T, dyn> : public fort_array_base<T, dyn>
    {
    public:
        using base_type = fort_array_base<T, dyn>;

        //using base_type::operator =;
        //using base_type::base_type;

        fort_array_cr_(base_type const& other)
            : base_type(other)
        {
        }

        //template<typename S>
        //fort_array_cr_(fort_array_base<T, S> const& other)
        //    : base_type(other)
        //{
        //}

        fort_array_cr_(blitz_type const& other)
            : base_type(other)
        {
        }

        template<int I1>
        fort_array_cr_(fixed_fort_array<T, I1> const& other)
            : base_type(other.noConst().data(), blitz::TinyVector<int, 1>(I1), blitz::fortranArray)
        {
        }

        //fort_array_cr_(T_array const& data)
        //    : base_type(const_cast<T_array&>(data), blitz::TinyVector<int, 1>(const_length_), blitz::fortranArray)
        //{
        //}
        //fort_array_cr_(T_array& data)
        //    : base_type(data, blitz::TinyVector<int, 1>(const_length_), blitz::fortranArray)
        //{
        //}
        fort_array_cr_(T_array const& data)
            : base_type(const_cast<T_array&>(data), blitz::TinyVector<int, 1>(-1), blitz::fortranArray)
        {
        }
        fort_array_cr_(T_array& data)
            : base_type(data, blitz::TinyVector<int, 1>(-1), blitz::fortranArray)
        {
        }


        operator base_type&() = delete;
    };


    // dynamic

    template<typename T>
    class fort_array<T, dyn> : public fort_array_base<T, dyn>
    {
    public:
        using base_type = fort_array_base<T, dyn>;

        using base_type::operator =;
        using base_type::base_type;

        fort_array() = default;

        fort_array(dyn_alloc_tag, const dyn_alloc& da)
        {
            resize(da.length);
        }

//        fort_array()
//#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
//            : base_type(blitz::Range(R::lower, R::upper), blitz::fortranArray)
//#else
//            : base_type(tag_tinybasedvector_ctor())
//#endif
//        {}
//
//        fort_array(std::initializer_list<T> const & init)
//#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
//            : base_type(blitz::Range(R::lower, R::upper), blitz::fortranArray)
//#else
//            : base_type(tag_tinybasedvector_ctor())
//#endif
//        {
//            auto i = R::lower;
//            for(auto&& v : init)
//            {
//                (*this)(i) = v;
//                ++i;
//            }
//        }
//
//        template<int I1>
//        fort_array(fixed_fort_array<T, I1> const& other)
//            : base_type(other.noConst().data(), blitz::TinyVector<int, 1>(I1), blitz::fortranArray)
//        {
//        }
//
//#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
//        fort_array(T_array const& data)
//            : base_type(const_cast<T_array&>(data), blitz::TinyVector<int, 1>(const_length_), blitz::fortranArray)
//        {
//        }
//        fort_array(T_array& data)
//            : base_type(data, blitz::TinyVector<int, 1>(const_length_), blitz::fortranArray)
//        {
//        }
//#endif
//
//        template<class T>
//        fort_array(T&& expr)
//#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
//            : base_type(blitz::Range(R::lower, R::upper), blitz::fortranArray)
//#else
//            : base_type(tag_tinybasedvector_ctor())
//#endif
//        {
//            *this = expr;
//        }


        //fort_array( typename base_type::T_numtype(&x)[base_type::n_length]) : base_type(x){
        //}

        //operator const T*() const {
        //    return data();
        //}
        //operator T*() {
        //    return data();
        //}

        //operator base_type&(){
        //    return *this
        //}
        //operator base_type const &() const {
        //    return *this
        //}

        using base_type::operator();
        decltype(auto) operator()(allocating_rng const& r){
            return make_allocator_wrap(*this, r);
        }
    };


    template<typename T, int Lower>
    class fort_array<T, fixed_lb_dyn_ub<Lower>> : public fort_array_base<T, fixed_lb_dyn_ub<Lower>> {
    public:
        using base_type = fort_array_base<T, fixed_lb_dyn_ub<Lower>>;

        using base_type::operator =;
        using base_type::base_type;

        fort_array() = default;

        fort_array(dyn_alloc_tag, const dyn_alloc& da) {
            resize(da.length);
        }

        // prohibit reindexing !!!
        void reindexSelf(const T_index&) = delete;


        using base_type::operator();
        decltype(auto) operator()(allocating_fixed_lb_dyn_ub<Lower> const& r) {
            return make_allocator_wrap(*this, r);
        }
    };




    template<typename T, typename R>
    struct make_fort_array_alloc;

    template<typename T>
    struct make_fort_array_alloc<T, dyn_dim>{
        using type = fort_array<T, dyn>;
    };


    template<typename T, typename R>
    using fort_array_alloc = typename make_fort_array_alloc<T, R>::type;



#define FT_USE_REFERENCES
#ifdef FT_USE_REFERENCES


    template<typename T, typename R>
    struct make_fort_array_r{
        using type = fort_array<T, R> &;
    };

    template<typename T>
    struct make_fort_array_r<T, dyn> {
        using type = fort_array_r_<T, dyn>;
    };

    template<typename T, int Lower>
    struct make_fort_array_r<T, fixed_lb_dyn_ub<Lower>> {
        using type = fort_array<T, fixed_lb_dyn_ub<Lower>> &;
    };

    template<typename T, typename R>
    using fort_array_r = typename make_fort_array_r<T, R>::type;




    template<typename T, typename R>
    struct make_fort_array_cr{
        using type = fort_array<T, R> const&;
    };

    template<typename T>
    struct make_fort_array_cr<T, dyn>{
        using type = fort_array_cr_<T, dyn>;
    };

    template<typename T, int Lower>
    struct make_fort_array_cr<T, fixed_lb_dyn_ub<Lower>> {
        using type = fort_array<T, fixed_lb_dyn_ub<Lower>> const &;
    };

    template<typename T, typename R>
    using fort_array_cr = typename make_fort_array_cr<T, R>::type;

#else
    template<typename T, typename R>
    using fort_array_cr = fort_array_cr_<T, R>;

    template<typename T, typename R>
    using fort_array_r = fort_array_r_<T, R>;

#endif

    template<typename T, typename R>
    struct make_fort_array_alloc_cr;

    template<typename T>
    struct make_fort_array_alloc_cr<T, dyn_dim>{
        using type = fort_array_cr<T, dyn>;
    };


    template<typename T, typename R>
    using fort_array_alloc_cr = typename make_fort_array_alloc_cr<T, R>::type;



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 2 - dimensions
//



#ifdef FT_USE_TINYBASEDVECTOR_AS_BASE
    template<typename T, typename R1, typename R2>
    struct make_blitz_base_array_2{
        using type = blitz::TinyBasedMatrix<T, R1::upper - R1::lower + 1, R2::upper - R2::lower + 1, R1::lower, R2::lower, true>;
    };
#else
    template<typename T, typename R>
    struct make_blitz_base_array{
        using type = blitz::Array<T, 2, blitz::array_policy::GeneralFortranArrayPolicy>;
    };
#endif

    template<typename T, typename R1, typename R2>
    using blitz_base_array_2 = typename make_blitz_base_array_2<T, R1, R2>::type;








    template<typename T, typename R1, typename R2>
    class fort_array_2_base : public blitz_base_array_2<T, R1, R2>
    {
    public:
        using base_type = blitz_base_array_2<T, R1, R2>;
        using base_type::base_type;
        using base_type::operator =;

        template<typename T, typename S1, typename S2>
        fort_array_2_base(fort_array_2_base<T, S1, S2> const & other)
            : base_type(other)
        {}


#ifdef FT_USE_TINYBASEDVECTOR_AS_BASE
        fort_array_2_base(tag_tinybasedmatrix_ctor)
        {
        }
#endif

        //operator base_type&(){
        //    return *this
        //}
        //operator base_type const &() const {
        //    return *this
        //}
        static constexpr int length1_ = (R1::upper - R1::lower + 1);
        static constexpr int length2_ = (R2::upper - R2::lower + 1);
        typedef T T_array[length1_ * length2_];
        operator const T_array&() const {
            return (const T_array&)*data();
        }
        operator T_array&() {
            return (T_array&)*data();
        }
    };

    template<typename T>
    class fort_array_2_base<T, dyn, dyn> : public blitz::Array<T, 2, blitz::array_policy::GeneralFortranArrayPolicy>
    {
    public:
        using blitz_type = blitz::Array<T, 2, blitz::array_policy::GeneralFortranArrayPolicy>;
        using base_type = blitz::Array<T, 2, blitz::array_policy::GeneralFortranArrayPolicy>;
        using base_type::base_type;
        using base_type::operator =;

        template<typename T, typename S1, typename S2>
        fort_array_2_base(fort_array_2_base<T, S1, S2> const & other)
            : base_type(other)
        {}

        // copy-ctor is not inherited
        fort_array_2_base(blitz_type const & other)
            : base_type(other)
        {}

        fort_array_2_base() 
            : base_type(blitz::fortranArray)
        {
        }

        fort_array_2_base(dyn_alloc_tag, const dyn_alloc& da1, const dyn_alloc& da2)
            : base_type(blitz::fortranArray)
        {
            resize(da1.length, da2.length);
        }


        typedef T T_array[];
        operator const T_array&() const {
            return reinterpret_cast<T_array const&>(*data());
        }
        operator T_array&() {
            return reinterpret_cast<T_array&>(*data());
        }

        using base_type::operator();
        decltype(auto) operator()(allocating_rng const& r1, allocating_rng const& r2){
            return make_allocator_2_wrap(*this, r1, r2);
        }

    };


    template<typename T, int Lower1, int Lower2>
    class fort_array_2_base<T, fixed_lb_dyn_ub<Lower1>, fixed_lb_dyn_ub<Lower2>> : public blitz::Array<T, 2, blitz::array_policy::GeneralFortranArrayPolicy> {
    public:
        using blitz_type = blitz::Array<T, 2, blitz::array_policy::GeneralFortranArrayPolicy>;
        using base_type = blitz::Array<T, 2, blitz::array_policy::GeneralFortranArrayPolicy>;
        using base_type::base_type;
        using base_type::operator =;

        //        template<typename T, typename S>
        //        fort_array_base(fort_array_base<T, S> const & other)
        //            : base_type(other) {}
        //
        //        // copy-ctor is not inherited
        //        fort_array_base(blitz_type const & other)
        //            : base_type(other) {}

        fort_array_2_base()
            : base_type(blitz::fortranArray) {}


        typedef T T_array[];
        operator const T_array&() const {
            return reinterpret_cast<T_array const&>(*data());
        }
        operator T_array&() {
            return reinterpret_cast<T_array&>(*data());
        }
    };


    template<typename T, typename R1, typename R2>
    class fort_array_2 : public fort_array_2_base<T, R1, R2>
    {
    public:
        using base_type = fort_array_2_base<T, R1, R2>;

        using base_type::operator =;
        using base_type::base_type;


        fort_array_2()
#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
            : base_type(blitz::Range(R1::lower, R1::upper), blitz::Range(R2::lower, R2::upper), blitz::fortranArray)
#else
            : base_type(tag_tinybasedmatrix_ctor())
#endif
        {}

        fort_array_2(std::initializer_list<T> const & init)
#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
            : base_type(blitz::Range(R1::lower, R1::upper), blitz::Range(R2::lower, R2::upper), blitz::fortranArray)
#else
            : base_type(tag_tinybasedmatrix_ctor())
#endif
        {
            auto it = init.begin();
            for(int i2 = R2::lower; i2 <= R2::upper; ++i2)
            {
                for(int i1 = R1::lower; i1 <= R1::upper; ++i1)
                {
                    (*this)(i1,i2) = *it;
                    ++it;
                }
            }
        }

#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
        fort_array_2(T_array const& data)
#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
            : base_type(const_cast<T_array&>(data), blitz::TinyVector<int, 2>(length1_, length2_), blitz::fortranArray)
#else
            : base_type(tag_tinybasedmatrix_ctor())
#endif
        {
        }

        fort_array_2(T_array& data)
#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
            : base_type(data, blitz::TinyVector<int, 2>(length1_, length2_), blitz::fortranArray)
#else
            : base_type(tag_tinybasedmatrix_ctor())
#endif
        {
        }
#endif


        template<class T>
        fort_array_2(T&& expr)
#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
            : base_type(blitz::Range(R1::lower, R1::upper), blitz::Range(R2::lower, R2::upper), blitz::fortranArray)
#else
            : base_type(tag_tinybasedmatrix_ctor())
#endif
        {
            (base_type&)*this = expr;
        }

        fort_array_2(std::initializer_list<fort_array<T, R1>> const & init)
#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
            : base_type(blitz::Range(R1::lower, R1::upper), blitz::Range(R2::lower, R2::upper), blitz::fortranArray)
#else
            : base_type(tag_tinybasedmatrix_ctor())
#endif
        {
            auto i = R2::lower;
            for(auto&& v : init)
            {
                (*this)(blitz::Range(), i) = v;
                ++i;
            }
        }


        //operator base_type&(){
        //    return *this
        //}
        //operator base_type const &() const {
        //    return *this
        //}



        template<
            typename T1,
            typename T2,
            typename ...Ts
        >
            fort_array_2(
                T1&& t1,
                T2&& t2,
                Ts&&... ts
            )
            : base_type(tag_tinybasedmatrix_ctor())
        {
            int index = -1;
            values_init(
                index,
                std::forward<T1>(t1),
                std::forward<T2>(t2),
                std::forward<Ts>(ts)...
            );
        }

    private:
        void values_init(
            int index
        )
        {}

        template<
            typename T1,
            typename ...Ts
        >
            void values_init(
                int index,
                T1&& t1,
                Ts&&... ts
            )
        {
            value_init(
                index,
                std::forward<T1>(t1)
            );
            values_init(
                index,
                std::forward<Ts>(ts)...
            );
        }


        void value_init(
            int& index,
            std::initializer_list<T> const & init
        )
        {
            for(auto&& v : init)
            {
                data()[++index] = v;
            }
        }

        template<typename S>
        void value_init(
            int& index,
            fort_array<T, S> const & init
        )
        {
            auto i = S::lower;
            while(i <= S::upper)
            {
                data()[++index] = init(i);
                ++i;
            }
        }

        //template<typename S>
        //void value_init(
        //    int& index,
        //    const_expr_fort_array<T, S> const & init
        //)
        //{
        //    auto i = S::lower;
        //    while(i <= S::upper)
        //    {
        //        data()[++index] = init(i);
        //        ++i;
        //    }
        //}

        void value_init(
            int& index,
            T const & init
        )
        {
            data()[++index] = init;
        }


        void value_init(
            int& index,
            MULT_INIT_wrap<T> const & init
        )
        {
            auto i = init.lower;
            while(i <= init.upper)
            {
                data()[++index] = init.t;
                ++i;
            }
        }



    };

    // fortran array reference
    template<typename T, typename R1, typename R2>
    class fort_array_2_r_ : public fort_array_2_base<T, R1, R2>
    {
    public:
        using base_type = fort_array_2_base<T, R1, R2>;
        using base_type::operator =;


        fort_array_2_r_(fort_array_2_base<T, R1, R2>& other) = delete;
        //fort_array_2_r_(fort_array_2_base<T, R1, R2>& other)
        //    : base_type(other)
        //{
        //}

        // checked ok
        fort_array_2_r_(T& data)
            : base_type(blitz::array_policy::tag_generalfortranarraypolicy{}, &data, blitz::TinyVector<int, 2>(-1, -1), blitz::fortranArray)
        {
        }
        // checked ok
        template<typename S1>
        fort_array_2_r_(fort_array_base<T, S1>& other)
            : base_type(blitz::array_policy::tag_generalfortranarraypolicy{}, other.data(), blitz::TinyVector<int, 2>(-1, -1), blitz::fortranArray)
        {
        }
        //fort_array_2_r_& operator = (fort_array_2_base<T, R> const& other)
        //{

        //}

        //operator base_type&(){
        //    return *this
        //}
        //operator base_type const &() const {
        //    return *this
        //}
    };

    // fortran array const-reference
    template<typename T, typename R1, typename R2>
    class fort_array_2_cr_ : public fort_array_2_base<T, R1, R2>
    {
    public:
        using base_type = fort_array_2_base<T, R1, R2>;

        fort_array_2_cr_(base_type const& other) = delete;

        //fort_array_2_cr_(base_type const& other)
        //    : base_type(other)
        //{
        //}


        operator base_type&() = delete;
    };

    // fortran array const-reference
    template<typename T>
    class fort_array_2_cr_<T, dyn, dyn> : public fort_array_2_base<T, dyn, dyn>
    {
    public:
        using base_type = fort_array_2_base<T, dyn, dyn>;

        fort_array_2_cr_(base_type const& other)
            : base_type(other)
        {
        }

        template<typename S1, typename S2>
        fort_array_2_cr_(fort_array_2_base<T, S1, S2> const& other)
            : base_type(other)
        {
        }


        operator base_type&() = delete;
    };


    template<typename T>
    class fort_array_2<T,dyn,dyn> : public fort_array_2_base<T, dyn, dyn>
    {
    public:
        using base_type = fort_array_2_base<T, dyn, dyn>;

        using base_type::operator =;
        using base_type::base_type;


        fort_array_2()
//#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
//            : base_type(blitz::Range(R1::lower, R1::upper), blitz::Range(R2::lower, R2::upper), blitz::fortranArray)
//#else
//            : base_type(tag_tinybasedmatrix_ctor())
//#endif
        {}

//        fort_array_2(std::initializer_list<T> const & init)
//#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
//            : base_type(blitz::Range(R1::lower, R1::upper), blitz::Range(R2::lower, R2::upper), blitz::fortranArray)
//#else
//            : base_type(tag_tinybasedmatrix_ctor())
//#endif
//        {
//            auto it = init.begin();
//            for(int i2 = R2::lower; i2 <= R2::upper; ++i2)
//            {
//                for(int i1 = R1::lower; i1 <= R1::upper; ++i1)
//                {
//                    (*this)(i1, i2) = *it;
//                    ++it;
//                }
//            }
//        }
//
//#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
//        fort_array_2(T_array const& data)
//#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
//            : base_type(const_cast<T_array&>(data), blitz::TinyVector<int, 2>(length1_, length2_), blitz::fortranArray)
//#else
//            : base_type(tag_tinybasedmatrix_ctor())
//#endif
//        {
//        }
//
//        fort_array_2(T_array& data)
//#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
//            : base_type(data, blitz::TinyVector<int, 2>(length1_, length2_), blitz::fortranArray)
//#else
//            : base_type(tag_tinybasedmatrix_ctor())
//#endif
//        {
//        }
//#endif
//
//
//        template<class T>
//        fort_array_2(T&& expr)
//#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
//            : base_type(blitz::Range(R1::lower, R1::upper), blitz::Range(R2::lower, R2::upper), blitz::fortranArray)
//#else
//            : base_type(tag_tinybasedmatrix_ctor())
//#endif
//        {
//            (base_type&)*this = expr;
//        }
//
//        fort_array_2(std::initializer_list<fort_array<T, R1>> const & init)
//#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
//            : base_type(blitz::Range(R1::lower, R1::upper), blitz::Range(R2::lower, R2::upper), blitz::fortranArray)
//#else
//            : base_type(tag_tinybasedmatrix_ctor())
//#endif
//        {
//            auto i = R2::lower;
//            for(auto&& v : init)
//            {
//                (*this)(blitz::Range(), i) = v;
//                ++i;
//            }
//        }


        //operator base_type&(){
        //    return *this
        //}
        //operator base_type const &() const {
        //    return *this
        //}
    };


    template<typename T, int Lower1, int Lower2>
    class fort_array_2<T, fixed_lb_dyn_ub<Lower1>, fixed_lb_dyn_ub<Lower2>> : public fort_array_2_base<T, fixed_lb_dyn_ub<Lower1>, fixed_lb_dyn_ub<Lower2>> {
    public:
        using base_type = fort_array_2_base<T, fixed_lb_dyn_ub<Lower1>, fixed_lb_dyn_ub<Lower2>>;

        using base_type::operator =;
        using base_type::base_type;

        fort_array_2() = default;

        fort_array_2(dyn_alloc_tag, const dyn_alloc& da1, const dyn_alloc& da2)
        {
            resize(da1.length, da2.length);
        }

        // prohibit reindexing !!!
        void reindexSelf(const T_index&) = delete;

        using base_type::operator();
        decltype(auto) operator()(allocating_fixed_lb_dyn_ub<Lower1> const& r1, allocating_fixed_lb_dyn_ub<Lower2> const& r2) {
            return make_allocator_2_wrap(*this, r1, r2);
        }
    };



    template<typename T, typename R1, typename R2>
    struct make_fort_array_alloc_2;

    template<typename T>
    struct make_fort_array_alloc_2<T, dyn_dim, dyn_dim>{
        using type = fort_array_2<T, dyn, dyn>;
    };


    template<typename T, typename R1, typename R2>
    using fort_array_alloc_2 = typename make_fort_array_alloc_2<T, R1, R2>::type;



    template<typename T, typename R1, typename R2>
    struct make_fort_array_2_r {
        using type = fort_array_2<T, R1, R2> &;
    };

    template<typename T>
    struct make_fort_array_2_r<T, dyn, dyn> {
        using type = fort_array_2_r_<T, dyn, dyn>;
    };

    template<typename T, int Lower1, int Lower2>
    struct make_fort_array_2_r<T, fixed_lb_dyn_ub<Lower1>, fixed_lb_dyn_ub<Lower2>> {
        using type = fort_array_2<T, fixed_lb_dyn_ub<Lower1>, fixed_lb_dyn_ub<Lower2>> &;
    };

    template<typename T, typename R1, typename R2>
    using fort_array_2_r = typename make_fort_array_2_r<T, R1, R2>::type;




    template<typename T, typename R1, typename R2>
    struct make_fort_array_2_cr {
        using type = fort_array_2<T, R1, R2> const&;
    };

    template<typename T>
    struct make_fort_array_2_cr<T, dyn, dyn> {
        using type = fort_array_2_cr_<T, dyn, dyn>;
    };

    template<typename T, int Lower1, int Lower2>
    struct make_fort_array_2_cr<T, fixed_lb_dyn_ub<Lower1>, fixed_lb_dyn_ub<Lower2>> {
        using type = fort_array_2<T, fixed_lb_dyn_ub<Lower1>, fixed_lb_dyn_ub<Lower2>> const &;
    };

    template<typename T, typename R1, typename R2>
    using fort_array_2_cr = typename make_fort_array_2_cr<T, R1, R2>::type;





    template<typename T, typename R1, typename R2, typename R3>
    class fort_array_3_base : public blitz::Array<T, 3, blitz::array_policy::GeneralFortranArrayPolicy>
    {
    public:
        using base_type = blitz::Array<T, 3, blitz::array_policy::GeneralFortranArrayPolicy>;
        using base_type::base_type;
        using base_type::operator =;

        template<typename T, typename S1, typename S2, typename S3>
        fort_array_3_base(fort_array_3_base<T, S1, S2, S3> const & other)
            : base_type(other)
        {}


        //operator base_type&(){
        //    return *this
        //}
        //operator base_type const &() const {
        //    return *this
        //}
    };

    template<typename T, typename R1, typename R2, typename R3>
    class fort_array_3 : public fort_array_3_base<T, R1, R2, R3>
    {
    public:
        using base_type = fort_array_3_base<T, R1, R2, R3>;

        using base_type::operator =;
        using base_type::base_type;


        fort_array_3()
            : base_type(blitz::Range(R1::lower, R1::upper), blitz::Range(R2::lower, R2::upper), blitz::Range(R3::lower, R3::upper), blitz::fortranArray)
        {}

        fort_array_3(std::initializer_list<T> const & init)
            : base_type(blitz::Range(R1::lower, R1::upper), blitz::Range(R2::lower, R2::upper), blitz::Range(R3::lower, R3::upper), blitz::fortranArray)
        {
            auto it = init.begin();
            for(int i3 = R3::lower; i3 <= R3::upper; ++i3)
            {
                for(int i2 = R2::lower; i2 <= R2::upper; ++i2)
                {
                    for(int i1 = R1::lower; i1 <= R1::upper; ++i1)
                    {
                        (*this)(i1, i2, i3) = *it;
                        ++it;
                    }
                }
            }
        }

        fort_array_3(std::initializer_list<fort_array_2<T,R1,R2>> const & init)
            : base_type(blitz::Range(R1::lower, R1::upper), blitz::Range(R2::lower, R2::upper), blitz::Range(R3::lower, R3::upper), blitz::fortranArray)
        {
            auto i = R3::lower;
            for(auto&& v : init)
            {
                (*this)(blitz::Range(), blitz::Range(), i) = v;
                ++i;
            }
        }

        template<class T>
        fort_array_3(T&& expr)
            : base_type(blitz::Range(R1::lower, R1::upper), blitz::Range(R2::lower, R2::upper), blitz::Range(R3::lower, R3::upper), blitz::fortranArray)
        {
            *this = expr;
        }


        //operator base_type&(){
        //    return *this
        //}
        //operator base_type const &() const {
        //    return *this
        //}
    };

    // fortran array reference
    template<typename T, typename R1, typename R2, typename R3>
    class fort_array_3_r : public fort_array_3_base<T, R1, R2, R3>
    {
    public:
        using base_type = fort_array_3_base<T, R1, R2, R3>;
        using base_type::operator =;


        fort_array_3_r(fort_array_3_base<T, R1, R2, R3>& other)
            : base_type(other)
        {
        }

        //fort_array_3_r& operator = (fort_array_3_base<T, R> const& other)
        //{

        //}

        //operator base_type&(){
        //    return *this
        //}
        //operator base_type const &() const {
        //    return *this
        //}
    };

    // fortran array const-reference
    template<typename T, typename R1, typename R2, typename R3>
    class fort_array_3_cr : public fort_array_3_base<T, R1, R2, R3>
    {
    public:
        using base_type = fort_array_3_base<T, R1, R2, R3>;

        fort_array_3_cr(base_type const& other)
            : base_type(other)
        {
        }


        operator base_type&() = delete;
    };

    // fortran array const-reference
    template<typename T>
    class fort_array_3_cr<T, dyn, dyn, dyn> : public fort_array_3_base<T, dyn, dyn, dyn>
    {
    public:
        using base_type = fort_array_3_base<T, dyn, dyn, dyn>;

        fort_array_3_cr(base_type const& other)
            : base_type(other)
        {
        }

        template<typename S1, typename S2, typename S3>
        fort_array_3_cr(fort_array_3_base<T, S1, S2, S3> const& other)
            : base_type(other)
        {
        }


        operator base_type&() = delete;
    };








    //inline blitz::Range rng_t() { return blitz::Range()
    //}




    static constexpr int rng_first = blitz::fromStart;
    static constexpr int rng_last = blitz::toEnd;

    template<int Lower = blitz::fromStart, int Upper = blitz::toEnd, int Stride = 1>
    rng_t srng(){
        return rng_t(Lower, Upper, Stride);
    }

    
    inline auto rng(int first, int last) {
        return rng_t(first, last);
    }

    inline auto rng() {
        return rng_t();
    }

    inline auto stride(int first, int last, int stride) {
        return blitz::Stride(first, last, stride);
    }





#define FT_USE_ARRAY_ADJUSTMENT
#ifdef FT_USE_ARRAY_ADJUSTMENT
    template<typename Array_>
    class arrray_adjuster {
        Array_& arr;
        dyn_bounds::T_numtype old_first;
    public:
        arrray_adjuster(Array_& arr, dyn_bounds const & bs)
        : arr(arr)
            , old_first(arr.base()(0))
        {
            arr.reindexSelf(bs.first(1));
        }

        ~arrray_adjuster()
        {
            arr.reindexSelf(old_first);
        }
    };

    template<typename Array_>
    class arrray_adjuster_2 {
        Array_& arr;
        dyn_bounds::T_numtype old_first_1;
        dyn_bounds::T_numtype old_first_2;
        blitz::TinyVector<int, 2> old_shape;
    public:
        arrray_adjuster_2(Array_& arr, dyn_bounds const & bs1, dyn_bounds const & bs2)
            : arr(arr)
            , old_first_1(arr.base()(0))
            , old_first_2(arr.base()(1))
            , old_shape(arr.shape())
        {
            arr.resize_static(bs1.length(), bs2.length());
            arr.reindexSelf({bs1.first(1), bs2.first(1)});
        }

        ~arrray_adjuster_2()
        {
            arr.resize_static(old_shape);
            arr.reindexSelf({old_first_1, old_first_2});
        }
    };
#else
    template<typename Array_>
    class arrray_adjuster {
        //Array_& arr;
        //dyn_bounds::T_numtype old_first;
    public:
        arrray_adjuster(Array_& arr, dyn_bounds const & bs)
            //: arr(arr)
            //, old_first(arr.base()(0)) {
            //arr.reindexSelf(bs.first(1));
        {}

        //~arrray_adjuster() {
        //    arr.reindexSelf(old_first);
        //}
    };

    template<typename Array_>
    class arrray_adjuster_2 {
        //Array_& arr;
        //dyn_bounds::T_numtype old_first_1;
        //dyn_bounds::T_numtype old_first_2;
    public:
        arrray_adjuster_2(Array_& arr, dyn_bounds const & bs1, dyn_bounds const & bs2)
            //: arr(arr)
            //, old_first_1(arr.base()(0))
            //, old_first_2(arr.base()(1)) {
            //arr.reindexSelf({bs1.first(1), bs2.first(1)});
        {}

        //~arrray_adjuster_2() {
        //    arr.reindexSelf({old_first_1, old_first_2});
        //}
    };
#endif








    template <int Dim, typename S>
    std::true_type is_base_of_array_dim_template_impl(const blitz::Array<S, Dim, blitz::array_policy::GeneralFortranArrayPolicy>*);

    template <int Dim>
    std::false_type is_base_of_array_dim_template_impl(...);

    template <typename T, int Dim>
    using is_base_of_array_dim_template = decltype(is_base_of_array_dim_template_impl<Dim>(std::declval<T*>()));




//not working with VS 2015
    //template <template <typename...> class C, typename...Ts>
    //std::true_type is_base_of_template_impl(const C<Ts...>*);

    //template <template <typename...> class C>
    //std::false_type is_base_of_template_impl(...);

    //template <typename T, template <typename...> class C>
    //using is_base_of_template = decltype(is_base_of_template_impl<C>(std::declval<T*>()));

    template <template <typename T1, typename T2> class C, typename T1, typename T2>
    std::true_type is_base_of_template_2_impl(const C<T1, T2>*);

    template <template <typename...> class C>
    std::false_type is_base_of_template_2_impl(...);

    template <typename T, template <typename...> class C>
    using is_base_of_template = decltype(is_base_of_template_2_impl<C>(std::declval<T*>()));




    template<
        typename Array_
//        , typename std::enable_if<is_base_of_array_dim_template<Array_, 1>::value>::type* = nullptr
//        , typename std::enable_if<is_base_of_template<Array_, fort_array_base>::value>::type* = nullptr     //not working with VS 2015
    >
        arrray_adjuster<Array_> make_arrray_adjuster(Array_& arr, dyn_bounds const & bs
            
            //, typename std::enable_if<is_base_of_template<Array_, fort_array_base>::value>::type* = nullptr

            ) {
        return arrray_adjuster<Array_>(arr, bs);
    }

    template<
        typename Array_
        //, typename std::enable_if<is_base_of_array_dim_template<Array_, 2>::value>::type* = nullptr
//        , typename std::enable_if<is_base_of_template<Array_, fort_array_2_base>::value>::type* = nullptr  //not working with VS 2015
    >
        arrray_adjuster_2<Array_> make_arrray_adjuster(Array_& arr, dyn_bounds const & bs1, dyn_bounds const & bs2
            //, typename std::enable_if<is_base_of_template<Array_, fort_array_2_base>::value>::type* = nullptr
        ) {
        return arrray_adjuster_2<Array_>(arr, bs1, bs2);
    }






    //typedef long int integer;
    //typedef char *address;
    typedef short int shortint;
    //typedef float real;
    //typedef double doublereal;
    //typedef struct {
    //    real r, i;
    //} complex;
    //typedef struct {
    //    doublereal r, i;
    //} doublecomplex;
    typedef long int logical;
    typedef short int shortlogical;
    typedef char logical1;
    typedef char integer1;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// functions

template<typename Arg>
bool is_not_empty(Arg&& arg){
    return arg != -999.0;
}

template<typename Arg>
bool is_empty(Arg&& arg){
    return !is_not_empty(std::forward<Arg>(arg));
}

template<typename Arg>
bool is_not_0(Arg&& arg){
    return arg != 0.0;
}

template<typename Arg>
bool is_0(Arg&& arg){
    return is_0(std::forward<Arg>(arg));
}


template<typename Arg>
bool is_tiny6(Arg&& arg){
    return (arg < 1.0e-6 && arg > -1.0e-6  );
}

template<typename Arg>
bool is_not_tiny6(Arg&& arg){
    return !is_tiny6(std::forward<Arg>(arg));
}

template<typename ...Args>
decltype(auto) dabs(Args&& ...args){
    return ft::abs(std::forward<Args>(args)...);
}


//template<typename Arg, typename std::enable_if<std::is_arithmetic<std::remove_reference_t<Arg>>::value>::type* = nullptr>
//decltype(auto) abs(Arg&& arg){
//    return std::abs(std::forward<Arg>(arg));
//}

template<typename ...Args>
decltype(auto) iabs(Args&& ...args){
    return ft::abs(std::forward<Args>(args)...);
}


template<typename ...Args>
decltype(auto) dsign(Args&& ...args){
    return std::copysign(std::forward<Args>(args)...);
}

template<typename ...Args>
decltype(auto) sign(Args&& ...args){
    return std::copysign(std::forward<Args>(args)...);
}

template<typename ...Args>
decltype(auto) dsqrt(Args&& ...args){
    return ft::sqrt(std::forward<Args>(args)...);
}

//template<typename ...Args>
//decltype(auto) exp(Args&& ...args){
//    return std::exp(std::forward<Args>(args)...);
//}

template<typename ...Args>
decltype(auto) dexp(Args&& ...args){
    return ft::exp(std::forward<Args>(args)...);
}

template<typename ...Args>
decltype(auto) dlog(Args&& ...args){
    return ft::log(std::forward<Args>(args)...);
}

template<typename ...Args>
decltype(auto) dlog10(Args&& ...args){
    return ft::log10(std::forward<Args>(args)...);
}

//template<typename ...Args>
//decltype(auto) log(Args&& ...args){
//    return std::log(std::forward<Args>(args)...);
//}

//template<typename ...Args>
//decltype(auto) sqrt(Args&& ...args){
//    return std::sqrt(std::forward<Args>(args)...);
//}

//template<typename ...Args>
//decltype(auto) pow(Args&& ...args){
//    return std::pow(std::forward<Args>(args)...);
//}

template<typename Arg>
intptr_t loc(Arg&& arg){
    return reinterpret_cast<intptr_t>(&arg);
}

template<typename Arg>
double real(Arg&& arg){
    return static_cast<double>(arg);
}

template<typename Arg>
double dble(Arg&& arg){
    return static_cast<double>(arg);
}


inline double secnds(double x){
    __time64_t t = _time64(nullptr);
    t %= 24 * 60 * 60;
    return static_cast<double>(t) - x;
}



//template<typename ...Args>
//decltype(auto) min(Args&& ...args){
//    return std::min(std::forward<Args>(args)...);
//}

template<typename Arg>
decltype(auto) maxval(Arg&& arg){
    return blitz::max(std::forward<Arg>(arg));
}

template<typename Arg>
decltype(auto) maxloc(Arg&& arg){
    return blitz::maxIndex(std::forward<Arg>(arg));
}

template<typename Arg, typename Mask>
decltype(auto) maxloc(Arg&& arg, int i, Mask&& mask){
    return blitz::maxIndex(blitz::where(std::forward<Mask>(mask), std::forward<Arg>(arg), INT_MIN))(i - 1);
}

template<typename Arg>
decltype(auto) maxloc(Arg&& arg, int i){
    return blitz::maxIndex(std::forward<Arg>(arg))(i - 1);
}


template<typename Arg>
decltype(auto) minval(Arg&& arg){
    return blitz::min(std::forward<Arg>(arg));
}

template<typename Arg>
decltype(auto) minloc(Arg&& arg){
    return blitz::minIndex(std::forward<Arg>(arg));
}

template<typename Arg, typename Mask>
decltype(auto) minloc(Arg&& arg, int i, Mask&& mask){
    return blitz::minIndex(blitz::where(std::forward<Mask>(mask), std::forward<Arg>(arg), INT_MIN))(i - 1);
}

template<typename Arg>
decltype(auto) minloc(Arg&& arg, int i){
    return blitz::minIndex(std::forward<Arg>(arg))(i - 1);
}

using std::exp;
using std::log;
using std::log10;
using std::abs;
using std::sqrt;
using std::pow;

using std::sin;
using std::cos;
using std::asin;
using std::acos;
using std::tan;
using std::atan;

using std::sinh;
using std::cosh;
using std::asinh;
using std::acosh;
using std::tanh;
using std::atanh;


using blitz::exp;
using blitz::log;
using blitz::log10;
using blitz::pow;

using blitz::sum;
using blitz::max;
using blitz::min;
using blitz::abs;

using blitz::sin;
using blitz::cos;
using blitz::asin;
using blitz::acos;
using blitz::tan;
using blitz::atan;

using blitz::sinh;
using blitz::cosh;
using blitz::asinh;
using blitz::acosh;
using blitz::tanh;
using blitz::atanh;

template<typename... Args>
decltype(auto) dcos(Args&&... args)
{
    return cos(std::forward<Args>(args)...);
}

template<typename... Args>
decltype(auto) dtan(Args&&... args)
{
    return tan(std::forward<Args>(args)...);
}

template<typename... Args>
decltype(auto) dtanh(Args&&... args)
{
    return tanh(std::forward<Args>(args)...);
}

inline double angle(double r, double img){
    return (is_not_0(r) ? atan(img / r) : 3.14159265358979 * (img > 0 ? 0.5 : (img < 0 ? -0.5 : 0.0)));
}

inline constexpr int max(int a, int b){
    return std::max(a, b);
}
inline constexpr long max(long a, long b){
    return std::max(a, b);
}
inline constexpr double max(double a, double b){
    return std::max(a, b);
}

inline constexpr int min(int a, int b){
    return std::min(a, b);
}
inline constexpr long min(long a, long b){
    return std::min(a, b);
}
inline constexpr double min(double a, double b){
    return std::min(a, b);
}

// only for 3 or more args (otherwise it gets messed up with blitz::min/max)
template<typename Arg1, typename Arg2, typename Arg3, typename... Args>
decltype(auto) constexpr min(Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Args&&... args)
{
    return std::min({
        std::forward<Arg1>(arg1),
        std::forward<Arg2>(arg2),
        std::forward<Arg3>(arg3),
        std::forward<Args>(args)...});
}

template<typename Arg1, typename Arg2, typename Arg3, typename... Args>
decltype(auto) constexpr max(Arg1&& arg1, Arg2&& arg2, Arg3&& arg3, Args&&... args)
{
    return std::max({
        std::forward<Arg1>(arg1),
        std::forward<Arg2>(arg2),
        std::forward<Arg3>(arg3),
        std::forward<Args>(args)...});
}

//merge is only used for single values -> use conditional-operator
//template<typename Arg1, typename Arg2, typename Arg3>
//decltype(auto) merge(Arg1&& arg1, Arg2&& arg2, Arg3&& mask){
//    return  blitz::where(
//        std::forward<Arg3>(mask),
//        std::forward<Arg1>(arg1),
//        std::forward<Arg2>(arg2)
//        );
//}
template<typename Arg1, typename Arg2, typename Arg3>
decltype(auto) constexpr merge(Arg1&& arg1, Arg2&& arg2, Arg3&& mask){
    return  mask ? std::forward<Arg1>(arg1) : std::forward<Arg2>(arg2);
}

//template<typename Arg1, typename Arg2>
//decltype(auto) matmul_unadjusted(Arg1&& arg1, Arg2&& arg2){
//    using namespace blitz;
//    firstIndex i;
//    secondIndex j;
//    return sum(std::forward<Arg1>(arg1)(j) * std::forward<Arg2>(arg2)(j, i), j);
//}

//template<typename Arg1, typename Arg2>
//decltype(auto) matmul(Arg1&& arg1, Arg2&& arg2){
//    using namespace blitz;
//    firstIndex i;
//    secondIndex j;
//    blitz::GeneralArrayStorage<1> storage;
//    storage.setBase(arg2.lbound(0));
//    blitz::Array<double, 1, blitz::array_policy::GeneralFortranArrayPolicy> B(arg1.noConst().data(), arg1.length(), storage);
//    return blitz::Array<double, 1, blitz::array_policy::GeneralFortranArrayPolicy>(sum(B(j) * std::forward<Arg2>(arg2)(j, i), j));
//}


// clamps val to minval, maxval returns 0|-1|1 if val is within range|smaller|greater
template<typename Arg>
decltype(auto) clamp_test(Arg& val, Arg const & minval, Arg const & maxval) {
    if(val < minval) {
        val = minval;
        return -1;
    } else if(val > maxval) {
        val = maxval;
        return 1;
    } else {
        return 0;
    }
}



namespace aux{
    template <typename T, int N>
    std::true_type is_tbv_1_help_impl(const blitz::TinyBasedVector<T, N, 1>*);

    std::false_type is_tbv_1_help_impl(...);

    template <typename Arg>
    using is_tbv_1_help = decltype(is_tbv_1_help_impl(std::declval<Arg*>()));


    template<typename Arg>
    struct is_tbv_1 :
        std::conditional <
        is_tbv_1_help<std::remove_cv_t<std::remove_reference_t<Arg>>>::value,
        std::true_type,
        std::false_type
        >::type
    {};



    template <typename T, int M, int N, int N_base, bool B_column_major>
    std::true_type is_tbm_1_r_help_impl(const blitz::TinyBasedMatrix<T, M, N, 1, N_base, B_column_major>*);

    std::false_type is_tbm_1_r_help_impl(...);

    template <typename Arg>
    using is_tbm_1_r_help = decltype(is_tbm_1_r_help_impl(std::declval<Arg*>()));


    template<typename Arg>
    struct is_tbm_1_r :
        std::conditional <
        is_tbm_1_r_help<std::remove_cv_t<std::remove_reference_t<Arg>>>::value,
        std::true_type,
        std::false_type
        >::type
    {};


    template <typename T, int M, int N, int M_base, bool B_column_major>
    std::true_type is_tbm_1_l_help_impl(const blitz::TinyBasedMatrix<T, M, N, M_base, 1, B_column_major>*);

    std::false_type is_tbm_1_l_help_impl(...);

    template <typename Arg>
    using is_tbm_1_l_help = decltype(is_tbm_1_l_help_impl(std::declval<Arg*>()));


    template<typename Arg>
    struct is_tbm_1_l :
        std::conditional <
        is_tbm_1_l_help<std::remove_cv_t<std::remove_reference_t<Arg>>>::value,
        std::true_type,
        std::false_type
        >::type
    {};






    template <typename T, int N, int B>
    std::true_type is_tbv_help_impl(const blitz::TinyBasedVector<T, N, B>*);

    std::false_type is_tbv_help_impl(...);

    template <typename Arg>
    using is_tbv_help = decltype(is_tbv_help_impl(std::declval<Arg*>()));


    template<typename Arg>
    struct is_tbv : std::identity<is_tbv_help<std::remove_cv_t<std::remove_reference_t<Arg>>>>::type
    {};



    template <typename T, int M, int N, int M_base, int N_base, bool B_column_major>
    std::true_type is_tbm_help_impl(const blitz::TinyBasedMatrix<T, M, N, M_base, N_base, B_column_major>*);

    std::false_type is_tbm_help_impl(...);

    template <typename Arg>
    using is_tbm_help = decltype(is_tbm_help_impl(std::declval<Arg*>()));


    template<typename Arg>
    struct is_tbm :
        std::conditional <
        is_tbm_help<std::remove_cv_t<std::remove_reference_t<Arg>>>::value,
        std::true_type,
        std::false_type
        >::type
    {};



    template<typename Arg>
    struct is_tb : std::disjunction<is_tbv<Arg>, is_tbm<Arg>>
    {};
}

template<typename Arg1, typename Arg2>
decltype(auto) matmul(Arg1&& arg1, Arg2&& arg2
    , typename std::enable_if<
    aux::is_tbv_1<Arg1>::value && aux::is_tbm_1_r<Arg2>::value,
    void*
    >::type = nullptr
){
    using namespace blitz;
    firstIndex i;
    secondIndex j;
    return sum(std::forward<Arg1>(arg1)(j) * std::forward<Arg2>(arg2)(j, i), j);
}

template<typename Arg1, typename Arg2>
decltype(auto) matmul(Arg1&& arg1, Arg2&& arg2
    , typename std::enable_if<
    !(aux::is_tb<Arg1>::value) || !(aux::is_tb<Arg2>::value),
    void*
    >::type = nullptr
){
#pragma message( "Calling matmul with dynamic arrays! Ensure both arrays have the same base!!!\n")
    using namespace blitz;
    firstIndex i;
    secondIndex j;
    return sum(std::forward<Arg1>(arg1)(j) * std::forward<Arg2>(arg2)(j, i), j);
}


template<typename Arg1, typename Arg2>
decltype(auto) matmul_verified(Arg1&& arg1, Arg2&& arg2){
    using namespace blitz;
    firstIndex i;
    secondIndex j;
    return sum(std::forward<Arg1>(arg1)(j) * std::forward<Arg2>(arg2)(j, i), j);
}





// we define a generalized dot product for all classes as sum(a*b)
namespace aux
{
    //template <template <typename...> class C, typename...Ts>
    //std::true_type is_base_of_template_impl(const C<Ts...>*);

    //template <template <typename...> class C>
    //std::false_type is_base_of_template_impl(...);

    //template <typename T, template <typename...> class C>
    //using is_base_of_template = decltype(is_base_of_template_impl<C>(std::declval<T*>()));


    template<typename Arg1, typename Arg2>
    inline
        decltype(auto) dot(
            Arg1&& arg1,
            const int lb_1,
            Arg2&& arg2,
            const int lb_2
            //,
            //typename std::enable_if<is_base_of_template<T1, blitz::Array>::value>::type* = nullptr)
        )
    {
        return sum(arg1 * arg2(rng_t()).reindex(lb_1));
    }
}


namespace aux{
    template <typename> struct Debug;
}


template<typename T>
struct class_tag { };

template<typename Arg1, typename Arg2>
decltype(auto) dot_product(Arg1&& arg1, Arg2&& arg2
    , typename std::enable_if<
    !(aux::is_tbv<Arg1>::value) || !(aux::is_tbv<Arg2>::value),
    void*
    >::type = nullptr
){
    // ATTENTION
    // THE ERROR HERE IS INTENDED !!!
    //
    // Problem: Calling dot_product with dynamic arrays! Please revisit and 
    // call dot_product_verified when bases are equal
    // otherwise change code or call  dot_product_adjusting
    //
    aux::Debug<Arg1>{};
    aux::Debug<Arg2>{};
}

template<typename Arg1, typename Arg2>
decltype(auto) dot_product_adjusting(Arg1&& arg1, Arg2&& arg2
    , typename std::enable_if<
    !(aux::is_tbv<Arg1>::value) || !(aux::is_tbv<Arg2>::value),
    void*
    >::type = nullptr
){
    using namespace blitz;
    const int lb_1 = arg1.unwrap().lbound(0);
    const int lb_2 = arg2.unwrap().lbound(0);
    if(lb_1 == lb_2)
    {
        return dot(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2));
    }
    else
    {
        return aux::dot(
            std::forward<Arg1>(arg1),
            lb_1,
            std::forward<Arg2>(arg2),
            lb_2
        );
    }
}



template<typename Arg1, typename Arg2>
decltype(auto) dot_product(Arg1&& arg1, Arg2&& arg2
    , typename std::enable_if<
    aux::is_tbv_1<Arg1>::value && aux::is_tbv_1<Arg2>::value,
    void*
    >::type = nullptr
){
    using namespace blitz;
    return dot(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2));
}

template<typename Arg1, typename Arg2>
decltype(auto) dot_product_verified(Arg1&& arg1, Arg2&& arg2){
    return dot_product_verified_impl(
        std::forward<Arg1>(arg1), 
        class_tag<typename std::decay<Arg1>::type>{},
        std::forward<Arg2>(arg2),
        class_tag<typename std::decay<Arg2>::type>{}
        );
}

template<typename Arg1, typename Tag1, typename Arg2, typename Tag2>
auto dot_product_verified_impl(Arg1&& arg1, Tag1, Arg2&& arg2, Tag2){
    using namespace blitz;
    assert(arg1.unwrap().lbound(0) == arg2.unwrap().lbound(0));
    return dot(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2));
}


template<typename Arg1>
decltype(auto) transpose(Arg1&& arg1)
{
    using namespace blitz;
#ifndef FT_USE_TINYBASEDVECTOR_AS_BASE
    return arg1.transpose(1, 0);
#else
    return arg1(ft::rng_t(), ft::rng_t()).transpose(1, 0);
#endif
}

template<typename Arg1>
decltype(auto) size(Arg1&& arg1){
    using namespace blitz;
    return arg1.extent(0);
}



template<typename Arg1>
double float_(Arg1&& arg1){
    return static_cast<double>(arg1);
}

//template<typename Arg1, typename Arg2>
//Arg1 matmul(Arg1&& arg1, Arg2&& arg2){
//    using namespace blitz;
//    firstIndex i;
//    secondIndex j;
//    thirdIndex k;
//    return sum(A(i, k) * B(k, j), k);
//}



//template<typename ...Args>
//double dfloat(Args&& ...args){
//    return static_cast<double>(args...);
//}

template<typename Arg>
double dfloat(Arg&& arg){
    return static_cast<double>(arg);
}

template<typename Arg>
int int_(Arg&& arg, int s = 0){
    return static_cast<int>(arg);
}

template<typename Arg1, typename Arg2>
auto modulo(Arg1&& arg1, Arg2&& arg2){
    return arg1 % arg2;
}

template<typename Arg1, typename Arg2>
auto mod(Arg1&& arg1, Arg2&& arg2){
    return arg1 % arg2;
}

template<typename Arg1, typename Arg2>
auto and_ (Arg1&& arg1, Arg2&& arg2){
    return arg1 & arg2;
}

template<typename Arg1, typename Arg2>
auto iand (Arg1&& arg1, Arg2&& arg2){
    return arg1 & arg2;
}

template<typename Arg1, typename Arg2>
auto or_ (Arg1&& arg1, Arg2&& arg2){
    return arg1 | arg2;
}

template<typename Arg1, typename Arg2>
auto ior (Arg1&& arg1, Arg2&& arg2){
    return arg1 | arg2;
}

template<typename Arg1>
auto not_(Arg1&& arg1){
    return ~arg1;
}

template<typename Arg>
int aint(Arg&& arg){
    return static_cast<int>(arg);
}

inline int int_cast(double x) {
//    return boost_ext::numeric_converter<int, double>::convert(x);
    return x <= -2147483648.0
        ? INT_MIN
        : x >= 2147483647.0
        ? INT_MAX
        : static_cast<int>(x);
}


inline int int4(double x)
{
    return x <= -2147483648.0
        ? INT_MIN
        : x >= 2147483647.0
        ? INT_MAX
        : static_cast<int>(x >= 0.0
            ? x + 0.5
            : x - 0.5
            );
}
//inline int int4(double x)
//{
//
//    double y = std::min(std::max(x, -2147483648.0), 2147483647.0);
//    return static_cast<int>(y >= 0.0
//        ? y + 0.5
//        : y - 0.5
//        );
//}

inline int nintd4(double x)
{
    return int4(x);
}

inline int idnint(double x)
{
    return int4(x);
}

inline int nint(double x)
{
    return int4(x);
}

inline char char_(int i)
{
    return static_cast<char>(i);
}


template<typename T>
inline auto epsilon(T x) {
    return std::numeric_limits<T>::epsilon();
}
template<typename T>
inline auto tiny(T x) {
    return std::numeric_limits<T>::lowest();
}
template<typename T>
inline auto huge(T x) {
    return std::numeric_limits<T>::max();
}

template<typename T>
inline auto radix(T x) {
    return std::numeric_limits<T>::radix;
}


template<typename T>
inline auto digits(T x) {
    return std::numeric_limits<T>::digits;
}

template<typename T>
inline auto minexponent(T x) {
    return std::numeric_limits<T>::min_exponent;
}

template<typename T>
inline auto maxexponent(T x) {
    return std::numeric_limits<T>::max_exponent;
}

template<typename T>
inline bool present(T&& t){
    return t.is_initialized();
}


class string_ex;

inline bool equal_trim_right(std::string const& lhs, std::string const& rhs) {
    auto const sl = lhs.size();
    auto const sr = rhs.size();
    if (sl == sr) {
        return lhs == rhs;
    }
    else if (sl < sr) {
        if (strncmp(lhs.c_str(), rhs.c_str(), sl) != 0) {
            return false;
        }
        for (auto index = sl; index < sr; ++index) {
            if (rhs[index] != ' ') {
                return false;
            }
        }
        return true;
    }
    else {
        if (strncmp(lhs.c_str(), rhs.c_str(), sr) != 0) {
            return false;
        }
        for (auto index = sr; index < sl; ++index) {
            if (lhs[index] != ' ') {
                return false;
            }
        }
        return true;
    }
}


class string_view {
    char const * begin_;
    int size_;

public:
    constexpr string_view(string_view const&) = default;

    constexpr string_view() : begin_{}, size_{} {
    }
    template< int N >
    constexpr string_view(const char(&arr)[N]) : begin_(arr), size_(N - 1) {
        static_assert(N >= 1, "not a string literal");
    }

    //static constexpr int requires_inRange(int i, int len) {
    //    return i >= len ? throw std::out_of_range("out of range") : i;
    //}
    constexpr char const& operator[](int i) const {
        //return requires_inRange(i, size_), begin_[i];
        return begin_[i];
    }

    constexpr operator const char *() const {
        return begin_;
    }

    constexpr char const* begin() const {
        return begin_;
    }

    constexpr char const* end() const {
        return begin_ + size_;
    }

    constexpr int size() const {
        return size_;
    }

    operator std::string() const {
        return std::string(begin_, size_);
    }

    template<typename Other>
    inline bool operator==(Other const& rhs) const {
        return equal_trim_right(boost::implicit_cast<std::string const&>(*this), boost::implicit_cast<std::string const&>(rhs));
    }
    inline bool operator==(string_view const& rhs) const {
        return equal_trim_right(boost::implicit_cast<std::string const&>(*this), boost::implicit_cast<std::string const&>(rhs));
    }
    inline bool operator==(string_ex const& rhs) const;

    string_ex operator()(rng_t r) const;

};

class string_ex : public std::string {
public:
    using base_type = std::string;
    using base_type::base_type;
    using base_type::operator =;

    string_ex() = default;
    string_ex(std::string const& s)
        : base_type(s) {}

    string_ex(string_view const& v)
    : base_type((std::string const&)v) {}

    string_ex operator()(rng_t r) const {
        int start = r.first(1) - 1;
        int last = r.last(0);
        int count = last == 0 ? -1 : last - start;
        return substr(start, count);
    }

    string_ex& operator = (char c) {
        (*this)[0] = c;
        return *this;
    }

    string_ex& operator = (string_view const& v) {
        (*this) = (std::string const&)v;
        return *this;
    }

    template<typename Other>
    inline bool operator==(Other const& rhs) const {
        return equal_trim_right(boost::implicit_cast<std::string const&>(*this), boost::implicit_cast<std::string const&>(rhs));
    }                                                                          
    inline bool operator==(string_ex const& rhs) const {                       
        return equal_trim_right(boost::implicit_cast<std::string const&>(*this), boost::implicit_cast<std::string const&>(rhs));
    }                                                                          
    inline bool operator==(string_view const& rhs) const {                     
        return equal_trim_right(boost::implicit_cast<std::string const&>(*this), boost::implicit_cast<std::string const&>(rhs));
    }
};


inline bool string_view::operator==(string_ex const& rhs) const {
    return equal_trim_right(boost::implicit_cast<std::string const&>(*this), boost::implicit_cast<std::string const&>(rhs));
}

inline string_ex string_view::operator()(rng_t r) const {
    int start = r.first(1) - 1;
    int last = r.last(0);
    int count = last == 0 ? -1 : last - start;
    return ((std::string const&)(*this)).substr(start, count);
}


template<typename Other>
inline bool operator==(Other const& lhs, string_ex const& rhs) {
    return equal_trim_right(boost::implicit_cast<std::string const&>(lhs), boost::implicit_cast<std::string const&>(rhs));
}


template<typename Other>
inline bool operator==(Other const& lhs, string_view const& rhs) {
    return equal_trim_right(boost::implicit_cast<std::string const&>(lhs), boost::implicit_cast<std::string const&>(rhs));
}


#ifndef FT_NATIVE_SUPPORT_STRING_SUBTRINGS
using string = std::string;
#else
using string = string_ex;
#endif

template<typename LHS, typename RHS>
inline bool string_equal(LHS&& lhs, RHS&& rhs) {
    return std::forward<LHS>(lhs) == std::forward<RHS>(rhs);
}
template<typename LHS, typename RHS>
inline bool string_not_equal(LHS&& lhs, RHS&& rhs) {
    return !(std::forward<LHS>(lhs) == std::forward<RHS>(rhs));
}

inline int string_cmp(string const& lhs, string const& rhs) {
    string l = boost::trim_right_copy(lhs);
    string r = boost::trim_right_copy(rhs);
    return l.compare(r);
    //ToDo

    //auto const sl = lhs.size();
    //auto const sr = rhs.size();
    //for (int il = sl - 1; il >= 0; --il) {
    //    if (lhs[il] != ' ') {
    //        break;
    //    }
    //}
    //for (int ir = sr - 1; ir >= 0; --ir) {
    //    if (rhs[ir] != ' ') {
    //        break;
    //    }
    //}

    //if (sl == sr) {
    //    return lhs.compare(rhs);
    //}
    //else {
    //    string l = boost::trim_right_copy(lhs);
    //    string r = boost::trim_right_copy(rhs);
    //    return l.compare(r);
    //}
}


inline int ichar(string const& s)
{
    return s[0];
}

using int_ptr = INT_PTR;

inline string strcat(string const& l, string const& r) {
    return l + r;
}


inline int index(string const& str, string const& sub, ft::logical back = FALSE) {
    if (back) {
        return static_cast<int>(str.rfind(sub) + 1);
    }
    else {
        return static_cast<int>(str.find(sub) + 1);
    }
}



struct not_a_string {

    constexpr operator const char* () const {
        return "";
    }
};

///////////////////////////////////////////////
//
// format
//

struct format_string {
    format_string()
        {}
};

struct format_string_n {
    int width;
    format_string_n(int width = -1)
        :width(width) {}
};

struct format_int{
    int width;
    format_int(int width = -1)
        :width(width)
    {}
};

enum class format_double_type {
    F,
    G,
    ES
};
struct format_double{
    format_double_type type;
    int width;
    int precision;
    format_double(format_double_type type,
        int width,
        int precision)
        :type(type)
        , width(width)
        , precision(precision)
    {}
};

inline format_double format_double_F(
    int width,
    int precision
)
{
    return format_double(
        format_double_type::F,
        width,
        precision
    );
}

inline format_double format_double_G(
    int width,
    int precision
)
{
    return format_double(
        format_double_type::G,
        width,
        precision
    );
}

inline format_double format_double_ES(
    int width,
    int precision
)
{
    return format_double(
        format_double_type::ES,
        width,
        precision
    );
}


template<typename F>
struct format_t_trait;

template<>
struct format_t_trait<format_string> {
    using type = std::string;
};

template<>
struct format_t_trait<format_string_n> {
    using type = std::string;
};

template<>
struct format_t_trait<format_int>{
    using type = int;
};

template<>
struct format_t_trait<format_double>{
    using type = double;
};

namespace aux{

    inline void format_helper(std::stringstream& ss, format_int const& form, int const& arg)
    {
        if(form.width >= 0)
        {
            ss << std::setfill(' ') << std::setw(form.width) << arg;
        }
        else
        {
            ss << arg;
        }
    }


    inline void format_helper(std::stringstream& ss, format_double const& form, double const& arg)
    {
        switch(form.type)
        {
        case format_double_type::F :
            {
                ss << std::fixed << std::setprecision(form.precision) << std::setw(form.width) << arg;

            }
            break;

        case format_double_type::G:
            {
                ss << std::defaultfloat << std::setprecision(form.precision) << std::setw(form.width) << arg;
            }
            break;

        case format_double_type::ES:
            {
                ss << std::scientific << std::setprecision(form.precision) << std::setw(form.width) << arg;
            }
            break;
        }
    }

    inline void format_helper(std::stringstream& ss, format_string const& form, std::string const& arg)
    {
        ss << arg;
    }

    inline void format_helper(std::stringstream& ss, format_string_n const& form, std::string const& arg)
    {
        if(form.width >= 0)
        {
            if(arg.length() > static_cast<size_t>(form.width))
            {
                ss << arg.substr(0, form.width);
            }
            else
            {
                ss << arg;
                std::fill_n(std::ostream_iterator<char>(ss), form.width - arg.length(), ' ');
            }
        }
        else
        {
            ss << arg;
        }
    }

    template<
        int I,
        typename F,
        typename... Args
    >
        void format(std::stringstream&, F&&)
    {}

    template<
        int I,
        typename F,
        typename Arg,
        typename... Args
    >
    void format(std::stringstream& ss, F&& form, Arg&& arg, Args&&... args)
    {
        format_helper(ss, std::get<I>(form), arg);

        format<I+1>(ss, std::forward<F>(form), std::forward<Args>(args)...);
    }

}

template<typename... F>
struct format_t{
    string& dest;
    typedef std::tuple<F&...> forms_t;
    forms_t forms;
    format_t(string& dest, F&&... form)
        : dest(dest)
        , forms(std::forward_as_tuple(form...))
    {}

    void operator()(typename format_t_trait<F>::type const &... arg) const
    {
        std::stringstream ss;
        aux::format<0>(ss, forms, arg...);
        dest = ss.str();
    }
};

template<typename... F>
format_t<F...>
format(string& dest, F&&... form){
    return format_t<F...>(dest, std::forward<F>(form)...);
}


template<typename... F>
struct formatstr_t {
    typedef std::tuple<F&...> forms_t;
    forms_t forms;
    formatstr_t(F&&... form)
        : forms(std::forward_as_tuple(form...))
    {}

    auto operator()(typename format_t_trait<F>::type const &... arg) const
    {
        std::stringstream ss;
        aux::format<0>(ss, forms, arg...);
        return ss.str();
    }
};

template<typename... F>
formatstr_t<F...>
formatstr(F&&... form) {
    return formatstr_t<F...>(std::forward<F>(form)...);
}



// not_a_string-format

template<typename... F>
struct not_a_format_t {
    void operator()(typename format_t_trait<F>::type const &... arg) const {
    }
};

template<typename... F>
decltype(auto)
format(not_a_string, F&&...){
    return not_a_format_t<F...>();
}









template<typename T>
class allocatable {
    std::unique_ptr<T> t;

public:
    operator T&() { return *t; }
    operator T const &() const { return *t; }
    T& operator =(T const& d) { *t = d; return *t; }
    T& operator()() { return *t; }
    T const & operator ()() const { return *t; }

    T* operator->()
    {
        return t.get();
    }

    T const* operator->() const
    {
        return t.get();
    }

    template<typename T>
    friend void allocate(allocatable<T>&);
};

template<typename T>
void
allocate(allocatable<T>& value) {
    value.t = std::make_unique<T>();
}




struct allocate_stat{
    int& stat;
    allocate_stat(int& stat)
        :stat(stat)
    {}
};

template<typename T>
struct allocate_source_t{
    T& t;
    allocate_source_t(T&& t)
        :t(t)
    {}
};

template<typename T>
decltype(auto) allocate_source(T&& t) {
    return allocate_source_t<T>(std::forward<T>(t));
};


inline void allocate(){
}


namespace detail {
    BOOST_MPL_HAS_XXX_TRAIT_DEF(is_allocator_wrap)
    BOOST_MPL_HAS_XXX_TRAIT_DEF(is_allocator_2_wrap)
}
template<class T>
struct is_instance_of_allocator_wrap {
    static constexpr bool value = detail::has_is_allocator_wrap<std::decay_t<T>>::value;

};
template<class T>
struct is_instance_of_allocator_2_wrap {
    static constexpr bool value = detail::has_is_allocator_2_wrap<std::decay_t<T>>::value;

};
template<class T>
struct is_not_an_allocator {
    static constexpr bool value = 
        !detail::has_is_allocator_wrap<std::decay_t<T>>::value 
        &&
        !detail::has_is_allocator_2_wrap<std::decay_t<T>>::value
        ;

};



template<typename AllocatorWrap, typename ...Args>
typename std::enable_if_t<is_instance_of_allocator_wrap<AllocatorWrap>::value>
allocate(AllocatorWrap&& wrap, Args&& ...args){

    wrap.t.resize(wrap.r.length());

    allocate(std::forward<Args>(args)...);
}

template<typename AllocatorWrap, typename ...Args>
typename std::enable_if_t<is_instance_of_allocator_wrap<AllocatorWrap>::value>
allocate(AllocatorWrap&& wrap, allocate_stat stat, Args&& ...args){

    wrap.t.resize(wrap.r.length());
    stat.stat = 0;

    allocate(std::forward<Args>(args)...);
}

template<typename AllocatorWrap, typename SourceT, typename ...Args>
typename std::enable_if_t<is_instance_of_allocator_wrap<AllocatorWrap>::value>
allocate(AllocatorWrap&& wrap, allocate_source_t<SourceT>&& source, Args&& ...args){

    wrap.t.resize(wrap.r.length());
    wrap.t = source.t;

    allocate(std::forward<Args>(args)...);
}

template<typename AllocatorWrap, typename SourceT, typename ...Args>
typename std::enable_if_t<is_instance_of_allocator_wrap<AllocatorWrap>::value>
allocate(AllocatorWrap&& wrap, allocate_source_t<SourceT>&& source, allocate_stat stat, Args&& ...args){

    wrap.t.resize(wrap.r.length());
    wrap.t = source.t;
    stat.stat = 0;

    allocate(std::forward<Args>(args)...);
}

// version without length but with source
template<typename T, typename SourceT, typename ...Args>
typename std::enable_if_t<is_not_an_allocator<T>::value>
allocate(T&& arr, allocate_source_t<SourceT>&& source, Args&& ...args){

    arr.reference(source.t.copy());

    allocate(std::forward<Args>(args)...);
}

template<typename T, typename SourceT, typename ...Args>
typename std::enable_if_t<is_not_an_allocator<T>::value>
allocate(T&& arr, allocate_source_t<SourceT>&& source, allocate_stat stat, Args&& ...args){

    arr.reference(source.t.copy());
    stat.stat = 0;

    allocate(std::forward<Args>(args)...);
}




// 2-dim version
template<typename AllocatorWrap, typename ...Args>
typename std::enable_if_t<is_instance_of_allocator_2_wrap<AllocatorWrap>::value>
allocate(AllocatorWrap&& wrap, Args&& ...args){

    wrap.t.resize(wrap.r1.length(), wrap.r2.length());

    allocate(std::forward<Args>(args)...);
}


template<typename AllocatorWrap, typename ...Args>
typename std::enable_if_t<is_instance_of_allocator_2_wrap<AllocatorWrap>::value>
allocate(AllocatorWrap&& wrap, allocate_stat stat, Args&& ...args){

    wrap.t.resize(wrap.r1.length(), wrap.r2.length());
    stat.stat = 0;

    allocate(std::forward<Args>(args)...);
}

template<typename AllocatorWrap, typename SourceT, typename ...Args>
typename std::enable_if_t<is_instance_of_allocator_2_wrap<AllocatorWrap>::value>
allocate(AllocatorWrap&& wrap, allocate_source_t<SourceT>&& source, Args&& ...args){

    wrap.t.resize(wrap.r1.length(), wrap.r2.length());
    wrap.t = source.t;

    allocate(std::forward<Args>(args)...);
}

template<typename AllocatorWrap, typename SourceT, typename ...Args>
typename std::enable_if_t<is_instance_of_allocator_2_wrap<AllocatorWrap>::value>
allocate(AllocatorWrap&& wrap, allocate_source_t<SourceT>&& source, allocate_stat stat, Args&& ...args){

    wrap.t.resize(wrap.r1.length(), wrap.r2.length());
    wrap.t = source.t;
    stat.stat = 0;

    allocate(std::forward<Args>(args)...);
}

















template<typename Arg>
bool allocated(Arg&& arg){
    return arg.dataZero() != nullptr;
}

template<typename Arg>
void deallocate(Arg&& arg, allocate_stat stat){
    arg.free();
    stat.stat = 0;

}

template<typename Arg>
void deallocate(Arg&& arg){
    arg.free();
}


template<typename T, typename S>
void move_alloc(T&& t, S&& s){
    s.reference(t);
    t.free();
}



/////////////////////////////////////////////////////////////////////////////////
//
// conversion helpers
//
// cannot be implemented as constuctors as we don't won't to use any new storage

template<typename T, int N>
ft::fort_array<T, ft::bs<1, N>>& as_fort_array(std::array<T, N>& t)
{
    return reinterpret_cast<ft::fort_array<T, ft::bs<1, N>>&>(t);
}
template<typename T, int N>
ft::fort_array<T, ft::bs<1, N>> const& as_fort_array(std::array<T, N> const& t)
{
    return reinterpret_cast<ft::fort_array<T, ft::bs<1, N>> const&>(t);
}

template<typename T, int N>
ft::fort_array<T, ft::bs<1, N>>* as_fort_array(std::array<T, N>* t)
{
    return reinterpret_cast<ft::fort_array<T, ft::bs<1, N>>*>(t);
}
template<typename T, int N>
ft::fort_array<T, ft::bs<1, N>> const* as_fort_array(std::array<T, N> const* t)
{
    return reinterpret_cast<ft::fort_array<T, ft::bs<1, N>> const*>(t);
}


template<typename T, int N>
ft::fort_array<T, ft::bs<1, N>>& as_fort_array(boost::array<T, N>& t)
{
    return reinterpret_cast<ft::fort_array<T, ft::bs<1, N>>&>(t);
}
template<typename T, int N>
ft::fort_array<T, ft::bs<1, N>> const& as_fort_array(boost::array<T, N> const& t)
{
    return reinterpret_cast<ft::fort_array<T, ft::bs<1, N>> const&>(t);
}

template<typename T, int N>
ft::fort_array<T, ft::bs<1, N>>* as_fort_array(boost::array<T, N>* t)
{
    return reinterpret_cast<ft::fort_array<T, ft::bs<1, N>>*>(t);
}
template<typename T, int N>
ft::fort_array<T, ft::bs<1, N>> const* as_fort_array(boost::array<T, N> const* t)
{
    return reinterpret_cast<ft::fort_array<T, ft::bs<1, N>> const*>(t);
}





template<typename T, int N>
ft::fort_array<T, ft::bs<1, N>>& as_fort_array(T(&t)[N])
{
    return reinterpret_cast<ft::fort_array<T, ft::bs<1, N>>&>(t);
}
template<typename T, int N>
ft::fort_array<T, ft::bs<1, N>> const& as_fort_array(T const(&t)[N])
{
    return reinterpret_cast<ft::fort_array<T, ft::bs<1, N>> const&>(t);
}
template<typename T, int N>
ft::fort_array<T, ft::bs<1, N>>* as_fort_array(T(*t)[N])
{
    return reinterpret_cast<ft::fort_array<T, ft::bs<1, N>>*>(t);
}
template<typename T, int N>
ft::fort_array<T, ft::bs<1, N>> const* as_fort_array(T const(*t)[N])
{
    return reinterpret_cast<ft::fort_array<T, ft::bs<1, N>> const*>(t);
}


template<typename T, int N, int M>
ft::fort_array_2<T, ft::bs<1, N>, ft::bs<1, M>>& as_fort_array_2(T(&t)[M][N])
{
    return reinterpret_cast<ft::fort_array_2<T, ft::bs<1, N>, ft::bs<1, M>>&>(t);
}
template<typename T, int N, int M>
ft::fort_array_2<T, ft::bs<1, N>, ft::bs<1, M>> const& as_fort_array_2(T const(&t)[M][N])
{
    return reinterpret_cast<ft::fort_array_2<T, ft::bs<1, N>, ft::bs<1, M>> const&>(t);
}
template<typename T, int N, int M>
ft::fort_array_2<T, ft::bs<1, N>, ft::bs<1, M>>* as_fort_array_2(T(*t)[M][N])
{
    return reinterpret_cast<ft::fort_array_2<T, ft::bs<1, N>, ft::bs<1, M>>*>(t);
}
template<typename T, int N, int M>
ft::fort_array_2<T, ft::bs<1, N>, ft::bs<1, M>> const* as_fort_array_2(T const(*t)[M][N])
{
    return reinterpret_cast<ft::fort_array_2<T, ft::bs<1, N>, ft::bs<1, M>> const*>(t);
}



namespace aux{
    template<typename T>
    struct make_array_return_type{
        using type = T(&)[];
    };
}
template<typename T>
constexpr typename aux::make_array_return_type<T>::type& as_c_array(T* t)
{
    return *(T(*)[])t;
}
//template<typename T, int N>
//ft::fort_array<T, ft::bs<1, N>> const& as_fort_array(T const(&t)[N])
//{
//    return reinterpret_cast<ft::fort_array<T, ft::bs<1, N>> const&>(t);
//}






}

namespace std {
    // helper template-specilisation

    template< class T, typename Bounds >
    class tuple_size< ft::fort_array<T, Bounds> > :
        public integral_constant<size_t, ft::fort_array<T, Bounds>::size()> {};


}

// member-funcs
namespace mf{

#define MF_DECLARE_MEMFUNC(TYPE, FUNC) \
    template<typename Arg> \
    decltype(auto) FUNC(Arg&& arg) \
    { \
        /*auto&& functor = blitz::make_member_projecting_functor(&TYPE::FUNC);*/ \
        return blitz::make_member_projecting_functor(&TYPE::FUNC)(std::forward<Arg>(arg)); \
    }

    MF_DECLARE_MEMFUNC(fluid_type, gat);
    MF_DECLARE_MEMFUNC(gat_type, coeff);
    MF_DECLARE_MEMFUNC(coeff_type, xi);
    MF_DECLARE_MEMFUNC(error_type, category);
    MF_DECLARE_MEMFUNC(error_type, id);
    MF_DECLARE_MEMFUNC(baut046_type, valwert);
    MF_DECLARE_MEMFUNC(baut046_type, valwert_old);
    MF_DECLARE_MEMFUNC(rawdata_coeff_type, rcoeff);
    MF_DECLARE_MEMFUNC(rawdata_coeff_type, icoefftyp);
    MF_DECLARE_MEMFUNC(rawdata_coeff_type, icoeffline);
    MF_DECLARE_MEMFUNC(rawdata_type, rest);
    MF_DECLARE_MEMFUNC(rawdata_rest_type, merkg);
    MF_DECLARE_MEMFUNC(rawdata_rest_type, pivot_coeff_index_zero_based);

}




















#if _MSC_VER > 1900
//#error check if fort_array can't be made to literal-type
#endif

//namespace blitz {
//
//    template<typename Type, typename Range>
//    class ETBase<ft::const_expr_fort_array<Type, Range>> {
//    public:
//        using DataT = ft::const_expr_fort_array<Type, Range>;
//        using T = ft::fort_array<Type, Range>;
//        typedef T T_unwrapped;
//
//        decltype(auto) as_fort_array() {
//            return static_cast<DataT&>(*this).as_fort_array();
//        }
//
//        decltype(auto) as_fort_array() const {
//            return static_cast<DataT const&>(*this).as_fort_array();
//        }
//
//        T& unwrap() {
//            return static_cast<T&>(as_fort_array());
//        }
//
//        const T& unwrap() const {
//            return static_cast<const T&>(as_fort_array());
//        }
//
//        ETBase<T>& wrap() {
//            return static_cast<ETBase<T>&>(as_fort_array());
//        }
//
//        const ETBase<T>& wrap() const {
//            return static_cast<const ETBase<T>&>(as_fort_array());
//        }
//    };
//}

namespace ft {
    template<typename Type, typename Range>
    using const_expr_fort_array = fort_array<Type, Range>;


    //template<typename Type, typename Range>
    //class const_expr_fort_array : public blitz::ETBase<const_expr_fort_array<Type, Range>> {         // blitz-calculation are not supported yet, this is just to prohibit selection of "constexpr operator const T*()" in arithmetic expressions
    //public:
    //    //template<typename ...Args>
    //    //constexpr const_expr_fort_array(Args&& ...args) : fort_array<Type, Range>{std::forward<Args>(args)...}
    //    //{}

    //    template<typename ...Args>
    //    constexpr const_expr_fort_array(Args&& ...args) : data_zero_lower_bound{static_cast<Type>(args)...} {}

    //    static constexpr int size = Range::upper - Range::lower + 1;

    //    // this operator interfeares with blitz-calculations
    //    constexpr operator const Type*() const {
    //        return data_zero_lower_bound.data();
    //    }

    //    constexpr Type const& operator()(int index) const {
    //        return data_zero_lower_bound[index - Range::lower];
    //    }

    //    template<int index>
    //    constexpr Type const& operator()() const {
    //        static_assert(index >= Range::lower && index <= Range::upper, "index out of range");
    //        return data_zero_lower_bound[index - Range::lower];
    //    }

    //    constexpr operator fort_array_cr<Type, dyn>() const {
    //        return as_fort_array();
    //    }

    //    constexpr operator fort_array<Type, dyn> const &() const {
    //        return as_fort_array();
    //    }


    //    constexpr operator fort_array<Type, Range> const &() const {
    //        return as_fort_array();
    //    }

    //    constexpr fort_array<Type, Range> const & as_fort_array() const {
    //        return *reinterpret_cast<fort_array<Type, Range> const *>(data_zero_lower_bound.data());
    //    }


    //    constexpr Type const* data() const {
    //        return data_zero_lower_bound.data();
    //    }

    //    constexpr int extent() const {
    //        return size;
    //    }

    //    constexpr int extent(int /*rank*/) const {
    //        return size;
    //    }

    //    template<typename Arg>
    //    decltype(auto) operator()(Arg&& arg
    //        , typename std::enable_if<
    //        !std::is_integral_v<std::decay_t<Arg>>,
    //        void*
    //        >::type = nullptr
    //        ) const {
    //        return as_fort_array()(std::forward<Arg>(arg));
    //    }



    //    std::array<Type, size> data_zero_lower_bound;
    //};


}





namespace ft {

    template<typename Type, typename Range>
    class simple_const_expr_fort_array : public blitz::ETBase<const_expr_fort_array<Type, Range>> {         // blitz-calculation are not supported yet, this is just to prohibit selection of "constexpr operator const T*()" in arithmetic expressions
    public:
        //template<typename ...Args>
        //constexpr const_expr_fort_array(Args&& ...args) : fort_array<Type, Range>{std::forward<Args>(args)...}
        //{}

        template<typename ...Args>
        constexpr simple_const_expr_fort_array(Args&& ...args) : data_zero_lower_bound{static_cast<Type>(args)...} {}

        static constexpr int size = Range::upper - Range::lower + 1;

        // this operator interfeares with blitz-calculations
        constexpr operator const Type*() const {
            return data_zero_lower_bound.data();
        }

        constexpr Type const& operator()(int index) const {
            return data_zero_lower_bound[index - Range::lower];
        }

        template<int index>
        constexpr Type const& operator()() const {
            static_assert(index >= Range::lower && index <= Range::upper, "index out of range");
            return data_zero_lower_bound[index - Range::lower];
        }

        constexpr operator fort_array_cr<Type, dyn>() const {
            return as_fort_array();
        }

        constexpr operator fort_array<Type, dyn> const &() const {
            return as_fort_array();
        }


        constexpr operator fort_array<Type, Range> const &() const {
            return as_fort_array();
        }

        constexpr fort_array<Type, Range> const & as_fort_array() const {
            return reinterpret_cast<fort_array<Type, Range> const&>(*data_zero_lower_bound.data());
        }


        constexpr Type const* data() const {
            return data_zero_lower_bound.data();
        }

        constexpr int extent() const {
            return size;
        }

        constexpr int extent(int /*rank*/) const {
            return size;
        }

        template<typename Arg>
        constexpr decltype(auto) operator()(Arg&& arg
            , typename std::enable_if<
            !std::is_integral_v<std::decay_t<Arg>>,
            void*
            >::type = nullptr
            ) const {
            return as_fort_array()(std::forward<Arg>(arg));
        }



        std::array<Type, size> data_zero_lower_bound;
    };


    template<typename T>
    fort_array_base<T, dyn> fort_array_from_list(std::initializer_list<T> list) {
        fort_array_base<T, dyn> arr;
        arr.resize(static_cast<int>(list.size()));

        T* data_ = arr.data();
        for (auto&& item : list) {
            *data_ = item;
            ++data_;
        }

        return arr;
    }

}


namespace ft {
    template<int lb, int ub, typename F>
    auto implied_do_helper(F&& f) {
        ft::fort_array<double, ft::bs<lb, ub>> a;
        for (int i = lb; i <= ub; ++i) {
            a(i) = f(i);
        }
        return a;
    }
}

#define PP_IMPLIED_DO(lambda, v, lb, ub) ft::implied_do_helper<lb, ub>([](double v){ return lambda;})


