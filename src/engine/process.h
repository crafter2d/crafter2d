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

#include <string>

#include "engine/engine_base.h"
#include "engine/script/scriptmanager.h"

#include "net/netconnection.h"

#include "scenegraph.h"

class ActionMap;
class BitStream;
class NetEvent;
class Script;

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

   virtual bool   create(const std::string& name);
   virtual bool   destroy();
   virtual void   update (float delta);

  // get/set
   NetConnection* getConnection();
   SceneGraph&    getSceneGraph();
   ScriptManager& getScriptManager();

   ActionMap*     getActionMap();
   void           setActionMap(ActionMap* map);

   bool           isInitialized();
   void           setInitialized(bool init);

   bool isActive() const;
   void setActive(bool active);

 // operations
   virtual bool   loadWorld(const std::string& filename, const std::string& name);

   void           sendScriptEvent(BitStream* stream, uint client=INVALID_CLIENTID);

  // events
   virtual int    allowNewConnection();
   virtual int    onClientEvent(int client, const NetEvent& event) = 0;

protected:
   NetConnection     conn;
   SceneGraph        graph;
   ScriptManager     mScriptManager;
   Script*           mpScript;
   ActionMap*        actionMap;
   bool              initialized;

private:
   bool initializeScript(const std::string& name);

 // members
   bool mActive;
};

#ifdef JENGINE_INLINE
#  include "process.inl"
#endif

#endif
