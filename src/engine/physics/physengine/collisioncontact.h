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
#ifndef COLLISION_CONTACT_H_
#define COLLISION_CONTACT_H_

#include "core/math/vector.h"
#include "core/math/matrix2x2.h"

class Body;
class CollisionContactBody;

class CollisionContact
{
public:
   CollisionContact();
   ~CollisionContact();

   bool hasLeft();
   CollisionContactBody& getLeft();
   void setLeft(CollisionContactBody* left);

   bool hasRight();
   CollisionContactBody& getRight();
   void setRight(CollisionContactBody* right);

   void prepare(float timestep);

   Vector   mPoint;
   Vector   mNormal;
   Vector   mVelocity;
   float    mPenetration;
   float    mRestitution;

private:
 // operations
   void   calculateContactBasis();
   Vector calculateLocalVelocity(const CollisionContactBody& contactbody, float timestep);
   void   calculateDesiredDeltaVelocity(float timestep);

 // members
   CollisionContactBody* mpLeft;
   CollisionContactBody* mpRight;
   Matrix2x2             mContactBasis;
   Vector                mContactVelocity;
   float                 mDesiredDeltaVelocity;
};

#endif
