// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    15.06.2013 17:18:44 EDT
// File:    checksum.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __CHECKSUM_HEADER
#define __CHECKSUM_HEADER

namespace util {
    //------------------- checksum generator -------------------
    template<typename T>
    uint8_t checksum(T const & t) {
        uint8_t res = 0;
        uint8_t const * const p = (uint8_t*)(&t);
        for(uint16_t i = 0; i < sizeof(t); ++i) {
            res += p[i];
        }
        return res;
    }
}//end namespace util

#endif //__CHECKSUM_HEADER
