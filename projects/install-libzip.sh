#!/bin/sh
set -ex
wget http://www.nih.at/libzip/libzip-1.0.1.tar.gz -O libzip-1.0.1.tar.gz
tar -zxvf libzip-1.0.1.tar.gz
cd libzip-1.0.1 && ./configure --prefix=/usr && make && sudo make install
cd ..
