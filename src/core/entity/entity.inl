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

INLINE uint32_t Entity::getId() const
{
   return mId;
}

INLINE void Entity::setId(uint32_t id)
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

INLINE const XForm& Entity::getTransform() const
{
   return mTransform;
}

INLINE const Vector& Entity::getPosition() const
{
   return mTransform.getPosition();
}

INLINE float Entity::getRotation() const
{
   return mTransform.getAngle();
}

INLINE const Vector& Entity::getOffset() const
{
   return mOffset;
}

INLINE c2d::Controller& Entity::getController()
{
   ASSERT_PTR(mpController);
   return *mpController;
}

INLINE void Entity::setController(c2d::Controller* pcontroller)
{
   mpController = pcontroller;
}

INLINE bool Entity::getDirection() const
{
   return mDirection;
}

INLINE void Entity::setDirection(bool direction)
{
   mDirection = direction;
}

bool Entity::isDirty() const
{
   return mDirtyFlag != 0;
}

/// \fn Entity::isDirty(int flag) const
/// \brief Checks if this dirty flag is set.
bool Entity::isDirty(int flag) const
{
   return IS_SET(mDirtyFlag, flag);
}

/// \fn Entity::setDirty(bool dirt)
/// \brief Set the dirty flag.
void Entity::setDirty(int flag)
{
   SET_FLAG(mDirtyFlag, flag);
}

/// \fn Entity::resetDirty()
void Entity::resetDirty()
{
   mDirtyFlag = 0;
}

