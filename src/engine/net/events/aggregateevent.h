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
#ifndef AGGREGATE_EVENT_H
#define AGGREGATE_EVENT_H

#include <vector>

#include "netevent.h"

class AggregateEvent : public NetEvent
{
public:
   DEFINE_REPLICATABLE(AggregateEvent)

   typedef std::vector<NetEvent*> Events;

   AggregateEvent();
   virtual ~AggregateEvent();

 // get/set
   const Events& getEvents() const { return mEvents; } 

 // query
   bool isEmpty() const { return mEvents.empty(); }

 // operations
   void add(NetEvent* pevent);

protected:
 // streaming
   virtual void   doPack(DataStream& stream) const;
   virtual void   doUnpack(DataStream& stream);

private:

   Events mEvents;
};

#endif // AGGREGATE_EVENT_H
