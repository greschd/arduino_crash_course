// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    26.08.2013 19:13:33 CEST
// File:    string_support.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __STRING_SUPPORT_HEADER
#define __STRING_SUPPORT_HEADER

#include <string>

//------------------- serialize -------------------
namespace com {
    template<typename Archive>
    void serialize(Archive & ar, std::string & arg) {
        typedef uint16_t size_type;
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

#endif //__STRING_SUPPORT_HEADER
