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
#include "defines.h"

INLINE Process& SceneGraph::getProcess()
{
   return mProcess;
}

INLINE bool SceneGraph::hasWorld() const
{
   return mpWorld != NULL;
}

INLINE World* SceneGraph::getWorld()
{
   return mpWorld;
}

INLINE SceneObject& SceneGraph::getRoot()
{
   return mRoot;
}

INLINE bool SceneGraph::hasListener() const
{
   return mpListener != NULL;
}

INLINE SceneGraphListener& SceneGraph::getListener()
{
   ASSERT_PTR(mpListener)
   return *mpListener;
}

INLINE void SceneGraph::setListener(SceneGraphListener& listener)
{
   mpListener = &listener; 
}

INLINE void SceneGraph::clearListener()
{
   mpListener = NULL;
}

INLINE void SceneGraph::setControler(Object* c)
{
   mpControler = c;
}

INLINE Object* SceneGraph::getControler()
{
   return mpControler;
}
