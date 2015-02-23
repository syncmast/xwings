#!/bin/bash

cd /home/user/Projects/xwings
g++ -Wall -I./include -L./lib/debug -o ./examples/HandleMsg/HandleMsg ./examples/HandleMsg/*.cpp -lsys -lxgdi -lstd -ggdb

