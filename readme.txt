Crafter 2D, version 0.7, Jeroen Broekhuizen
Copyright 2009-2014

Welcome to this new update of Crafter 2D! This new version improves the workflow
in the Crafter Workshop application. It is now possible to create a new project
from scratch and start editing new worlds! The new world wizard allows you to
import art from another place and copies it to your local project folder.

Editing script files is very limited and involves some outside the editor editing
of project files (check wiki if you are interested). In later versions this will
be greatly improved.

The biggest change in this version is the brand new command line script compiler
YASC. It compiles the files into intermediary language files, which are loaded by
the engine. This makes loading a lot faster at runtime as most of the checking
has already been performed. Crafter Workshop uses this utility to compile the script
files in the script folder.

With this version most of the modules are created (still need to do the box2d module
and perhaps some more). But my main focus will be on enhancing the Crafter Workshop
to improve its usability.

More information about the engine can be found on the wiki at the SourceForge.net
website: http://sourceforge.net/projects/jenginesse/

UPDATING:
When upgrading from a previous version of Crafter 2D (or JEngine SSE), you will need
to use the new basic.fx effect file. It is currently coded used by the engine. The
old format is no longer supported. Check the wiki for more information on the format
of the new effect file.

RUNNING DEMO:
To run the demo application, navigate to the bin directory and execute the game.exe 
file by double clicking on it on windows or by running ./game in the bin directory
on Linux.

TROUBLESHOOTING:

If for some reason the application does not start, first check your log file located
in the bin directory. It might contain an error which you can solve, like unsupported
hardware.

In case you find a software bug in the engine, please report them on in the ticket
application. Please fill in as many fields as possible and a clear description of what
is wrong. A reproduction scenario would be appreciated.

For questions or functionality requests you can move to the forums.

KNOWN BUGS:

* Script compiler leaks memory, will be fixed in future patch
* Bugs/compile errors in scripts might crash the engine (not fool proof yet)

