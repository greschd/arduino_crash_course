// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    24.10.2013 18:28:42 CEST
// File:    serial_io.cpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#include <Arduino.h>

// Nothing needs to be attached to the arduino for this to work

class program {
public:
    program() {
        setup();
    }
    void setup() {
        Serial.begin(460800);
        Serial.println("Press a key");
    }
    void loop() {
        if(Serial.available() > 0) { //something is in the buffer
            char in = Serial.read(); //get one character
            
            Serial.print("You pressed: ");
            Serial.println(in);
            delay(1); //1 ms delay to flush the serial buffer (optional Serial.flush())
        }
    }
private:
    
};

#include <main.hpp>
