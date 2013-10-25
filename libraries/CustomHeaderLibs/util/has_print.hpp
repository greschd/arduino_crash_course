// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    02.07.2013 17:19:27 EDT
// File:    has_print.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __HAS_PRINT_HEADER
#define __HAS_PRINT_HEADER

namespace util {
    //------------------- checks if T is a class and has a template method called print -------------------
    template<typename T, bool is_a_class> 
    struct has_print_impl {
        struct stream_proto_type {
            template<typename U> stream_proto_type & operator<<(U const & u);
        };
        template<void(T::*)(stream_proto_type &)const> struct helper {typedef char type;};
        
        template<typename U> static char check(typename helper<&U::template print<stream_proto_type>>::type);
        template<typename U> static double check(...);
        
        enum { value = (sizeof(char) == sizeof(check<T>(0))) };
    };
    template<typename T> 
    struct has_print_impl<T, false> { //if T is not a class it cannot have print
        enum { value = false };
    };
    template<typename T> 
    struct has_print {
        enum { value = has_print_impl<T, ustd::is_class<T>::value>::value };
    };
}//end namespace util

#endif //__HAS_PRINT_HEADER
