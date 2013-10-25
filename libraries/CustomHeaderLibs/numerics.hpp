// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    04.07.2013 17:27:20 EDT
// File:    numerics.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */
 
#ifndef __NUMERICS_HEADER
#define __NUMERICS_HEADER

#define OPERATOR(op) \
    template<typename T> \
    T operator op(T a, T const & b) { \
    ASSERT(a.size() == b.size()) \
    for(typename T::size_type i = 0; i < a.size(); ++i) { \
        a[i] op##= b[i]; \
    } \
    return a; \
}

OPERATOR(+)
OPERATOR(-)
OPERATOR(*)
OPERATOR(/)

#undef OPERATOR

template<typename T>
typename T::value_type scalar(T const & a, T const & b) {
    ASSERT(a.size() == b.size())
    
    typename T::value_type res = typename T::value_type();
    for(typename T::size_type i = 0; i < a.size(); ++i) {
        res += a[i] * b[i];
    }
    return res;
}

template<typename T>
typename T::value_type d1(T const & a, T const & b) {
    ASSERT(a.size() == b.size())
    
    typename T::value_type res = typename T::value_type();
    for(typename T::size_type i = 0; i < a.size(); ++i) {
        if(a[i] > b[i]) //bc of unsigned
            res += (a[i] - b[i]);
        else
            res += (b[i] - a[i]);
    }
    return res;
}

template<typename T>
typename T::value_type d2(T const & a, T const & b) {
    ASSERT(a.size() == b.size())
    
    typename T::value_type res = typename T::value_type();
    for(typename T::size_type i = 0; i < a.size(); ++i) {
        if(a[i] > b[i]) //bc of unsigned
            res += (a[i] - b[i])*(a[i] - b[i]);
        else
            res += (b[i] - a[i])*(b[i] - a[i]);
    }
    return sqrt(res);
}

template<typename T>
typename T::value_type accumulate(T const & a) {
    typename T::value_type res = typename T::value_type();
    for(typename T::size_type i = 0; i < a.size(); ++i) {
        res += a[i];
    }
    return res;
}


//=================== custom returntype versions ===================
template<typename R, typename T>
R scalar(T const & a, T const & b) {
    ASSERT(a.size() == b.size())
    
    R res = R();
    for(typename T::size_type i = 0; i < a.size(); ++i) {
        res += a[i] * b[i];
    }
    return res;
}

template<typename R, typename T>
R d1(T const & a, T const & b) {
    ASSERT(a.size() == b.size())
    
    R res = R();
    for(typename T::size_type i = 0; i < a.size(); ++i) {
        if(a[i] > b[i]) //bc of unsigned
            res += (a[i] - b[i]);
        else
            res += (b[i] - a[i]);
    }
    return res;
}

template<typename R, typename T>
R d2(T const & a, T const & b) {
    ASSERT(a.size() == b.size())
    
    R res = R();
    for(typename T::size_type i = 0; i < a.size(); ++i) {
        if(a[i] > b[i]) //bc of unsigned
            res += (a[i] - b[i])*(a[i] - b[i]);
        else
            res += (b[i] - a[i])*(b[i] - a[i]);
    }
    return sqrt(res);
}

template<typename R, typename T>
R accumulate(T const & a) {
    R res = R();
    for(typename T::size_type i = 0; i < a.size(); ++i) {
        res += a[i];
    }
    return res;
}

#endif //__NUMERICS_HEADER
