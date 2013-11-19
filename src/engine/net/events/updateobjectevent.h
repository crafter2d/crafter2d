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
#ifndef UPDATE_OBJECT_EVENT_H_
#define UPDATE_OBJECT_EVENT_H_

#include "core/entity/idmanager.h"
#include "core/streams/bufferedstream.h"

#include "netevent.h"

class Entity;

/**
@author Jeroen Broekhuizen
*/
class UpdateObjectEvent : public NetEvent
{
public:
   DEFINE_REPLICATABLE(UpdateObjectEvent)

            UpdateObjectEvent();
   virtual ~UpdateObjectEvent();
   
 // get/set
   Id                getId() const;
   const DataStream& getDataStream() const;

 // update
   void initialize(Entity& entity);
   void update(Entity& entity) const;

protected:
 // streaming
   virtual void   doPack(DataStream& stream) const;
   virtual void   doUnpack(DataStream& stream);

private:
   Id             mId;
   BufferedStream mDataStream;
};

#ifdef JENGINE_INLINE
#  include "updateobjectevent.inl"
#endif

#endif
