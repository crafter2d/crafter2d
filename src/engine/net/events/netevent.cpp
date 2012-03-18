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
#include "netevent.h"
#ifndef JENGINE_INLINE
#  include "netevent.inl"
#endif

#include "engine/net/bitstream.h"

IMPLEMENT_REPLICATABLE(NetEventId, NetEvent, NetObject)

NetEvent::NetEvent(EventType type): 
   mType(type)
{
}

void NetEvent::doPack(BitStream& stream) const
{
   //stream << (int)type;
}

void NetEvent::doUnpack(BitStream& stream, int dirtyflag)
{
   //stream >> (int&)type;
}