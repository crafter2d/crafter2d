/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jeroen@jengine.homedns.org                                            *
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

#include "scriptmanager.h"
#ifndef JENGINE_INLINE
#  include "scriptmanager.inl"
#endif

#include <iostream>
#include <sstream>
#include <tolua++.h>

#include "script/scriptcollection.h"

#include "console.h"
#include "game.h"
#include "gameconfiguration.h"

#include "tolua_editor.h"
#include "tolua_general.h"
#include "tolua_gui.h"
#include "tolua_network.h"

//////////////////////////////////////////////////////////////////////////
// - Lua error interface
//////////////////////////////////////////////////////////////////////////

int onLuaAlert(lua_State *L)
{
   Console::getLog() << "Lua error: " << lua_tostring(L, -1) << "\n";
   return 0;
}

int onLuaPanic (lua_State *L)
{
   Console::getLog() << "Lua error: " << lua_tostring(L, -1) << "\n";
   return 0;
}

//////////////////////////////////////////////////////////////////////////
// - ScriptManager stuff
//////////////////////////////////////////////////////////////////////////

ScriptManager::Collections* ScriptManager::MPCollections = NULL;

ScriptManager::ScriptManager():
   luaState(NULL),
   tempScript(),
   requests(),
   MPActiveCollection(NULL),
   job(0)
{
}

/// \fn ScriptManager::registerCollection(std::string name, initializer function)
/// \brief Registers a collection of script functions.
ScriptCollection* ScriptManager::registerCollection(std::string name, initializer function)
{
   if ( MPCollections == NULL )
      MPCollections = new Collections();

   ScriptCollection* pcollection = new ScriptCollection(name, function);
   Collections().push_back(pcollection);
   return pcollection;
}

/// \fn ScriptManager::initialize()
/// \brief Initializes the Lua scripting environment with the default modules and the
/// engines exported classes. Call this function before any of the other functions.
/// \returns false if the Lua state could not be created, true otherwise
bool ScriptManager::initialize()
{
	// see if we can link lua
	luaState = lua_open();
	if (luaState == NULL)
		return false;

   lua_register(luaState, "_ALERT", onLuaAlert);
   lua_atpanic(luaState, onLuaPanic);

	luaL_openlibs(luaState);

   tolua_open(luaState);
	tolua_general_open(luaState);
   tolua_gui_open(luaState);
   tolua_network_open(luaState);
   tolua_editor_open(luaState);

   initializeCollections();

   registerGlobals();

   tempScript.setState(luaState);
	return true;
}

/// \fn ScriptManager::initializeCollections()
/// \brief Initialize all registered collections in lua.
void ScriptManager::initializeCollections()
{
   for ( Collections::size_type idx = 0; idx < Collections().size(); ++idx )
   {
      ScriptCollection* pcollection = Collections()[idx];
      activeCollection(pcollection);

      pcollection->initialize(luaState);
   }
}

/// \fn ScriptManager::destroy()
/// \brief Releases the Lua scripting environment. After calling this function you must
/// first call initialize() before calling any other functions
/// \returns false if the Lua state could not be created, true otherwise
void ScriptManager::destroy()
{
	if (luaState)
   {
		// release lua
		lua_close (luaState);
		luaState = NULL;
	}
}

/// \fn ScriptManager::update(Uint32 tick)
/// \brief Checks the request list and executes scripts when necessary.
void ScriptManager::update(Uint32 tick)
{
   Requests::iterator it = requests.begin();
   while ( it != requests.end() )
   {
      Requests::iterator next = it;
      Request& request = *it;
      ++next;

      if ( tick >= request.startTime )
      {
         // execute the function
         Script& script = getTemporaryScript();
         script.prepareCall(request.fnc);
         script.run();

         // remove request from the list
         requests.erase(it);
      }

      it = next;
   }
}

/// \fn ScriptManager::loadModule(int (*tolua_Module)(lua_State* tolua_S))
/// \brief Calls the function tolua_Module to load it in the global Lua state. These tolua_Module
/// functions are generated automatically by tolua.
/// \param *tolua_Module pointer to the module function generated by tolua
void ScriptManager::loadModule (initializer module)
{
	// load the module in this state
	(*module)(luaState);
}

/// \fn ScriptManager::executeScript(const char* script)
/// \brief Runs a script once in either the main lua state, or a child state.
/// \param script Name of the script file
/// \param child Flag which should be set if the script should be run in a child state (default is false)
bool ScriptManager::executeScript(const char* script, bool child)
{
   const std::string& scriptpath = Game::getInstance().getConfiguration().getScriptPath();
   std::string filename = scriptpath + script;

   Script luaScript(luaState, child);
   return luaScript.load(filename);
}

/// \fn ScriptManager::executeLine(const char* line, bool child)
/// \brief Executes a line of Lua code.
bool ScriptManager::executeLine(const char* line)
{
   if ( luaL_dostring(luaState, line) != 0 )
   {
      Console::getInstance().printf("%s", lua_tostring(luaState, -1));
      return false;
   }
   return true;
}

/// \fn ScriptManager::setObject(void* obj, const char* type, const char* var)
/// \brief Sets a global variable in the Lua scripting environment with name 'var'.
/// \param obj pointer to a valid object
/// \param type type of the object (must be a valid type specifier)
/// \param var name of the variable as known within Lua
void ScriptManager::setObject(void* obj, const char* type, const char* var)
{
   tolua_pushusertype (luaState, obj, type);
	lua_setglobal (luaState, var);
}

//////////////////////////////////////////////////////////////////////////
// - Registered functions
//////////////////////////////////////////////////////////////////////////

static int schedule(lua_State* L)
{
   ASSERT(lua_gettop(L) == 2);

   const char* pfunction = luaL_checkstring(L, 1);
   int delay             = luaL_checkint(L, 2);

   int jobid = ScriptManager::getInstance().addRequest(pfunction, 1, delay);

   lua_pushnumber(L, jobid);

   return 1;
}

static int unschedule(lua_State* L)
{
   ASSERT(lua_gettop(L) == 1);

   int jobid = luaL_checkint(L, 1);
   ScriptManager::getInstance().removeRequest(jobid);

   return 0;
}

static int include(lua_State* L)
{
   ASSERT(lua_gettop(L) == 1);

   const char* pfile = luaL_checkstring(L, 1);
   ScriptManager::getInstance().executeScript(pfile, false);

   return 0;
}

void ScriptManager::registerGlobals()
{
   lua_register(luaState, "schedule", schedule);
   lua_register(luaState, "unschedule", unschedule);
   lua_register(luaState, "include", include);
}

//////////////////////////////////////////////////////////////////////////
// - Scheduling interface
//////////////////////////////////////////////////////////////////////////

/// \fn ScriptManager::addRequest(const char* fnc, int type, int time)
/// \brief Add a script request to the list. Will be executed after a certain
/// time interval.
Uint32 ScriptManager::addRequest(const char* fnc, int type, int time)
{
   Request req;
   req.job = job;
   req.fnc = fnc;
   req.type = type;
   req.startTime = SDL_GetTicks() + time;
   requests.push_back(req);
   job++;

   return req.job;
}

/// \fn ScriptManager::removeRequest(const int jobid)
/// \brief Removes a job from the request list.
void ScriptManager::removeRequest(const Uint32 jobid)
{
   std::list<Request>::iterator it = requests.begin();
   for (; it != requests.end(); ++it) {
      if ((*it).job == jobid) {
         requests.erase(it);
         return;
      }
   }
}

/// \fn ScriptManager::removeAllRequests()
/// \brief Removes all requests from the list.
void ScriptManager::removeAllRequests()
{
   requests.clear();
}

//////////////////////////////////////////////////////////////////////////
// - Function interface
//////////////////////////////////////////////////////////////////////////

bool ScriptManager::hasFunction(const std::string& name) const
{
   lua_pushstring(luaState, name.c_str());
   lua_rawget(luaState, LUA_GLOBALSINDEX);
   bool res = lua_isfunction(luaState, -1);
   lua_pop(luaState, 1);

   return res;
}

std::string ScriptManager::generateUniqueFunctionName(const std::string& name)
{
   int index = 0;
   std::ostringstream uniquename;

   uniquename << name;

   while ( hasFunction(uniquename.str()) )
   {
      uniquename.clear();
      uniquename << name << index;
   }

   return uniquename.str();
}

//////////////////////////////////////////////////////////////////////////
// - Global retreival
//////////////////////////////////////////////////////////////////////////

int ScriptManager::getInt(const char* var)
{
   lua_getglobal(luaState, var);
   if (!lua_isnumber(luaState, -1))
      return -1;
   else
      return (int)lua_tonumber(luaState, -1);
}

bool ScriptManager::getBool(const char* var)
{
   lua_getglobal(luaState, var);
   return (bool)lua_toboolean(luaState, -1);
}

void* ScriptManager::getClass(const char* var)
{
   lua_getglobal(luaState, var);
   return *((void**)lua_touserdata(luaState,-1));
}
