#!/bin/sh
cd data
libdir=$(pwd)/libs
export LD_LIBRARY_PATH=$libdir
./cubigrav
