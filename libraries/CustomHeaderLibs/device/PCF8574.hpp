// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    19.08.2013 22:29:52 CEST
// File:    PCF8574.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __PCF8574_HEADER
#define __PCF8574_HEADER

#include "../com/i2c.hpp"


namespace device {
    class PCF8574_class {
    public:
        PCF8574_class(): dev_ard_(0x20) { //the addres is 0x50 + (A2 << 2 | A1 << 1 | A0)
        }
        void write(uint8_t const & data) { //if a write happens put input bits high
            com::i2cout(dev_ard_) << data << ustd::endl;
        }
        //use low to drive high (25mA per port) currents. Continuouse high works with 300uA per port...
        uint8_t read() const {
            uint8_t res;
            com::i2cin(dev_ard_) >> res;
            return res;
        }
    private:
        uint8_t dev_ard_;
    };
}//end namespace device
#endif //__PCF8574_HEADER
