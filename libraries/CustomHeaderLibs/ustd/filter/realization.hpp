// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    23.07.2013 05:10:01 EDT
// File:    realization.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __REALIZATION_HEADER
#define __REALIZATION_HEADER

#include "tags.hpp"

namespace ustd {
    //------------------- convert tags to realizations -------------------
    template<typename T, typename tag, typename _base>
    struct realization {
    };
    
    #define EMPTY_FILTER(NAME, VALUE) \
    template<typename T, typename _base> \
    class NAME##_filter<T, VALUE, _base>: public _base { \
        typedef _base base; \
    public: \
        NAME##_filter & operator=(T const & in) { \
            base::operator=(in); \
            return (*this); \
        } \
        NAME##_filter & operator<<(T const & in) { \
            base::operator<<(in); \
            return (*this); \
        } \
        T value() const { \
            return base::value(); \
        } \
        operator T() const { \
            return value(); \
        } \
    };
}//end namespace ustd
#endif //__REALIZATION_HEADER
