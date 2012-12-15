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
#include "viewportevent.h"
#ifndef JENGINE_INLINE
#  include "viewportevent.inl"
#endif

#include "core/graphics/viewport.h"
#include "core/streams/datastream.h"

IMPLEMENT_REPLICATABLE(ViewportEventId, ViewportEvent, NetEvent)

ViewportEvent::ViewportEvent():
   NetEvent(viewportEvent),
   mX(0),
   mY(0)
{
}

ViewportEvent::ViewportEvent(const Graphics::Viewport& viewport):
   NetEvent(viewportEvent),
   mX(viewport.getLeft()),
   mY(viewport.getTop())
{
}

void ViewportEvent::update(Graphics::Viewport& viewport) const
{
   viewport.setLeft(mX);
   viewport.setTop(mY);
}

// - Streaming

void ViewportEvent::doPack(DataStream& stream) const
{
   NetEvent::doPack(stream);

   stream << mX << mY;
}

void ViewportEvent::doUnpack(DataStream& stream)
{
   NetEvent::doUnpack(stream);

   stream >> mX >> mY;
}
