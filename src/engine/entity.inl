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
#include "core/defines.h"

INLINE uint Entity::getId() const
{
   return mId;
}

INLINE void Entity::setId(uint id)
{
   mId = id;
}

INLINE bool Entity::hasParent() const
{
   return mpParent != NULL;
}

INLINE const Entity& Entity::getParent() const
{
   ASSERT(hasParent());
   return *mpParent;
}

INLINE Entity& Entity::getParent()
{
   ASSERT(hasParent());
   return *mpParent;
}

INLINE void Entity::setParent(Entity& parent)
{
   mpParent = &parent;
}

INLINE Entities& Entity::getChildren()
{
   return mChildren;
}

INLINE bool Entity::hasFilename() const
{
   return !mXmlFile.isEmpty();
}

INLINE const String& Entity::getFilename() const
{
   return mXmlFile;
}

INLINE void Entity::setFilename(const String& filename)
{
   mXmlFile = filename;
}

INLINE bool Entity::hasName() const
{
   return !mName.isEmpty();
}

/// \fn SceneObject::getName()
/// \brief Returns the name of this object.
INLINE const String& Entity::getName() const
{
   return mName;
}

const String& Entity::getClassName() const
{
   return mClassName;
}

void Entity::setClassName(const String& classname)
{
   mClassName = classname;
}
