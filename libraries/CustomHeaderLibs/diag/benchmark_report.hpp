// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    25.05.2013 19:14:55 EDT
// File:    benchmark.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __BENCHMARK_HEADER
#define __BENCHMARK_HEADER

#include <ustd/iostream.hpp>

#include <Arduino.h>

namespace diag {
    template<typename T>
    double benchmark_clocks(T fct, uint16_t const & n = 100) {
        uint16_t const N = n * clockCyclesPerMicrosecond();
        //------------------- measure for-loop overhead -------------------
        auto s = micros();
        for(uint16_t i = 0; i < N; ++i)
            ;
            
        auto e = micros();
        double overhead = e - s;
        overhead *= clockCyclesPerMicrosecond() / double(N);
        
        //------------------- measure actual fct -------------------
        s = micros();
        for(uint16_t i = 0; i < N; ++i)
            fct();
            
        e = micros();
        double res = e - s;
        res *= clockCyclesPerMicrosecond() / double(N);
        res -= overhead;
        return res;
    }

    template<typename T>
    double benchmark_micros(T fct, uint16_t const & n = 100) {
        return benchmark_clocks(fct, n) / clockCyclesPerMicrosecond();
    }

    template<typename T>
    void benchmark_report(T fct, uint16_t const & n = 100) {
        ustd::cout << ustd::endl;
        ustd::cout << F("----------------------") << ustd::endl;
        ustd::cout << F("   Benchmark Report   ") << ustd::endl;
        ustd::cout << F("----------------------") << ustd::endl;
        double us = benchmark_micros(fct, n);
        if(us > 1)
            ustd::cout << GREEN << F(" used time: ") << GREENB << us << F(" us") << ustd::endl;
        else
            ustd::cout << YELLOW << F(" used time: ") << YELLOWB << us * 1000 << F(" ns") << ustd::endl;
            
        ustd::cout << GREEN << F(" cycles   : ") << GREENB << benchmark_clocks(fct, n) << NONE << ustd::endl;
        
        if(us > 1)
            ustd::cout << GREEN << F(" freqency : ") << GREENB << 1000/us << F(" kHz") << NONE << ustd::endl;
        else
            ustd::cout << YELLOW << F(" freqency : ") << YELLOWB << 1/us << F(" MHz") << NONE << ustd::endl;
        
        ustd::cout << F(" (running at ") << clockCyclesPerMicrosecond() << F(" MHz)") << ustd::endl;
        ustd::cout << F("----------------------") << ustd::endl;
    }
}//end namespace diag
#endif //__BENCHMARK_HEADER
