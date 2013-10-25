// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    18.06.2013 17:31:25 EDT
// File:    button.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __BUTTON_HEADER
#define __BUTTON_HEADER

#include "clock.hpp"

#include <Arduino.h>

namespace state {
    enum button_enum : uint8_t {
          released = 1
        , pressed = 2
        , falling = 4
        , raising = 8
        , changed = (falling | raising)
        , auto_falling = 16
    };
    uint8_t const auto_repeat = 33;
    uint16_t const auto_delay = 500;
}//end namespace state

namespace tool {
    template<uint8_t pin, bool pressed_state = HIGH, uint16_t _auto_repeat = state::auto_repeat, uint16_t _auto_delay = state::auto_delay>
    class button_class {
        typedef uint8_t state_type;
    public:
        //------------------- ctors -------------------
        button_class(): start_(0), state_(state::released), old_read_(!pressed_state) {
            if(pressed_state)
                pinMode(pin, INPUT);
            else
                pinMode(pin, INPUT_PULLUP);
        }
        operator bool() const {
            return state_ == state::pressed;
        }
        //------------------- update -------------------
        void update() {
            
            if(state_ & (state::falling | state::auto_falling)) {
                state_ = state::pressed;
                return;
            }
            if(state_ & state::raising) {
                state_ = state::released;
                return;
            }
            if(state_ & state::pressed) {
                if((tool::clock.micros() - start_press_) > _auto_repeat * 1000.0) {
                    start_press_ = tool::clock.micros();
                    state_ = (state::auto_falling + state::pressed);
                }
            }
            
            bool read = digitalRead(pin);
            if(read != old_read_) {
                if(start_ == 0)
                    start_ = tool::clock.micros();
                else
                    if(tool::clock.micros() - start_ > 2000u) {
                        state_ <<= 2; //brings pressed -> raising and released -> falling
                        start_press_ = tool::clock.micros() + _auto_delay * 1000.0;
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
        bool operator==(state_type const & arg) {
            return arg & state_;
        }
    private:
        double start_;
        double start_press_;
        state_type state_;
        bool old_read_;
    };
    
}//end namespace tool

#endif //__BUTTON_HEADER
