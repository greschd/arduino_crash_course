// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    15.06.2013 23:53:44 EDT
// File:    static_vector.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __STATIC_VECTOR_HEADER
#define __STATIC_VECTOR_HEADER

#include "ard_assert.hpp"
#include "array.hpp"

namespace ustd {
    
    template<typename T, uint16_t capacity_>
    class static_vector {
    public:
        typedef T value_type;
        typedef uint16_t size_type;
        //------------------- ctors -------------------
        static_vector(): size_(0) {
        }
        explicit static_vector(size_type const & size, T const & val = T()): size_(size) {
            for(size_type i = 0; i < size_; ++i) {
                array_[i] = val;
            }
        }
        static_vector(static_vector const & arg): size_(arg.size_), array_(arg.array_) {
        }
        static_vector & operator=(static_vector const & rhs) {
            size_ = rhs.size_;
            array_ = rhs.array_;
            return (*this);
        }
        //------------------- ops -------------------
        void push_back(T const & in) {
            ASSERT_MSG(size_ < capacity_, "static_vector too small")
            array_[size_] = in;
            ++size_;
        }
        void pop_back() {
            --size_;
        }
        size_type find(T const & val) const {
            size_type pos = 0;
            for(; pos < size_; ++pos) {
                if(array_[pos] == val)
                    return pos;
            }
            return end();
        }
        void clear() {
            size_ = 0;
        }
        void erase(size_type const & pos) {
            if(pos < end()) {
                for(size_type i = pos; i < size_; ++i) {
                    array_[i] = array_[i + 1];
                }
                --size_;
            }
        }
        //------------------- getter -------------------
        T & operator[](size_type const & pos) {
            return array_[pos];
        }
        T const & operator[](size_type const & pos) const {
            return array_[pos];
        }
        T & back() {
            ASSERT(size_ > 0)
            return array_[size_ - 1];
        }
        T const & back() const {
            ASSERT(size_ > 0)
            return array_[size_ - 1];
        }
        T & front() {
            ASSERT(size_ > 0)
            return array_[0];
        }
        T const & front() const {
            ASSERT(size_ > 0)
            return array_[0];
        }
        T * data() {
            return array_.data();
        }
        //------------------- special fct to support serializer -------------------
    private:
        template<typename U>
        uint8_t read_impl(size_type const & adr, U const & val) {
            ASSERT_MSG(0, "do not use unless T = byte")
            return 0xFF;
        }
        uint8_t read_impl(size_type const & adr, uint8_t const & val) {
            return array_[adr];
        }
        template<typename U>
        void write_impl(size_type const & adr, U const & val) {
            ASSERT_MSG(0, "do not use unless T = byte")
        }
        void write_impl(size_type const & adr, uint8_t const & val) {
            array_[adr] = val;
        }
    public:
        uint8_t read(size_type const & adr) {
            return read_impl(adr, T());
        }
        void write(size_type const & adr, T const & val) {
            write_impl(adr, val);
        }
        //------------------- size/capacity -------------------
        size_type const & size() const {
            return size_;
        }
        void set_size(size_type const & new_size) {
            ASSERT(new_size <= capacity_);
            size_ = new_size;
        }
        size_type  capacity() const {
            return capacity_;
        }
        void resize(size_type const & new_size, T const & val = T()) {
            if(new_size > size_) {
                while(new_size > size_) {
                    push_back(val);
                }
            }
            else {
                size_ = new_size;
            }
        }
        //------------------- info -------------------
        bool empty() const {
            return (size_ == size_type());
        }
        size_type const & begin() const {
            return 0;
        }
        size_type const & end() const {
            return size_;
        }
        //------------------- print & serialize-------------------
        template<typename S>
        void print(S & os) const {
            os << array_;
        }
        template<typename Archive>
        void serialize(Archive & ar) {
            ar & size_;
            ar & array_;
        }
    private:
        size_type size_;
        array<T, capacity_> array_;
    };
}//end namespace ustd

#endif //__STATIC_VECTOR_HEADER
