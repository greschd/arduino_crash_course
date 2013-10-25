// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    14.06.2013 20:43:00 EDT
// File:    vector_support.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __VECTOR_SUPPORT_HEADER
#define __VECTOR_SUPPORT_HEADER

#include <vector>

//------------------- print & serialize -------------------
template<typename T, typename S>
S & operator<<(S & os, std::vector<T> const & arg) {
    typedef typename std::vector<T>::size_type size_type;
    os << "[";
    for(size_type i = 0; i < arg.size(); ++i) {
        os << arg[i];
        if(i != arg.size() - 1)
            os << ", ";
    }
    os << "]";
    return os;
}
namespace com {
    template<typename T, typename Archive>
    void serialize(Archive & ar, std::vector<T> & arg) {
        typedef typename ustd::vector<T>::size_type size_type;
        size_type size_ = arg.size();
        ar & size_;
        if(Archive::type == archive_enum::input) {
            arg.resize(size_);
        }
        for(size_type i = 0; i < size_; ++i) {
            ar & arg[i];
        }
    }
}//end namespace com
#endif //__VECTOR_SUPPORT_HEADER
