// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    20.07.2013 12:41:39 EDT
// File:    mean_trait.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __MEAN_TRAIT_HEADER
#define __MEAN_TRAIT_HEADER

#include "../ustd/type_traits.hpp"

namespace util {
    namespace detail {
        template<typename T>
        struct use_double_identifier {
            static T t;
            
            static char check(T);
            static double check(double);
            
            enum {value = (sizeof(check((t+t)/double(2))) == sizeof(double))};
        };

        //------------------- full specialisation -------------------
        template<>
        struct use_double_identifier<double> {
            enum {value = true}; //doesn't matter if true or false
        };
    }//end namespace detail

    //------------------- mean_trait -------------------
    template<typename T>
    struct mean_trait {
        typedef 
            typename 
                ustd::conditional<detail::use_double_identifier<T>::value
                      , double
                      , T
            >::type
        type;
    };
}//end namespace util

#endif //__MEAN_TRAIT_HEADER
