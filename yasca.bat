echo off
echo JEngine SSE Static source code analysis launcher.

if "%1" == "" goto Help
if "%1" == "--help" goto Help

echo Running analysis, this may take a couple of minutes...

set CURDIR=%CD%
chdir /d %1
yasca -s -o jenginesse_yasca.html "%CURDIR%\src"
chdir /d %CURDIR%

echo Finished.

cp %1\jenginesse_yasca.html .
del %1\jenginesse_yasca.html

if "%2" == "--show" goto Show
goto End

:Help
echo Use yasca.bat to run the static code analysis tool Yasca.
echo http://www.scovetta.com/yasca.html
echo.
echo Usage: yasca "path-to-yasca" [options]
echo  path-to-yasca : the full path to the yasca directory.
echo  --show : show the analysis report
echo  --help : show this help information
goto End

:Show
start jenginesse_yasca.html

:End
