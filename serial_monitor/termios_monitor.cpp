#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

#include "color.hpp"

class serial_class {
public:
    serial_class(): avail(false), c(0) {
        tcgetattr(STDOUT_FILENO, &old_stdio);
    }
    void init() {
        
        memset(&stdio,0,sizeof(stdio));
        stdio.c_iflag=0;
        stdio.c_oflag=0;
        stdio.c_cflag=0;
        stdio.c_lflag=0;
        stdio.c_cc[VMIN]=1;
        stdio.c_cc[VTIME]=0;
        tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
        tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);       // make the reads non-blocking

        memset(&tio,0,sizeof(tio));
        tio.c_iflag=0;
        tio.c_oflag=0;
        tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
        tio.c_lflag=0;
        tio.c_cc[VMIN]=1;
        tio.c_cc[VTIME]=5;
    }
    ~serial_class() {
        close();
    }
    bool open(std::string const & name, uint32_t const & baud = 460800lu) {
        
        tty_fd=::open(name.c_str(), O_RDWR | O_NONBLOCK);
        if(tty_fd == -1) {
            std::cout << RED << "couldn't open port: " << REDB << name << NONE << std::endl;
            return false;
        }
        std::cout << GREEN << "port " << GREENB << name << GREEN << " is open" << NONE << std::endl;
        init();
        
        #define lazyswitch(X) else if(baud == X) {cfsetospeed(&tio, X);cfsetispeed(&tio, X);}
        
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
            reset();
            std::cout << RED << "baud rate " << REDB << baud << RED << " is not supported!" << NONE << std::endl;
            return false;
        }
        
        tcsetattr(tty_fd, TCSANOW, &tio);
        
        flush();
        
        return true;
    }
    struct termios tio;
    struct termios stdio;
    struct termios old_stdio;
    int tty_fd;
    bool avail;
    char c;
    
    int write(char const & t) {
        return ::write(tty_fd, &t, 1);
    }
    
    bool available() {
        if(!avail) {
            avail = (::read(tty_fd,&c,1) > 0);
        }
        return avail;
    }
    char read() {
        if(available()) {
            avail = false;
            return c;
        }
        return 0;
    }
    void flush() {
        for(uint32_t i = 0; i < 10000; ++i) { //just heuristics...
            read();
        }
        while(available()) {
            read();
        }
    }
    void reset() {
        tcsetattr(STDOUT_FILENO,TCSANOW,&old_stdio);
        tcsetattr(STDOUT_FILENO,TCSAFLUSH,&old_stdio);
    }
    void close() {
        flush();
        if(tty_fd != -1)
            ::close(tty_fd);
        reset();
        std::cout << std::endl << GREEN << "exit termios: old cout settings restored" << NONE << std::endl;
    }
    
} serial;

class terminal_class {
public:
    terminal_class(): avail(false), c(0) {
    }
    bool avail;
    char c;
    int write(char const & t) {
        return ::write(STDOUT_FILENO,&t,1);
    }
    bool available() {
        if(!avail) {
            avail = (::read(STDIN_FILENO,&c,1) > 0);
        }
        return avail;
    }
    char read() {
        if(available()) {
            avail = false;
            return c;
        }
        return 0;
    }
} terminal;

int main(int argc,char** argv) {
    std::string name = "/dev/ttyACM0";
    uint32_t baud = 460800lu;
    if(argc > 1)
        name = argv[1];
    if(argc > 2)
        baud = atoi(argv[2]);
    
    std::cout << YELLOW << "starting termios" << std::endl;
    std::cout << "trying to open port: " << YELLOWB << name << YELLOW << " at " << YELLOWB << baud << YELLOW << " baud" << NONE << std::endl;
    
    bool open = serial.open(name, baud);
    if(!open)
        return 1;
    
    unsigned char c = 0;
    while(c!='q')
    {
        if(serial.available()) {
           terminal.write(serial.read());
        }
        if(terminal.available()) {
            c = terminal.read();
            serial.write(c);
        }
    }
    
    return 0;
}
