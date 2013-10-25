// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    16.09.2013 00:38:25 CEST
// File:    usb_hid.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __USB_HID_HEADER
#define __USB_HID_HEADER

#include <Arduino.h>

#include <UsbKeyboardMod.h>
#include <device/hid_keys.hpp>

namespace device {
    class usb_hid_class {
    public:
        //------------------- ctors -------------------
        usb_hid_class() {
            data_[0] = 0;
            data_[1] = 0;
            data_[2] = 0;
            data_[3] = 0;
        }
        //------------------- connect -------------------
        void connect() {
            TIMSK0 &= !(1<<TOIE0);
            cli();
            usbDeviceDisconnect();
            delay_ms(250);
            usbDeviceConnect();
            sei();
            
            for(int i = 0; i < 150; ++i) {
                UsbKeyboard.update();
                delay_ms(10);
            }
        }
        //------------------- helper -------------------
        void delay_ms(unsigned int const & ms) {
            for(unsigned int i = 0; i < ms; i++) {
                delayMicroseconds(1000);
            }
        }
        //=================== press/release helper ===================
        void set_key(uint8_t const & _key) {
            if(data_[1] == 0) {
                data_[1] = data_[2];
                data_[2] = data_[3];
                data_[3] = _key;
                return;
            }
            if(data_[2] == 0) {
                data_[2] = data_[3];
                data_[3] = _key;
                return;
            }
            if(data_[3] == 0) {
                data_[3] = _key;
                return;
            }
        }
        void unset_key(uint8_t const & _key) {
            if(data_[1] == _key) {
                data_[1] = 0;
                return;
            }
            if(data_[2] == _key) {
                data_[2] = 0;
                return;
            }
            if(data_[3] == _key) {
                data_[3] = 0;
                return;
            }
        }
        void set_mod(uint8_t const & _mod) {
            if(_mod == 0)
                return;
            if((_mod & 1) == 1) {
                data_[0] |= _mod;
                ++(mod_vec_[0]);
            }
            if((_mod & 2) == 2) {
                data_[0] |= _mod;
                ++(mod_vec_[1]);
            }
            if((_mod & 4) == 4) {
                data_[0] |= _mod;
                ++(mod_vec_[2]);
            }
            if((_mod & 8) == 8) {
                data_[0] |= _mod;
                ++(mod_vec_[3]);
            }
        }
        void unset_mod(uint8_t const & _mod) {
            if(_mod == 0)
                return;
            
            if((_mod & 1) == 1) {
                if(--(mod_vec_[0]) == 0)
                    data_[0] &= ~_mod;
            }
            if((_mod & 2) == 2) {
                if(--(mod_vec_[1]) == 0)
                    data_[0] &= ~_mod;
            }
            if((_mod & 4) == 4) {
                if(--(mod_vec_[2]) == 0)
                    data_[0] &= ~_mod;
            }
            if((_mod & 8) == 8) {
                if(--(mod_vec_[3]) == 0)
                    data_[0] &= ~_mod;
            }
        }
        //=================== press and release ===================
        void press(uint8_t const & _key, uint8_t const & _mod = (uint8_t)key::none) {
            set_key(_key);
            set_mod(_mod);
            //~ UsbKeyboard.sendBuffer(data_);
        }
        void release(uint8_t const & _key, uint8_t const & _mod = (uint8_t)key::none) {
            unset_key(_key);
            unset_mod(_mod);
            //~ UsbKeyboard.sendBuffer(data_);
        }
        void press(key const & _key, key const & _mod = key::none) {
            press((uint8_t)_key, (uint8_t)_mod);
        }
        void release(key const & _key, key const & _mod = key::none) {
            release((uint8_t)_key, (uint8_t)_mod);
        }
        void release_all() {
            data_[0] = 0;
            data_[1] = 0;
            data_[2] = 0;
            data_[3] = 0;
            mod_vec_.clear();
            
            //~ UsbKeyboard.sendBuffer(data_);
        }
        bool any_pressed() {
            return (data_[0] + data_[1] + data_[2] + data_[3]);
        }
        //------------------- update -------------------
        void update() {
            UsbKeyboard.sendBuffer(data_);
            UsbKeyboard.update();
        }
    private:
        uint8_t data_[BUFFER_SIZE]; //first is mod, last three keys
        ustd::array<uint8_t, 4> mod_vec_;
    } ;
}//end namespace device

#endif //__USB_HID_HEADER
