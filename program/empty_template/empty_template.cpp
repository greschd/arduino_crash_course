// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    15.10.2013 01:07:01 CEST
// File:    empty template.cpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#include <Arduino.h>

class program {
public:
    program() : led_(13), delaytime_(1000) {
        setup();
    }
    void setup() {
        pinMode(led_, OUTPUT);
    }
        
    void loop() {
        digitalWrite(led_, HIGH);
        delay(delaytime_);
        digitalWrite(led_, LOW);
        delay(delaytime_);
        //~ if(digitalRead(btn_) == HIGH)
            //~ digitalWrite(led_, HIGH);
        //~ else 
            //~ digitalWrite(led_, LOW);
    }
private:
    const int led_;
    int delaytime_;
};

#include <main.hpp>
