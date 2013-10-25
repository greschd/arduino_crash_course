// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    23.05.2013 20:57:32 EDT
// File:    string.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __STRING_HEADER
#define __STRING_HEADER

#include <string.h>

namespace ustd {
    struct string {
        string(): ptr(NULL) {
        }
        string(char const * arg): ptr(arg) {
        }
        string & operator=(char * arg) {
            ptr = arg;
            return *this;
        }
        operator char const * () const {
            return ptr;
        }
        char const * ptr;
    };
    
    
    inline bool operator==(string const a, string const b) {
        return (strcmp(a, b) == 0);
    }
}
#endif //__STRING_HEADER
