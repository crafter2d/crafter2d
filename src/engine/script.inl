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
#include "defines.h"

INLINE void Script::setState(lua_State* l)
{
   childState = l;
}

/// \fn Script::prepareCall(const char* function)
/// \brief Pushes a parameter on top of the stack which will be use by Lua as a parameter to the function.
/// \param val a floating point value
INLINE void Script::prepareCall (const char* function)
{
	lua_getglobal (childState, function);
}

/// \fn Script::addParam(int val)
/// \brief Pushes a parameter on top of the stack which will be use by Lua as a parameter to the function.
/// \param val a numerical value
INLINE void Script::addParam(int val)
{
	lua_pushnumber(childState, val);
}

/// \fn Script::addParam(float val)
/// \brief Pushes a parameter on top of the stack which will be use by Lua as a parameter to the function.
/// \param val a floating point value
INLINE void Script::addParam(float val)
{
	lua_pushnumber(childState, val);
}

INLINE void Script::addParam(bool val)
{
   lua_pushboolean(childState, val);
}

/// \fn Script::addParam(const char* val)
/// \brief Pushes a parameter on top of the stack which will be use by Lua as a parameter to the function.
/// \param val a pointer to a character string
INLINE void Script::addParam(const char* val)
{
	lua_pushstring(childState, val);
}

/// \fn Script::addParam(void* object, const char* typeName)
/// \brief Pushes a custom type parameter on top of the stack which will be use by Lua as a parameter to the
/// function.
/// \param object a pointer to an object
/// \param typeName the type name of the object (class name)
INLINE void Script::addParam(void* object, const char* typeName)
{
	tolua_pushusertype(childState, object, typeName);
}

/// \fn Script::setSelf(void* object, const char* typeName)
/// \brief Set the global 'self' variable of type typeName for this script. When setting this
/// variable, scripters can use the self variable as object in their scripts.
INLINE void Script::setSelf (void* object, const char* typeName)
{
	tolua_pushusertype (childState, object, typeName);
	lua_setglobal (childState, "self");
}
