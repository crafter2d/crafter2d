@echo off
echo Running tolua to generate code...
echo Jeroen Broekhuizen, 2005

tolua++ -o tolua_general.cpp -H tolua_general.h -n general general.cpp
tolua++ -o tolua_gui.cpp -H tolua_gui.h -n gui gui.cpp
tolua++ -o tolua_network.cpp -H tolua_network.h -n network network.cpp
tolua++ -o tolua_editor.cpp -H tolua_editor.h -n editor editor.cpp
tolua++ -o tolua_physics.cpp -H tolua_physics.h -n physics physics.cpp

echo Copying to the script directory...
echo copy /Y tolua_* ..\src\tolua_*

echo Ready!