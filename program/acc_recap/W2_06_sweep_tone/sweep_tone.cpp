// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    24.10.2013 19:11:27 CEST
// File:    sweep_tone.cpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#include <Arduino.h>
#include <notes.hpp>

// Just add a potentiometer to VCC/GND and to A0 and a piezo speaker to pin 13 and GND

class program {
public:
    program():  speaker_(13) {
        setup();
    }
    void setup() {
        //nothing to do here
    }
    void loop() {
        int sound = map(analogRead(A0), 0, 1023, note::c4, note::c6); //fct of Arduino.h
        //map(value, from_low, from_high, to_low, to_high)
        tone(speaker_, sound);
    }
private:
    uint8_t const speaker_;
};

#include <main.hpp>
