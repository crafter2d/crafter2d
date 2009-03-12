/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "net/netconnection.h"
#include "net/bitstream.h"

#include "scenegraph.h"

class ActionMap;
class NetEvent;

/// @author Jeroen Broekhuizen
/// \brief Provides the basic functionality for the process.
///
/// Abstract base class for processes. This class provides the common functionality 
/// needed for client and server processes.
class Process
{
public:
                  Process();
   virtual        ~Process();

   virtual bool   create();
   virtual bool   destroy();
   virtual void   update (Uint32 tick);

   void           sendScriptEvent(const BitStream* stream, Uint32 client=INVALID_CLIENTID);

   virtual int    onClientEvent(int client, NetEvent* event, BitStream& stream) = 0;

   void           setActionMap(ActionMap* map);
   void           setInitialized(bool init);

   NetConnection* getConnection();
   SceneGraph&    getSceneGraph();
   ActionMap*     getActionMap();
   bool           isInitialized();

protected:
   NetConnection  conn;
   BitStream      stream;
   SceneGraph     graph;
   ActionMap*     actionMap;
   bool           initialized;
};

#ifdef JENGINE_INLINE
#  include "process.inl"
#endif

#endif
