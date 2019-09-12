#!/bin/sh
set -ex

git clone https://github.com/google/googletest.git googletest
cd googletest
cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr
make && sudo make install
cd ../
