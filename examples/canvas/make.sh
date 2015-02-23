#!/bin/bash

cd /home/user/Projects/xwings
g++ -Wall -I./include -L./lib/debug -o ./examples/canvas/canvas.exe ./examples/canvas/*.cpp ./lib/debug/gobj1.o -lsys -lxgdi -lstd -ggdb

