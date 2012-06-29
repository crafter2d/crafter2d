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

Script::Script(ScriptManager& manager, const std::string& name):
   mScriptManager(manager),
   mClassName(name),
   mObject()
{
}

// - Get/set

void Script::setThis(const VirtualObjectReference& object)
{
   mObject = object;
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
   try
   {
      mScriptManager.mpVirtualMachine->execute(mObject, function);
   }
   catch ( VirtualFunctionNotFoundException* pe )
   {
      Log::getInstance().error("Could not find function %s.%s", pe->getClassName().c_str(), pe->getFunction().c_str());
      return false;
   }
   catch ( VirtualException* )
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
   return mScriptManager.mpVirtualMachine->popBoolean();
}

int Script::getInteger()
{
   return mScriptManager.mpVirtualMachine->popInt();
}
