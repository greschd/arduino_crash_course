// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    25.04.2013 18:09:04 EDT
// File:    color.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __COLOR_HEADER
#define __COLOR_HEADER

#include <avr/pgmspace.h>

namespace ustd {
    namespace detail {
        struct dummy_struct {
        } do_nothing;
    }//end namespace detail
}//end namespace ustd

#ifdef COLOR_ON

#define CLRSCR F("\033[2J\033[100A")

#define BLACK     F("\033[0;30m")
#define BLACKB    F("\033[1;30m")
#define RED       F("\033[0;31m")
#define REDB      F("\033[1;31m")
#define GREEN     F("\033[0;32m")
#define GREENB    F("\033[1;32m")
#define YELLOW    F("\033[0;33m")
#define YELLOWB   F("\033[1;33m")
#define BLUE      F("\033[0;34m")
#define BLUEB     F("\033[1;34m")
#define MAGENTA   F("\033[0;35m")
#define MAGENTAB  F("\033[1;35m")
#define CYAN      F("\033[0;36m")
#define CYANB     F("\033[1;36m")
#define WHITE     F("\033[0;37m")
#define WHITEB    F("\033[1;37m")

#define BLACKBG   F("\033[0;40m")
#define REDBG     F("\033[0;41m")
#define GREENBG   F("\033[0;42m")
#define YELLOWBG  F("\033[0;43m")
#define BLUEBG    F("\033[0;44m")
#define MAGENTABG F("\033[0;45m")
#define CYANBG    F("\033[0;46m")
#define WHITEBG   F("\033[0;47m")

#define NONE      F("\033[0m")

#else //NO COLOR

#define CLRSCR    ustd::detail::do_nothing

#define BLACK     ustd::detail::do_nothing
#define BLACKB    ustd::detail::do_nothing
#define RED       ustd::detail::do_nothing
#define REDB      ustd::detail::do_nothing
#define GREEN     ustd::detail::do_nothing
#define GREENB    ustd::detail::do_nothing
#define YELLOW    ustd::detail::do_nothing
#define YELLOWB   ustd::detail::do_nothing
#define BLUE      ustd::detail::do_nothing
#define BLUEB     ustd::detail::do_nothing
#define MAGENTA   ustd::detail::do_nothing
#define MAGENTAB  ustd::detail::do_nothing
#define CYAN      ustd::detail::do_nothing
#define CYANB     ustd::detail::do_nothing
#define WHITE     ustd::detail::do_nothing
#define WHITEB    ustd::detail::do_nothing

#define BLACKBG   ustd::detail::do_nothing
#define REDBG     ustd::detail::do_nothing
#define GREENBG   ustd::detail::do_nothing
#define YELLOWBG  ustd::detail::do_nothing
#define BLUEBG    ustd::detail::do_nothing
#define MAGENTABG ustd::detail::do_nothing
#define CYANBG    ustd::detail::do_nothing
#define WHITEBG   ustd::detail::do_nothing

#define NONE      ustd::detail::do_nothing

#endif //NO_COLOR

#endif //__COLOR_HEADER
