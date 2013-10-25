// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    09.08.2013 19:01:49 CEST
// File:    MMA8452.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __MMA8452_HEADER
#define __MMA8452_HEADER

/*
MMA8452Q Basic Example Code
Nathan Seidle
SparkFun Electronics
November 5, 2012
License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
This example code shows how to read the X/Y/Z accelerations and basic functions of the MMA5842. It leaves out
all the neat features this IC is capable of (tap, orientation, and inerrupts) and just displays X/Y/Z. See
the advanced example code to see more features.
Hardware setup:
MMA8452 Breakout ------------ Arduino
3.3V --------------------- 3.3V
SDA -------^^(330)^^------- A4
SCL -------^^(330)^^------- A5
GND ---------------------- GND
The MMA8452 is 3.3V so we recommend using 330 or 1k resistors between a 5V Arduino and the MMA8452 breakout.
The MMA8452 has built in pull-up resistors for I2C so you do not need additional pull-ups.
*/

#include <Arduino.h>
#include <ustd.hpp>
#include <com/i2c.hpp>

namespace device {
    //Define a few of the registers that we will be accessing on the MMA8452
    uint8_t const OUT_X_MSB  = 0x01;
    #define XYZ_DATA_CFG 0x0E
    #define WHO_AM_I 0x0D
    #define CTRL_REG1 0x2A

    #ifndef MMA8452_ADDRESS
        #define MMA8452_ADDRESS 0x1D // 0x1D if SA0 is high, 0x1C if low
    #endif
    class MMA8452_class {
    public:
        //------------------- ctor/init -------------------
        void init(uint8_t const & g_scale = 2) {
            delay(100);
            g_scale_ = g_scale;
            uint8_t c = read_register(WHO_AM_I); // WHO_AM_I should always be 0x2A
            ASSERT_MSG(c == 0x2A, "Couldn't connect to MMA8452")

            standby(); // Must be in standby to change registers

            byte fsr = g_scale_;
            if(fsr > 8) 
                fsr = 8;
            fsr >>= 2; // Neat trick, see page 22. 00 = 2G, 01 = 4A, 10 = 8G
            write_register(XYZ_DATA_CFG, fsr);

            //The default data rate is 800Hz and we don't modify it in this example code

            active(); // Set to active to start reading
        }
        void standby() {
            byte c = read_register(CTRL_REG1);
            write_register(CTRL_REG1, c & ~(0x01)); //Clear the active bit to go into standby
        }
        void active() {
            byte c = read_register(CTRL_REG1);
            write_register(CTRL_REG1, c | 0x01); //Set the active bit to begin detection
        }
        //------------------- registers -------------------
        uint8_t read_register(byte address) {
            uint8_t res;
            com::i2cout(MMA8452_ADDRESS) << address << ustd::endl(false);
            com::i2cin(MMA8452_ADDRESS) >> res;
            return res;
        }

        void write_register(byte address, byte data) {
            com::i2cout(MMA8452_ADDRESS) << address  << data << ustd::endl;
        }
        //------------------- update -------------------
        void update() {
            com::i2cout(MMA8452_ADDRESS) << OUT_X_MSB << ustd::endl(false);
            com::i2cin(MMA8452_ADDRESS) >> raw_data;
            
            // Loop to calculate 12-bit ADC and g value for each axis
            for(int i = 0; i < 3 ; i++) {
                int g_count = (raw_data[i*2] << 8) | raw_data[(i*2)+1]; //Combine the two 8 bit registers into one 12-bit number
                g_count >>= 4; //The registers are left align, here we right align the 12-bit integer

                // If the number is negative, we have to make it so manually (no 12-bit data type)
                if (raw_data[i*2] > 0x7F) {
                    g_count = ~g_count + 1;
                    g_count *= -1; // Transform into negative 2's complement #
                }
                
                accel[i] = double(g_count) / ((1<<12)/(2*g_scale_));
            }
        }
        //------------------- getter -------------------
        double const & operator[](uint8_t const & idx) const {
            ASSERT(idx < 3)
            return accel[idx];
        }
        double const & x() const {
                return accel[0];
        }
        double const & y() const {
                return accel[1];
        }
        double const & z() const {
                return accel[2];
        }
    private:
        uint8_t g_scale_;
        double accel[3];
        uint8_t raw_data[6];
    };
}//end namespace device

#endif //__MMA8452_HEADER
