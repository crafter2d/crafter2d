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
#include "connectreplyevent.h"
#ifndef JENGINE_INLINE
#  include "connectreplyevent.inl"
#endif

#include "core/streams/datastream.h"

IMPLEMENT_REPLICATABLE(ConnectReplyEventId, ConnectReplyEvent, NetEvent)

ConnectReplyEvent::ConnectReplyEvent():
   NetEvent(connectReplyEvent),
   mReply(eDenite),
   mReason(0)
{
}

ConnectReplyEvent::ConnectReplyEvent(Reply reply, int reason):
   NetEvent(connectReplyEvent),
   mReply(reply),
   mReason(reason)
{
}

// - Streaming

void ConnectReplyEvent::doPack(DataStream& stream) const
{
   NetEvent::doPack(stream);
   stream.writeInt(mReply);
   stream.writeInt(mReason);
}

void ConnectReplyEvent::doUnpack(DataStream& stream)
{
   NetEvent::doUnpack(stream);
   stream.readInt((int&)mReply);
   stream.readInt(mReason);
}
