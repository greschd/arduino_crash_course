// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    16.06.2013 00:05:59 EDT
// File:    ring_buffer.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __RING_BUFFER_HEADER
#define __RING_BUFFER_HEADER

#include "../ustd/ard_assert.hpp"
#include "../ustd/array.hpp"

namespace tool {
    template<typename T, uint16_t N>
    class ring_buffer {
    public:
        typedef uint16_t size_type;
        typedef uint16_t count_type;
        //------------------- ctors -------------------
        ring_buffer(): count_(0) {
            for(size_type i = 0; i < N; ++i) {
                array_[i] = 0;
            }
        }
        ring_buffer(ring_buffer const & arg): count_(arg.count_), array_(arg.array_) {
        }
        ring_buffer & operator=(ring_buffer const & rhs) {
            count_ = rhs.count_;
            array_ = rhs.array_;
        }
        //------------------- ops -------------------
        void push_front(T const & t) {
            ++count_;
            (*this)[0] = t;
        }
        void pop_front() {
            (*this)[0] = T();
            --count_;
        }
        //------------------- getter -------------------
        T & operator[](size_type const & pos) {
            ASSERT(pos < size())
            return array_[(count_ - 1 - pos) % size()];
        }
        T const & operator[](size_type const & pos) const {
            ASSERT(pos < size())
            return array_[(count_ - 1 - pos) % size()];
        }
        T & back() {
            ASSERT(count_ > 0)
            return (*this)[size() - 1];
        }
        T const & back() const {
            ASSERT(count_ > 0)
            return (*this)[size() - 1];
        }
        T & front() {
            ASSERT(count_ > 0)
            return (*this)[0];
        }
        T const & front() const {
            ASSERT(count_ > 0)
            return (*this)[0];
        }
        //------------------- size -------------------
        size_type size() const {
            return ((count_ < N) ? count_ : N);
        }
        //------------------- info -------------------
        count_type const & count() const {
            return count;
        }
        //------------------- print & serialize-------------------
        template<typename S>
        void print(S & os) const {
            os << "[";
            for(size_type i = 0; i < size(); ++i) {
                os << (*this)[i];
                if(i != size() - 1)
                    os << ", ";
            }
            os << "]";
        }
        template<typename Archive>
        void serialize(Archive & ar) {
            ar & count_;
            ar & array_;
        }
    private:
        count_type count_;
        ustd::array<T, N> array_;
    };
    template<typename T, uint16_t N, typename S>
    S & operator<<(S & os, ring_buffer<T, N> const & arg) {
        arg.print(os);
        return os;
    }
}//end namespace tool

#endif //__RING_BUFFER_HEADER
