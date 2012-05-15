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
#include "joinevent.h"

#include "core/streams/datastream.h"

IMPLEMENT_REPLICATABLE(JoinEventId, JoinEvent, NetEvent)

JoinEvent::JoinEvent():
   NetEvent(joinEvent),
   mPlayerName(),
   mId(-1)
{
}

JoinEvent::JoinEvent(int id, const std::string& name):
   NetEvent(joinEvent),
   mPlayerName(name),
   mId(id)
{
}

// - Get/set

int JoinEvent::getId() const
{
   return mId;
}

const std::string& JoinEvent::getPlayerName() const
{
   return mPlayerName;
}

// - Streaming
   
void JoinEvent::doPack(DataStream& stream) const
{
   NetEvent::doPack(stream);
   stream << mId << mPlayerName;
}

void JoinEvent::doUnpack(DataStream& stream)
{
   NetEvent::doUnpack(stream);
   stream >> mId >> mPlayerName;
}
