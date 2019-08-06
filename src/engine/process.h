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
#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <map>

#include "engine/engine_base.h"
#include "engine/net/netconnection.h"

#include "processnetobserver.h"

class DataStream;
class NetEvent;
class String;
class World;

namespace c2d
{
   class ActionMap;
   class ContentManager;
   class ModuleManager;
   class ScriptManager;
   class ScriptObject;

   /// @author Jeroen Broekhuizen
   /// \brief Provides the basic functionality for the process.
   ///
   /// Abstract base class for processes. This class provides the common functionality 
   /// needed for client and server processes.
   class Process
   {
   public:
      explicit       Process();
      virtual        ~Process() = 0;

      virtual bool   create(const String& classname, const String& basedir);
      virtual bool   destroy();
      virtual void   update(float delta);

      // get/set
      NetConnection*    getConnection();
      ModuleManager&    getModuleManager();
      ContentManager&   getContentManager();
      ScriptManager&    getScriptManager();

      ActionMap*     getActionMap();
      void           setActionMap(ActionMap* map);

      bool           hasWorld() const;
      const World&   getWorld() const;
            World&   getWorld();

      bool           isInitialized();
      void           setInitialized(bool init);

      bool           isActive() const;
      void           setActive(bool active);

      // operations
      World* loadWorld(const String& filename);
      void sendScriptEvent(int clientid, const DataStream& stream);
      void swapLeakDetection();

      // events
      virtual void onNetEvent(int clientid, const NetEvent& event) = 0;

   protected:
      // notifications
      virtual void notifyWorldChanged();

      ProcessNetObserver   mNetObserver;
      NetConnection        conn;
      ScriptObject*        mpScript;
      ActionMap*           actionMap;

   private:

      // members
      ModuleManager*    mpModuleManager;
      ContentManager*   mpContentManager;
      ScriptManager*    mpScriptManager;
      World*            mpWorld;
      bool              mInitialized;
      bool              mActive;
      bool              mDetecting;
   };
}

#ifdef JENGINE_INLINE
#  include "process.inl"
#endif

#endif
