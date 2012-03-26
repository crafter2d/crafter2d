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

#include "script/vm/virtualclass.h"

#include "engine/script/script.h"
#include "engine/script/scriptmanager.h"
#include "engine/net/events/scriptevent.h"
#include "engine/world/world.h"

#include "actionmap.h"
#include "script_engine.h"

Process::Process():
   conn(*this),
   mContentManager(*this),
   mpScriptManager(NULL),
   mpScript(NULL),
   actionMap(NULL),
   initialized(false),
   mpWorld(NULL),
   mActive(true)
{
}

Process::~Process()
{
   delete mpScript;
   mpScript = NULL;

   delete mpScriptManager;
   mpScriptManager = NULL;

   delete actionMap;
   actionMap = NULL;
}

// - Get/set

void Process::setWorld(World* pworld)
{
   if ( mpWorld != pworld )
   {
      mpWorld = pworld;

      if ( mpWorld != NULL )
      {
         pworld->setScript(getScriptManager().loadNative("World", pworld, false));
      }

      notifyWorldChanged();
   }
}

// - Operations

bool Process::create(const VirtualObjectReference& self)
{
   if ( mpScriptManager == NULL )
   {
      // throw exception with warning that the scriptmanager must be set first
      return false;
   }

   mpScript = new Script(getScriptManager());
   mpScript->setThis(getScriptManager().shareObject(self));
   
   return true;
}

bool Process::destroy()
{
   conn.disconnect();
   
   return true;
}

// - Notifications

void Process::notifyWorldChanged()
{
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

