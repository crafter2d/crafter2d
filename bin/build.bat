
@echo off
yascd -p ..\scripts;..\src\mods\mod_ui\scripts -r -o ..\demo\build\scripts ..\demo\scripts\*.as
Compilerd.exe ..\demo\* ..\demo\build -bundle -move