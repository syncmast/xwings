#!/bin/bash

cd /home/user/Projects/xwings/examples/xrhello
g++ -Wall -o ./xrhello ./*.c -lX11 -lXrender -ggdb

