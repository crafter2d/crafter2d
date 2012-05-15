/***************************************************************************
 *   Copyright (C) 2010 by Jeroen Broekhuizen                              *
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
#include "worldchangedevent.h"
#ifndef JENGINE_INLINE
#  include "worldchangedevent.inl"
#endif

#include "core/streams/datastream.h"

#include "engine/world/world.h"

IMPLEMENT_REPLICATABLE(WorldChangedEventId, WorldChangedEvent, NetEvent)

WorldChangedEvent::WorldChangedEvent():
   NetEvent(worldchangedEvent),
   mFilename()
{
}

WorldChangedEvent::WorldChangedEvent(const World& world):
   NetEvent(worldchangedEvent),
   mFilename(world.getFilename())
{
}

// - Streaming

void WorldChangedEvent::doPack(DataStream& stream) const
{
   NetEvent::doPack(stream);

   stream << mFilename;
}

void WorldChangedEvent::doUnpack(DataStream& stream)
{
   NetEvent::doUnpack(stream);

   stream >> mFilename;
}
