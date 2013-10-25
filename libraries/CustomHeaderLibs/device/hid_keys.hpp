// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    09.08.2013 10:14:01 CEST
// File:    hid_keys.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __HID_KEYS_HEADER
#define __HID_KEYS_HEADER

enum class key : uint8_t {
      none = 0
    , ctrl = 1
    , shift = 2
    , alt = 4
    , gui = 8
    , ctrl_right = 16
    , shift_right = 32
    , alt_right = 64
    , gui_right = 128
    
    , a = 4
    , b = 5
    , c = 6
    , d = 7
    , e = 8
    , f = 9
    , g = 10
    , h = 11
    , i = 12
    , j = 13
    , k = 14
    , l = 15
    , m = 16
    , n = 17
    , o = 18
    , p = 19
    , q = 20
    , r = 21
    , s = 22
    , t = 23
    , u = 24
    , v = 25
    , w = 26
    , x = 27
    , y = 28
    , z = 29
    
    , k1 = 30
    , k2 = 31
    , k3 = 32
    , k4 = 33
    , k5 = 34
    , k6 = 35
    , k7 = 36
    , k8 = 37
    , k9 = 38
    , k0 = 39
    
    , enter = 40
    , escape = 41
    , backspace = 42
    , tab = 43
    , space = 44
    , minus = 45
    , circumflex = 46
    , ue = 47
    , diaresis = 48
    , dollar = 49

    , oe = 51
    , ae = 52
    , section = 53
    , comma = 54
    , dot = 55

    , caps_lock = 57
    , F1 = 58
    , F2 = 59
    , F3 = 60
    , F4 = 61
    , F5 = 62
    , F6 = 63
    , F7 = 64
    , F8 = 65
    , F9 = 66
    , F10 = 67
    , F11 = 68
    , F12 = 69
    , prt_scr = 70
    , scr_lock = 71
    , pause = 72
    , insert = 73
    , home = 74
    , page_up = 75
    , del = 76
    , end = 77
    , page_down = 78
    , right = 79
    , left = 80
    , down = 81
    , up = 82
    , num_lock = 83
    
    , pad_div = 84
    , pad_mult = 85
    , pad_minus = 86
    , pad_plus = 87
    , pad_enter = 88
    , pad_1 = 89
    , pad_2 = 90
    , pad_3 = 91
    , pad_4 = 92
    , pad_5 = 93
    , pad_6 = 94
    , pad_7 = 95
    , pad_8 = 96
    , pad_9 = 97
    , pad_0 = 98
    , pad_dot = 99
    , angle = 100

    , power = 102
    , pad_equal = 103
    
    , F13 = 104
    , F14 = 105
    , F15 = 106
    , F16 = 107
    , F17 = 108
    , F18 = 109
    , F19 = 110
    , F20 = 111
    , F21 = 112
    , F22 = 113
    , F23 = 114
    , F24 = 115

    , mute = 127
    , volume_up = 128
    , volume_down = 129
};

#endif //__HID_KEYS_HEADER
