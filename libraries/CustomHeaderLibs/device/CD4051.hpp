// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    28.09.2013 13:43:34 CEST
// File:    CD4051.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __CD4051_HEADER
#define __CD4051_HEADER

//8:1 MUX

#include "../tool/out_pin.hpp"
#include "../tool/fake_button.hpp"

namespace device {
    template<uint8_t master, uint8_t d0, uint8_t d1, uint8_t d2, bool high_state = 0>
    class CD4051_class {
    public:
        CD4051_class() {
            d0_ = 0;
            d1_ = 0;
            d2_ = 0;
            
            if(high_state == 0)
                pinMode(master, INPUT_PULLUP);
            else
                pinMode(master, INPUT);
        }
        void update() {
            #define READ(x) in_[x].update(high_state == digitalRead(master));
            
            READ(0)
            d0_ = 1;
            READ(1)
            d1_ = 1;
            READ(3)
            d0_ = 0;
            READ(2)
            d2_ = 1;
            READ(6)
            d0_ = 1;
            READ(7)
            d1_ = 0;
            READ(5)
            d0_ = 0;
            READ(4)
            d2_ = 0;
            
            #undef READ
        }
        template<typename T>
        void copy(T & vector, uint8_t const & pos) {
            for(uint8_t i = 0; i < 8; ++i) {
                vector[pos + i] = in_[i].state();
            }
        }
        tool::fake_button_class const & operator[](uint8_t const & pos) const {
            return in_[pos];
        }
    private:
        tool::out_pin_class<d0> d0_;
        tool::out_pin_class<d1> d1_;
        tool::out_pin_class<d2> d2_;
        
        ustd::array<tool::fake_button_class, 8> in_;
    };
}//end namespace device

#endif //__CD4051_HEADER
