// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    14.06.2013 23:08:37 EDT
// File:    prototype_uart.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __PROTOTYPE_UART_HEADER
#define __PROTOTYPE_UART_HEADER

#include "serializer.hpp"

namespace com {
    //------------------- of class -------------------
    template<typename D>
    class osw_class {
    public:
        static archive_enum const type = archive_enum::output;
        
        osw_class(D & data): data(data) {
        }
        template<typename T>
        osw_class & operator&(T const & t) {
            serialize(*this, t);
            return (*this);
        }
        void write(uint8_t const & in) {
            data.write(in);
        }
    private:
        D & data;
    };

    //------------------- if class -------------------
    template<typename D>
    class isw_class {
    public:
        static archive_enum const type = archive_enum::input;
        
        isw_class(D & data): data(data) {
        }
        template<typename T>
        isw_class & operator&(T & t) {
            serialize(*this, t);
            return (*this);
        }
        uint8_t read() {
            while(!data.available());
            return data.read();
        }
    private:
        D & data;
    };
    //------------------- combined class -------------------
    template<typename D>
    class uart_class_template {
        typedef uint16_t size_type;
    public:
        uart_class_template(D & data): data_(data), osw_(data), isw_(data) {
        }
        operator bool() {
            return available() > 0;
        }
        
        size_type available() const {
            return data_.available();
        }
        
        template<typename T>
        uart_class_template & operator<<(T const & t) {
            osw_ & t;
            return (*this);
        }
        template<typename T>
        uart_class_template & operator<<(T const & t) {
            osw_ & t;
            return (*this);
        }
        template<typename T>
        uart_class_template & operator>>(T & t) {
            isw_ & t;
            return (*this);
        }
        void wait() {
            while(!available()) {};
        }
        //~ void reflect() {
            //~ osw_.write(isw_.read());
        //~ }
    private:
        D & data_;
        osw_class<D> osw_;
        isw_class<D> isw_;
    };
    
    #ifdef Arduino_h
        uart_class_template<HardwareSerial> uart(Serial);
    #endif //Arduino_h
}//end namespace com
#endif //__PROTOTYPE_UART_HEADER
