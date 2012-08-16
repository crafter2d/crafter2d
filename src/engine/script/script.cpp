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

#include "script/vm/virtualfunctionnotfoundexception.h"

Script::Script(ScriptManager& manager, const String& name):
   mScriptManager(manager),
   mClassName(name),
   mpObject(NULL)
{
}

// - Get/set

VirtualObject& Script::getThis()
{
   ASSERT_PTR(mpObject);
   return *mpObject;
}

void Script::setThis(VirtualObject& object)
{
   mpObject = &object;
}

void Script::setThis(void* pthis)
{
   if ( !mClassName.isEmpty() )
   {
      mpObject = mScriptManager.mVirtualMachine.instantiateNative(mClassName, pthis, false);
   }
   else
   {
      mpObject = mScriptManager.mVirtualMachine.lookupNative(pthis);
      ASSERT_PTR(mpObject);
   }
}

//-----------------------------------------
// - Run
//-----------------------------------------

/// \fn Script::run(int params=0)
/// \brief Runs the script. You must first call prepareCall and optionaly the addParam functions
/// to set up the function name and arguments.
/// \returns always returns true
bool Script::run(const String& function)
{
   ASSERT_PTR(mpObject);

   try
   {
      mScriptManager.mVirtualMachine.execute(*mpObject, function);
   }
   catch ( VirtualFunctionNotFoundException* pe )
   {
      Log::getInstance().error("Could not find function %s.%s", pe->getClassName().getBuffer(), pe->getFunction().getBuffer());
      return false;
   }
   catch ( VirtualException* pe )
   {
      Log::getInstance().error("Unhandled exception");
      return false;
   }

   return true;
}

//-----------------------------------------
// - Retrieval
//-----------------------------------------

bool Script::getBoolean()
{
   return mScriptManager.mVirtualMachine.popBoolean();
}

int Script::getInteger()
{
   return mScriptManager.mVirtualMachine.popInt();
}

void Script::addParam(void* pobject)
{
   VirtualObject* pvirtualobject = mScriptManager.mVirtualMachine.lookupNative(pobject);
   ASSERT_MSG(pobject != NULL, "Object should have been registered already when using this method.");

   mScriptManager.mVirtualMachine.push(*pvirtualobject);
}

/// \fn Script::addParam(const String& classname, void* pobject)
/// \brief Pushes a custom type parameter on top of the stack which will be use by Lua as a parameter to the
/// function.
/// \param object a pointer to an object
/// \param typeName the type name of the object (class name)
void Script::addParam(const String& classname, void* pobject)
{
   VirtualObject* pvirtualobject = mScriptManager.mVirtualMachine.instantiateNative(classname, pobject, false);

   mScriptManager.mVirtualMachine.push(*pvirtualobject);
}

void Script::addParam(VirtualObject& object)
{
   mScriptManager.mVirtualMachine.push(object);
}
