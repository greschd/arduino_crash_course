// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    29.05.2013 23:04:33 EDT
// File:    serializer.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __SERIALIZER_HEADER
#define __SERIALIZER_HEADER

//~ #define __TYPE_SIZE_CHECK
//~ #define __LESS_ZERO

#include "archive_enum.hpp"
#include "../ustd/type_traits.hpp"
#include "../ustd/ard_assert.hpp"
#include "../util/byte_operation.hpp"
#include "../util/has_serialize.hpp"

namespace com {
    //------------------- implementation -------------------
    template<typename T, typename Archive>
    void serialize(Archive & ar, T & t);
    //------------------- recursively serializes arrays :D -------------------
    template<typename T, typename Archive, archive_enum type, bool is_an_array> //default == true
    struct impl_version {
        static void get(T & t, Archive & ar) {
            for(uint16_t i = 0; i < sizeof(t)/sizeof(t[0]); ++i) {
                serialize(ar, t[i]); //<-calls serialize for subarrays
            }
        }
    };
    template<typename T, typename Archive>
    struct impl_version<T, Archive, archive_enum::output, false> { //if no serialize, then byte-serialize
        static void get(T const & t, Archive & ar) {
            #ifdef __TYPE_SIZE_CHECK
                ar.write(sizeof(t));
            #endif //__TYPE_SIZE_CHECK
            #ifdef __LESS_ZERO
                uint8_t idx = sizeof(t);
                while(idx != 0) {
                    if(util::read_byte(t, idx - 1) != 0)
                        break;
                    else
                        --idx;
                }
                ar.write(idx);
                for(uint8_t i = 0; i < idx; ++i)
                    ar.write(util::read_byte(t, i));
            #else //__LESS_ZERO
                for(uint8_t i = 0; i < sizeof(t); ++i)
                    ar.write(util::read_byte(t, i));
            #endif //__LESS_ZERO
        }
    };
    template<typename T, typename Archive>
    struct impl_version<T, Archive, archive_enum::input, false> {
        static void get(T & t, Archive & ar) {
            #ifdef __TYPE_SIZE_CHECK
                uint8_t exp_size = ar.read();
                ASSERT(sizeof(t) == exp_size);
            #endif //__TYPE_SIZE_CHECK
            #ifdef __LESS_ZERO
                uint8_t idx = ar.read();
                for(uint8_t i = 0; i < idx; ++i)
                    util::write_byte(t, i, ar.read());
                for(uint8_t i = idx; i < sizeof(t); ++i) {
                    util::write_byte(t, i, 0x00);
                }
            #else //__LESS_ZERO
                for(uint8_t i = 0; i < sizeof(t); ++i)
                    util::write_byte(t, i, ar.read());
            #endif //__LESS_ZERO
        }
    };

    template<typename T, bool has_serialize_tpl, typename Archive> //default == true
    struct serialize_helper {
        static void get(Archive & ar, T & t) {
            t.serialize(ar);
        }
    };

    template<typename T, typename Archive> //has no serialize_tpl
    struct serialize_helper<T, false, Archive> {
        static void get(Archive & ar, T & t) {
            impl_version<T, Archive, Archive::type, ustd::is_array<T>::value>().get(t, ar);
        }
    };

    template<typename T, typename Archive>
    void serialize(Archive & ar, T & t) {
        serialize_helper<T, util::has_serialize<T>::value, Archive>().get(ar, t);
    }
    //------------------- endian helper for of class -------------------
    template<typename T>
    T little_endian(T const & t) { //just mirrors the bytes
        T res(0);
        
        for(uint8_t i = 0; i < sizeof(t); ++i) {
            res |= ((t >> ((sizeof(t) - i - 1))*8 & 0xFF) << (8 * i));
        }
        
        return res;
    }
    //------------------- of class -------------------
    template<typename D, uint16_t max_buf>
    class oss_class {
    public:
        typedef uint16_t size_type;
        static archive_enum const type = archive_enum::output;
        
        oss_class(D & data): pos_(0), data(data) {
        }
        template<typename T>
        oss_class & operator&(T const & t) {
            serialize(*this, t);
            return (*this);
        }
        template<typename S>
        void print(S & os) const {
            #ifdef __TYPE_SIZE_CHECK
                size_type sub = 0;
            #endif //__TYPE_SIZE_CHECK
            
            os << F("oss: ");
            for(size_type i = 0; i < pos_; ++i) {
                #ifdef __TYPE_SIZE_CHECK
                    if(i == sub) {
                        uint8_t size = data.read(i);
                        os << GREENB << size << NONE << F(" ");
                        sub += size + 1;
                    }
                    else
                        os << data.read(i) << F(" ");
                #else //__TYPE_SIZE_CHECK
                    os << data.read(i) << F(" ");
                #endif //__TYPE_SIZE_CHECK
            }
        }
        void write(uint8_t const & in) {
            ASSERT_MSG(pos_ < max_buf, "buffer_size to small in oss_class::operator<<")
            data.write(pos_, in);
            ++(pos_);
        }
        size_type & pos() {
            return pos_;
        }
    private:
        size_type pos_;
        D & data;
    };
    //------------------- os stream operator -------------------
    template<typename D, uint16_t max_buf, typename S>
    S & operator<<(S & os, oss_class<D, max_buf> const & arg) {
        arg.print(os);
        return os;
    }

    //------------------- if class -------------------
    template<typename D, uint16_t max_buf>
    class iss_class {
    public:
        typedef uint16_t size_type;
        static archive_enum const type = archive_enum::input;
        
        iss_class(D & data): pos_(0), data(data) {
        }
        template<typename T>
        iss_class & operator&(T & t) {
            serialize(*this, t);
            return (*this);
        }
        template<typename S>
        void print(S & os) const {
            #ifdef __TYPE_SIZE_CHECK
                size_type sub = 0;
            #endif //__TYPE_SIZE_CHECK
            
            os << F("iss: ");
            for(size_type i = 0; i < pos_; ++i) {
                #ifdef __TYPE_SIZE_CHECK
                    if(i == sub) {
                        uint8_t size = data.read(i);
                        os << REDB << size << NONE << F(" ");
                        sub += size + 1;
                    }
                    else {
                        os << data.read(i) << F(" ");
                    }
                #else //__TYPE_SIZE_CHECK
                    os << data.read(i) << F(" ");
                #endif //__TYPE_SIZE_CHECK
            }
        }
        uint8_t read() {
            ASSERT_MSG(pos_ < max_buf, "buffer_size to small in iss_class::operator<<")
            return data.read(pos_++);
        }
        size_type & pos() {
            return pos_;
        }
    private:
        size_type pos_;
        D & data;
    };
    //------------------- os<<stream operator -------------------
    template<typename D, uint16_t max_buf, typename S>
    S & operator<<(S & os, iss_class<D, max_buf> const & arg) {
        arg.print(os);
        return os;
    }
}//end namespace com
#endif //__SERIALIZER_HEADER
