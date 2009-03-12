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

echo - Building tolua files
tolua++ -o src/tolua_editor.cpp -H src/tolua_editor.h -n editor tolua/editor.cpp
tolua++ -o src/tolua_general.cpp -H src/tolua_general.h -n general tolua/general.cpp
tolua++ -o src/tolua_gui.cpp -H src/tolua_gui.h -n gui tolua/gui.cpp
tolua++ -o src/tolua_network.cpp -H src/tolua_network.h -n network tolua/network.cpp

echo - Building project
premake --target $1 --silent

echo
echo Finished
