// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    20.07.2013 11:27:01 EDT
// File:    mean_module.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __MEAN_MODULE_HEADER
#define __MEAN_MODULE_HEADER

#include "module.hpp"
#include "../../util/mean_trait.hpp"

namespace ustd {
    namespace detail {
        template<>
        struct requirement<tag::mean> {
            typedef typename util::make_list<tag::count>::type type;
        };
    }//end namespace detail
    
    template<typename T, typename _base>
    class module<T, tag::mean, _base>: public _base {
        typedef _base base;
        typedef typename util::mean_trait<T>::type mean_type;
    public:
        //------------------- ctor -------------------
        module(): base(), sum_() {
        }
        //------------------- ops -------------------
        void operator<<(T const & in) {
            base::operator<<(in);
            sum_ += in;
        }
        //------------------- fct -------------------
        mean_type mean() const {
            return sum_ / base::count();
        }
        void clear() {
            base::clear();
            sum_ = mean_type();
        }
    private:
        mean_type sum_;
    };
}//end namespace ustd 

#endif //__MEAN_MODULE_HEADER
