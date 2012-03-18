#!/bin/sh

echo JEngine SSE project generator, Version 1.0 for Linux
echo Written by Jeroen Broekhuizen
echo

# check argument
if [ $# != 1 ]; then
   echo Missing required argument, use --help to view possible options.
   exit 127
fi

if [ "$1" == "--help" ]; then
   echo Use gen.sh to set up the required projects for the engine.
   echo
   echo Usage: gen.sh "target"  --help
   echo   target 	Any supported format by Premake
   echo   --help	Display this information
   echo
   echo Notes: either supply a target or '--help' as argument.
   exit 0
fi

echo - Building project
./premake4 $1

echo
echo Finished
