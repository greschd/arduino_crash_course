// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    20.07.2013 11:27:09 EDT
// File:    error_module.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __ERROR_MODULE_HEADER
#define __ERROR_MODULE_HEADER

#ifndef Arduino_h
    #include <cmath>
#endif

#include "module.hpp"

namespace ustd {
    namespace detail {
        template<>
        struct requirement<tag::error> {
            typedef typename util::make_list<tag::count, tag::mean>::type type;
        };
    }//end namespace detail
    
    template<typename T, typename _base>
    class module<T, tag::error, _base>: public _base {
        typedef _base base;
        typedef typename util::mean_trait<T>::type error_type;
    public:
        //------------------- ctor -------------------
        module(): base(), sum2_(0) {
        }
        //------------------- ops -------------------
        void operator<<(T const & in) {
            base::operator<<(in);
            sum2_ += error_type(in) * in;
        }//------------------- fct -------------------
        error_type std() const {
            //~ using std::sqrt;
            return sqrt((sum2_ - base::mean() * base::mean() * base::count()) / (base::count() - 1));
        }
        error_type m_std() const {
            //~ using std::sqrt;
            return std() / sqrt(base::count());
        }
        void clear() {
            base::clear();
            sum2_ = error_type();
        }
    private:
        error_type sum2_;
    };
}//end namespace ustd 

#endif //__ERROR_MODULE_HEADER
