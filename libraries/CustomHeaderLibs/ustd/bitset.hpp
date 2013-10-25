// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    15.06.2013 19:00:12 EDT
// File:    bitset.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __BITSET_HEADER
#define __BITSET_HEADER

#include "ard_assert.hpp"
#include "../util/byte_operation.hpp"

namespace ustd {
    template<uint16_t Nbit> //number of bits
    class bitset {
        typedef uint16_t size_type;
        
        class reference {
        friend class bitset;
            reference() {}
            reference(uint8_t & unit, uint8_t const & subpos): unit_(unit), subpos_(subpos) {
            }
        public:
            operator bool() const {
                return util::read_bit(unit_, subpos_);
            }
            reference& operator=(bool state) {
                util::write_bit(unit_, subpos_, state);
                return (*this);
            }
            reference& operator=(const reference& state) {
                util::write_bit(unit_, subpos_, state);
                return (*this);
            }
            reference& flip() {
                util::flip_bit(unit_, subpos_);
                return (*this);
            }
            bool operator~() const {
                flip();
                return (*this);
            }
        private:
            uint8_t & unit_;
            uint8_t const subpos_;
        };
        
    public:
        //------------------- ctors -------------------
        bitset() {
            reset();
        }
        bitset(bitset const & arg) {
            for(size_type i = 0; i < (Nbit + 7) / 8; ++i) {
                array_[i] = arg.array_[i];
            }
        }
        bitset & operator=(bitset const & rhs) {
            for(size_type i = 0; i < (Nbit + 7) / 8; ++i) {
                array_[i] = rhs.array_[i];
            }
            return (*this);
        }
        //------------------- ops -------------------
        bitset & set() {
            for(size_type i = 0; i < (Nbit + 7) / 8; ++i) {
                array_[i] = 0xFF;
            }
            return (*this);
        }
        bitset & set(size_type const & pos, bool const & val = true) {
            (*this)[pos] = val;
            return (*this);
        }
        bitset & flip() {
            for(size_type i = 0; i < (Nbit + 7) / 8; ++i) {
                array_[i] ^= 0xFF;
            }
            return (*this);
        }
        bitset & flip(size_type const & pos) {
            (*this)[pos].flip();
            return (*this);
        }
        bitset & operator~() {
            flip();
        }
        bitset & reset() {
            for(size_type i = 0; i < (Nbit + 7) / 8; ++i) {
                array_[i] = 0;
            }
            return (*this);
        }
        bitset & reset(size_type const & pos) {
            set(pos, 0);
            return (*this);
        }
        //------------------- getter -------------------
        reference operator[](size_type const & pos) {
            ASSERT(pos >= 0)
            ASSERT(pos < Nbit)
            return reference(array_[pos/8], pos % 8); //&7 === %8
        }
        bool operator[](size_type const & pos) const {
            ASSERT(pos >= 0)
            ASSERT(pos < Nbit)
            return util::read_bit(array_[pos/8], pos % 8);
        }
        //------------------- info -------------------
        size_type size() const {
            return Nbit;
        }
        bool any() const {
            for(size_type i = 0; i < (Nbit + 7) / 8; ++i) {
                if(array_[i] != 0)
                    return true;
            }
            return false;
        }
        bool none() const {
            return !any();
        }
        size_type count() const {
            return 0;
        }
        //------------------- print & serialize-------------------
        template<typename S>
        void print(S & os) const {
            for(size_type i = 0; i < Nbit; ++i) {
                os << (*this)[Nbit - i - 1];
            }
        }
        template<typename Archive>
        void serialize(Archive & ar) {
            ar & array_;
        }
    private:
        uint8_t array_[(Nbit + 7) / 8];
    };
}//end namespace ustd
#endif //__BITSET_HEADER
