// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    02.07.2013 17:20:07 EDT
// File:    has_serialize.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __HAS_SERIALIZE_HEADER
#define __HAS_SERIALIZE_HEADER

namespace util {
    //------------------- checks if T is a class and has a template method called serialize -------------------
    template<typename T, bool is_a_class> 
    struct has_serialize_impl {
        struct archive_proto_type {
            static archive_enum const type = archive_enum::undef;
            template<typename U> void operator &(U & u);
        };
        template<void(T::*)(archive_proto_type &)> struct helper {typedef char type;};
        
        template<typename U> static char check(typename helper<&U::template serialize<archive_proto_type>>::type);
        template<typename U> static double check(...);
        
        enum { value = (sizeof(char) == sizeof(check<T>(0))) };
    };
    template<typename T> 
    struct has_serialize_impl<T, false> { //if T is not a class it cannot have serialize
        enum { value = false };
    };
    template<typename T> 
    struct has_serialize {
        enum { value = has_serialize_impl<T, ustd::is_class<T>::value>::value };
    };
}//end namespace util
#endif //__HAS_SERIALIZE_HEADER
