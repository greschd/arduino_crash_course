// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    23.07.2013 05:01:24 EDT
// File:    ema_module.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __EMA_MODULE_HEADER
#define __EMA_MODULE_HEADER

//exponentially moving average

#ifndef Arduino_h
    #include <cmath>
#endif

#include "module.hpp"
#include "../../util/mean_trait.hpp"

namespace ustd {
    namespace detail {
        template<int N>
        struct requirement<tag::ema<N> > {
            typedef util::list_end type;
        };
    }//end namespace detail
    
    template<typename T, typename _base, int N>
    class module<T, tag::ema<N>, _base>: public _base {
        typedef _base base;
        typedef typename util::mean_trait<T>::type mean_type;
    public:
        //------------------- ctor -------------------
        module(): base(), sum_() { //just heuristics for now
        }
        //------------------- ops -------------------
        void operator<<(T const & in) {
            base::operator<<(in);
            sum_ = ((N-1) * (sum_)) / N + in;
        }
        //------------------- fct -------------------
        T ema() const {
            return ema_impl<mean_type, T>();
        }
        void clear() {
            base::clear();
            sum_ = mean_type();
        }
    private:
        //------------------- impl -------------------
        template<typename M, typename U>
        typename enable_if<is_same<M, U>::value, M>::type ema_impl() const {
            return sum_ / N;
        }
        template<typename M, typename U>
        typename enable_if<not is_same<M, U>::value, M>::type ema_impl() const {
            return round(sum_ / N); //round for integer types
        }
    private:
        mean_type sum_;
    };
}//end namespace ustd 

#endif //__EMA_MODULE_HEADER
