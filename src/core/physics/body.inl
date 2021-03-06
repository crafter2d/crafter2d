/***************************************************************************
 *   Copyright (C) 2009 by Jeroen Broekhuizen                              *
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

INLINE const Simulator& Body::getSimulator() const
{
   return mSimulator;
}

INLINE Simulator& Body::getSimulator()
{
   return mSimulator;
}

INLINE const Entity& Body::getEntity() const
{
   ASSERT_PTR(mpEntity);
   return *mpEntity;
}

INLINE Entity& Body::getEntity()
{
   ASSERT_PTR(mpEntity);
   return *mpEntity;
}

INLINE void Body::setEntity(Entity& entity)
{
   mpEntity = &entity;
}

INLINE const Vector& Body::getPosition() const
{
   return mTransform.getPosition();
}

INLINE void Body::setPosition(const Vector& pos)
{
   mTransform.setPosition(pos);
   notifyPositionChanged();
}

INLINE float Body::getAngle() const
{
   return mTransform.getAngle();
}

INLINE void Body::setAngle(float angle)
{
   mTransform.setAngle(angle);
   notifyPositionChanged();
}

INLINE const XForm& Body::getTransform() const
{
   return mTransform;
}

INLINE void Body::setTransform(const XForm& transform)
{
   mTransform = transform;
   notifyPositionChanged();
}

INLINE void Body::updateTransform(const XForm& xform)
{
   mTransform = xform;
}

INLINE ForceGenerators& Body::getForceGenerators()
{
   return mForceGenerators;
}
