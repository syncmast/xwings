#!/bin/bash

cd "Projects/xwings"
g++ -Wall -I./include -L./lib -fPIC -shared -o ./lib/libwm.so ./source/wm/*.cpp -lxgdi -ggdb

