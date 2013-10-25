// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    24.10.2013 22:09:19 CEST
// File:    secure_laser.cpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#define COLOR_ON

#include <Arduino.h>
#include <notes.hpp>
#include <ustd.hpp>
#include <diag.hpp>
#include <tool.hpp>

// Attach the piezo speaker to pin 13 and GND and the photoresistor to GND and A0
// You also need a 10k resistor form VCC to A0 for the voltage devider
// (The arduino cannot measure resistance, only voltages)
// The laser has to be at pin 2 and GND

int const crit_val = 60;

class program {
public:
    program(): speaker_(13) {
        setup();
    }
    void setup() {
        ustd::cout.init();
        
        randomSeed(analogRead(A0)); //analogRead is a very good seed source
        
        laser_ = HIGH;
        delayMicroseconds(300); // saturate the photoresistor
        state_ = (analogRead(A0) > crit_val);
        old_state_ = state_;
        active_ = false;
        i = 0;
        
        ustd::cout << "Press some button to activate alarm" << ustd::endl;
    }
    void update() {
        tool::clock.update();
        bool on = random(2);
        laser_ = on; // switch laser random
        delayMicroseconds(300); // saturate the photoresistor
        if(on)
            state_ = (analogRead(A0) > crit_val);
        else
            state_ = (analogRead(A0) < crit_val);
    }
    int i;
    void loop() {
        update();
        
        if(ustd::cin) {
            uint8_t code = ustd::cin.read();
            if(code == 'r') {
                active_ = false;
                ustd::cout << GREENB << "DEACTIVATED" << NONE << ustd::endl;
            }
            else
                if(!active_) {
                    active_ = true;
                    ustd::cout << REDB << "ACTIVATED" << NONE << ustd::endl;
                }
                else {
                    state_ = !old_state_; // triggers alarm, b.c. false button
                }
        }
        if(state_ != old_state_) {
            old_state_ = state_;
            
            if(active_) {
                if(state_ == 1) {
                    ++i;
                    ustd::cout << YELLOW << "TRIGGER " << YELLOWB << i << NONE << ustd::endl;
                    tone(13, 2000);
                    delay(20); //for a real alarm this would be infinity
                }
                else {
                    noTone(13);
                }
            }
        }
        //~ diag::speed_report();
    }
private:
    uint8_t speaker_;
    bool state_;
    bool old_state_;
    bool active_;
    tool::analog_class<A1> poti_;
    tool::out_pin_class<2> laser_;
};

#include <main.hpp>
