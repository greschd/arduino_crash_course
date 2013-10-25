This is a short setup guide for linux and mac:

you need to have installed:

avr-gcc (or gcc-avr on mac)
avr-libc
avrdude

you need to be part of the dialout group

Linux:

01: clone/fork this repo
02: compile serial_monitor/termios_monitor.cpp
03: name the binary termios
04: move termios to ~/bin
05: log out and in again
06: termios should now be a valid command in the terminal
07: run ./rename_path.sh

08: goto program/empty_template
09: now call make with an arduino attached and everything should work

Mac:

01: clone/fork this repo
02: compile serial_monitor/termios_monitor.cpp
03: name the binary termios
04: move termios to ~/bin
05: open ~/.bash_profile or if there is no ~/.bash_profile just open ~/.profile
06: add somewhere the line:
    export PATH = /Users/your_user_name/bin:$PATH
07: log out and in again
08: termios should now be a valid command in the terminal
09: run ./rename_path.sh
10: if the avr tools aren't in /opt/local/bin
    10.5: change the file mac_only.sh by changing the line NEW_DIR="/opt/local/bin" to the right path
11: if the Arduino serial port isn't something like /dev/tty.usbmodem*
    11.5: change the file mac_only.sh by changing the line NEW_PORT="/dev/tty.usbmodem*" to the right port
12: run ./mac_only.sh

13: goto program/empty_template
14: now call make with an arduino attached and everything should work
