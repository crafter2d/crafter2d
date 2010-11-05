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
#include "script.h"
#ifndef JENGINE_INLINE
#  include "script.inl"
#endif

#include "core/log/log.h"

#include "core/vfs/file.h"
#include "core/vfs/filesystem.h"

#include "core/autoptr.h"

#include "scriptcontext.h"

Script::Script():
   childState(0),
   child(false)
{
}

/*!
	\fn Script::Script(lua_State* l)
	\brief Initializes member variables
*/
Script::Script(lua_State* l, bool c):
   childState(l),
   child(c)
{
   //if (child) childState = lua_newthread(l);
   //else childState = l;
}

//-----------------------------------------
// - Load & run
//-----------------------------------------

/// \fn Script::load(const char* file)
/// \brief Loads a script file into the lua state and prepares it to run.
/// \returns true is loading was successfull, false otherwise
bool Script::load(ScriptContext& context, const std::string& filename)
{
   AutoPtr<File> file = FileSystem::getInstance().open(filename, File::ERead);
   
   if ( file.hasPointer() && file->isValid() )
   {
      int size = file->size();
      AutoPtr<char> data = new char[size+1];
      memset(data.getPointer(), 0, size);
      file->read(data.getPointer(), size);
      std::string code = data.getPointer();

      return loadString(context, code);
   }
   else
   {
      std::string error = "Could not load script file " + filename;
      context.info(Log::eError, error);
   }

   return true;
}

/// \fn Script::loadString(const std::string& code)
/// \brief Loads the given code string into the lua state. When using this function, you must
/// call run to execute the code.
/// \returns true is successfull, false otherwise
bool Script::loadString(ScriptContext& context, const std::string& code)
{
   if ( luaL_loadbuffer(childState, code.c_str(), code.length(), NULL) != 0 )
   {
      return false;
   }

   return true;
}

/// \fn Script::run(int params=0, int returns=0)
/// \brief Runs the script. You must first call prepareCall and optionaly the addParam functions
/// to set up the function name and arguments.
/// \returns always returns true
bool Script::run(ScriptContext& context, int params, int returns)
{
	// call the function
   if ( lua_pcall(childState, params, returns, 0) != 0 ) 
   {
      std::string error = std::string("An error occured while running script: ") + lua_tostring(childState, -1);
      context.info(Log::eError, error);
      return false;
   }
   return true;
}

//-----------------------------------------
// - Retreival
//-----------------------------------------

bool Script::getBoolean()
{
   bool ret = false;
   if ( lua_isboolean(childState, -1) )
      ret = static_cast<bool>(lua_toboolean(childState, -1));

   lua_pop(childState, 1);
   return ret;
}

int Script::getInteger()
{
   int ret = 0;
   if ( lua_isnumber(childState, -1) )
      ret = static_cast<int>(lua_tonumber(childState, -1));

   lua_pop(childState, 1);
   return ret;
}
