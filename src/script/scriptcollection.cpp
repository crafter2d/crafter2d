/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "scriptcollection.h"
#include "../script.h"
#include "scriptfunction.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

ScriptCollection::ScriptCollection(std::string name, initializer function):
   MName(name),
   MInitFunction(function),
   MFunctions()
{
}

ScriptCollection::~ScriptCollection()
{
   destroy();
}

void ScriptCollection::initialize(lua_State* tolua_S)
{
   (*MInitFunction)(tolua_S);
}

void ScriptCollection::destroy()
{
   for ( int idx = 0; idx < MFunctions.size(); ++idx )
   {
      delete MFunctions[idx];
   }
   MFunctions.clear();
}

void ScriptCollection::add(ScriptFunction* pfunction)
{
   MFunctions.push_back(pfunction);
}
