// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    19.08.2013 20:44:12 CEST
// File:    24LC256.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __24LC256_HEADER
#define __24LC256_HEADER

#include "../com/i2c.hpp"

namespace device {
    class _24LC256_class {
    public:
        _24LC256_class(uint8_t const & dev_adr = 0x50): dev_adr_(dev_adr) { //the addres is 0x50 + (A2 << 2 | A1 << 1 | A0)
        }
        void write(uint16_t const & adr, uint8_t const & data) {
            if(read(adr) != data) {
                com::i2cout(dev_adr_) << com::little_endian(adr) << data << ustd::endl;
                delayMicroseconds(4000);
            }
        }
        uint8_t read(uint16_t const & adr) const {
            uint8_t res;
            com::i2cout(dev_adr_) << com::little_endian(adr) << ustd::endl(false);
            com::i2cin(dev_adr_) >> res;
            return res;
        }
    private:
        uint8_t dev_adr_;
    };
}//end namespace device
#endif //__24LC256_HEADER
