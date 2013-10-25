// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    23.07.2013 05:09:57 EDT
// File:    identity.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __IDENTITY_HEADER
#define __IDENTITY_HEADER

#include "realization.hpp"

namespace ustd {
    template<typename T>
    class identity_filter {
    public:
        //------------------- ctors -------------------
        identity_filter(): val_() {
        }
        identity_filter(identity_filter const & arg): val_(arg.val_) {
        }
        identity_filter(T const & arg): val_(arg) {
        }
        //------------------- ops -------------------
        identity_filter & operator=(T const & in) {
            val_ = in;
            return (*this);
        }
        identity_filter & operator<<(T const & in) {
            val_ = in;
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
    //------------------- realization for filter -------------------
    template<typename T, typename _base>
    struct realization<T, tag::identity, _base> {
        typedef identity_filter<T> type;
    };
}//end namespace ustd
#endif //__IDENTITY_HEADER
