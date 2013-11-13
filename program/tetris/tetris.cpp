// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    11.11.2013 23:34:34 CET
// File:    tetris.cpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */


#include <Arduino.h>
#include <ustd.hpp>
#include <tool.hpp>
#include <assert.h>


namespace cst 
{
    typedef int8_t size_type;
    size_type const height = 18;
    size_type const width = 10;
    size_type const border_size = 1;
    enum tetris_enum {square = 0, curve_right , curve_left, nose, line, var_L, reg_L, number_of_shapes, empty, border, clear_moving = 127, moving_flag = 128};
}

class tetris_class {
public:
    typedef cst::size_type size_type;
    typedef int8_t val_type;
    typedef int8_t orientation_type;

    /* ----------------------------------------*/
    /*          setting up the field           */
    /* ----------------------------------------*/

    tetris_class(): lines_count_(0) {
        
        /// setting all to empty except the boundary
        reset();
        
        ///setting up the boundary
        for(size_type i = 0; i < (cst::height + 2 * cst::border_size); ++i) {
            field_[i][0] = cst::border;
            field_[i][cst::width + cst::border_size] = cst::border;
        }
        for(size_type i = cst::border_size; i < (cst::width + cst::border_size); ++i) {
            field_[0][i] = cst::border;
            field_[cst::height + cst::border_size][i] = cst::border;
        }
    }
    
    // setting all to 0 except the boundary
    void reset() {
        for(size_type i = cst::border_size; i < (cst::height + cst::border_size); ++i) {
            for(size_type j = cst::border_size; j < (cst::width + cst::border_size); ++j) {
                field_[i][j] = cst::empty;
            }
        }
        lines_count_ = 0;
    }
    
    // sets the element at (i,j) to val
    void set(size_type const & i, size_type const & j, val_type const & val) {
        assert(0 < i && i < (cst::height + 2 * cst::border_size) && 0 < j && j < (cst::width + 2 * cst::border_size));
        field_[i][j] = val;
    }
    
    // returns the value at (i,j)
    val_type const & get(size_type i, size_type j) const {
        return field_[i][j];
    }
    
    // deletes full lines
    void clear_lines() {
        for(size_type i = cst::border_size; i < cst::height + cst::border_size; ++i) {
            bool all = true;
            for(size_type j = cst::border_size; j < cst::width + cst::border_size; ++j) {
                if(field_[i][j] == cst::empty) {
                    all = false;
                }
            }
            if(all) {
                ++lines_count_;
                for(size_type k = i; k > cst::border_size ; --k) {
                    for(size_type m = cst::border_size; m < cst::width + cst::border_size; ++m) {
                        field_[k][m] = field_[k - 1][m];
                    }
                }
                for(size_type m = cst::border_size; m < cst::width + cst::border_size; ++m) {
                    field_[1][m] = cst::empty;
                }
            }
        }
    }
    
    // checks if (i,j) is inside the border of the field
    bool in_field(size_type i, size_type j) {
        return (cst::border_size <= i && i < (cst::border_size + cst::height)) && (cst::border_size <= j && j < cst::border_size + cst::width);
    }
    
    // sets all elements == temp to zero
    void remove_temp() {
        for(size_type i = cst::border_size; i < (cst::height + cst::border_size); ++i) {
            for(size_type j = cst::border_size; j < (cst::width + cst::border_size); ++j) {
                if((field_[i][j] & cst::moving_flag) == cst::moving_flag)
                    field_[i][j] = cst::empty;
            }
        }
    }
    
    // output of the playing field
    void print() {
        char* color_list[] = {"\033[1;37m", "\033[1;30m", "\033[1;31m", "\033[1;32m", "\033[1;33m", "\033[1;34m", "\033[1;35m", "\033[1;36m", "\033[1;37m", "\033[1;30m"}; // White, Black, Red, Green, Yellow, Blue, Magenta, Cyan, Black
        for(size_type i = 0; i < cst::height + 2 * cst::border_size; ++i) {   
            for(size_type j = 0; j < cst::width + 2 * cst::border_size; ++j) {
                // black background - put in if you don't use a black terminal (not recommended - acute danger of eye cancer!)
                Serial.print("\033[0;40m");  
                if(get(i, j) == cst::empty) {
                    Serial.print(" ");
                }
                else if(get(i, j) == cst::border) {
                    Serial.print(color_list[1]);
                    Serial.print("O");
                }
                else {
                    Serial.print(color_list[get(i, j) & cst::clear_moving]);
                    Serial.print("X");
                }
            }
            Serial.println("");
        }
            Serial.println("----------------------");
    }
    
    // checking the lines count
    size_type const & get_lines_count () const{ 
        return lines_count_;
    }
    
    
    /* ----------------------------------------*/
    /*          setting up the blocks          */
    /* ----------------------------------------*/
    
    // sets shape_ to sh
    void set_shape(cst::tetris_enum const & sh) {
        shape_ = sh;
    }
    
    // sets shape_ to a random value
    void random_shape() {
        shape_ = cst::tetris_enum(random() % cst::number_of_shapes);
    }
    
    // sets block_ to be an array containing the coordinates of the four squares
    void set_block() {
        for(size_type i = 0; i < 4; ++i) {
            for(size_type j = 0; j < 2; ++j) {
                block_[i][j] = get_coordinates(shape_, orientation_, i, j);
            }
        }
    }
    
    // defining the different block types and their orientation
    // i in (0, 3) indicates the 4 squares of each block j in (0,1) their (negative) y / (positive) x coordinate
    size_type get_coordinates(cst::tetris_enum const & sh, orientation_type const & orientation, size_type const & i, size_type const & j) {
        if(sh == cst::square) { 
            size_type b[4][2] = {{0,2},{0,1},{1,1},{1,2}};
            return b[i][j];
        }
        else if(sh == cst::curve_right) { 
            if((orientation & 1) == 0) {
                size_type b[4][2] = {{0,0},{0,1},{1,1},{1,2}};
                return b[i][j];
            }
            else {
                    size_type b[4][2] = {{0,1},{1,0},{1,1},{2,0}};
                    return b[i][j];
            }
        }
        else if(sh == cst::curve_left) {
            if((orientation & 1) == 0) {
                size_type b[4][2] = {{1,0},{0,1},{1,1},{0,2}};
                return b[i][j];
            }   
            else {
                size_type b[4][2] = {{-1,1},{0,1},{0,2},{1,2}};
                return b[i][j];
            }
        }
        else if(sh == cst::line) {
            if((orientation & 1) == 0) {
                size_type b[4][2] = {{0,0},{0,1},{0,2},{0,3}};
                return b[i][j];
            }
            else {
                size_type b[4][2] = {{0,1},{1,1},{2,1},{3,1}};
                return b[i][j];
            }
        }
        else if(sh == cst::var_L) {
            if((orientation & 3) == 0) {
                size_type b[4][2] = {{0,0},{0,1},{0,2},{1,2}};
                return b[i][j];
            }
            else if((orientation & 3) == 1) {
                size_type b[4][2] = {{1,0},{1,1},{0,1},{-1,1}};
                return b[i][j];
            }
            else if((orientation & 3) == 2) {
                size_type b[4][2] = {{-1,0},{0,0},{0,1},{0,2}};
                return b[i][j];
            }
            else {
                    size_type b[4][2] = {{1,1},{0,1},{-1,1},{-1,2}};
                    return b[i][j];
            }
        }
        else if(sh == cst::reg_L) {
            if((orientation & 3) == 0) {
                size_type b[4][2] = {{1,0},{0,0},{0,1},{0,2}};
                return b[i][j];
            }
            else if((orientation & 3) == 1) { 
                size_type b[4][2] = {{-1,1},{0,1},{1,1},{1,2}};
                return b[i][j];
            }
            else if((orientation & 3) == 2) { 
                size_type b[4][2] = {{0,0},{0,1},{0,2},{-1,2}};
                return b[i][j];
                }
            else { 
                size_type b[4][2] = {{-1,0},{-1,1},{0,1},{1,1}};
                return b[i][j];
            }
        }
        else {
            if((orientation & 3) == 0) {
                size_type b[4][2] = {{0,0},{0,1},{0,2},{1,1}};
                return b[i][j];
            }
            else if((orientation & 3) == 1) {
                size_type b[4][2] = {{-1,1},{0,1},{1,1},{0,0}};
                return b[i][j];
            }
            else if((orientation & 3) == 2) {
                size_type b[4][2] = {{0,0},{0,1},{0,2},{-1,1}};
                return b[i][j];
            }
            else {
                size_type b[4][2] = {{-1,1},{0,1},{1,1},{0,2}};
                return b[i][j];
            }
        }
    }
    
    // sets the position for a new block
    void new_position() {
        position_[0] = 1;
        position_[1] = 4;
    }
    
    // sets up a new block
    bool new_block() { // returns true if the new block fits in, false when game is over
        clear_lines();
        new_position();
        random_shape();
        orientation_ = 0;
        set_block();
        if(check_blockfits(orientation_, position_)) {
            return true;
        }
        return false;
    }
    
    // prints the block to the playing field
    void fix() {
        for(size_type i = 0; i < 4; ++i) {
            set(block_[i][0] + position_[0], block_[i][1] + position_[1], shape_ );
        }
    }
    
    // checks if the block with shape_ at 'position' with 'orientation' fits into 'field'
    bool check_blockfits(orientation_type const & orientation, size_type position[2]) { 
        for(size_type i = 0; i < 4; ++i) {
            size_type coordinates[2] = {get_coordinates(shape_, orientation_, i, 0) + position[0],get_coordinates(shape_, orientation_, i, 1) + position[1]};
            if(not(in_field(coordinates[0], coordinates[1]))){
                return false;
            }
            val_type value = get(coordinates[0], coordinates[1]);
            if(value != cst::empty && ((value & cst::moving_flag) == 0))
                return false;
        }
        return true;
    }
    
    // rotates the block if possible
    void rotate() {
        if(check_blockfits(orientation_ + 1, position_)) {
            orientation_ = (orientation_ + 1) & 3;
            set_block();
            update_field();
        }
    }
    
    // moves the block down one if possible, generates new block if it isn't. 
    // returns true if the new block fits into the field, false if not (i.e. the game is over).
    bool down() {
        if(!move(1,0)) {
            fix();
            return new_block();
        }
    }
    
    // moves the block down as much as possible, generates new block. 
    // returns true if the new block fits into the field, false if not (i.e. the game is over).
    bool fast_down() {
        while(move(1,0)) {
        }
        fix();
        return new_block();
    }
    
    
    // moves the block by (i,j)
    
    bool move(size_type const & i, size_type const & j) {
        size_type newposition[2];
        newposition[0] = position_[0] + i;
        newposition[1] = position_[1] + j;
        if(check_blockfits(orientation_, newposition)){
            position_[0] = newposition[0];
            position_[1] = newposition[1];
            update_field();
            return true;
        }
        return false;
    }
    
    void left() {
        move(0, -1);
    }
    
    void right() {
        move(0, 1);
    }
    
    // updates 'field' to account for current block position 
    void update_field() {
        remove_temp();
        for(size_type i = 0; i < 4; ++i) {
            set(block_[i][0] + position_[0], block_[i][1] + position_[1], shape_ | cst::moving_flag);
        }
    }
    
    void game_over() {
        for(size_type i = cst::border_size; i < cst::height + cst::border_size; ++i) {
            for(size_type j = cst::border_size; j < cst::width + cst::border_size; ++j) {
                set(i, j, (i + j) & 1);
            }
        }
        print();
        delay(1000);
        new_game();
    }
    
    void new_game() {
        reset();
        new_block();
        update_field();
        print();
    }
    
private:
    size_type lines_count_;
    val_type field_[cst::height + 2 * cst::border_size][cst::width + 2 * cst::border_size];
    cst::tetris_enum shape_;
    size_type block_[4][2];
    size_type position_[2];
    orientation_type orientation_;
};



class program {
public:

        
    program(){
        setup();
    }
    
    void setup() {
        randomSeed(analogRead(0)); // shows much too predictable behaviour if you put it in the class... maybe A0 is connected to a potential at reset
        random(); // first is always a square else
        Serial.begin(460800);
        tetris_.new_block();
        tetris_.update_field();
        tetris_.print();
        tool::clock.update();
        zero_time_ = tool::clock.millis();
    }
    
    void update() {
        btn_left_.update();
        btn_right_.update();
        btn_up_.update();
        btn_down_.update();
        tool::clock.update();
        time_ = tool::clock.millis();
    }
    
    void loop() {
        update();
        
        
        if(btn_up_ == state::falling) {
            tetris_.rotate();
            tetris_.print();
        }
        
        if(btn_left_ == state::falling) {
            tetris_.left();
            tetris_.print();
        }
        
        if(btn_right_ == state::falling) {
            tetris_.right();
            tetris_.print();
        }
        
        if(btn_down_ == state::falling) {
            not_over_ = tetris_.down();
            tetris_.print();
        }
        
        if(time_ - zero_time_ > timeout_) {
            zero_time_ = time_;
            not_over_ = tetris_.down();
            tetris_.print();
            
        }
        
        if(!not_over_) {
            tetris_.game_over();
            not_over_ = true;
        }
        
    }

        
private:
    tool::button_class<2, LOW> btn_left_;
    tool::button_class<3, LOW> btn_right_;
    tool::button_class<4, LOW> btn_up_;
    tool::button_class<5, LOW> btn_down_;
    
    double time_;
    double zero_time_;
    double timeout_ = 500;
    tetris_class tetris_;
    bool not_over_ = true;
    
};

#include <main.hpp>
