#!/bin/sh
set -ex
wget https://github.com/premake/premake-core/releases/download/v5.0.0-alpha6/premake-5.0.0-alpha6-src.zip -O /tmp/premake5-src.zip
unzip /tmp/premake5-src.zip
cd premake-5.0.0-alpha6/build/gmake.unix
make
cd ../../../
cd premake-5.0.0-alpha6/bin/release
ls -all
./premake5 --version
cd ../../../
