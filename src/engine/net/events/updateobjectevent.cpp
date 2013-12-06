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
#include "updateobjectevent.h"
#ifndef JENGINE_INLINE
#  include "updateobjectevent.inl"
#endif

#include "core/entity/entity.h"
#include "core/entity/componentmessages/netstreamcomponentmessage.h"

#include "engine/net/netstream.h"

IMPLEMENT_REPLICATABLE(UpdateObjectEventId, UpdateObjectEvent, NetEvent)

UpdateObjectEvent::UpdateObjectEvent():
   NetEvent(updobjectEvent),
   mId(),
   mDataStream()
{
}

UpdateObjectEvent::~UpdateObjectEvent()
{
}

void UpdateObjectEvent::initialize(Entity& entity)
{
   mId = entity.getId();
   mDataStream.clear();

   NetStream stream(mDataStream);
   NetStreamComponentMessage message(stream, true);
   entity.sendComponentMessage(message);
}

void UpdateObjectEvent::update(Entity& entity) const
{
   NetStream stream(const_cast<BufferedStream&>(mDataStream));
   NetStreamComponentMessage message(stream, false);
   entity.sendComponentMessage(message);
}

// - Streaming

void UpdateObjectEvent::doPack(DataStream& stream) const
{
   NetEvent::doPack(stream);

   stream.writeUint(mId);
   stream.write(mDataStream);
}

void UpdateObjectEvent::doUnpack(DataStream& stream)
{
   NetEvent::doUnpack(stream);

   stream.readUint(mId);
   stream.read(mDataStream);
}
