// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    20.05.2013 12:21:03 EDT
// File:    vector.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __VECTOR_HEADER
#define __VECTOR_HEADER

#include "ard_assert.hpp"

#include "../com/archive_enum.hpp"

namespace ustd {
    
    template<typename T>
    class vector {
    public:
        typedef T value_type;
        typedef uint16_t size_type;
        //------------------- ctors -------------------
        vector(): array_((T*)malloc(sizeof(T))), size_(0) {
        }
        ~vector() {
            free(array_);
        }
        explicit vector(size_type const & size, T const & val = T()): array_((T*)malloc(size * sizeof(T))), size_(size) {
            for(size_type i = 0; i < size_; ++i) {
                array_[i] = val;
            }
        }
        vector(vector const & arg): array_((T*)malloc(arg.size_ * sizeof(T))), size_(arg.size_) {
            for(size_type i = 0; i < size_; ++i) {
                array_[i] = arg.array_[i];
            }
        }
        vector & operator=(vector const & rhs) {
            clear();
            reserve(rhs.size());
            shrink_to_fit();
            for(size_type i = 0; i < rhs.size(); ++i)
                push_back(rhs[i]);
            return (*this);
        }
        //------------------- ops -------------------
        void push_back(T const & in) {
            if(size_ == capacity()) {
                array_ = (T*)realloc(array_, size_ * 2 * sizeof(T));
            }
            ASSERT_MSG(array_ != NULL, "no RAM for vector of this size")
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
            ASSERT(pos >= 0)
            ASSERT(pos < size_)
            return array_[pos];
        }
        T const & operator[](size_type const & pos) const {
            ASSERT(pos >= 0)
            ASSERT(pos < size_)
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
            return array_;
        }
        //------------------- size/capacity -------------------
        size_type const & size() const {
            return size_;
        }
        size_type capacity() const {
            return (*((size_type *)(int(array_) - 2))) / sizeof(T);
        }
        void reserve(size_type const & new_capacity) {
            if(new_capacity > capacity()) {
                //this 'hack' makes the vector not portable, but I don't need to save the capacity => -2 bytes ;)
                array_ = (T*)realloc(array_, new_capacity * sizeof(T));
            }
        }
        void shrink_to_fit() {
            if(size_ != 0)
                array_ = (T*)realloc(array_, size_ * sizeof(T));
        }
        void resize(size_type const & new_size, T const & val = T()) {
            reserve(new_size);
            while(new_size > size_)
                push_back(val);
            
            size_ = new_size;
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
            os << F("[");
            for(size_type i = 0; i < size_; ++i) {
                os << array_[i];
                if(i != size_ - 1)
                    os << F(", ");
            }
            os << F("]");
        }
        template<typename Archive>
        void serialize(Archive & ar) {
            ar & size_;
            if(Archive::type == archive_enum::input) {
                reserve(size_);
            }
            for(size_type i = 0; i < size_; ++i) {
                ar & array_[i];
            }
        }
    private:
        T * array_;
        size_type size_;
    };
}//end namespace ustd

#endif //__VECTOR_HEADER
