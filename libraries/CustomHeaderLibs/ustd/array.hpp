// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    15.06.2013 23:45:24 EDT
// File:    array.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __ARRAY_HEADER
#define __ARRAY_HEADER

#include "ard_assert.hpp"

namespace ustd {
    /**\brief basically just a wrap for an cpp-array
     * \tparam T is the type in the container
     * \tparam N is the size of the array
     */
    template<typename T, uint16_t N>
    class array {
    public:
        typedef T value_type;
        typedef uint16_t size_type;
        //------------------- ctors -------------------
        /**\brief just initialises all with "zeros"
         */
        array() {
            for(size_type i = 0; i < N; ++i) {
                array_[i] = T();
            }
        }
        /**\brief initialises all with init
         * \param init all positions are set to this parameter
         */
        array(T const & init) {
            for(size_type i = 0; i < N; ++i) {
                array_[i] = init;
            }
        }
        /**\brief copy constructor
         */
        array(array const & arg) {
            for(size_type i = 0; i < N; ++i) {
                array_[i] = arg.array_[i];
            }
        }
        /**\brief asignment operator
         */
        array & operator=(array const & rhs) {
            for(size_type i = 0; i < N; ++i)
                array_[i] = rhs.array_[i];
            return (*this);
        }
        //------------------- getter -------------------
        /**\brief returns element at a position
         * \param pos is the position of interest
         * 
         * bounds are checked if debug is active, else no bound check
         */
        T & operator[](size_type const & pos) {
            ASSERT(pos >= 0)
            ASSERT(pos < N)
            return array_[pos];
        }
        /**\brief returns element at a position (const version)
         * \param pos is the position of interest
         * 
         * bounds are checked if debug is active, else no bound check
         */
        T const & operator[](size_type const & pos) const {
            ASSERT(pos >= 0)
            ASSERT(pos < N)
            return array_[pos];
        }
        /**\brief just return the internal cpp-array
         */
        T * data() {
            return array_;
        }
        //------------------- size/capacity -------------------
        /**\brief retuns the size (equal to N)
         */
        size_type size() const {
            return N;
        }
        //------------------- print & serialize-------------------
        /**\brief print to any stream-concept
         * \tparam stream-concept
         * \param os is an instance of a stream-concept
         */
        template<typename S>
        void print(S & os) const {
            os << F("[");
            for(size_type i = 0; i < N; ++i) {
                os << array_[i];
                if(i != N - 1)
                    os << F(", ");
            }
            os << F("]");
        }
        /**\brief serializes to or from archive
         * \tparam archive-concept
         * \param ar is the archive
         */
        template<typename Archive>
        void serialize(Archive & ar) {
            for(size_type i = 0; i< N; ++i)
                ar & array_[i];
        }
        //------------------- custom operations / not in std -------------------
        /**\brief set all elements to "zero"
         */
        void clear() {
            for(size_type i = 0; i< N; ++i)
                array_[i] = T();
        }
    private:
        T array_[N];
    };
}//end namespace ustd


#endif //__ARRAY_HEADER
