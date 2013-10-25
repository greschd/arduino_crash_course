// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    14.06.2013 20:45:56 EDT
// File:    print_uint_8.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */
 
#ifndef __PRINT_UINT_8_HEADER
#define __PRINT_UINT_8_HEADER

//------------------- i want uint8_t to display as a number -------------------
inline std::ostream & operator<<(std::ostream & os, uint8_t const & a) {
    os << int(a);
    return os;
}
namespace ustd {
    inline std::ostream & operator<<(std::ostream & os, uint8_t const & a) {
        os << int(a);
        return os;
    }
}//end namespace ustd
namespace com {
    inline std::ostream & operator<<(std::ostream & os, uint8_t const & a) {
        os << int(a);
        return os;
    }
}//end namespace com
namespace tool {
    inline std::ostream & operator<<(std::ostream & os, uint8_t const & a) {
        os << int(a);
        return os;
    }
}//end namespace tool

#endif //__PRINT_UINT_8_HEADER
