JEngine SSE, version 0.4.4, Jeroen Broekhuizen

Thank you for using JEngine SSE. To start the demo application, enter the 
bin directory and execute the jengine.exe file by double clicking on it on windows
or by running ./jengine in the bin directory on Linux.

BUILDING:

Windows
From the command prompt launch gen.bat file with the target IDE you want to
use. Valid options are: cb-gcc (Code::Blocks), vs2005 and vs2008 for the
Visual Studio 2005 and 2008 IDE's (express editions are also supported).

Linux
From the console launch gen.sh with a target. Currently only the
cb-gcc (Code::Blocks) and gnu (generated makefile) targets are supported.

The generated project files can be found in the build directory.

More information about the engine can be found on the documentation pages
of the JEngine SSE website at: 

http://jengine.homedns.org/index.php?page=10


TROUBLESHOOTING:

If for some reason the application does not start, first check your log file located
in the bin directory. It might contain an error which you can solve, like unsupported
hardware.

If it is a software problem, please check whether your issue is already reported
in Mantis. If is not reported yet, please file a new issue, so that I can solve
the problem as soon as possible.

In case you have a functionality request, please report them in the mantis system as well
(if you are unsure, you can first make a post on the forums).

http://jengine.homedns.org/mantis
http://jengine.homedns.org/forum

FAQ:

General
-----------------

* The images look very bad
Make sure your videocard supports 24/32 bits pixels and that the current desktop
resolution is also 32 bits per pixel.

Lua
-----------------

* What Lua commands are available?
Currently this documentation has not been written. To find out what classes and
methods are available, you can take a look in the *.cpp files in the Tolua 
subdirectory of the main package.

Editing
-----------------

* How do I start the world editor?
From version 0.4.3 JEngine SSE comes with an in-game world editor. You can start 
the in-game editor by pressing the F4 key when you are in the main screen. Please 
report any issues/ideas you find in the Mantis system!

* How do I create a new world?
Using the in-game world editor should be used. While in the main menu of the game, press
F3 to start the world editor. Right click in the tile editor (left control) and select
'New' in the popup menu. Fill in the required information in the dialog to actually
create the world.

* How do I add a layer to the world?
While in the world editor right click in the layer list (bottom right list) and select
'New Layer' in the popup menu. In the dialog enter the required information:
 - shader: only the filename (without extention and path)
 - width & height: number of tiles

* How can I add bounds?
You should go into bound mode in the editor by right clicking on the map and click
on 'bound mode' in the popup menu. Then by dragging the mouse over the map you can 
create new bounds.