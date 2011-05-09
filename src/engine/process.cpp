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

#include "process.h"
#ifndef JENGINE_INLINE
#  include "process.inl"
#endif

#include "core/log/log.h"

#include "engine/script/script.h"
#include "engine/script/scriptmanager.h"

#include "net/netevent.h"
#include "net/events/scriptevent.h"

#include "world/world.h"

#include "actionmap.h"
#include "scenegraph.h"

Process::Process(const std::string& name):
   mProcessName(name),
   conn(*this),
   graph(*this),
   mScriptManager(),
   mpScript(NULL),
   actionMap(NULL),
   initialized(false)
{
}

Process::~Process()
{
}

bool Process::create()
{
   return mScriptManager.initialize() && initializeScript();
}

bool Process::destroy()
{
   conn.disconnect();

   graph.removeAll();

   return true;
}

bool Process::initializeScript()
{
   mpScript = mScriptManager.loadClass(mProcessName);
   if ( mpScript == NULL )
   {
      Log& log = Log::getInstance();
      log << "Failed to load the " << mProcessName.c_str() << " class.";
      return false;
   }

   mpScript->setThis(this);
   return true;
}

// - Get/set

/// \fn Process::setActionMap(ActionMap* map)
/// \brief Attach the new action map for this process.
void Process::setActionMap(ActionMap* map)
{
   actionMap = map;
   if ( actionMap != NULL )
      actionMap->setProcess(*this);
}

// - Operations

bool Process::loadWorld(const std::string& filename, const std::string& name)
{
   World* pworld = new World();
   if ( pworld->create(getSceneGraph().getRoot(), filename) )
   {
      pworld->setName(name);

      graph.setWorld(pworld);
   }
   else
   {
      delete pworld;
      pworld = NULL;
   }

   return pworld != NULL;
}

// - Updating

void Process::update(float tick)
{
   if ( conn.isConnected() )
      conn.update();
}

// - Events

int Process::allowNewConnection()
{
   return 0;
}

void Process::sendScriptEvent(BitStream* pstream, uint client)
{
   if ( conn.isConnected() )
   {
      ScriptEvent event(pstream);
      if (client != INVALID_CLIENTID)
         conn.setClientId(client);
      conn.send(&event);
   }
}

