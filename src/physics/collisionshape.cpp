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
#include "collisionshape.h"

#include "../defines.h"

#include "body.h"

CollisionShape::CollisionShape(Type type):
   mpNext(NULL),
   mpBody(NULL),
   mType(type)
{
}

CollisionShape::~CollisionShape()
{
}

CollisionShape::Type CollisionShape::getType() const
{
   return mType;
}

void CollisionShape::setBody(Body& body)
{
   mpBody = &body;
}

const CollisionShape* CollisionShape::getNext() const
{
   return mpNext;
}

CollisionShape* CollisionShape::getNext()
{
   return mpNext;
}

void CollisionShape::setNext(CollisionShape* pshape)
{
   mpNext = pshape;
}

const Vector& CollisionShape::getPosition() const
{
   return mpBody->getPosition();
}
