#!/bin/sh
set -ex

git clone https://github.com/OpenELEC/libsquish.git libsquish
cd libsquish
make && sudo make INSTALL_DIR=/usr install
