// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    25.05.2013 19:22:25 EDT
// File:    eeprom_report.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __EEPROM_REPORT_HEADER
#define __EEPROM_REPORT_HEADER

#include <ustd/iostream.hpp>

#include <Arduino.h>
#include <EEPROM.h>

namespace diag {
    class EEPROM_mirror_class {
        typedef uint16_t size_type;
    public:
        static size_type const eeprom_capacity = 1024;
        
        void reset(byte r = 0xFF) {
            for(size_type i = 0; i < eeprom_capacity; ++i) {
                if(EEPROM.read(i) != r)
                    EEPROM.write(i, r);
            }
        }
        void eeprom_report() {
            const uint8_t mod = 32;
            ustd::cout << ustd::endl 
                       << F("+----------------");
            for(uint8_t i = 0; i < mod; ++i) {
                ustd::cout << F("---");
            }
            ustd::cout << F("+") << ustd::endl 
                       << F("| EEPROM Report  ");
            for(uint8_t i = 0; i < mod; ++i) {
                ustd::cout << F("   ");
            }
            ustd::cout << F("|") << ustd::endl 
                       << F("+----------------");
            for(uint8_t i = 0; i < mod; ++i) {
                ustd::cout << F("---");
            }
            ustd::cout << F("+");
            
            for(size_type i = 0; i < eeprom_capacity; ++i) {
                if(i % mod == 0) {
                    if(i != 0)
                        ustd::cout << NONE << F("|");
                    
                    ustd::cout << ustd::endl
                               << F("| ");
                    if(i%(3*mod) == 0)
                        ustd::cout << GREEN;
                    ustd::cout << ustd::setw(4) << i << F(" - ") << ustd::setw(4) << i + mod-1 << F(" :  ");
                    if(i%(3*mod) == 0)
                        ustd::cout << REDB;
                }
                ustd::cout << ustd::setfill('0') << ustd::setbase(16) << ustd::setw(2) << EEPROM.read(i);
                ustd::cout(1) << F(" "); //cout() means reset all modifier
                if(i == eeprom_capacity - 1)
                    ustd::cout << NONE << F("|");
            }
            ustd::cout << ustd::endl;
            ustd::cout << F("+----------------");
            for(uint8_t i = 0; i < mod; ++i) {
                ustd::cout << F("---");
            }
            ustd::cout << F("+") << ustd::endl;
        }
        void eeprom_report_data() {
            for(size_type i = 0; i < eeprom_capacity; ++i) {
                Serial.write(EEPROM.read(i));
            }
            ustd::cout << ustd::endl;
        }
        void check(char id = 'e') {
            if(ustd::cin)
            {
                byte x = ustd::cin.peek();
                if(x == id) {
                    ustd::cin.read();
                    eeprom_report();
                }
            }
        }
        void check_byte(char id = 'e') {
            if(ustd::cin) {
                byte x = ustd::cin.peek();
                if(x == id) {
                    ustd::cin.read();
                    eeprom_report_data();
                }
            }
        }
    }mirror;
    
    void eeprom_report() {
        mirror.eeprom_report();
    }
}//end namespace diag
#endif //__EEPROM_REPORT_HEADER
