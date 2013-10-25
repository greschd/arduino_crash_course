// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    29.08.2013 18:01:29 CEST
// File:    depreller.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __DEPRELLER_HEADER
#define __DEPRELLER_HEADER

#include "button.hpp"

namespace tool {
    class depreller_class {
        typedef uint8_t state_type;
    public:
        //------------------- ctors -------------------
        depreller_class(): start_(0), old_read_(0), new_read_(0) {
        }
        //------------------- update -------------------
        void update(state_type const & read) {
            
            if(read != new_read_) {
                if(start_ == 0)
                    start_ = tool::clock.micros();
                else
                    if(tool::clock.micros() - start_ > 2000u) {
                        old_read_ = new_read_;
                        new_read_ = read;
                    }
            }
            else
                start_ = 0;
        }
        //------------------- getter -------------------
        state_type state() const {
            return new_read_;
        }
        state_type old_state() const {
            return old_read_;
        }
    private:
        double start_;
        state_type old_read_;
        state_type new_read_;
    };
}//end namespace tool

#endif //__DEPRELLER_HEADER
