// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    23.07.2013 07:58:22 EDT
// File:    highpass.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __HIGHPASS_HEADER
#define __HIGHPASS_HEADER

#include "realization.hpp"
#include "../../util/mean_trait.hpp"


namespace ustd {
    template<typename T, unsigned PROMIL, typename _base = identity_filter<T> >
    class highpass_filter: public _base {
        typedef typename util::mean_trait<T>::type mean_type;
        typedef _base base;
    public:
        //------------------- ctors -------------------
        highpass_filter(): val_(), last_in_() {
        }
        highpass_filter(highpass_filter const & arg): val_(arg.val_) {
        }
        highpass_filter(T const & arg): val_(arg) {
        }
        //------------------- ops -------------------
        highpass_filter & operator=(T const & in) {
            base::operator=(in);
            val_ = base::value();
            last_in_ = base::value();
            return (*this);
        }
        highpass_filter & operator<<(T const & in) {
            base::operator<<(in);
            val_ = (val_ + base::value() - last_in_) * PROMIL / 1000.0;
            last_in_ = base::value();
            return (*this);
        }
        //------------------- converter -------------------
        T value() const {
            return highpass_impl<mean_type, T>();
        }
        operator T() const {
            return value();
        }
    private:
        //------------------- impl -------------------
        template<typename M, typename U>
        typename enable_if<is_same<M, U>::value, M>::type highpass_impl() const {
            return val_;
        }
        template<typename M, typename U>
        typename enable_if<not is_same<M, U>::value, M>::type highpass_impl() const {
            return val_ + 0.5; //round for integer types
        }
    private:
        mean_type val_;
        T last_in_;
    };
    //------------------- realization for filter -------------------
    template<typename T, typename _base, unsigned PROMIL>
    struct realization<T, tag::highpass<PROMIL>, _base> {
        typedef highpass_filter<T, PROMIL, _base> type;
    };
}//end namespace ustd
#endif //__HIGHPASS_HEADER
