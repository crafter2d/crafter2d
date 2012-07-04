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
   mpVirtualContext(new VirtualContext()),
   mpVirtualMachine(NULL),
   mpScript(NULL),
   requests(),
   job(0),
   mChild(false)
{
}

ScriptManager::ScriptManager(VirtualContext& context):
   mpVirtualContext(&context),
   mpVirtualMachine(NULL),
   mpScript(NULL),
   requests(),
   job(0),
   mChild(true)
{
}

ScriptManager::~ScriptManager()
{
   delete mpScript;
   mpScript = NULL;

   delete mpVirtualMachine;
   mpVirtualMachine = NULL;

   if ( !mChild )
   {
      delete mpVirtualContext;
      mpVirtualContext = NULL;
   }
}

/// \fn ScriptManager::initialize()
/// \brief Initializes the Lua scripting environment with the default modules and the
/// engines exported classes. Call this function before any of the other functions.
/// \returns false if the Lua state could not be created, true otherwise
bool ScriptManager::initialize()
{
   ASSERT_PTR(mpVirtualContext);
   mpVirtualMachine = new VirtualMachine(*mpVirtualContext);
   mpVirtualMachine->initialize();

   script_engine_register(*this);

   mpScript = loadNative("engine.game.ScriptManager", this, false);
	return mpScript != NULL;
}

/// \fn ScriptManager::destroy()
/// \brief Releases the Lua scripting environment. After calling this function you must
/// first call initialize() before calling any other functions
/// \returns false if the Lua state could not be created, true otherwise
void ScriptManager::destroy()
{
   delete mpScript;
   mpScript = NULL;

   delete mpVirtualMachine;
   mpVirtualMachine = NULL;

   delete mpVirtualContext;
   mpVirtualContext = NULL;
}

// - Loading

Script* ScriptManager::loadNative(const std::string& classname, void* pobject, bool owned)
{
   Script* pscript = NULL;

   VirtualObject* pvirtualobject = mpVirtualMachine->instantiateNative(classname, pobject, owned);
   if ( pvirtualobject != NULL )
   {
      pscript = new Script(*this);
      pscript->setThis(*pvirtualobject);
   }

   return pscript;
}


// - Operations

ScriptManager*  ScriptManager::spawnChild()
{
   ASSERT_PTR(mpVirtualContext);
   ScriptManager* pmanager = new ScriptManager(*mpVirtualContext);
   if ( !pmanager->initialize() )
   {
      delete pmanager;
      pmanager = NULL;
   }
   return pmanager;
}

VirtualObject& ScriptManager::shareObject(const VirtualObject& origin)
{
   VirtualObject* pshare = mpVirtualMachine->instantiateShare(origin);
   ASSERT_PTR(pshare);

   return *pshare;
}

/// \fn ScriptManager::executeScript(const std::string& classname, const std::string& function)
/// \brief Runs a class function once. The function must be a static function.
/// \param classname Name of the class
/// \param function Name of the static function to be executed
bool ScriptManager::executeScript(const std::string& classname, const std::string& function)
{
   ASSERT_PTR(mpVirtualMachine);
   return mpVirtualMachine->execute(classname, function);
}

//-----------------------------------------
// - Scheduling interface
//-----------------------------------------

/// \fn ScriptManager::update(Uint32 tick)
/// \brief Checks the request list and executes scripts when necessary.
void ScriptManager::update(float delta)
{
   Requests::iterator it = requests.begin();
   while ( it != requests.end() )
   {
      Requests::iterator next = it;
      Request& request = *it;
      ++next;

      request.mCurrentTime += delta;

      if ( request.mCurrentTime >= request.mStartTime )
      {
         // execute the function
         mpScript->run(request.mFunction);

         // remove request from the list
         requests.erase(it);
      }

      it = next;
   }
}

/// \fn ScriptManager::schedule(const std::string& fnc, float time)
/// \brief Schedules a script for running after time milli seconds passed.
uint ScriptManager::schedule(const std::string& fnc, float time)
{
   Request req;
   req.mJobId       = job++;
   req.mFunction    = fnc;
   req.mStartTime   = time;
   req.mCurrentTime = 0;
   requests.push_back(req);

   return req.mJobId;
}

/// \fn ScriptManager::unschedule(uint jobid)
/// \brief Removes a job from the request list.
void ScriptManager::unschedule(uint jobid)
{
   Requests::iterator it = requests.begin();
   for ( ; it != requests.end(); ++it )
   {
      Request& request = *it;
      if ( request.mJobId == jobid)
      {
         requests.erase(it);
         break;
      }
   }
}

/// \fn ScriptManager::unscheduleAll()
/// \brief Removes all requests from the list.
void ScriptManager::unscheduleAll()
{
   requests.clear();
}

//-----------------------------------------
// - Function interface
//-----------------------------------------

bool ScriptManager::hasFunction(const std::string& name) const
{
   return false;
}

std::string ScriptManager::generateUniqueFunctionName(const std::string& name)
{
   int index = 0;
   std::ostringstream uniquename;

   uniquename << name;

   while ( hasFunction(uniquename.str()) )
   {
      uniquename.clear();
      uniquename << name << index;
   }

   return uniquename.str();
}

//-----------------------------------------
// - Global retreival
//-----------------------------------------

int ScriptManager::getInt(const char* var)
{
   return -1;
}

bool ScriptManager::getBool(const char* var)
{
   return false;
}

void* ScriptManager::getClass(const char* var)
{
   return NULL;
}
