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

#include "../../viewport.h"

IMPLEMENT_REPLICATABLE(ViewportEventId, ViewportEvent, NetEvent)

ViewportEvent::ViewportEvent():
   NetEvent(viewportEvent),
   mX(0),
   mY(0)
{
}

ViewportEvent::ViewportEvent(const Viewport& viewport):
   NetEvent(viewportEvent),
   mX(viewport._left),
   mY(viewport._top)
{
}

void ViewportEvent::update(Viewport& viewport) const
{
   viewport.setPosition(mX, mY);
}

void ViewportEvent::pack(BitStream& stream) const
{
   NetEvent::pack(stream);

   stream << mX << mY;
}

void ViewportEvent::unpack(BitStream& stream)
{
   NetEvent::unpack(stream);

   stream >> mX >> mY;
}
