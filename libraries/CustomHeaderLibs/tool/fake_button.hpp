// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    09.08.2013 19:23:18 CEST
// File:    fake_button.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __FAKE_BUTTON_HEADER
#define __FAKE_BUTTON_HEADER

#include "button.hpp"

namespace tool {
    class fake_button_class {
        typedef uint8_t state_type;
    public:
        //------------------- ctors -------------------
        fake_button_class(): start_(0), state_(state::released), old_read_(false) {
        }
        operator bool() const {
            return state_ == state::pressed;
        }
        //------------------- update -------------------
        void update(bool const & read) {
            
            if(state_ & (state::falling | state::auto_falling)) {
                state_ = state::pressed;
                return;
            }
            if(state_ & state::raising) {
                state_ = state::released;
                return;
            }
            if(state_ & state::pressed) {
                if((tool::clock.micros() - start_press_) > state::auto_repeat * 1000.0) {
                    start_press_ = tool::clock.micros();
                    state_ = (state::auto_falling + state::pressed);
                }
            }
            
            if(read != old_read_) {
                if(start_ == 0)
                    start_ = tool::clock.micros();
                else
                    if(tool::clock.micros() - start_ > 2000u) {
                        state_ <<= 2; //brings pressed -> raising and released -> falling
                        start_press_ = tool::clock.micros() + state::auto_delay * 1000.0;
                        old_read_ = read;
                    }
            }
            else
                start_ = 0;
        }
        //------------------- getter -------------------
        state_type state() const {
            return state_;
        }
        //------------------- compare operator -------------------
        bool operator==(state::button_enum const & arg) {
            return arg & state_;
        }
    private:
        double start_;
        double start_press_;
        state_type state_;
        bool old_read_;
    };
}//end namespace tool

#endif //__FAKE_BUTTON_HEADER
