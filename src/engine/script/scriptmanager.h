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
#ifndef SCRIPT_MANAGER_H_
#define SCRIPT_MANAGER_H_

#include "core/defines.h"

#include "engine/engine_base.h"

#include <list>
#include <vector>

#include "script/vm/virtualmachine.h"

class Script;
class String;
class VirtualObject;

/**
@author Jeroen Broekhuizen
\brief The script manager for loading and executing scripts.

@section scriptManager_introduction Introduction
The script manager is a wrapper around the virtual machine. It makes using the script engine
in your game easier and cleaner.

@section scriptManager_rootobjects Root Objects
The garbage collector used by the script engine, uses the so called 'mark & sweep' algorithm.
This two step process first marks objects that are still in use and then frees all objects that
have not been marked. Root objects are used as starting point for the marking phase. So, any 
object that can not be reached through a root object, will be released as it's not in use any
more.

A typical example of a root object would be the server class. The server keeps track of all
the entities, players, etc. The engine marks both server and client automatically as root
objects.

A common pitfall are native objects. It is easy to create a native class and script class 
counter part. You then have to ensure whenever this class is instantiated, that you store
a reference to it as it otherwise will be freed soon again.
*/
class ENGINE_API ScriptManager
{
public:   
   ScriptManager();
   ~ScriptManager();

   bool                    initialize();
   void                    destroy();
   
 // loading
   Script*                 load(const String& classname, void* pobject, bool owned);
   
 // execution
   bool                    executeScript(const String& classname, const String& function);
   
 // operations
   void                    addRootObject(VirtualObject& object);

private:
   friend class Script;
   friend class ScriptRegistrator;
   
   void operator=( const ScriptManager& mgr );

   void registerGlobals();
  
   VirtualContext    mVirtualContext;
   VirtualMachine    mVirtualMachine;
};

#endif // SCRIPT_MANAGER_H_
