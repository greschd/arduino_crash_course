// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    30.03.2013 12:55:28 CET
// File:    fast_io.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __FAST_IO_HEADER
#define __FAST_IO_HEADER

#include <Arduino.h>

namespace detail {
    inline void old_digital_write(int const p, bool const state) {
        digitalWrite(p, state); // calls the arduino function for pins bigger than 13
    }

    inline bool old_digital_read(int const p) {
        return digitalRead(p); // calls the arduino function for pins bigger than 13
    }
    template<int p>
    struct dw {
        inline static void write(bool const state) {
            if(state) {
                if(p >= 0 and p <= 7)
                    PORTD |= (1<<p);
                else if(p >= 8 and p <= 13)
                    PORTB |= (1<<(p-8*int(p/8))); //*int(p/8) is only there to get rid of the error "negative shift"
                else
                    old_digital_write(p, HIGH);
            }
            else {
                if(p >= 0 and p <= 7)
                    PORTD &= ~(1<<p);
                else if(p >= 8 and p <= 13)
                    PORTB &= ~(1<<(p-8*int(p/8))); //see comment above
                else
                    old_digital_write(p, LOW);
            }
        }
    };

    template<int p>
    struct dr {
        inline static bool read() {
            if(p >= 0 and p <= 7)
                return (PIND >> p) & 1;
            else if(p >= 8 and p <= 13)
                return (PINB >> (p-8*int(p/8))) & 1; //*int(p/8) is only there to get rid of the error "negative shift"
            else
                return old_digital_read(p);
        }
    };
}//end namespace detail

#define digitalWrite(x, y) detail::dw<x>::write(y)
#define digitalRead(x) detail::dr<x>::read()

#endif //__FAST_IO_HEADER
