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
#include "scriptmanager.h"
#ifndef JENGINE_INLINE
#  include "scriptmanager.inl"
#endif

#include <sstream>

#include "core/smartptr/autoptr.h"
#include "core/log/log.h"

#include "script/vm/virtualcontext.h"
#include "script/vm/virtualmachine.h"
#include "script/vm/virtualobject.h"

#include "script.h"
#include "scriptobject.h"

#include "engine/script_engine.h"

ScriptManager::ScriptManager():
   mVirtualContext(),
   mVirtualMachine(mVirtualContext)
{
}

ScriptManager::~ScriptManager()
{
   destroy();
}

/// \fn ScriptManager::initialize()
/// \brief Initializes the Lua scripting environment with the default modules and the
/// engines exported classes. Call this function before any of the other functions.
/// \returns false if the Lua state could not be created, true otherwise
bool ScriptManager::initialize()
{
   mVirtualMachine.initialize();

   script_engine_register(*this);

	return true;
}

/// \fn ScriptManager::destroy()
/// \brief Releases the Lua scripting environment. After calling this function you must
/// first call initialize() before calling any other functions
/// \returns false if the Lua state could not be created, true otherwise
void ScriptManager::destroy()
{
}

// - Loading

/// \fn ScriptManager::load(const String& classname, void* pobject, bool owned)
/// \brief Loads a script class from file into the virtual machine and optionally attaches
/// a pointer to the native class (required for native classes).
/// \param classname Fully qualified name of the class
/// \param pobject Pointer to the native class (may be NULL)
/// \param owned If true, the virtual machine takes ownership of the pointer, and deletes it
/// when the virtual class is deleted.
/// \returns A script object if class is successfully instantiated, or NULL otherwise
Script* ScriptManager::load(const String& classname, void* pobject, bool owned)
{
   Script* pscript = NULL;

   VirtualObject* pvirtualobject = mVirtualMachine.instantiateNative(classname, pobject, owned);
   if ( pvirtualobject != NULL )
   {
      pscript = new Script(*this);
      pscript->setThis(*pvirtualobject);
   }

   return pscript;
}


// - Operations

/// \fn ScriptManager::addRootObject(VirtualObject& object)
/// \brief Adds a root object to the garbage collector. A root object is an object
/// that is used by the garbage collector as starting point for usage detection.
/// \param object Root object to be added
void ScriptManager::addRootObject(VirtualObject& object)
{
   mVirtualMachine.addRootObject(object);
}

/// \fn ScriptManager::executeScript(const String& classname, const String& function)
/// \brief Runs a class function once. The function must be a static function.
/// \param classname Name of the class
/// \param function Name of the static function to be executed
bool ScriptManager::executeScript(const String& classname, const String& function)
{
   return mVirtualMachine.execute(classname, function);
}
