// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    19.08.2013 22:23:43 CEST
// File:    CAT24C16.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __CAT24C16_HEADER
#define __CAT24C16_HEADER

#include "../com/i2c.hpp"


namespace device {
    class CAT24C16_class {
    public:
        CAT24C16_class(): dev_ard_(0x50) { //the addres is 0x50 + (A2 << 2 | A1 << 1 | A0)
        }
        void write(uint8_t const & adr, uint8_t const & data) {
            if(read(adr) != data) {
                com::i2cout(dev_ard_) << adr << data << ustd::endl;
                delay(5);
            }
        }
        uint8_t read(uint8_t const & adr) const {
            uint8_t res;
            com::i2cout(dev_ard_) << adr << ustd::endl(false);
            com::i2cin(dev_ard_) >> res;
            return res;
        }
    private:
        uint8_t dev_ard_;
    };
}//end namespace device
#endif //__CAT24C16_HEADER
