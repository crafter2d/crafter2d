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
#ifndef COLLISION_DETECTOR_H_
#define COLLISION_DETECTOR_H_

#include "collisionshape.h"

class Body;
class CollisionData;
class CollisionContactGenerator;
class CollisionShapes;

class CollisionDetector
{
public:
 // statics
   static void initRegistry();

   CollisionDetector(CollisionData& data);
   ~CollisionDetector();

   void collectContactData(const Body& left, const Body& right);
   void collectContactData(const Body& body, const CollisionShapes& shapes);

private:
   static CollisionContactGenerator* mRegistry[CollisionShape::eTypeCount][CollisionShape::eTypeCount];

   bool collideShapes(const CollisionShape& one, const CollisionShape& two);

   CollisionData& mData;
};

#endif
