// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    18.06.2013 15:53:18 EDT
// File:    light.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __LIGHT_HEADER
#define __LIGHT_HEADER

#include "clock.hpp"

#include <Arduino.h>

namespace state {
    enum light_enum : uint8_t {
          off
        , on
        , flash
        , raise
        , pwm
        , slow
        , blink
        , fast
    };
}//end namespace state

namespace tool {
    template<uint8_t pin, bool on_state = HIGH>
    class light_class {
    public:
        //------------------- ctors -------------------
        light_class(): state_(state::off) {
            pinMode(pin, OUTPUT);
            update();
        }
        //------------------- assignment -------------------
        template<uint8_t pin2, bool on_state2>
        light_class & operator=(light_class<pin2, on_state2> const & rhs) {
            state_ = rhs.state();
            return (*this);
        }
        light_class & operator=(state::light_enum const & s) {
            state_ = s;
            return (*this);
        }
        light_class & operator=(bool const & s) {
            state_ = (s ? state::on : state::off);
            return (*this);
        }
        light_class & operator=(int const & s) {
            state_ = state::pwm;
            if(on_state)
                analogWrite(pin, s);
            else
                analogWrite(pin, 255 - s);
            return (*this);
        }
        //------------------- modes -------------------
        void on() {
            digitalWrite(pin, on_state);
        }
        void off() {
            digitalWrite(pin, !on_state);
        }
        void flash() {
            digitalWrite(pin, (tool::clock.msec() < 100) == on_state);
        }
        void pwm() {
            ASSERT_MSG(pin == 3 or pin == 5 or pin == 6 or pin == 9 or pin == 10 or pin == 11, "only PWM pins are capable of state::pwm")
        }
        void raise() {
            ASSERT_MSG(pin == 3 or pin == 5 or pin == 6 or pin == 9 or pin == 10 or pin == 11, "only PWM pins are capable of state::pwm")
            analogWrite(pin, tool::clock.msec()/6);
        }
        void fast() {
            digitalWrite(pin, (tool::clock.msec(500) < 250) == on_state);
        }
        void blink() {
            digitalWrite(pin, (tool::clock.msec(1000) < 500) == on_state);
        }
        void slow() {
            digitalWrite(pin, (tool::clock.sec() & 1) == on_state);
        }
        //------------------- update -------------------
        void update() {
            #define UPDATE(STATE) if(state_ == state::STATE) STATE(); else
            
            UPDATE(on)
            UPDATE(off)
            UPDATE(flash)
            UPDATE(pwm)
            UPDATE(raise)
            UPDATE(fast)
            UPDATE(blink)
            UPDATE(slow)
            ; //last else
            
            #undef UPDATE
        }
        //------------------- getter -------------------
        state::light_enum const & state() const {
            return state_;
        }
        //------------------- ops -------------------
        void flip() {
            if(state_ == state::on)
                state_ = state::off;
            else if(state_ == state::off)
                state_ = state::on;
        }
    private:
        state::light_enum state_;
    };
}//end namespace tool

#endif //__LIGHT_HEADER
