#!/bin/bash 

export OLD_DIR="/home/msk/ArduinoMin"
export NEW_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
find ./program/  -name *.mk     | xargs perl -p -i -e 's/$ENV{OLD_DIR}/$ENV{NEW_DIR}/g'
find ./program/  -name Makefile | xargs perl -p -i -e 's/$ENV{OLD_DIR}/$ENV{NEW_DIR}/g'
find ./makefile/ -name *.mk     | xargs perl -p -i -e 's/$ENV{OLD_DIR}/$ENV{NEW_DIR}/g'
