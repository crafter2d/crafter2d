Crafter 2D, version 0.8, Jeroen Broekhuizen
Copyright 2009-2014

Welcome to this latest update of Crafter 2D! In this release many improvements are
implemented in the Crafter Workshop. The most visible improvement is the redesigned
Tile selection component. In previous versions the tiles were displayed in a long
vertical list, making it difficult to find the right tile. In this version, the new
component displays the tiles in a grid. Effort has also been spend in the scripting
department of the Workshop. It is now possible to compile the scripts. Any errors
are now click-able, leading you directly to the location where the error was found.

In Crafter 2D the particle system has been improved. Using the newly added geometry
shader support it can now render a huge amount of particles. In upcoming versions
more capabilities will be added. Also the OpenGL driver has been repaired and is fully
functional again (on par with the DirectX driver). This has been done for Linux 
support which still is on the roadmap (even numerous GCC compile issues have been
fixed). 

Last but not least, a new text rendering system has been implemented allowing us to
render text on screen. Currently it is only used for rendering the frame counter,
but more uses will be added in a later version.

More information about the engine can be found on the wiki at the SourceForge.net
website: http://sourceforge.net/projects/jenginesse/

REMARKS:
This engine still is in pre-alpha phase and many developments still take place, so
features can be broken, replaced or removed in later versions. The aim is to keep
games created in these early versions running or able to be upgraded with minimal effort.

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

