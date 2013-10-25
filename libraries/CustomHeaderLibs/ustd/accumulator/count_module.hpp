// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    20.07.2013 11:27:14 EDT
// File:    countag::module.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __COUNT_MODULE_HEADER
#define __COUNT_MODULE_HEADER

#include "module.hpp"

namespace ustd {
    namespace detail {
        template<>
        struct requirement<tag::count> {
            typedef util::list_end type;
        };
    }//end namespace detail
    
    template<typename T, typename _base>
    class module<T, tag::count, _base>: public _base {
        typedef _base base;
    public:
        typedef module type;
        typedef size_t size_type;
        //------------------- ctor -------------------
        module(): base(), count_(0) {
        }
        //------------------- ops -------------------
        void operator<<(T const & in) {
            base::operator<<(in);
            ++count_;
        }
        //------------------- fct -------------------
        size_type const & count() const {
            return count_;
        }
        void clear() {
            base::clear();
            count_ = size_type();
        }
    private:
        size_type count_;
    };
}//end namespace ustd 

#endif //__COUNT_MODULE_HEADER
