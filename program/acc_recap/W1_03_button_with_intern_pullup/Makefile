#---------------------------name----------------------------------------
TARGET := $(shell ls | grep *.cpp | sed 's/.\{4\}$$//')

ARDLIBS = 

MODEL = ATMega16MHz

PORT ?= /dev/ttyACM*

OPT = s

include /home/msk/ArduinoMin/makefile/Master_Makefile.mk
