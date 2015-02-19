#!/bin/bash

cd "Projects/xwings"
g++ -Wall -L./lib -I./include -fPIC -shared -o ./lib/libstd.so ./source/stdctrls/*.cpp -lxgdi -ggdb

