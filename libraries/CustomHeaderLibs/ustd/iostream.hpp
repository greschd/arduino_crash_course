// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    23.05.2013 20:57:19 EDT
// File:    iostream.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __IOSTREAM_HEADER
#define __IOSTREAM_HEADER

#include "color.hpp"
#include "iomanip.hpp"
#include "type_traits.hpp"
#include "../util/has_print.hpp"

#include <Print.h>
#include <Arduino.h>

namespace ustd {
    
    struct endl_class {
        endl_class(): end(true) {
        }
        endl_class & operator()(bool break_connection) {
            end = break_connection; //for i2c for now
            return (*this);
        }
        bool end;
    } endl;
    
    class cout_class : public Print{
    private:
        #define TYPEDEF(NAME) typedef typename NAME::NAME ## _type NAME ## _type; NAME ## _type NAME ## _;
        
        TYPEDEF(setw)
        TYPEDEF(setfill)
        TYPEDEF(setprecision)
        TYPEDEF(setbase)
        
        #undef TYPEDEF

        size_t write(uint8_t) {
            return 1;
        }
        //------------------- same fct as in Print.h-------------------
        size_t impl_chooser(const __FlashStringHelper * arg, Print * printer) {return printer->print(arg);}
        size_t impl_chooser(const String & arg, Print * printer) {return printer->print(arg);}
        size_t impl_chooser(const char arg[], Print * printer) {return printer->print(arg);}
        size_t impl_chooser(char arg, Print * printer) {return printer->print(arg);}
        size_t impl_chooser(const Printable& arg, Print * printer) {return printer->print(arg);}
        
        size_t impl_chooser(unsigned char arg, Print * printer) {return printer->print(arg, setbase_);}
        size_t impl_chooser(int arg, Print * printer) {return printer->print(arg, setbase_);}
        size_t impl_chooser(unsigned int arg, Print * printer) {return printer->print(arg, setbase_);}
        size_t impl_chooser(long arg, Print * printer) {return printer->print(arg, setbase_);}
        size_t impl_chooser(unsigned long arg, Print * printer) {return printer->print(arg, setbase_);}
        size_t impl_chooser(uint64_t arg, Print * printer) {return printer->print((unsigned long)arg, setbase_);}
        
        size_t impl_chooser(double arg, Print * printer) {return printer->print(arg, setprecision_);}
        
    public:
        void init(long const & speed = 460800lu) {
            Serial.begin(speed);
            Serial.setTimeout(100000000L);
            setw_ = 0;
            setfill_ = ' ';
            setprecision_ = 2;
            setbase_ = 10;
        }
        template<typename T>
        typename enable_if<util::has_print<T>::value, cout_class>::type & operator<<(T const & arg) {
            arg.print(*this);
            return *this;
        }
        template<typename T>
        typename disable_if<util::has_print<T>::value, cout_class>::type & operator<<(T const & arg) {
            setw_type s = impl_chooser(arg, this);
            if(s < setw_)
                for(setw_type i = s; i < setw_; ++i)
                    Serial.print(setfill_);
            
            setw_ = 0;
            
            impl_chooser(arg, &Serial);
            Serial.flush();
            return *this;
        }
        cout_class & flush() {
            Serial.flush();
            return (*this);
        }
        cout_class & operator<<(endl_class & endl_) {
            Serial.println();
            Serial.flush();
            return (*this);
        }
        cout_class & operator<<(detail::dummy_struct const &) {
            return (*this);
        }
        
        #define IOMANIP_IMPL(NAME) \
        cout_class & operator<<(NAME const & NAME ## _in) { \
            NAME ## _ = NAME ## _in(); \
            return (*this); \
        }
        
        IOMANIP_IMPL(setw)
        IOMANIP_IMPL(setfill)
        IOMANIP_IMPL(setprecision)
        IOMANIP_IMPL(setbase)
        
        #undef IOMANIP_IMPL
        cout_class & operator()(uint8_t const & option = 2) {
            if(option == 0) {
                (*this) << NONE;
            }
            else if(option == 1) {
                setbase_ =  10;
                setfill_ = ' ';
                setprecision_ = 2;
            }
            else if(option == 2) {
                (*this) << NONE;
                setbase_ =  10;
                setfill_ = ' ';
                setprecision_ = 2;
            }
            return (*this);
        }
    } cout;
//=================== cin ===================
    struct cin_class {
        cin_class(): silent_(false) {
        }
        operator bool() const {
            return Serial.available();
        }
        //~ template<typename T>
        //~ cin_class const & operator>>(T & in) const {
            //~ while(!Serial.available())
                //~ ;
            //~ in = Serial.read();
            //~ return *this;
        //~ }
        template<typename T>
        typename enable_if<is_arithmetic<T>::value, cin_class>::type const & operator>>(T & in) const {
            const uint8_t key_enter = 13;
            const uint8_t key_space = ' ';
            
            uint8_t chr;
            
            auto read = [&](){
                while(!Serial.available());
                chr = Serial.read();
            };
            
            read();
            
            in = 0;
            bool neg = false;
            uint8_t dot_pos = 0xFF;
            
            if(!is_unsigned<T>::value)
                if(chr == '-') {
                    neg = true;
                    ustd::cout << '-';
                    read();
                }
            
            while(1) {
                if(chr == key_enter or chr == key_space) {
                    break;
                }
                if(chr >= '0' and chr <= '9') {
                    in *= 10;
                    in += chr - '0';
                    ustd::cout << chr - '0';
                    if(dot_pos != 0xFF)
                        ++dot_pos;
                }
                else if(is_floating_point<T>::value) {
                    if(dot_pos == 0xFF and chr == '.') {
                        dot_pos = 0;
                        ustd::cout << '.';
                    }
                }
                else break;
                
                
                read();
            }
            if(is_floating_point<T>::value)
                if(dot_pos != 0xFF)
                    for(;dot_pos; --dot_pos)
                        in /= 10;
            
            if(!is_unsigned<T>::value)
                if(neg)
                    in *= -1;
            
            return *this;
        }
        
        char peek() const {
            return Serial.peek();
        }
        char read() const {
            return Serial.read();
        }
        void set_silent(bool const & s) {
            silent_ = s;
        }
    private:
        bool silent_;
    } cin;
}

#endif //__IOSTREAM_HEADER
