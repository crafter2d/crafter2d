#!/bin/sh
set -ex
wget https://github.com/premake/premake-core/releases/download/v5.0.0-alpha6/premake-5.0.0-alpha6-linux.tar.gz -O /tmp/premake5.tar.gz
tar -xvf /tmp/premake5.tar.gz
export PATH=$PATH:$PWD/premake5/bin/release
