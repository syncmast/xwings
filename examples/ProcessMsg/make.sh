#!/bin/bash

cd /home/user/Projects/xwings
g++ -Wall -I./include -L./lib/debug -o ./examples/ProcessMsg/ProcessMsg.exe ./examples/ProcessMsg/*.cpp ./lib/debug/gobj1.o -lsys -lxgdi -lstd -ggdb

