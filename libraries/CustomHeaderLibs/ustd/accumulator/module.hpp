// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    20.07.2013 12:09:08 EDT
// File:    module.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __MODULE_HEADER
#define __MODULE_HEADER

#include "tags.hpp"
#include "../../util/meta_list.hpp"

namespace ustd {
    namespace detail {
        template<typename T>
        struct requirement {
            typedef util::list_end type;
        };
    }//end namespace detail
    
    template<typename T>
    struct accumulator_base {
        void operator<<(T const & in) {
        }
        void clear() {
        }
    };
    template<typename T, typename mod, typename _base>
    class module {
    };
}//end namespace ustd

#endif //__MODULE_HEADER
