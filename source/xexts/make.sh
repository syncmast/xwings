#!/bin/bash

cd "Projects/xwings"
g++ -Wall -L./lib -I./include -fPIC -shared -o ./lib/libxexts.so ./source/xexts/*.cpp -lxgdi -lXrandr -ggdb

