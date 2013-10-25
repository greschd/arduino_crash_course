#---------------------------name----------------------------------------
TARGET := $(shell ls | grep *.cpp | sed 's/.\{4\}$$//')

ARDLIBS = 

MODEL = ATMega16MHz

PORT ?= /dev/ttyACM*

OPT = 3

include /home/greschd/programming/Arduino/arduino_crash_course/makefile/Master_Makefile.mk
