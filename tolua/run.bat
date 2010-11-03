@echo off

set silent=true
if not "%1" == "--silent" set silent=

if not defined silent (
	echo Running tolua to generate code...
	echo Jeroen Broekhuizen, 2005
)

rem core tolua files
tolua++ -o ..\src\core\script\tolua_core.cpp -H ..\src\core\script\tolua_core.h -n core core.cpp

rem engine tolua files
tolua++ -o ..\src\engine\tolua_general.cpp -H ..\src\engine\tolua_general.h -n general general.cpp
tolua++ -o ..\src\engine\tolua_network.cpp -H ..\src\engine\tolua_network.h -n network network.cpp
tolua++ -o ..\src\engine\tolua_physics.cpp -H ..\src\engine\tolua_physics.h -n physics physics.cpp

rem user interface files
tolua++ -o ..\src\gui\tolua_gui.cpp -H ..\src\gui\tolua_gui.h -n gui gui.cpp
tolua++ -o ..\src\gui\tolua_editor.cpp -H ..\src\gui\tolua_editor.h -n editor editor.cpp

rem game interface files
tolua++ -o ..\src\game\tolua_game.cpp -H ..\src\game\tolua_game.h -n game game.cpp

if not defined silent (
	echo Ready!
)
