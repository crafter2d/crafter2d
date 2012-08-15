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
#ifndef SCRIPT_H_
#define SCRIPT_H_

#include "engine/engine_base.h"

#include "core/string/string.h"

class ScriptManager;
class ScriptObject;
class VirtualObject;

/**
@author Jeroen Broekhuizen
\brief Implements a general script object.

This class implements a basic script object. It has its own Lua child state in which you
can run this script. Creating this script object should be done with the
ScriptManager::createScript() function. Currently the update function is not yet
implemented and therefor shouldn't be used.
*/
class ENGINE_API Script
{
public:
   explicit    Script(ScriptManager& manager, const String& name = "");
   
   void        addParam(int val);
   void        addParam(float val);
   void        addParam(bool val);
   void        addParam(const std::string& val);
   void        addParam(void* pobject);
   void        addParam(const std::string& classname, void* pobject);
   void        addParam(VirtualObject& object);

   bool        getBoolean();
   int         getInteger();

   VirtualObject& getThis();
   void           setThis(VirtualObject& object);
   void           setThis(void* pthis);

   bool        run(const String& function);

private:
   ScriptManager&  mScriptManager;
   String          mClassName;
   VirtualObject*  mpObject;
};

#ifdef JENGINE_INLINE
#  include "script.inl"
#endif

#endif
