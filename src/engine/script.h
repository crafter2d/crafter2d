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
#ifndef _SCRIPTMANAGER_H_
#define _SCRIPTMANAGER_H_

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <SDL/SDL.h>
#include <list>
#include <vector>
#include <string>
#include <tolua++.h>

/**
@author Jeroen Broekhuizen
\brief Implements a general script object.

This class implements a basic script object. It has its own Lua child state in which you
can run this script. Creating this script object should be done with the
ScriptManager::createScript() function. Currently the update function is not yet
implemented and therefor shouldn't be used.
*/
class Script
{
public:
               Script();
   explicit    Script(lua_State* l, bool c=false);

   bool        load (const std::string& file);
   bool        loadString(const std::string& code);

   void        addParam (int val);
   void        addParam (float val);
   void        addParam (bool val);
   void        addParam (const char* str);
   void        addParam (void* object, const char* typeName);

   bool        getBoolean();
   int         getInteger();

   void        prepareCall(const char* function);
   bool        run(int params=0, int returns=0);

   void        setSelf(void* p, const char* typeName);
   void        setState(lua_State* l);

protected:
   lua_State *childState; /**< Lua state of this object */
   bool child;
};

#ifdef JENGINE_INLINE
#  include "script.inl"
#endif

#endif
