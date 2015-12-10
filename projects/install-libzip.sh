#!/bin/sh
set -ex
wget http://www.nih.at/libzip/libzip-1.0.1.tar.gz -O /tmp/libzip-1.0.1.tar.gz
tar -zxvf /tmp/libzip-1.0.1.tar.gz
cd libzip-1.0.1
./configure
make
cd ..
