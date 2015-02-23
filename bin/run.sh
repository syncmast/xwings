#!/bin/bash

cd "Projects/xwings/bin"
LD_LIBRARY_PATH=$(pwd)/../lib:MALLOC_CHECK_=2 ./project

