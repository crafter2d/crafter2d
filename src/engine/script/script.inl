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
#include "core/defines.h"

#include "script/vm/virtualmachine.h"

#include "scriptmanager.h"
#include "scriptobject.h"

/// \fn Script::addParam(int val)
/// \brief Pushes a parameter on top of the stack which will be use by Lua as a parameter to the function.
/// \param val a numerical value
INLINE void Script::addParam(int val)
{
   mScriptManager.mpVirtualMachine->push(val);
}

/// \fn Script::addParam(float val)
/// \brief Pushes a parameter on top of the stack which will be use by Lua as a parameter to the function.
/// \param val a floating point value
INLINE void Script::addParam(float val)
{
   mScriptManager.mpVirtualMachine->push(val);
}

/// \fn Script::addParam(bool val)
/// \brief Pushes a parameter on top of the stack which will be use by Lua as a parameter to the function.
/// \param val a floating point value
INLINE void Script::addParam(bool val)
{
   mScriptManager.mpVirtualMachine->push(val);
}

/// \fn Script::addParam(IScriptable& val)
/// \brief Pushes the scriptable object on the top of the stack.
INLINE void Script::addParam(IScriptable& val)
{
   mScriptManager.mpVirtualMachine->push(val.getScript().mObject);
}

/// \fn Script::addParam(const char* val)
/// \brief Pushes a parameter on top of the stack which will be use by Lua as a parameter to the function.
/// \param val a pointer to a character string
INLINE void Script::addParam(const std::string& val)
{
   mScriptManager.mpVirtualMachine->push(val);
}

/// \fn Script::addParam(const VirtualObjectReference& object)
/// \brief Pushes the object on the stack as argument to the function
INLINE void Script::addParam(const VirtualObjectReference& object)
{
   mScriptManager.mpVirtualMachine->push(object);
}
