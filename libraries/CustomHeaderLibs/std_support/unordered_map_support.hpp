// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    14.06.2013 20:43:37 EDT
// File:    unordered_map_support.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __UNORDERED_MAP_SUPPORT_HEADER
#define __UNORDERED_MAP_SUPPORT_HEADER

#include <unordered_map>
#include <algorithm>
#include <vector>

#include "vector_support.hpp"

//------------------- print & serialize -------------------
template<typename K, typename V, typename S>
S & operator<<(S & os, std::unordered_map<K, V> const & arg) {
    os << "[";
    uint16_t i = 0;
    std::for_each(arg.begin(), arg.end(), 
        [&](std::pair<K const, V> const & p) {
            os << "{" << p.first << ": " << p.second << "}";
            if(++i != arg.size())
                os << ", ";
        }
    );
    os << "]";
    return os;
}
namespace com {
    template<typename K, typename V, typename Archive>
    void serialize(Archive & ar, std::unordered_map<K, V> & arg) {
        typedef typename ustd::vector<K>::size_type size_type;
        size_type size_ = arg.size();
        if(Archive::type == archive_enum::input) {
            arg.clear();
            K key;
            V val;
            std::vector<K> keys;
            std::vector<V> vals;
            ar & keys;
            ar & vals;
            
            for(int i = keys.size() - 1; i >= 0; --i) {
                arg[keys[i]] = vals[i];
            }
        }
        else if(Archive::type == archive_enum::output) {
            ar & size_;
            std::for_each(arg.begin(), arg.end(), 
                [&](std::pair<K const, V> const & p) {
                    ar & (p.first);
                }
            );
            ar & size_;
            std::for_each(arg.begin(), arg.end(), 
                [&](std::pair<K const, V> const & p) {
                    ar & (p.second);
                }
            );
        }
    }
}//end namespace com
#endif //__UNORDERED_MAP_SUPPORT_HEADER
