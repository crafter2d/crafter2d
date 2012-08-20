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
#include "neteventfactory.h"

#include "events/updateobjectevent.h"

NetEventFactory* NetEventFactory::mpInstance = new NetEventFactory();

NetEventFactory& NetEventFactory::getInstance()
{
   return *mpInstance;
}

NetEventFactory::NetEventFactory()
{
}

// - Factory interface

NetEvent* NetEventFactory::create(int type)
{
   switch ( type )
   {
      case UpdateObjectEventId:  return createUpdateEvent();
      default:                   break;
   }

   return NULL;
}

UpdateObjectEvent* NetEventFactory::createUpdateEvent()
{
   return mUpdatePool.alloc();
}

void NetEventFactory::release(NetEvent* pevent)
{
   switch ( pevent->getType() )
   {
      case updobjectEvent:
         {
            mUpdatePool.free(static_cast<UpdateObjectEvent*>(pevent));
         };
         break;
   }
}
