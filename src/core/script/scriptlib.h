/***************************************************************************
 *   Copyright (C) 2009 by Jeroen Broekhuizen                              *
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
#ifndef SCRIPT_LIB_H_
#define SCRIPT_LIB_H_

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

class ScriptLib
{
public:
   ScriptLib(lua_State* pstate);

   lua_State* getLuaState();

private:
   lua_State*  mpState;
};

#ifdef JENGINE_INLINE
#  include "scriptlib.inl"
#endif

#endif // SCRIPT_LIB_H_
