// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    24.10.2013 18:39:07 CEST
// File:    analog_read.cpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#include <Arduino.h>

// Just add a potentiometer to VCC/GND and to A0

class program {
public:
    program() {
        setup();
    }
    void setup() {
        Serial.begin(460800);
        Serial.println("Analog Read");
        
        //no need for pinmode since analog pins are read only
    }
    void loop() {
        Serial.println(analogRead(A0)); //just use analogRead(pin) on an analog pin
        Serial.flush(); //redundant b.c. println should do a flush...
    }
private:
    
};

#include <main.hpp>
