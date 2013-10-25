// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    12.06.2013 19:36:38 EDT
// File:    reset.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __RESET_HEADER
#define __RESET_HEADER

namespace util {
    void reset() {
        asm volatile ("  jmp 0");
    }
}//end namespace util

#endif //__RESET_HEADER
