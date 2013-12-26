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
#include "serverdirtyset.h"

#include "core/entity/entity.h"

#include "net/neteventfactory.h"
#include "net/events/aggregateevent.h"
#include "net/events/updateobjectevent.h"

namespace c2d
{

   ServerDirtySet::ServerDirtySet() :
      DirtySet(),
      mObjects()
   {
   }

   ServerDirtySet::~ServerDirtySet()
   {
   }

   // - operations

   void ServerDirtySet::reportDirty(Entity& entity)
   {
      mObjects.push_back(&entity);
   }

   void ServerDirtySet::collect(AggregateEvent& event)
   {
      NetEventFactory& factory = NetEventFactory::getInstance();
      for ( std::size_t index = 0; index < mObjects.size(); ++index )
      {
         Entity& object = *mObjects[index];
         UpdateObjectEvent* pevent = factory.createUpdateEvent();
         pevent->initialize(object);

         object.resetDirty();

         event.add(pevent);
      }
   }

} // namespace c2d
