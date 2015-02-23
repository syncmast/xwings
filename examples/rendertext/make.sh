#!/bin/bash

cd /home/user/Projects/xwings/examples/rendertext
g++ -Wall -o ./rendertext.exe ./*.c -lX11 -lXrender -ggdb

