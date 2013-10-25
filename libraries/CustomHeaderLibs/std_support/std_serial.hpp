// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    14.06.2013 22:27:34 EDT
// File:    std_uart.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __STD_UART_HEADER
#define __STD_UART_HEADER

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

#include "color.hpp"

class serial_class {
    typedef int16_t size_type;
public:
    serial_class(): avail(false), c(0) {
    }
    ~serial_class() {
        close();
    }
    void set_tio() {
        memset(&tio,0,sizeof(tio));
        tio.c_iflag=0;
        tio.c_oflag=0;
        tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
        tio.c_lflag=0;
        tio.c_cc[VMIN]=1;
        tio.c_cc[VTIME]=5;//50
    }
    bool init(std::string const & name, uint const & baud = 460800ul) {
        tty_fd=::open(name.c_str(), O_RDWR | O_NONBLOCK | O_NOCTTY);
        if(tty_fd == -1) {
            std::cout << RED << "couldn't open port: " << REDB << name << NONE << std::endl;
            return false;
        }
        std::cout << GREEN << "port " << GREENB << name << GREEN << " is open" << NONE << std::endl;
        set_tio();
        
        #define lazyswitch(X) else if(baud == X) {cfsetospeed(&tio, B##X);cfsetispeed(&tio, B##X);}
        
        if(0);
        lazyswitch(460800)
        lazyswitch(230400)
        lazyswitch(115200)
        lazyswitch(57600)
        lazyswitch(38400)
        lazyswitch(19200)
        lazyswitch(9600)
        lazyswitch(4800)
        lazyswitch(2400)
        lazyswitch(1800)
        lazyswitch(1200)
        lazyswitch(600)
        lazyswitch(300)
        lazyswitch(200)
        lazyswitch(150)
        lazyswitch(134)
        lazyswitch(110)
        lazyswitch(75)
        lazyswitch(50)
        lazyswitch(0)
        else {
            std::cout << RED << "baud rate " << REDB << baud << RED << " is not supported!" << NONE << std::endl;
            return false;
        }
        
        tcsetattr(tty_fd, TCSANOW, &tio);
        
        flush();
        
        return true;
    }
    
    size_type available() {
        if(!avail) {
            avail = (::read(tty_fd,&c,1) > 0);
        }
        return avail;
    }
    size_type write(char const & data, uint8_t const & len = 1) {
        
        int sent = ::write(tty_fd, &data, len);
        while(sent != 1) //resend on failure
            sent = ::write(tty_fd, &data, len);
        
        //~ if(sent == 1)
            //~ std::cout << GREENB;
        //~ else
            //~ std::cout << REDB;
        //~ std::cout << int(data) << " " << NONE;
        //~ std::cout.flush();
        
        return sent;
    }
    int wait() {
        int counter = 0;
        while(!available()) 
            ++counter;
        return counter;
    }
    
    char read() {
        if(available()) {
            avail = false;
            return c;
        }
        return 0;
    }
    void flush() {
        for(uint i = 0; i < 10000; ++i) { //just heuristics...
            if(available()) {
                avail = false;
            }
        }
        while(available()) {
            avail = false;
        }
    }
    void close() {
        flush();
        if(tty_fd != -1)
            ::close(tty_fd);
        std::cout << std::endl << GREEN << "close port" << NONE << std::endl;
    }
private:
    struct termios tio;
    int tty_fd;
    bool avail;
    char c;
};

extern serial_class Serial;

#endif //__STD_UART_HEADER
