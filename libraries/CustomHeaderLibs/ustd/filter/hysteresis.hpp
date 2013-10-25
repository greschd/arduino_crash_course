// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    23.07.2013 05:09:54 EDT
// File:    hysteresis.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __HYSTERESIS_HEADER
#define __HYSTERESIS_HEADER

#include "realization.hpp"
#include "../../util/mean_trait.hpp"

#ifndef Arduino_h
    #include <cmath>
    using std::min;
    using std::max;
#endif

namespace ustd {
    template<typename T, unsigned W, typename _base = identity_filter<T> >
    class hysteresis_filter: public _base {
        typedef _base base;
    public:
        //------------------- ctors -------------------
        hysteresis_filter(): val_() {
        }
        hysteresis_filter(hysteresis_filter const & arg): val_(arg.val_) {
        }
        hysteresis_filter(T const & arg): val_(arg) {
        }
        //------------------- ops -------------------
        hysteresis_filter & operator=(T const & in) {
            base::operator=(in);
            val_ = base::value();
            return (*this);
        }
        hysteresis_filter & operator<<(T const & in) {
            base::operator<<(in);
            
            T temp = base::value();
            
            if(temp > val_ + (T)W)
                val_ = temp - W;
            else
                if(temp + (T)W < val_)
                    val_ = temp + W;
            
            return (*this);
        }
        //------------------- converter -------------------
        T value() const {
            return val_;
        }
        operator T() const {
            return value();
        }
    private:
        T val_;
    };
    //------------------- spez for W == 0 -------------------
    EMPTY_FILTER(hysteresis, 0)
    //------------------- realization for filter -------------------
    template<typename T, typename _base, unsigned W>
    struct realization<T, tag::hysteresis<W>, _base> {
        typedef hysteresis_filter<T, W, _base> type;
    };
}//end namespace ustd
#endif //__HYSTERESIS_HEADER
