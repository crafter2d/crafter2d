/***************************************************************************
 *   Copyright (C) 2012 by Jeroen Broekhuizen                              *
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
#include "aggregateevent.h"

#include "core/streams/datastream.h"

#include "engine/net/netobjectstream.h"
#include "engine/net/neteventfactory.h"

IMPLEMENT_REPLICATABLE(AggregateEventId, AggregateEvent, NetEvent)

AggregateEvent::AggregateEvent():
   NetEvent(aggregateEvent),
   mEvents()
{
}

AggregateEvent::~AggregateEvent()
{
   NetEventFactory& factory = NetEventFactory::getInstance();
   for ( auto pevent : mEvents )
   {
      factory.release(pevent);
   }
}

// - Operations

void AggregateEvent::add(NetEvent* pevent)
{
   mEvents.push_back(pevent);
}

// - Streaming
   
void AggregateEvent::doPack(DataStream& stream) const
{
   stream.writeInt(mEvents.size());

   NetObjectStream netstream(stream);
   for ( std::size_t index = 0; index < mEvents.size(); ++index )
   {
      NetEvent& event = *mEvents[index];
      netstream << event;
   }
}

void AggregateEvent::doUnpack(DataStream& stream)
{
   int count;
   stream.readInt(count);
   mEvents.reserve(count);

   NetObjectStream netstream(stream);
   for ( int index = 0; index < count; ++index )
   {
      NetEvent* pevent = netstream.readEvent();
      mEvents.push_back(pevent);
   }
}
