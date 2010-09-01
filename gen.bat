@echo off

echo JEngine SSE project generator, Version 1.0 for Windows
echo Written by Jeroen Broekhuizen
echo.

if "%1" == "" goto Help
if "%1" == "--help" goto Help
if not "%1" == "cb-gcc" (
   if not "%1" == "vs2005" (
      if not "%1" == "vs2008" ( 
         if not "%1" == "vs2010" (
            echo "%1" is not a valid target, check help for supported targets.
            goto End )
      )
   )
)

if not exist "..\externallibs" (
   echo "Make sure you also check out the externallibs directory"
   goto End
)

echo -- Setting up bin directory
cp %1

echo -- Building tolua files
tolua\tolua++.exe -o src\tolua_editor.cpp -H src\tolua_editor.h -n editor tolua\editor.cpp
tolua\tolua++.exe -o src\tolua_general.cpp -H src\tolua_general.h -n general tolua\general.cpp
tolua\tolua++.exe -o src\tolua_gui.cpp -H src\tolua_gui.h -n gui tolua\gui.cpp
tolua\tolua++.exe -o src\tolua_network.cpp -H src\tolua_network.h -n network tolua\network.cpp

echo -- Building project
echo.
premake4 %1

echo.
echo Finished

goto End

:Help
echo Use gen.bat to create the required projects for the engine.
echo.
echo Usage: gen "target" --help
echo  target can be
echo     - cb-gcc (default)
echo     - vc2005
echo     - vc2008
echo  --help
echo     Display this help information
echo.
echo Notes: either give a 'target' or '--help' as argument.
echo.
:End
