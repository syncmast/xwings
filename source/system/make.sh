#!/bin/bash

cd "Projects/xwings"
pwd
g++ -Wall -I./include -fPIC -shared -o ./lib/libsystem.so ./source/system/*.cpp -lrt -ggdb

