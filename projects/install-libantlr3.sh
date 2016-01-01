#!/bin/sh
set -ex
wget http://www.antlr3.org/download/C/libantlr3c-3.4.tar.gz -O libantlr3.tar.gz
tar -zxvf libantlr3.tar.gz
cd libantlr3c-3.4 && ./configure --prefix=/usr --enable-64bit && make && sudo make install
cd ..


