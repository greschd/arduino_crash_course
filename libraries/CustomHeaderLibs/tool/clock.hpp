// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    18.06.2013 15:54:46 EDT
// File:    clock.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __CLOCK_HEADER
#define __CLOCK_HEADER

#include <Arduino.h>

namespace tool {
    class clock_class {
    public:
        //------------------- ctors -------------------
        clock_class(): zero_ref_(0), us_now_(0), last_us_(0) {
        }
        //------------------- getter -------------------
        uint32_t sec() {
            return (us_now_ - zero_ref_) / 1000000.0;
        }
        uint16_t msec(uint16_t const & mod = 1000) {
            return ((us_now_ - zero_ref_) / 1000) % mod;
        }
        uint16_t usec(uint16_t const & mod) {
            return (us_now_ - zero_ref_) % mod;
        }
        uint64_t micros() const {
            return us_now_ - zero_ref_;
        }
        uint32_t millis() const {
            return (us_now_ - zero_ref_) / 1000.0;
        }
        //------------------- update -------------------
        void update() {
            uint32_t us = ::micros();
            us_now_ += uint32_t(us - last_us_); //works if there is an overflow
            last_us_ = us;
        }
        //------------------- ops -------------------
        void reset() {
            zero_ref_ = us_now_;
        }
    private:
        uint64_t zero_ref_;
        uint64_t us_now_;
        uint32_t last_us_;
    } clock;
}//end namespace tool

#endif //__CLOCK_HEADER
