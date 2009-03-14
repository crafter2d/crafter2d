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
#include "newobjectevent.h"
#ifndef JENGINE_INLINE
#  include "newobjectevent.inl"
#endif

#include "../scenegraph.h"

IMPLEMENT_REPLICATABLE(NewObjectEventId, NewObjectEvent, NetEvent)

NewObjectEvent::NewObjectEvent():
   NetEvent(newobjectEvent),
   _parent(),
   _pobject(NULL)
{
}

NewObjectEvent::NewObjectEvent(SceneObject& object):
   NetEvent(newobjectEvent),
   _parent(object.getParent()->getName()),
   _pobject(&object)
{
}

void NewObjectEvent::pack(BitStream& stream) const
{
   NetEvent::pack(stream);
   
   stream << _parent.c_str() << (NetObject*)_pobject << _pobject->getFilename().c_str();
}

void NewObjectEvent::unpack(BitStream& stream)
{
   NetEvent::unpack(stream);
   
   char parent[256] = {0};
   char filename[256] = {0};
   
   stream >> parent >> (NetObject**)&_pobject >> filename;
   
   _parent = parent;
   _pobject->setFilename(filename);
}


