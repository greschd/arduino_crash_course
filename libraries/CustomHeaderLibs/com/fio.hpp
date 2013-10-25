// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    26.08.2013 15:27:54 CEST
// File:    fio.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __FIO_HEADER
#define __FIO_HEADER

//THIS IS NOT A HEADER FOR THE ARDUINO

#include <fstream>
#include "serializer.hpp"

namespace com {
    
    class fio_class {
    public:
        template<typename T>
        fio_class & operator<<(T & t) {
            serialize(ofs_, t);
            return (*this);
        }
        template<typename T>
        fio_class & operator>>(T & t) {
            serialize(ifs_, t);
            return (*this);
        }
        void open_read(std::string const & path_name) {
            ifs_.open(path_name, std::ios_base::in);// | std::ios_base::binary);
            if(!ifs_.is_open()) {
                std::cout << "fio.hpp error: file " << path_name << " not found" << std::endl;
            }
        }
        void open_write(std::string const & path_name) {
            ofs_.open(path_name, std::ios_base::out);// | std::ios_base::binary);
            if(!ofs_.is_open()) {
                std::cout << "fio.hpp error: file " << path_name << " not found" << std::endl;
            }
        }
        void close() {
            ifs_.close();
            ofs_.close();
        }
    private:
        struct ofs_wrapper: public std::ofstream {
            typedef std::ofstream base;
            static archive_enum const type = archive_enum::output;
            
            template<typename T>
            ofs_wrapper & operator&(T & t) {
                serialize(*this, t);
                return (*this);
            }
            
            void write(uint8_t const & arg) {
                char c = arg;
                base::write(&c, 1);
            }
        };
        struct ifs_wrapper: public std::ifstream {
            typedef std::ifstream base;
            static archive_enum const type = archive_enum::input;
            
            template<typename T>
            ifs_wrapper & operator&(T & t) {
                serialize(*this, t);
                return (*this);
            }
            
            uint8_t read() {
                char c;
                base::read(&c, 1);
                return c;
            }
        };
        ofs_wrapper ofs_;
        ifs_wrapper ifs_;
        
    };
    
    
}//end namespace 

#endif //__FIO_HEADER
