// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    15.10.2013 01:07:01 CEST
// File:    empty template.cpp

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
