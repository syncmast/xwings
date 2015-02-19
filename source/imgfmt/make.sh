#!/bin/bash

cd "Projects/xwings"
g++ -Wall -L./lib -I./include -fPIC -shared -o ./lib/libimg.so ./source/imgfmt/*.cpp -lxgdi -lXpm -ljpeg -lpng -ggdb

