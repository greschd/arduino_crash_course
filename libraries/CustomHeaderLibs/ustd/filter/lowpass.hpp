// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    23.07.2013 05:09:59 EDT
// File:    lowpass.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __LOWPASS_HEADER
#define __LOWPASS_HEADER

#include "realization.hpp"
#include "../../util/mean_trait.hpp"


namespace ustd {
    template<typename T, unsigned N, typename _base = identity_filter<T> >
    class lowpass_filter: public _base {
        typedef typename util::mean_trait<T>::type mean_type;
        typedef _base base;
    public:
        //------------------- ctors -------------------
        lowpass_filter(): val_() {
        }
        lowpass_filter(lowpass_filter const & arg): val_(arg.val_) {
        }
        lowpass_filter(T const & arg): val_(arg) {
        }
        //------------------- ops -------------------
        lowpass_filter & operator=(T const & in) {
            base::operator=(in);
            val_ = base::value();
            return (*this);
        }
        lowpass_filter & operator<<(T const & in) {
            base::operator<<(in);
            val_ = ((N - 1) * val_ + base::value()) / N;
            return (*this);
        }
        //------------------- converter -------------------
        T value() const {
            return lowpass_impl<mean_type, T>();
        }
        operator T() const {
            return value();
        }
    private:
        //------------------- impl -------------------
        template<typename M, typename U>
        typename enable_if<is_same<M, U>::value, M>::type lowpass_impl() const {
            return val_;
        }
        template<typename M, typename U>
        typename enable_if<not is_same<M, U>::value, M>::type lowpass_impl() const {
            return val_ + 0.5; //round for integer types
        }
    private:
        mean_type val_;
    };
    //------------------- spez for N == 1 -------------------
    EMPTY_FILTER(lowpass, 1)
    //------------------- realization for filter -------------------
    template<typename T, typename _base, unsigned N>
    struct realization<T, tag::lowpass<N>, _base> {
        typedef lowpass_filter<T, N, _base> type;
    };
}//end namespace ustd
#endif //__LOWPASS_HEADER
