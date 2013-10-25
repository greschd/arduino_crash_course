// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    20.07.2013 11:26:59 EDT
// File:    min_module.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __MIN_MODULE_HEADER
#define __MIN_MODULE_HEADER

#include "module.hpp"

namespace ustd {
    namespace detail {
        template<>
        struct requirement<tag::min> {
            typedef util::list_end type;
        };
    }//end namespace detail
    
    template<typename T, typename _base>
    class module<T, tag::min, _base>: public _base {
        typedef _base base;
        T const init_min = 1023;
    public:
        //------------------- ctor -------------------
        module(): base(), min_(init_min) { //just heuristics for now
        }
        //------------------- ops -------------------
        void operator<<(T const & in) {
            base::operator<<(in);
            if(in < min_)
                min_ = in;
        }
        //------------------- fct -------------------
        T const & min() {
            return min_;
        }
        void clear() {
            base::clear();
            min_ = init_min;
        }
    private:
        T min_;
    };
}//end namespace ustd 

#endif //__MIN_MODULE_HEADER
