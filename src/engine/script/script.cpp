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
#include "core/smartptr/autoptr.h"

Script::Script(ScriptManager& manager, const std::string& name):
   mScriptManager(manager),
   mClassName(name),
   mObject()
{
}

// - Get/set

void Script::setThis(void* pthis)
{
   if ( !mClassName.empty() )
   {
      mObject = mScriptManager.mpVirtualMachine->instantiateNative(mClassName, pthis, false);
   }
}

//-----------------------------------------
// - Run
//-----------------------------------------

/// \fn Script::run(int params=0)
/// \brief Runs the script. You must first call prepareCall and optionaly the addParam functions
/// to set up the function name and arguments.
/// \returns always returns true
bool Script::run(const std::string& function)
{
   mScriptManager.mpVirtualMachine->execute(mObject, function);
   return true;
}

//-----------------------------------------
// - Retrieval
//-----------------------------------------

bool Script::getBoolean()
{
   return mScriptManager.mpVirtualMachine->popBoolean();
}

int Script::getInteger()
{
   return mScriptManager.mpVirtualMachine->popInt();
}

/// \fn Script::addParam(const std::string& classname, void* pobject)
/// \brief Pushes a custom type parameter on top of the stack which will be use by Lua as a parameter to the
/// function.
/// \param object a pointer to an object
/// \param typeName the type name of the object (class name)
void Script::addParam(const std::string& classname, void* pobject)
{
   VirtualObjectReference ref(mScriptManager.mpVirtualMachine->instantiateNative(classname, pobject, false));

   mScriptManager.mpVirtualMachine->push(ref);
}