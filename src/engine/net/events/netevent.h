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

#ifndef _NETEVENT_H_
#define _NETEVENT_H_

#include "engine/net/netobject.h"

class DataStream;

enum EventType {
   connectEvent      = 0x0000,
   connectReplyEvent = 0x0001,
   disconnectEvent   = 0x0002,
   joinEvent         = 0x0003,
   serverdownEvent   = 0x0004,

   // game events
   actionEvent       = 0x0020,
   scriptEvent       = 0x0023,
   newobjectEvent    = 0x0024,
   delobjectEvent    = 0x0025,
   updobjectEvent    = 0x0021,
   reqobjectEvent    = 0x0022,
   viewportEvent     = 0x0026,
   namechangeEvent   = 0x0027,
   worldchangedEvent = 0x0028,

   generalEvent      = 0x1000
};

/// NetEvent
/// The base class for events that should be sended to the client or server.
class NetEvent: public NetObject
{
public:
   DEFINE_REPLICATABLE(NetEvent)

   explicit       NetEvent(EventType _type = generalEvent);

 // get/set 
   void           setType(EventType eventtype);
   EventType      getType() const;

protected:
 // streaming
   virtual void   doPack(DataStream& stream) const;
   virtual void   doUnpack(DataStream& stream);

private:
   EventType mType;
};

#ifdef JENGINE_INLINE
#  include "netevent.inl"
#endif

#endif
