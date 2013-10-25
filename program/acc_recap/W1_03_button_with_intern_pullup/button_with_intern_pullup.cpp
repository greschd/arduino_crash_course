// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    16.10.2013 10:31:27 CEST
// File:    button_with_pulldown.cpp

#include <Arduino.h>

// This program changes an led on pin 13 to whatever state a button on pin 2 is in.
// Attach:
// LED as before
// Button with no pulldown/up resistor to pin 2

class program {
public:
    program(): led_(13), btn_(2) {
        setup();
    }
    void setup() {
        pinMode(led_, OUTPUT);
        pinMode(btn_, INPUT_PULLUP); // define led_ as INPUT with a pullup resistor activated. This means that the btn_ is HIGH if not pressed, and LOW if pressed. You find mostly pullup resistors in ICs...
    }
    void loop() {
        digitalWrite(led_, !digitalRead(btn_)); // set led_ to whatever state btn_ is in not. The ! is needed since the LOW is the new "HIGH"
    }
private:
    int const led_;
    int const btn_;
    
};

#include <main.hpp>
