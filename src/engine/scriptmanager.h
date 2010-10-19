/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef SCRIPT_MANAGER_H_
#define SCRIPT_MANAGER_H_

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <SDL/SDL.h>
#include <list>
#include <string>
#include <vector>

#include "script.h"

class ScriptCollection;

// Module prototype
extern int (*tolua_Module) (lua_State* tolua_S);


/**
@author Jeroen Broekhuizen
\brief The script manager singleton for creating and scheduling new scripts.

@section scriptManager_introduction Introduction
The script manager can be used for creating and managing your script files. The build in
scheduler can also be used for scheduling any script to run at a certain time. Besides complete
scripts it is also possible to just run one line of code only with the executeLine function.

@section scriptManager_initialize Initializing the manager
At start up of the game you must first initialize this singleton script manager to set up
the Lua scripting environment. This is done with the initialize function which creates the
Lua enviroment and loads the default modules, like string, math and io. Besides these default
modules it is also possible to load your own module. This can be done with the loadModule
function as is shown in the next code fragment.

@code
// somewhere in a tolua generated module header file
TOLUA_API int tolua_mymodule_open (lua_State* tolua_S);

void MyGame::loadCustomScriptLibraries()
{
   CScriptManager::getInstance().loadModule(tolua_mymodule_open);
}
@endcode

Finally you can set up some default objects during initialization of your game. JEngine SSE
default loads in the canvas, client, server, etc in thstruct Request {
      const char* fnc;
      Uint32 job;
      int type;
      int startTime;
   };e Lua environment for easy access in
your scripts.

@code
scriptMgr.setObject(&server, "Server", "server");
scriptMgr.setObject(&client, "Client", "client");
@endcode

@section scriptManager_runScripts Running scripts
Main usage of this class is for creating new scripts, or modifying the scripting enviroment,
which is explained mostly in the previous part. There are two main methods available for running
a scripts.

The first method is to create a completely new scripts object, which can either run in the
global Lua environment or in its own sub-enviroment (this will not modify the games global
environment). You should use this method only if you need to run the script multiple times
at a regular interval, otherwise it can become an expensive operation.

When you only need to run a script once or on certain events, then method two is better. This
method uses the getTemporaryScript to retreive a temporary script object (which always runs
in the global enviroment). This call is much cheaper then the previous method, as the object
always exists (it returns a reference to the script object).
*/
class ScriptManager
{
   struct Request {
      const char* fnc;
      Uint32 job;
      Uint32 startTime;
      int type;
   };

public:
   typedef int(*initializer)(lua_State* tolua_S);
   typedef std::list<Request> Requests;
   
   ScriptManager();

   bool                    initialize();
   void                    destroy();
   void                    update( Uint32 tick );

   void                    loadModule(initializer module);
   bool                    loadString(const std::string& code);

   void                    setObject( void* obj, const char* type, const char* var );
   
 // execution
   bool                    executeScript( const std::string& script, bool child = false );
   bool                    executeLine( const char* line);
   Script*                 createScript( bool child = false );
   Script&                 getTemporaryScript();

 // requests
   Uint32                  addRequest( const char* cmd, int type, int time );
   void                    removeRequest( const Uint32 jobid );
   void                    removeAllRequests();

 // function interface
   bool                    hasFunction(const std::string& name) const;
   std::string             generateUniqueFunctionName(const std::string& name);

 // global retreival
   int                     getInt(const char* var);
   bool                    getBool(const char* var);
   void*                   getClass(const char* var);

private:
   void                    operator=( const ScriptManager& mgr );

   void                    registerGlobals();

   void                    initializeCollections();
   
   lua_State*           luaState;
   Script               tempScript;
   Requests             requests;
   ScriptCollection*    MPActiveCollection;
   Uint32               job;
};

#endif // SCRIPT_MANAGER_H_
