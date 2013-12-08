Crafter 2D, version 0.6, Jeroen Broekhuizen
Copyright 2009-2013

Welcome to the first update of Crafter 2D! The topic of this new version is the new
module system. With the modules developers can extend the engine with new features
without having to change the core engine. In this version the content modules have
been introduced. In later versions additional types will be added.

Besides the module system, the Crafter Workshop has been implemented and released
before. With the Workshop you can create your games in a WYSIWYG manner. Currently
the program is still limited in functionality, though in future release more will
be added. In this release it has become part of the standard distribution.

Note: this release works only properly on Windows systems with DirectX. In an upcoming
release the OpenGL path will be fixed and Linux support be restored.

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

