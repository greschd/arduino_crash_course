// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    23.07.2013 05:10:03 EDT
// File:    tags.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __FILTER_TAGS_HEADER
#define __FILTER_HEADER

namespace ustd {
    namespace tag {
        struct identity {};
        template<unsigned N>
        struct lowpass {};
        template<unsigned PROMIL>
        struct highpass {};
        template<unsigned W>
        struct hysteresis {};
        
    }//end namespace tag
}//end namespace ustd
#endif //__FILTER_HEADER
