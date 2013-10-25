// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    25.05.2013 21:47:03 EDT
// File:    speed_report.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __SPEED_REPORT_HEADER
#define __SPEED_REPORT_HEADER

#include <ustd/iostream.hpp>
#include "../tool/clock.hpp"

namespace diag {
    class frequency_class {
        const int lower_time_limit = 1;
        const int upper_time_limit = 3;
    public:
        frequency_class(): last_print_(0), mod_(2), i_(0), n_(0) {
        }
        inline void speed_report() {
            ++i_;
            if((i_&(mod_-1)) == 0) {
                n_ += mod_;
                double e = tool::clock.millis();
                if(e - last_print_ > lower_time_limit * 1000) {
                    if(e - last_print_ > upper_time_limit * 1000) {
                        mod_ >>= 1;
                    }
                        
                    auto fl = mod_ / (e - last_print_);
                    last_print_ = e;
                    auto f = n_ / e;
                    
                    ustd::cout << ustd::endl;
                    ustd::cout << F("----------------------") << ustd::endl;
                    ustd::cout << F("     Speed Report     ") << ustd::endl;
                    ustd::cout << F("----------------------") << ustd::endl;
                    if(f < 1)
                        ustd::cout << GREEN << F(" mean freq: ") << GREENB << f * 1000 << F(" Hz") << ustd::endl;
                    else
                        ustd::cout << YELLOW << F(" mean freq: ") << YELLOWB << f << F(" kHz") << ustd::endl;
                    if(fl < 1)
                        ustd::cout << GREEN << F(" loop freq: ") << GREENB << fl * 1000 << F(" Hz") << ustd::endl;
                    else
                        ustd::cout << YELLOW << F(" loop freq: ") << YELLOWB << fl << F(" kHz") << ustd::endl;
                    if(f < 1)
                        ustd::cout << GREEN << F(" loop time: ") << GREENB << 1 / f << F(" ms") << NONE << ustd::endl;
                    else
                        ustd::cout << YELLOW << F(" loop time: ") << YELLOWB << 1000 / f << F(" us") << NONE << ustd::endl;
                        
                    ustd::cout << F(" modulo   : ") << mod_ << ustd::endl;
                    ustd::cout << F("----------------------") << ustd::endl;
                
                }
                else {
                    n_ -= mod_;
                    mod_ <<= 1;
                }
            }
        }
    private:
        double last_print_;
        uint32_t mod_;
        uint32_t i_;
        double n_;
    } freq;
    
    inline void speed_report() {
        freq.speed_report();
    }
}

#endif //__SPEED_REPORT_HEADER
