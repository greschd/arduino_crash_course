// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    24.10.2013 18:39:03 CEST
// File:    tone.cpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#include <Arduino.h>
#include <notes.hpp>

// Attach the piezo speaker to pin 13 and GND and the photoresistor to GND and A0
// You also need a 10k resistor form VCC to A0 for the voltage devider
// (The arduino cannot measure resistance, only voltages)

class program {
public:
    program(): speaker_(13), sensor_(A0) {
        setup();
    }
    void setup() {
        Serial.begin(460800);
        Serial.println("Starting Fort Nox");
    }
    void loop() {
        //~ Serial.println(analogRead(sensor_)); delay(1); //for debugging
        if(analogRead(sensor_) > 500) {
            tone(speaker_, 2000);
            delay(5); //in order to hear short tones...
        }
        else
            noTone(speaker_);
    }
private:
    uint8_t const speaker_;
    uint8_t const sensor_;
};

#include <main.hpp>
