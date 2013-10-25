// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    24.10.2013 18:39:03 CEST
// File:    tone.cpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#include <Arduino.h>

// Attach an led to pin 11 (not 13 since it doesn't support PWM) and GND and the potentiometer to GND/VCC and A0

class program {
public:
    program(): led_(11), poti_(A0) {
        setup();
    }
    void setup() {
        pinMode(led_, OUTPUT);
        //no pinmode for analog input
    }
    void loop() {
        int value = analogRead(poti_) >> 2; //we need to transform the 10bit to an 8bit
        analogWrite(led_, value); //analog read only accepts bytes (0-255)
        //KEEP IN MIND: PWM isn't really analog!!!
    }
private:
    uint8_t const led_;
    uint8_t const poti_;
};

#include <main.hpp>
