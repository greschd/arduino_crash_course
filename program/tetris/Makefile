#---------------------------name----------------------------------------
TARGET := $(shell ls | grep *.cpp | sed 's/.\{4\}$$//')

ARDLIBS = 

MODEL = ATMega16MHz

PORT ?= /dev/ttyACM*

OPT = s

BAUD = 460800

include /home/greschd/programming/Arduino/arduino_crash_course/makefile/Master_Makefile.mk
