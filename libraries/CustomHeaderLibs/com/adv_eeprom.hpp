// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    11.06.2013 21:14:49 EDT
// File:    adv_eeprom.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __ADV_EEPROM_HEADER
#define __ADV_EEPROM_HEADER

//DO NOT INCLUDE THIS FILE AND eeprom.hpp AT THE SAME TIME

#include "serializer.hpp"
#include "../ustd/unordered_map.hpp"
#include "../ustd/pair.hpp"
#include "../util/reset.hpp"
#include "../util/checksum.hpp"

#include <EEPROM.h>

//vmv: virtual memory vector

namespace com {
    class version_class {
    public:
        version_class(): nr_(1) {
        }
        version_class & operator()(uint8_t const & nr) {
            nr_ = nr;
            return (*this);
        }
        uint8_t const & operator()() const {
            return nr_;
        }
    private:
        uint8_t nr_;
    } version;
    
    //=================== eeprom wrapper ===================
    template<typename EEPROM_concept, uint16_t EEPROM_size>
    class EEPROMV2_class {
        typedef uint16_t adress_type;
        typedef uint8_t version_type;
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
                init_vmv(t);
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
            version_type & v = map_[adress_type(&t)].second;
            adress_type vmv_begin = map_[adress_type(&t)].first + v * (sizeof(t) + 1);
            ASSERT_MSG(version() < v, "eeprom version overflow");
            
            //translate version to hardware_version
            ies_.pos() = vmv_begin + version();
            version_type hw_version;
            ies_ & hw_version;
            
            oes_.pos() = map_.at(adress_type(&t)).first + (sizeof(t) + 1) * hw_version;
            version(0);
            
            oes_ & t;
            oes_.write(util::checksum(t));
            return (*this);
        }
        //------------------- read from eeprom -------------------
        template<typename T>
        EEPROMV2_class & operator>>(T & t) {
            version_type & v = map_[adress_type(&t)].second;
            adress_type vmv_begin = map_[adress_type(&t)].first + v * (sizeof(t) + 1);
            ASSERT_MSG(version() < v, "eeprom version overflow");
            
            //translate version to hardware_version
            ies_.pos() = vmv_begin + version();
            version_type hw_version;
            ies_ & hw_version;
            
            ies_.pos() = map_.at(adress_type(&t)).first + (sizeof(t) + 1) * hw_version;
            version(0);
            
            ies_ & t;
            ASSERT_MSG(ies_.read() == util::checksum(t), "cannot access uninitialized version");
            
            return (*this);
        }
        //------------------- jump to the right adress -------------------
        template<typename T>
        void adjust_pos(T & t, size_type & pos) {
            if(!map_.contains(adress_type(&t))) {
                map_[adress_type(&t)].first = current_free_;
                
                if(version() == 0)
                    version(1);
                map_[adress_type(&t)].second = version();
                current_free_ += (sizeof(t) + 1) * version() + version(); //+1 for checksum / + version() for vmv
                version(0);
                
                ASSERT(current_free_ < EEPROM_size);
                pos = map_[adress_type(&t)].first;
            }
            else {
                pos = map_[adress_type(&t)].first;
            }
        }
        //------------------- init virtual memory vector -------------------
        template<typename T>
        void init_vmv(T & t) {
            version_type & v = map_[adress_type(&t)].second;
            oes_.pos() = map_[adress_type(&t)].first + v * (sizeof(t) + 1);
            for(version_type i = 0; i < v; ++i) {
                oes_ & i;
            }
        }
        //------------------- delete all eeprom -------------------
        void clear(uint8_t const & arg = 0xFF) {
            oes_.pos() = 0;
            for(adress_type i = 0; i < EEPROM_size; ++i) {
                oes_ & arg;
            }
            oes_.pos() = 0;
        }
        //------------------- just so version can be concatenated -------------------
        EEPROMV2_class & operator&(version_class & v) {
            return (*this);
        }
        EEPROMV2_class & operator<<(version_class & v) {
            return (*this);
        }
        EEPROMV2_class & operator>>(version_class & v) {
            return (*this);
        }
        //------------------- swap two versions -------------------
        template<typename T>
        void swap_versions(T & t, version_type const & v1, version_type const & v2) {
            version_type & v = map_[adress_type(&t)].second;
            adress_type vmv_begin = map_[adress_type(&t)].first + v * (sizeof(t) + 1);
            
            ASSERT(v1 < v and v2 < v);
            
            version_type s1, s2;
            ies_.pos() = vmv_begin + v1;
            ies_ & s1;
            ies_.pos() = vmv_begin + v2;
            ies_ & s2;
            
            oes_.pos() = vmv_begin + v1;
            oes_ & s2;
            oes_.pos() = vmv_begin + v2;
            oes_ & s1;
        }
        template<typename T>
        void move_version_to(T & t, version_type const & v1, version_type const & v2) {
            version_type & v = map_[adress_type(&t)].second;
            adress_type vmv_begin = map_[adress_type(&t)].first + v * (sizeof(t) + 1);
            
            ASSERT(v1 < v and v2 < v);
            version_type hw_v1, temp;
            oes_.pos() = vmv_begin + v1;
            ies_.pos() = vmv_begin + v1;
            
            ies_ & hw_v1;
            
            for(version_type i = v1; i < v2; ++i) {
                ies_ & temp;
                oes_ & temp;
            }
            oes_ & hw_v1;
        }
        template<typename T>
        void move_version_to_end(T & t, version_type const & v1) {
            version_type & v = map_[adress_type(&t)].second;
            move_version_to(t, v1, v - 1);
        }
    private:
        oss_class<EEPROM_concept, EEPROM_size> oes_; //out eeprom stream
        iss_class<EEPROM_concept, EEPROM_size> ies_;
        ustd::unordered_map<adress_type, ustd::pair<adress_type, version_type>> map_; //pair<adr_in_eeprom, nr_of_versions>
        adress_type current_free_;
        uint8_t corrupted_;
    };
    
    EEPROMV2_class<EEPROMClass, 1024> eeprom(EEPROM);
}//end namespace com
#endif //__ADV_EEPROM_HEADER
