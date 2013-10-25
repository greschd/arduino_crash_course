// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    21.09.2013 12:48:23 CEST
// File:    MCP4561.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __MCP4561_HEADER
#define __MCP4561_HEADER

#include "../com/i2c.hpp"

/*

MCP Pins     Arduino Pins
1 adr mod    GND, VCC or nothing (then it's the same as VCC)
2 SCL        A5
3 SDA        A4
4 GND        GND
5 Poti A     GND or other GND <= X <= VCC
6 Wimper 0   
7 Poti B     VCC or other GND <= X <= VCC
8 VCC        VCC

*/

namespace device {
    class MCP4561_class {
        enum{write = 0, incr = 1, decr = 2, read = 3};
        enum{ volatile_wimper_0 = 0x00
            , volatile_wimper_1 = 0x01
            , eeprom_wimper_0 = 0x02
            , eeprom_wimper_1 = 0x03
            , tcon_register = 0x04
            , status_register = 0x05
            };
    public:
    //------------------- ctors -------------------
        MCP4561_class(uint8_t adr = 46): dev_adr_(adr) { // ard = 46 + adr_mod
        }
        
        uint8_t cmd(uint8_t const & reg, uint8_t const & _cmd) const {
            return (reg << 4) | (_cmd << 2);
        }
        
        operator uint8_t() const {
            uint16_t res = 0;
            com::i2cout(dev_adr_) << cmd(volatile_wimper_0, read) << ustd::endl(false);
            com::i2cin(dev_adr_) >> res;
            return (res >> 8);
        }
        MCP4561_class & operator=(uint8_t const & val) {
            com::i2cout(dev_adr_) << cmd(volatile_wimper_0, write) << val << ustd::endl;
            return (*this);
        }
        MCP4561_class & operator++() {
            if((*this) < 255)
                com::i2cout(dev_adr_) << cmd(volatile_wimper_0, incr) << ustd::endl;
            return (*this);
        }
        MCP4561_class & operator--() {
            com::i2cout(dev_adr_) << cmd(volatile_wimper_0, decr) << ustd::endl;
            return (*this);
        }
        void save() {
            uint8_t val = uint8_t((*this)); //read volatile register
            com::i2cout(dev_adr_) << cmd(eeprom_wimper_0, write) << val << ustd::endl; //and write it to eeprom
        }
    private:
        uint8_t dev_adr_;
    };
    
}//end namespace device

#endif //__MCP4561_HEADER
