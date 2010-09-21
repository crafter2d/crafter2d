@echo off
echo JEngine SSE Static source code analysis launcher.

if "%1" == "" goto Help
if "%1" == "--help" goto Help

echo Running analysis, this may take a couple of minutes...

set CURDIR=%CD%
chdir /d %1
yasca -o "%CURDIR%\yasca_report.html" "%CURDIR%\src"
chdir /d %CURDIR%

echo Finished.

if "%2" == "--noshow" goto End
start yasca_report.html
goto End

:Help
echo Use yasca.bat to run the static code analysis tool Yasca.
echo http://www.scovetta.com/yasca.html
echo.
echo Usage: yasca "path-to-yasca" [options]
echo  path-to-yasca : the full path to the yasca directory.
echo  --noshow : only generate the analysis report
echo  --help : show this help information
goto End

:End
