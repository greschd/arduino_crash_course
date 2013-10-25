// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    12.06.2013 21:10:52 EDT
// File:    progress_bar.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __PROGRESS_BAR_HEADER
#define __PROGRESS_BAR_HEADER

#ifdef __IOSTREAM_HEADER
    #include <ustd/iostream.hpp>
    #include <ustd/color.hpp>
#endif //__IOSTREAM_HEADER
namespace util {
    void progress_bar(double const & p) {
        #ifdef __IOSTREAM_HEADER
            uint8_t const size = 50;
            uint8_t bars = p * size;
            
            if(p < .33)
                ustd::cout << RED << F("<");
            else if(p < .66)
                ustd::cout << YELLOW << F("<");
            else if(p >= 1)
                ustd::cout << CYAN << F("<");
            else
                ustd::cout << GREEN << F("<");
            
            for(uint8_t i = 0; i < bars; ++i)
                ustd::cout << F("|");
            for(uint8_t i = bars; i < size; ++i)
                ustd::cout << F(" ");
            
            ustd::cout << F("> ") << NONE << uint8_t(p * 100) << F("%") << ustd::endl;
        #endif //__IOSTREAM_HEADER
    }
}//end namespace util
#endif //__PROGRESS_BAR_HEADER
