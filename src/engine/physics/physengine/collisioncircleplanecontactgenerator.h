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
#ifndef COLLISION_CIRCLE_PLANE_CONTACT_GENERATOR_H_
#define COLLISION_CIRCLE_PLANE_CONTACT_GENERATOR_H_

#include "collisioncontactgenerator.h"

class CollisionCircle;
class CollisionPlane;

class CollisionCirclePlaneContactGenerator : public CollisionContactGenerator
{
public:
   CollisionCirclePlaneContactGenerator();
   virtual ~CollisionCirclePlaneContactGenerator();

   virtual bool collide(CollisionData& data, const CollisionShape& one, const CollisionShape& two);

private:
   bool docollide(CollisionData& data, const CollisionCircle& circle, const CollisionPlane& plane);
};

#endif
