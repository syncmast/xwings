#!/bin/bash

cd /home/user/Desktop/Projects/xvol++
g++ -Wall -I./include -L./lib -o ./bin/project.exe ./source/*.cpp ./lib/gobj1.o -lstd -limg -lxgdi -lsys -ggdb

