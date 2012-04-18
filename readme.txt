JEngine SSE, version 0.4.7, Jeroen Broekhuizen
Copyright 2009-2012

Thanks for downloading this latest version of the JEngine SSE. The biggest change
in this new version, which is released earlier than planned, finally fully supports 
the Linux operating system again!

Besides the Linux compatibility also the demo has gotten some attention. The enemy
model, called Grudge, is now running around and when close to the user hitting him.
In later versions this demo will be extended further to an actual simple game
showing the capabilities of the engine.

To run the demo application, navigate to the bin directory and execute the game.exe 
file by double clicking on it on windows or by running ./game in the bin directory
on Linux.

Note that the engine still in development and parts can be changed or removed. This
software currently is not bug free. Please submit all bugs you find in the bug tracker.
For questions, feature requests, etc. you can post a message on the forum.

Future plans:
The previous plan was to work hard on version 0.5. But new insights in for example
Agile working has resulted in changes of plans. New versions will be released more
often with a selected set of new functionalities/refactorings and bug fixes.

This way users like you can pick up these versions earlier and play-test with them.
New bugs and priorities for new functionalities can then be requested on the forums
so that the functionalities that users want are getting in sooner.

For this to work I will need people to test the engine and report the bugs, think
of new features you would like to have and file them on the forums.

BUILDING:
In order to build the engine, you will need the libaries as explained in the readme
file you can find in the externallibs directory. The libraries in this directory are
no longer updated in the repository (will be removed in the near future).

Windows
From the command prompt launch gen.bat file with the target IDE you want to
use. Valid options are: cb-gcc (Code::Blocks), vs2005, vs2008 and vs2010 for the
Visual Studio IDE's (express editions are also supported).

Linux
Use the gen shell script to generate the project files for the supported IDE or make
platform you use. Then follow the instructions of your IDE or default makefile
commands to build the engine.

The generated project files can be found in the 'build' directory.

More information about the engine can be found on the documentation pages
of the JEngine SSE website at: 

Website: http://jenginesse.sourceforge.net/
Wiki   : https://sourceforge.net/apps/mediawiki/jenginesse
Forum  : https://sourceforge.net/apps/phpbb/jenginesse/
Tracker: https://sourceforge.net/tracker/?group_id=255997&atid=1144317

TROUBLESHOOTING:

If for some reason the application does not start, first check your log file located
in the bin directory. It might contain an error which you can solve, like unsupported
hardware.

In case you find a software bug in the engine, please report them on in the bug tracker
application of the sourceforge website (http://sourceforge.com/projects/jenginesse).
Please fill in as many fields as possible and a clear description of what is wrong. A
reproduction scenario would also be very helpfull.

For questions or functionality requests you can move to the forums.

KNOWN BUGS:

* There are still some (major) memory leaks in the engine
* Bugs/compile errors in scripts might crash the engine (not fool proof yet)

FAQ:

General
-----------------

* The images look very bad
Make sure your videocard supports 24/32 bits pixels and that the current desktop
resolution is also 32 bits per pixel.

* How do I start creating my own game?
When you have the engine up and running with the demo game, you can start developing
your own game by altering the script files and resources. Note that as the engine
is still in development some things might change in later versions which require
you to redo parts of your game. If you need help you can read the wiki or visit the
forums and ask for help.

* My game crashed! Now what?
When you experience any kind of problem, please report the problem in the Tracker
app. This way I can fix your problem as soon as possible. Please add as much information
regarding the things you did that caused the crash.

* I need this functionality badly for my game!
When you some functionality that does not yet exist in the engine you can file a
request on the forum under "Feature requests".

Scripting
-----------------

* What language do you use?
From version 0.4.6 JEngine SSE uses a brand new script language. It is an object
oriented language based on Java and C#. On the wiki you can find more information
about how to program using this language.

Editing
-----------------
General note:
You should use the world editor as supplied in version 0.4.5 to make your levels.

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