// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    20.05.2013 11:44:18 EDT
// File:    aassert.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __ARD_ASSERT_HEADER
#define __ARD_ASSERT_HEADER

#ifdef DEBUG_ON
    #ifdef STD_SUPPORT
        #include <iostream>
        #include <assert.h>
        #define DEBUG_VAR(x) std::cout << REDB << F("DEBUG_VAR: ") << RED  << F(#x) << F(" = ") << x << NONE << std::endl;
        #define DEBUG_MSG(x) std::cout << REDB << F("DEBUG_MSG: ") << RED  << F(x) << NONE << std::endl;
        #define ASSERT(exp) if(!(exp)) {std::cout << REDB << F("ASSERT failed on line ") << __LINE__ << RED << F("  ") << F(#exp) << NONE << std::endl; assert(exp);}
        #define ASSERT_MSG(exp, msg) if(!(exp)) {std::cout << REDB << F("ASSERT_MSG failed on line ") << __LINE__ << RED << F("  ") << F(#exp) << REDB << F("  ") << F(msg) << NONE << std::endl; assert(exp);}
    #else
        #include "iostream.hpp"
        #include <avr/pgmspace.h>
        #define DEBUG_VAR(x) ustd::cout << REDB << F("DEBUG_VAR: ") << RED  << F(#x) << F(" = ") << x << NONE << ustd::endl;
        #define DEBUG_MSG(x) ustd::cout << REDB << F("DEBUG_MSG: ") << RED  << F(x) << NONE << ustd::endl;
        #define ASSERT(exp) if(!(exp)) {ustd::cout << REDB << F("ASSERT failed on line ") << __LINE__ << RED << F("  ") << F(#exp) << NONE << ustd::endl; while(true);}
        #define ASSERT_MSG(exp, msg) if(!(exp)) {ustd::cout << REDB << F("ASSERT_MSG failed on line ") << __LINE__ << RED << F("  ") << F(#exp) << REDB << F("  ") << F(msg) << NONE << ustd::endl; while(true);}
    #endif
#else
    #define DEBUG_VAR(x) ;
    #define DEBUG_MSG(x) ;
    #define ASSERT(exp) ;
    #define ASSERT_MSG(exp, msg) ;
#endif

#endif //__ARD_ASSERT_HEADER
