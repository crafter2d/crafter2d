#!/bin/sh
set -ex
#wget https://github.com/premake/premake-core/releases/download/v5.0.0-alpha6/premake-5.0.0-alpha6-src.zip -O premake5-src.zip
#unzip premake5-src.zip
#cd premake-5.0.0-alpha6/build/gmake.unix
#make
#cd ../../../

git clone https://github.com/premake/premake-core.git premake-core
cd premake-core
git submodule init
git submodule update
make -f Bootstrap.mak linux
