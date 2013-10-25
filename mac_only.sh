#!/bin/bash 

#set tool path in Master_Makefile.mk
export OLD_DIR="/usr/bin"
export NEW_DIR="/opt/local/bin"
find ./makefile/  -name *.mk | xargs perl -p -i -e 's/$ENV{OLD_DIR}/$ENV{NEW_DIR}/g'

#set serial port in all makefiles
export OLD_PORT="/dev/ttyACM\*"
export NEW_PORT="/dev/tty.usbmodem*"
find ./program/  -name *.mk     | xargs perl -p -i -e 's/$ENV{OLD_PORT}/$ENV{NEW_PORT}/g'
find ./program/  -name Makefile | xargs perl -p -i -e 's/$ENV{OLD_PORT}/$ENV{NEW_PORT}/g'
find ./makefile/ -name *.mk     | xargs perl -p -i -e 's/$ENV{OLD_PORT}/$ENV{NEW_PORT}/g'
