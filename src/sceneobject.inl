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

INLINE uint SceneObject::getId() const
{
   return mId;
}

/// \fn SceneObject::setParent(SceneObject* p)
INLINE void SceneObject::setParent(SceneObject* p)
{
   parent = p;
}

INLINE void SceneObject::setFilename(const std::string& filename)
{
   xmlfile = filename;
}

/// \fn SceneObject::getName()
/// \brief Returns the name of this object.
INLINE const std::string& SceneObject::getName() const
{
   return mName;
}

INLINE const std::string& SceneObject::getFilename() const
{
   return xmlfile;
}

INLINE bool SceneObject::hasParent() const
{
   return parent != NULL;
}

/// \fn SceneObject::getParent() const
/// \brief Returns the parent object of this node.
SceneObject* SceneObject::getParent()
{
   return parent;
}
