#!/bin/bash

cd /home/user/Projects/xwings/examples/circles
g++ -Wall -o ./circles ./*.c -lX11 -lXrender -ggdb

