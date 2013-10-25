// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    15.06.2013 21:35:16 EDT
// File:    fast_bitset.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __FAST_BITSET_HEADER
#define __FAST_BITSET_HEADER

//CAN ONLY BE USED UP TO N==64

#include "ard_assert.hpp"
#include "../util/byte_operation.hpp"

namespace ustd {
    namespace detail {
        
        template<bool, bool, bool, bool>
        struct size_trait_impl {
        };
        template<>
        struct size_trait_impl<false, false, false, false> {
            typedef uint8_t type;
        };
        template<>
        struct size_trait_impl<true, false, false, false> {
            typedef uint16_t type;
        };
        template<>
        struct size_trait_impl<true, true, false, false> {
            typedef uint32_t type;
        };
        template<>
        struct size_trait_impl<true, true, true, false> {
            typedef uint64_t type;
        };
    }//end namespace detail
    template<uint16_t N>
    struct size_trait {
        typedef typename detail::size_trait_impl<(N > 8), (N > 16), (N > 32), (N > 64)>::type type;
    };
    
    template<uint8_t N>
    class fast_bitset {
    public:
        typedef typename size_trait<N>::type T;
        typedef uint8_t size_type;
        //------------------- ctors -------------------
        fast_bitset(): data_() {
        }
        fast_bitset(fast_bitset const & arg): data_(arg.data_) {
        }
        fast_bitset & operator=(fast_bitset const & rhs) {
            data_ = rhs.data_;
            return (*this);
        }
        //------------------- ops -------------------
        void set() { data_ = T(-1); }
        void set(size_type const & pos, uint8_t const & val = true) { util::write_bit(data_, pos, val); }
        void flip() { data_ ^= T(-1); }
        void flip(size_type const & pos) { util::flip_bit(data_, pos); }
        void reset() { data_ = T(); }
        void reset(size_type const & pos) { util::clear_bit(data_, pos);}
        //------------------- getter -------------------
        bool operator[](size_type const & pos) const {
            return util::read_bit(data_, pos);
        }
        operator T&() {
            return data_;
        }
        //------------------- info -------------------
        bool any() const {
            return data_ == T();
        }
        bool none() const {
            return !any();
        }
        //------------------- print & serialize-------------------
        template<typename S>
        void print(S & os) const {
            for(size_type i = 0; i < N; ++i) {
                os << util::read_bit(data_, N - i - 1);
            }
        }
        template<typename Archive>
        void serialize(Archive & ar) {
            size_type size_ = N;
            ar & size_;
            ASSERT(size_ == N)
            ar & data_;
        }
    private:
        T data_;
    };
}//end namespace ustd

#endif //__FAST_BITSET_HEADER
