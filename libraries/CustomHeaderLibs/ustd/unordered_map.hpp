// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    11.06.2013 21:17:40 EDT
// File:    map.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __MAP_HEADER
#define __MAP_HEADER

#include "ard_assert.hpp"
#include "vector.hpp"

namespace ustd {
    
    template<typename K, typename V>
    class unordered_map {
        typedef typename vector<K>::size_type size_type;
    public:
        typedef K key_type;
        typedef V value_type;
        //------------------- ctors -------------------
        unordered_map(): keys_(), vals_() {
        }
        unordered_map(unordered_map const & arg): keys_(arg.keys_), vals_(arg.vals_) {
        }
        unordered_map & operator=(unordered_map const & rhs) {
            keys_ = rhs.keys_;
            vals_ = rhs.vals_;
            return (*this);
        }
        //------------------- ops -------------------
        //------------------- getter -------------------
        V & operator[](K const & key) {
            size_type pos = keys_.find(key);
            if(pos == keys_.end()) { //not found
                keys_.push_back(key);
                vals_.push_back(V());
            }
            return vals_[pos];
        }
        V const & operator[](K const & key) const {
            size_type pos = keys_.find(key);
            if(pos == keys_.end()) { //not found
                keys_.push_back(key);
                vals_.push_back(V());
            }
            return vals_[pos];
        }
        V & at(K const & key) {
            size_type pos = keys_.find(key);
            ASSERT_MSG(pos != keys_.end(), "key not found in unordered_map");
            return vals_[pos];
        }
        V const & at(K const & key) const {
            size_type pos = keys_.find(key);
            ASSERT_MSG(pos != keys_.end(), "key not found in unordered_map");
            return vals_[pos];
        }
        //------------------- info -------------------
        bool contains(K const & key) const {
            size_type pos = keys_.find(key);
            if(pos == keys_.end()) { //not found
                return false;
            }
            return true;
        }
        //------------------- print & serialize-------------------
        template<typename S>
        void print(S & os) const {
            os << F("[");
            for(size_type i = 0; i < keys_.size(); ++i) {
                os << F("{") << keys_[i] << F(": ") << vals_[i] << F("}");
                if(i != keys_.end() - 1)
                    os << F(", ");
            }
            os << F("]");
        }
        template<typename Archive>
        void serialize(Archive & ar) {
            ar & keys_;
            ar & vals_;
        }
    private:
        vector<K> keys_;
        vector<V> vals_;
    };
}//end namespace ustd

#endif //__MAP_HEADER
