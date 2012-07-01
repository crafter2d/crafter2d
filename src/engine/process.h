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
#include <string>

#include "engine/engine_base.h"
#include "engine/script/scriptmanager.h"

#include "content/contentmanager.h"
#include "net/netconnection.h"

#include "processnetobserver.h"

class ActionMap;
class DataStream;
class NetEvent;
class Script;
class VirtualObjectReference;

/// @author Jeroen Broekhuizen
/// \brief Provides the basic functionality for the process.
///
/// Abstract base class for processes. This class provides the common functionality 
/// needed for client and server processes.
class ENGINE_API Process
{
public:
   explicit       Process();
   virtual        ~Process();

   virtual bool   create(const VirtualObjectReference& self);
   virtual bool   destroy();
   virtual void   update (float delta);

  // get/set
   NetConnection* getConnection();

   ContentManager& getContentManager();

   ActionMap*     getActionMap();
   void           setActionMap(ActionMap* map);

   ScriptManager& getScriptManager();
   void           setScriptManager(ScriptManager* pscriptmanager);

   bool           hasWorld() const;
   const World&   getWorld() const;
         World&   getWorld();
   void           setWorld(World* pworld);

   bool           isInitialized();
   void           setInitialized(bool init);

   bool           isActive() const;
   void           setActive(bool active);

 // operations
   void sendScriptEvent(int clientid, const DataStream& stream);
   
  // events
   virtual void onNetEvent(int clientid, const NetEvent& event) = 0;

protected:
 // notifications
   virtual void notifyWorldChanged();

   ProcessNetObserver   mNetObserver;
   NetConnection        conn;
   ContentManager       mContentManager;
   ScriptManager*       mpScriptManager;
   Script*              mpScript;
   ActionMap*           actionMap;
   bool                 initialized;

private:

 // members
   World*      mpWorld;
   bool        mActive;
};

#ifdef JENGINE_INLINE
#  include "process.inl"
#endif

#endif
