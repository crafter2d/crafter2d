#!/bin/sh
set -ex
wget https://github.com/erincatto/Box2D/archive/master.zip -O /tmp/box2d-master.zip
unzip /tmp/box2d-master.zip
cd Box2D-master/Box2D/Build
cmake -DBOX2D_BUILD_SHARED=ON -DBOX2D_BUILD_EXAMPLES=OFF ..
make
cd ../../..
