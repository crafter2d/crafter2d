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
#include "newobjectevent.h"
#ifndef JENGINE_INLINE
#  include "newobjectevent.inl"
#endif

#include "core/streams/datastream.h"
#include "engine/entity.h"

IMPLEMENT_REPLICATABLE(NewObjectEventId, NewObjectEvent, NetEvent)

NewObjectEvent::NewObjectEvent():
   NetEvent(newobjectEvent),
   mId(0),
   mFileName()
{
}

NewObjectEvent::NewObjectEvent(const Entity& entity):
   NetEvent(newobjectEvent),
   mId(entity.getId()),
   mFileName(entity.getFilename())
{
}

// - Streaming

void NewObjectEvent::doPack(DataStream& stream) const
{
   NetEvent::doPack(stream);
   
   stream.writeUint(mId);
   stream.writeString(mFileName);
}

void NewObjectEvent::doUnpack(DataStream& stream)
{
   NetEvent::doUnpack(stream);
      
   stream.readUint(mId);
   stream.readString(mFileName);
}


