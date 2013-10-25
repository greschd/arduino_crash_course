// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    11.06.2013 21:14:49 EDT
// File:    eeprom.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __EEPROM_HEADER
#define __EEPROM_HEADER

//DO NOT INCLUDE THIS FILE AND adv_eeprom.hpp AT THE SAME TIME

#include "serializer.hpp"
#include "../ustd/unordered_map.hpp"
#include "../util/reset.hpp"
#include "../util/checksum.hpp"

#include <EEPROM.h>

namespace com {
    template<typename EEPROM_concept, unsigned int EEPROM_size>
    class EEPROMV2_class {
        typedef uint16_t adress_type;
        typedef typename oss_class<EEPROM_concept, EEPROM_size>::size_type size_type;
        enum{not_corrupt = 0, corrupt = 0xFF, not_set = 2};
    public:
        EEPROMV2_class(EEPROM_concept & eeprom): oes_(eeprom), ies_(eeprom), current_free_(1), corrupted_(not_set) {
        }
        //------------------- load from eeprom and reset if corrupted-------------------
        void reset() { //force a reset without clear
            corrupted_ = corrupt;
        }
        template<typename T>
        EEPROMV2_class & operator&(T & t) {
            if(corrupted_ != corrupt and corrupted_ != not_corrupt) {
                corrupted_ = ies_.read();
                oes_.write(not_corrupt);
            }
            if(corrupted_ == corrupt) {
                adjust_pos(t, oes_.pos());
                oes_ & t;
                oes_.write(util::checksum(t));
            }
            else {
                adjust_pos(t, ies_.pos());
                ies_ & t;
                if(ies_.read() != util::checksum(t)) {
                    DEBUG_MSG("eeprom is corrupted! reset and fresh init incomming")
                    oes_.pos() = 0;
                    oes_.write(corrupt);
                    util::reset();
                }
            }
            return (*this);
        }
        //------------------- write to eeprom -------------------
        template<typename T>
        EEPROMV2_class & operator<<(T & t) {
            oes_.pos() = map_.at(adress_type(&t));
            oes_ & t;
            oes_.write(util::checksum(t));
            return (*this);
        }
        //------------------- read from eeprom -------------------
        template<typename T>
        EEPROMV2_class & operator>>(T & t) {
            ies_.pos() = map_.at(adress_type(&t));
            ies_ & t;
            return (*this);
        }
        //------------------- jump to the right adress -------------------
        template<typename T>
        void adjust_pos(T & t, size_type & pos) {
            if(!map_.contains(adress_type(&t))) {
                map_[adress_type(&t)] = current_free_;
                current_free_ += sizeof(t) + 1; //+1 for checksum
                ASSERT(current_free_ < EEPROM_size);
                pos = map_[adress_type(&t)];
            }
            else
                pos = map_[adress_type(&t)];
        }
        //------------------- delete all eeprom -------------------
        void clear(uint8_t const & arg = 0xFF) {
            oes_.pos() = 0;
            for(adress_type i = 0; i < EEPROM_size; ++i) {
                oes_ & arg;
            }
            oes_.pos() = 0;
        }
    private:
        oss_class<EEPROM_concept, EEPROM_size> oes_;
        iss_class<EEPROM_concept, EEPROM_size> ies_;
        ustd::unordered_map<adress_type, adress_type> map_;
        adress_type current_free_;
        uint8_t corrupted_;
    };
    
    EEPROMV2_class<EEPROMClass, 1024> eeprom(EEPROM);
}//end namespace com
#endif //__EEPROM_HEADER
