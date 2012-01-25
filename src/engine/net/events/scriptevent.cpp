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
#include "scriptevent.h"

#include "../bitstream.h"

IMPLEMENT_REPLICATABLE(ScriptEventId, ScriptEvent, NetEvent)

ScriptEvent::ScriptEvent():
   NetEvent(scriptEvent),
   mpStream(NULL),
   mOwned(false)
{
}

ScriptEvent::ScriptEvent(BitStream* pstream):
   NetEvent(scriptEvent),
   mpStream(pstream),
   mOwned(false)
{
}

ScriptEvent::~ScriptEvent()
{
   if ( mOwned )
   {
      ASSERT_MSG(mpStream == NULL, "The stream should have been used by now!");
   }
}

// - Get/set

BitStream* ScriptEvent::useStream() const
{
   BitStream* pstream = mpStream;
   mpStream = NULL;
   return pstream;
}

// - Streaming

void ScriptEvent::doPack(BitStream& stream) const
{
   NetEvent::doPack(stream);

   stream << mpStream;
}

void ScriptEvent::doUnpack(BitStream& stream, int dirtyflag)
{
   NetEvent::doUnpack(stream, dirtyflag);
   
   mOwned = true;
   mpStream = new BitStream();
   stream >> *mpStream;
}
