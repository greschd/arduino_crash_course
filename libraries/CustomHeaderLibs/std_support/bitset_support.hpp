// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    15.06.2013 22:15:42 EDT
// File:    bitset_support.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __BITSET_SUPPORT_HEADER
#define __BITSET_SUPPORT_HEADER

#include <bitset>

//------------------- print & serialize -------------------
namespace com {
    template<size_t N, typename Archive>
    void serialize(Archive & ar, std::bitset<N> & arg) {
        typedef typename ustd::fast_bitset<1>::size_type size_type;
        size_type size_ = arg.size();
        ar & size_;
        if(size_ <= 8) {
            uint8_t data = arg.to_ulong();
            ar & data;
            arg = data;
        } else if(size_ <= 16) {
            uint16_t data = arg.to_ulong();
            ar & data;
            arg = data;
        } else if(size_ <= 32) {
            uint32_t data = arg.to_ulong();
            ar & data;
            arg = data;
        } else if(size_ <= 64) {
            uint64_t data = arg.to_ulong();
            ar & data;
            arg = data;
        }
    }
}//end namespace com
#endif //__BITSET_SUPPORT_HEADER
