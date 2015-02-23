#!/bin/bash

cd "Projects/xwings"
g++ -Wall -I./include -L./lib -o ./bin/project ./examples/progtest/*.cpp -lxexts -lstd -limg -lxgdi -lsystem -ggdb

