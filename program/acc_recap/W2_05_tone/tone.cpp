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

// Attach the piezo speaker to pin 13 and GND, then a button to pin 2 and GND. 
// You don't need a pulldown resistor, b.c. the chip has internal pullups

class program {
public:
    program(): btn_(2), speaker_(13), freq_(note::c7) {
        setup();
    }
    void setup() {
        pinMode(btn_, INPUT_PULLUP);
        //no need for pinmode for speaker_. tone manages this
        
        int duration = 500; //ms
        
        tone(speaker_, freq_, duration); // pin, frequency, duration (optional, if empty == forever)
        delay(duration); // the tone fct doesn't burn time, it runs seperatly so you need to delay here
        
        delay(500);
        
        sing_a_song();
    }
    void loop() {
        
        if(digitalRead(btn_) == LOW)
            tone(speaker_, freq_); //with two arguments -> forever
        else
            noTone(speaker_); //just the pin
    }
    void sing_a_song() {
        int speed = 200; //ms
        
        #define TONE(X, Y) tone(speaker_, X, speed * Y); delay(speed * Y);
        
        TONE(note::c7, 1)
        TONE(note::d7, 1)
        TONE(note::e7, 1)
        TONE(note::f7, 1)
        TONE(note::g7, 2)
        TONE(note::g7, 2)
        TONE(note::a7, 1)
        TONE(note::a7, 1)
        TONE(note::a7, 1)
        TONE(note::a7, 1)
        TONE(note::g7, 4)
        TONE(note::a7, 1)
        TONE(note::a7, 1)
        TONE(note::a7, 1)
        TONE(note::a7, 1)
        TONE(note::g7, 4)
        TONE(note::f7, 1)
        TONE(note::f7, 1)
        TONE(note::f7, 1)
        TONE(note::f7, 1)
        TONE(note::e7, 2)
        TONE(note::e7, 2)
        TONE(note::d7, 1)
        TONE(note::d7, 1)
        TONE(note::d7, 1)
        TONE(note::d7, 1)
        TONE(note::c7, 2)
        
        #undef TONE
    }
private:
    uint8_t const btn_;
    uint8_t const speaker_;
    uint16_t freq_;
};

#include <main.hpp>
