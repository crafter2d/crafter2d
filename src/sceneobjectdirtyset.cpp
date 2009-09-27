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
#include "sceneobjectdirtyset.h"

#include "net/bitstream.h"
#include "net/netevent.h"
#include "net/netconnection.h"

#include "sceneobject.h"

SceneObjectDirtySet::SceneObjectDirtySet():
   DirtySet(),
   mObjects()
{
}

SceneObjectDirtySet::~SceneObjectDirtySet()
{
   resetDirty();
}

// - operations

void SceneObjectDirtySet::reportDirty(NetObject& object)
{
   SceneObject* psceneobject = dynamic_cast<SceneObject*>(&object);
   if ( psceneobject != NULL )
   {
      mObjects.push_back(psceneobject);
   }
}

void SceneObjectDirtySet::send(NetConnection& conn)
{
   Objects::iterator it = mObjects.begin();
   for ( ; it != mObjects.end(); ++it )
   {
      SceneObject& object = *(*it);

      if ( object.isDirty() && !object.isStatic() )
      {
         BitStream stream;
         NetEvent event(updobjectEvent);

         // fill it in
         stream << &event;
         stream << object.getName();
         object.pack(stream);

         // send the package
         conn.send(&stream);
      }
   }
}

void SceneObjectDirtySet::resetDirty()
{
   Objects::iterator it = mObjects.begin();
   for ( ; it != mObjects.end(); ++it )
   {
      SceneObject& object = *(*it);
      object.resetDirty();
   }
}
