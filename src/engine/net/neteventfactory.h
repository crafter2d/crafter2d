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
#ifndef NET_EVENT_FACTORY_H
#define NET_EVENT_FACTORY_H

#include "core/memory/memorypool.h"

#include "engine/net/events/updateobjectevent.h"

class NetEvent;

class NetEventFactory
{
public:

   static NetEventFactory& getInstance();

 // update factory
   NetEvent*          create(int type);
   UpdateObjectEvent* createUpdateEvent();

   void release(NetEvent* event);

private:
   NetEventFactory();

   static NetEventFactory* mpInstance;

   MemoryPool<UpdateObjectEvent> mUpdatePool;
};

#endif // NET_EVENT_FACTORY_H
