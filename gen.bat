@echo off

echo Crafter 2D project generator script, version 1.0 for Windows
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

echo -- Setting up bin directory
cp %1

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
echo     - vs2010
echo  --help
echo     Display this help information
echo.
echo Notes: either give a 'target' or '--help' as argument.
echo.
:End
