// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    09.08.2013 13:13:05 CEST
// File:    bluesmirf_hid.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __BLUESMIRF_HID_HEADER
#define __BLUESMIRF_HID_HEADER

#include "hid_keys.hpp"

#include <ustd/array.hpp>

#include <SoftwareSerial.h>

namespace device {
    
    class bluesmirf_hid_class {
    public:
        //------------------- ctors -------------------
        bluesmirf_hid_class(uint8_t const & rx, uint8_t const & tx): serial_(rx, tx) {
            
            serial_.begin(57600);
            
            data_[0] = 0xFE;
            data_[1] = 0x04;
            data_[2] = 0x00;
            data_[3] = 0x00;
            data_[4] = 0x00;
            data_[5] = 0x00;
        }
        //------------------- cmd -------------------
        void init() {
            
        }
        void cmd_mode() {
            serial_.print(F("$$$"));
            serial_.flush();
            delay(200);
        }
        
        void reset() {
            //~ ustd::cout << F("R,1") << ustd::endl;
            serial_.println(F("R,1"));
            delay(300);
        }

        void connect() {
            reset();
            cmd_mode();
            //~ ustd::cout << F("C") << ustd::endl;
            serial_.println(F("C"));
            delay(200);
        }

        void break_connection() {
            ustd::cout << char(0) << ustd::endl;
            delay(400);
        }

        void change_ssp() {
            break_connection();
            reset();
            cmd_mode();
            ustd::cout << F("S~,0") << ustd::endl;
            delay(100);
            ustd::cout << F("SE,S") << ustd::endl;
            delay(200);
        }

        void change_hid() {
            break_connection();
            reset();
            cmd_mode();
            ustd::cout << F("S~,6") << ustd::endl;
            delay(100);
            
            ustd::cout << F("SE,C") << ustd::endl;
            delay(200);
            connect(); //only connect when HID
        }
        //=================== press/release helper ===================
        void set_key(uint8_t const & _key) {
            if(data_[3] == 0) {
                data_[3] = data_[4];
                data_[4] = data_[5];
                data_[5] = _key;
                return;
            }
            if(data_[4] == 0) {
                data_[4] = data_[5];
                data_[5] = _key;
                return;
            }
            if(data_[5] == 0) {
                data_[5] = _key;
                return;
            }
        }
        void unset_key(uint8_t const & _key) {
            if(data_[3] == _key) {
                data_[3] = 0;
                return;
            }
            if(data_[4] == _key) {
                data_[4] = 0;
                return;
            }
            if(data_[5] == _key) {
                data_[5] = 0;
                return;
            }
        }
        void set_mod(uint8_t const & _mod) {
            if(_mod == 0)
                return;
            if((_mod & 1) == 1) {
                data_[2] |= _mod;
                ++(mod_vec_[0]);
            }
            if((_mod & 2) == 2) {
                data_[2] |= _mod;
                ++(mod_vec_[1]);
            }
            if((_mod & 4) == 4) {
                data_[2] |= _mod;
                ++(mod_vec_[2]);
            }
            if((_mod & 8) == 8) {
                data_[2] |= _mod;
                ++(mod_vec_[3]);
            }
        }
        void unset_mod(uint8_t const & _mod) {
            if(_mod == 0)
                return;
            
            if((_mod & 1) == 1) {
                if(--(mod_vec_[0]) == 0)
                    data_[2] &= ~_mod;
            }
            if((_mod & 2) == 2) {
                if(--(mod_vec_[1]) == 0)
                    data_[2] &= ~_mod;
            }
            if((_mod & 4) == 4) {
                if(--(mod_vec_[2]) == 0)
                    data_[2] &= ~_mod;
            }
            if((_mod & 8) == 8) {
                if(--(mod_vec_[3]) == 0)
                    data_[2] &= ~_mod;
            }
            
        }
        //=================== press and release ===================
        void press(key const & _key, key const & _mod = key::none) {
            set_key((uint8_t)_key);
            set_mod((uint8_t)_mod);
            serial_.write(data_, 6);
        }
        void release(key const & _key, key const & _mod = key::none) {
            unset_key((uint8_t)_key);
            unset_mod((uint8_t)_mod);
            serial_.write(data_, 6);
        }
        void press(uint8_t const & _key, uint8_t const & _mod = (uint8_t)key::none) {
            set_key(_key);
            set_mod(_mod);
            serial_.write(data_, 6);
        }
        void release(uint8_t const & _key, uint8_t const & _mod = (uint8_t)key::none) {
            unset_key(_key);
            unset_mod(_mod);
            serial_.write(data_, 6);
        }
        void release_all() {
            data_[2] = 0;
            data_[3] = 0;
            data_[4] = 0;
            data_[5] = 0;
            serial_.write(data_, 6);
        }
    private:
        
        SoftwareSerial serial_;
        
        uint8_t data_[6]; //first two byte are reserved, third is the mod and the last three the keys
        ustd::array<uint8_t, 4> mod_vec_;
    };
    
    
}//end namespace device

#endif //__BLUESMIRF_HID_HEADER
