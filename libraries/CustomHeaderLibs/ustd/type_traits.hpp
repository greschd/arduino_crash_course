// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    21.10.2013 21:22:36 CEST
// File:    type_traits.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __TYPE_TRAITS_HEADER
#define __TYPE_TRAITS_HEADER

namespace ustd {
    //===================  ===================
    /// integral_constant
    template<typename T, T val>
    struct integral_constant {
        static constexpr T value = val;
        typedef T value_type;
        //~ typedef integral_constant<T, val> type;
        constexpr operator value_type() {
            return value; 
        }
    };
    //=================== true_type ===================
    typedef integral_constant<bool, true> true_type;
    typedef integral_constant<bool, false> false_type;
    //=================== conditional (meta if) ===================
    template<bool cond, typename T, typename F> //default true
    struct conditional {
        typedef T type;
    };
    template<typename T, typename F>
    struct conditional<false, T, F> {
        typedef F type;
    };
    //=================== is_const ===================
    template<typename T>
    struct is_const {
        enum{value = false};
    };
    template<typename T>
    struct is_const<T const> {
        enum{value = true};
    };
    //=================== remove_cv ===================
    template<typename T>
    struct remove_const {
        typedef T type;
    };
    template<typename T>
    struct remove_const<T const> {
        typedef T type;
    };
    template<typename T>
    struct remove_volatile {
        typedef T type;
    };
    template<typename T>
    struct remove_volatile<T volatile> {
        typedef T type;
    };
    template<typename T>
    struct remove_cv {
    typedef typename remove_const<
                typename remove_volatile<T>::type
            >::type 
        type;
    };
    //=================== is_unsigned ===================
    template<typename T>
    struct is_unsigned {
        enum{value = (T(0) < T(-1))};
    };
    //=================== is_integral ===================
    namespace detail {
        template<typename T>
        struct is_integral_helper {
            enum{value = false};
        };

        template<>
        struct is_integral_helper<bool> {
            enum{value = true};
        };
        template<>
        struct is_integral_helper<char> {
            enum{value = true};
        };
        template<>
        struct is_integral_helper<char16_t> {
            enum{value = true};
        };
        template<>
        struct is_integral_helper<char32_t> {
            enum{value = true};
        };
        template<>
        struct is_integral_helper<wchar_t> {
            enum{value = true};
        };
        template<>
        struct is_integral_helper<signed char> {
            enum{value = true};
        };
        template<>
        struct is_integral_helper<short int> {
            enum{value = true};
        };
        template<>
        struct is_integral_helper<int> {
            enum{value = true};
        };
        template<>
        struct is_integral_helper<long int> {
            enum{value = true};
        };
        template<>
        struct is_integral_helper<long long int> {
            enum{value = true};
        };
        template<>
        struct is_integral_helper<unsigned char> {
            enum{value = true};
        };
        template<>
        struct is_integral_helper<unsigned short int> {
            enum{value = true};
        };
        template<>
        struct is_integral_helper<unsigned int> {
            enum{value = true};
        };
        template<>
        struct is_integral_helper<unsigned long int> {
            enum{value = true};
        };
        template<>
        struct is_integral_helper<unsigned long long int> {
            enum{value = true};
        };
    }//end namespace detail
    template<typename T>
    struct is_integral {
        enum {value = detail::is_integral_helper<typename remove_cv<T>::type>::value};
    };
    //=================== is_floating_point ===================
    namespace detail {
        template<typename T>
        struct is_floating_point_helper {
            enum{value = false};
        };

        template<>
        struct is_floating_point_helper<float> {
            enum{value = true};
        };
        template<>
        struct is_floating_point_helper<double> {
            enum{value = true};
        };
        template<>
        struct is_floating_point_helper<long double> {
            enum{value = true};
        };
    }//end namespace detail
    template<typename T>
    struct is_floating_point {
        enum {value = detail::is_floating_point_helper<typename remove_cv<T>::type>::value};
    };
    //=================== is_arithmetic ===================
    template<typename T>
    struct is_arithmetic {
        enum{value = (is_integral<T>::value or is_floating_point<T>::value)};
    };
    
    //=================== is_class ===================
    template<typename T>
    struct is_class {
        template<typename U> static char check(void(U::*)(void));
        template<typename U> static double check(...);
        
        enum { value = (sizeof(char) == sizeof(check<T>(0))) };
    };
    //=================== is_array ===================
    template<typename>
    struct is_array {
        enum{value = false};
    };
    template<typename T, size_t N>
    struct is_array<T[N]> {
        enum{value = true};
    };
    template<typename T>
    struct is_array<T[]> {
        enum{value = true};
    };
    /*old but awesome implementation :-)
    template<typename T>
    struct is_array {
        static T t;
        
        template<int N> struct wrap {typedef char type;};
        template<typename U> static U * ptr(U u[]);
        template<typename U> static U ptr(U u);
        
        template<typename U> static typename wrap<sizeof(t = ptr(t))>::type check(int);
        template<typename U> static double check(...);
        
        enum{value = (sizeof(double) == sizeof(check<T>(int())))};
    };
    template<typename T> //remove constness before doing array checks since const i = j is also illegal
    struct is_array<T const > {
        enum{value = is_array<T>::value};
    };*/
    //=================== is_same ===================
    template<typename T, typename U>
    struct is_same {
        enum {value = false};
    };
    template<typename T>
    struct is_same<T, T> {
        enum {value = true};
    };
    //=================== enable_if ===================
    template<bool b, typename T>
    struct enable_if {
    };
    template<typename T>
    struct enable_if<true, T> {
        typedef T type;
    };
    template<bool b, typename T> //not in std
    struct disable_if {
    };
    template<typename T>  //not in std
    struct disable_if<false, T> {
        typedef T type;
    };
}//end namespace ustd
#endif //__TYPE_TRAITS_HEADER
