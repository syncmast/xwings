#!/bin/bash

cd "Projects/xwings"
g++ -Wall -I./include -L./lib -fPIC -shared -o ./lib/libxgdi.so ./source/xgdi/*.cpp -lX11 -lXext -lXrender -lXfixes -lsystem -ggdb

