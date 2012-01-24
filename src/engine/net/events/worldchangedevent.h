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
#ifndef WORLD_CHANGED_EVENT_H
#define WORLD_CHANGED_EVENT_H

#include <string>

#include "engine/net/netevent.h"

class World;

class WorldChangedEvent : public NetEvent
{
public:
   DEFINE_REPLICATABLE(WorldChangedEvent)

   WorldChangedEvent();
   WorldChangedEvent(const World& world);

 // get/set
   const std::string& getFilename() const;

protected:
 // streaming
   virtual void doPack(BitStream& stream) const;
   virtual void doUnpack(BitStream& stream, int dirtyflag);

private:

   std::string mFilename;
};

#ifdef JENGINE_INLINE
#  include "worldchangedevent.inl"
#endif

#endif // WORLD_CHANGED_EVENT_H
