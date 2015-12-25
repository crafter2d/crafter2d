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

// When testing with Visual Leak Detecter, uncomment the next line
// #include <vld.h>

#include "core/log/log.h"
#include "core/smartptr/autoptr.h"
#include "core/content/contentmanager.h"
#include "core/modules/modulemanager.h"
#include "core/modules/scriptmodule.h"
#include "core/script/scriptmanager.h"
#include "core/script/scriptobject.h"
#include "core/world/world.h"

#include "engine/net/events/scriptevent.h"

#include "actionmap.h"
#include "script_engine.h"

namespace c2d
{

   Process::Process() :
      mNetObserver(*this),
      conn(mNetObserver),
      mpContentManager(NULL),
      mpScriptManager(NULL),
      mpScript(NULL),
      actionMap(NULL),
      initialized(false),
      mpWorld(NULL),
      mActive(true),
      mDetecting(false)
   {
   }

   Process::~Process()
   {
   }

   // - Operations

   bool Process::create(const String& classname, const String& basedir)
   {
      // initialize the modules
      mpModuleManager = &ModuleManager::getInstance();

      // initialize the content manager
      mpContentManager = new ContentManager();
      mpContentManager->setBaseDir(basedir);
      mpContentManager->initialize(getModuleManager());
      
      // initialize the scripting engine
      ScriptModule* pmod = static_cast<ScriptModule*>(mpModuleManager->lookup(UUID_ScriptModule));
      if ( pmod == NULL )
      {
         return false;
      }

      mpScriptManager = &pmod->getManager();
      script_engine_register(*mpScriptManager);
      mpScript = mpScriptManager->load(classname, this, false);
      if ( mpScript == NULL )
      {
         return false;
      }

      // make me a root objects
      mpScriptManager->addRootObject(*mpScript);

      // run the onCreated function
      return mpScript->call(UTEXT("onCreated")).asBool();
   }

   bool Process::destroy()
   {
      conn.shutdown();

      delete mpWorld;
      mpWorld = NULL;

      delete mpScript;
      mpScript = NULL;

      delete actionMap;
      actionMap = NULL;

      return true;
   }

   World* Process::loadWorld(const String& filename)
   {
      mpWorld = getContentManager().loadContent<World>(filename);
      if ( mpWorld != NULL )
      {
         mpWorld->setScript(getScriptManager().load(UTEXT("engine.game.World"), mpWorld, false));

         notifyWorldChanged();
      }
      return mpWorld;
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

   void Process::sendScriptEvent(int clientid, const DataStream& stream)
   {
      ScriptEvent event(stream);

      conn.send(clientid, event);
   }

   void Process::swapLeakDetection()
   {
      mDetecting = !mDetecting;

      /*
      if ( mDetecting )
      {
      VLDEnable();
      }
      else
      {
      VLDDisable();
      }
      */
   }

}
