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
#include "collisiondetector.h"

#include "core/math/vector.h"

#include "collisioncircle.h"
#include "collisionplane.h"
#include "collisiondata.h"

#include "collisioncirclecirclecontactgenerator.h"
#include "collisioncircleplanecontactgenerator.h"

#include "physicsbody.h"

// ----------------------------------
// -- Generator registry
// ----------------------------------

CollisionContactGenerator* CollisionDetector::mRegistry[CollisionShape::eTypeCount][CollisionShape::eTypeCount] = {
   { nullptr, nullptr },
   { nullptr, nullptr }, 
   { nullptr, nullptr }
};

void CollisionDetector::initRegistry()
{
   mRegistry[CollisionShape::eCircle][CollisionShape::eCircle] = new CollisionCircleCircleContactGenerator();
   mRegistry[CollisionShape::eCircle][CollisionShape::ePlane]  = new CollisionCirclePlaneContactGenerator();
   mRegistry[CollisionShape::ePlane][CollisionShape::eCircle]  = new CollisionCirclePlaneContactGenerator();
}

// ----------------------------------
// -- Detector
// ----------------------------------

CollisionDetector::CollisionDetector(CollisionData& data):
   mData(data)
{
}

CollisionDetector::~CollisionDetector()
{
}

void CollisionDetector::collectContactData(const PhysicsBody& left, const PhysicsBody& right)
{
   collectContactData(left, right.getShapes());
}

void CollisionDetector::collectContactData(const PhysicsBody& body, const CollisionShapes& shapes)
{
   const CollisionShapes& bodyshapes = body.getShapes();

   CollisionShapes::const_iterator it = bodyshapes.begin();
   for ( ; it != bodyshapes.end(); ++it )
   {
      const CollisionShape& bodyshape = *(*it);

      CollisionShapes::const_iterator sit = shapes.begin();
      for ( ; sit != shapes.end(); ++sit )
      {
         const CollisionShape& shape = *(*sit);

         collideShapes(bodyshape, shape);
      }
   }
}

bool CollisionDetector::collideShapes(const CollisionShape& one, const CollisionShape& two)
{
   CollisionContactGenerator* pgenerator = mRegistry[one.getType()][two.getType()];

   if ( pgenerator != nullptr )
   {
      return pgenerator->collide(mData, one, two);
   }

   return false;
}
