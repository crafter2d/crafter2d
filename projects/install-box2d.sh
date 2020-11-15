#!/bin/sh
set -ex
wget https://github.com/erincatto/Box2D/archive/v2.3.1.zip -O /tmp/box2d-master.zip
unzip /tmp/box2d-master.zip
cd box2D-2.3.1/Box2D/Build
cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr -DBOX2D_BUILD_SHARED=ON -DBOX2D_BUILD_EXAMPLES=OFF ..
make && sudo make install
cd ../../..
