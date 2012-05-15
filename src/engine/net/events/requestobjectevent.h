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
#ifndef REQUEST_OBJECT_EVENT_H_
#define REQUEST_OBJECT_EVENT_H_

#include "engine/idmanager.h"

#include "netevent.h"

/**
@author Jeroen Broekhuizen
*/
class RequestObjectEvent : public NetEvent
{
public:
   DEFINE_REPLICATABLE(RequestObjectEvent)

            RequestObjectEvent();
   explicit RequestObjectEvent(const Id& id);
   
 // get/set
   Id   getId() const;

protected:
 // streaming
   virtual void   doPack(DataStream& stream) const;
   virtual void   doUnpack(DataStream& stream);

private:
   Id mId;
};

#ifdef JENGINE_INLINE
#  include "requestobjectevent.inl"
#endif

#endif
