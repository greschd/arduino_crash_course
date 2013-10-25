#---------------------------libs----------------------------------------
CUSTOM_LIB_REQ = EEPROM

ALLLIBS = $(ARDLIBS) $(CUSTOM_LIB_REQ)

BAUD ?= 460800
#---------------------------path----------------------------------------
PORT ?= /dev/ttyACM*
ARDUINO_DIR = /home/greschd/programming/Arduino/arduino_crash_course
ARDUINO_LIBS = $(ARDUINO_DIR)/libraries
ARDUINO_CORE = $(ARDUINO_LIBS)/Core
AVR_TOOLS_PATH = /usr/bin
BUILD_DIR = build

MAKEFILE ?= $(firstword $(MAKEFILE_LIST))
#---------------------------includes------------------------------------
CINCS = -I $(ARDUINO_CORE) $(patsubst %, -I $(ARDUINO_DIR)/libraries/%, $(ALLLIBS))
CXXINCS = $(CINCS) -I $(ARDUINO_LIBS)/CustomHeaderLibs

#---------------------------src-----------------------------------------
SRC = $(wildcard $(ARDUINO_CORE)/*.c) $(foreach l,$(ALLLIBS),$(wildcard $(ARDUINO_LIBS)/$l/*.c))

CXXSRC = $(TARGET).cpp $(wildcard $(ARDUINO_CORE)/*.cpp) $(foreach l,$(ALLLIBS),$(wildcard $(ARDUINO_LIBS)/$l/*.cpp))

ASRC = $(wildcard $(ARDUINO_CORE)/*.S) $(foreach l,$(ALLLIBS),$(wildcard $(ARDUINO_LIBS)/$l/*.S))

OBJ = $(SRC:.c=.o) $(CXXSRC:.cpp=.o) $(ASRC:.S=.o)
LST = $(ASRC:.S=.lst) $(CXXSRC:.cpp=.lst) $(SRC:.c=.lst)

#---------------------------cpu-----------------------------------------
FORMAT = ihex

MODEL ?= NoModel

#---------------------------ugly but works-----------------------------------------
ifeq "$(MODEL)" "ATMega16MHz"
	F_CPU = 16000000L
	UPLOAD_RATE = 115200
	MCU = atmega328p
	AVRDUDE_PROGRAMMER = arduino
else
	ifeq "$(MODEL)" "ATMega20MHz"
		F_CPU=20000000L
		UPLOAD_RATE=57600
		MCU = atmega328p
		AVRDUDE_PROGRAMMER = arduino
	else
		ifeq "$(MODEL)" "Fio"
			F_CPU=8000000L
			UPLOAD_RATE=57600
			MCU = atmega328p
			AVRDUDE_PROGRAMMER = arduino
		else
			ifeq "$(MODEL)" "ATMega32U4"
				F_CPU=16000000L
				UPLOAD_RATE=57600
				MCU = atmega32u4
				AVRDUDE_PROGRAMMER = avr109
				ADDITIONAL_DEFINES = -D USB_VID=0x2341 -D USB_PID=0x8036
			endif
		endif
	endif
endif


#---------------------------tools---------------------------------------
CC = 		$(AVR_TOOLS_PATH)/avr-gcc
CXX = 		$(AVR_TOOLS_PATH)/avr-g++
OBJCOPY = 	$(AVR_TOOLS_PATH)/avr-objcopy
OBJDUMP = 	$(AVR_TOOLS_PATH)/avr-objdump
SIZE = 		$(AVR_TOOLS_PATH)/avr-size
NM = 		$(AVR_TOOLS_PATH)/avr-nm
AVRDUDE = 	$(AVR_TOOLS_PATH)/avrdude
AR  = 		$(AVR_TOOLS_PATH)/avr-ar
REMOVE = 	rm -f
MV = 		mv -f

AVRDUDE_PORT = $(PORT)
AVRDUDE_WRITE_FLASH = -V -U flash:w:$(BUILD_DIR)/$(TARGET).hex
AVRDUDE_FLAGS = -F -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) -b $(UPLOAD_RATE)

#---------------------------flags---------------------------------------
CSTANDARD = -std=gnu99
CXXSTANDARD = -std=c++0x

OPT ?= 3 

DEBUG = stabs

CDEFS = -DF_CPU=$(F_CPU) $(ADDITIONAL_DEFINES)
CXXDEFS = -DF_CPU=$(F_CPU)
ADEFS = -DF_CPU=$(F_CPU)

CDEBUG = -g$(DEBUG)
CWARN = -Wall -Wstrict-prototypes
CEXTRA = -g -Wall -fno-exceptions -ffunction-sections -fdata-sections -DARDUINO=102
#~ -fno-inline-small-functions -fno-split-wide-types -mshort-calls

CFLAGS = $(CDEBUG) $(CDEFS) $(CINCS) -O$(OPT) $(CWARN) $(CSTANDARD) $(CEXTRA)
CXXFLAGS = $(CDEFS) $(CXXINCS) -O$(OPT) $(CXXSTANDARD) $(CEXTRA)
ASFLAGS = $(ADEFS)
LDFLAGS = -O$(OPT) -Wl,--relax -Wl,--gc-sections -mmcu=$(MCU) -lm

ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS)
ALL_CXXFLAGS = -mmcu=$(MCU) -I. $(CXXFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)

#---------------------------all-----------------------------------------
all: upload

setting:
	@echo $(MODEL)
	@echo $(F_CPU)
	@echo $(UPLOAD_RATE)
	@echo $(MCU)
#---------------------------control serialscreen-----------------------------------------
run:
	#~ if [ ""a"`screen -list | grep $(TARGET)`" = "a" ]; then screen -S $(TARGET) $(PORT) $(BAUD); else screen -S arduino -r; fi
	#~ cu -l $(PORT) -s $(BAUD)
	#~ cat $(PORT) $(BAUD)
	termios $(PORT) $(BAUD)

#~ kill:
	#~ if [ ""a"`screen -list | grep $(TARGET)`" = "a" ]; then echo already killed; else screen -S $(TARGET) -X quit; echo kill; sleep .1; fi

#---------------------------build "low-level" targets-------------------
.SUFFIXES: .elf .hex

.elf.hex: $(MAKEFILE)
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

%.o: %.cpp $(MAKEFILE)
	$(CXX) -c $(ALL_CXXFLAGS) `echo $@ | awk 'sub("\\\.o",".cpp")'`  -o $@

%.o: %.c $(MAKEFILE)
	$(CC) -c $(ALL_CFLAGS) `echo $@ | awk 'sub("\\\.o",".c")'`  -o $@

.c.s: $(MAKEFILE)
	$(CC) -S $(ALL_CFLAGS) $< -o $@

.S.o: $(MAKEFILE)
	$(CC) -c $(ALL_ASFLAGS) $< -o $@

#---------------------------build "high-level" targets------------------
$(BUILD_DIR)/$(TARGET).elf: $(TARGET).o $(BUILD_DIR)/core.a $(MAKEFILE)
	$(CC) -o $@ $(TARGET).o -L. $(BUILD_DIR)/core.a $(LDFLAGS)
	$(REMOVE) $(TARGET).o

$(BUILD_DIR)/core.a: $(OBJ) $(MAKEFILE)
	test -d $(BUILD_DIR)/ || mkdir $(BUILD_DIR)
	@for i in $(OBJ); do echo $(AR) rcs $(BUILD_DIR)/core.a $$i; $(AR) rcs $(BUILD_DIR)/core.a $$i; done

upload: $(BUILD_DIR)/$(TARGET).hex $(MAKEFILE)
	#~ make -f $(MAKEFILE) kill
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)
	make -f $(MAKEFILE) run

#---------------------------clean---------------------------------------
clean:
	rm -rf $(BUILD_DIR)
	$(REMOVE) $(OBJ) $(LST) $(SRC:.c=.s) $(SRC:.c=.d) $(CXXSRC:.cpp=.s) $(CXXSRC:.cpp=.d)

.PHONY:	all build elf hex clean run
