// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    16.10.2013 10:10:56 CEST
// File:    light_led.cpp

#include <Arduino.h>

// This program just truns on the pin 13
// Attach:
// Just put an led on the arduino (long leg -> 13, short leg -> GND)

class program {
public:
    program(): led_(13) { // initialisation of const members works only this way
        setup();
    }
    void setup() {
        pinMode(led_, OUTPUT); // define led_ as output. Default is INPUT
        digitalWrite(led_, HIGH); // HIGH is the same as true or 1, LOW is the opposite
    }
    void loop() {
        //do nothing
    }
private:
    int const led_;
    
};

#include <main.hpp>
