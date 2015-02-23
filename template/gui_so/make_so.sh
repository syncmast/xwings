#!/bin/bash

cd /home/user/Desktop/Projects/xvol++
g++ -Wall -I./include -L./lib -fPIC -shared -o ./lib/libproject.so ./source/*.cpp ./lib/gobjso.o ./lib/gobj1.o -lstd -limg -lxgdi -lsys -ggdb

