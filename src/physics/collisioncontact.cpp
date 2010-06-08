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
#include "collisioncontact.h"

#include "defines.h"

#include "body.h"
#include "collisioncontactbody.h"

CollisionContact::CollisionContact():
   mpLeft(NULL),
   mpRight(NULL),
   mPoint(),
   mNormal(),
   mVelocity(),
   mPenetration(0)
{
}

CollisionContact::~CollisionContact()
{
}

bool CollisionContact::hasLeft()
{
   return mpLeft != NULL;
}

CollisionContactBody& CollisionContact::getLeft()
{
   ASSERT_PTR(mpLeft);
   return *mpLeft;
}

void CollisionContact::setLeft(CollisionContactBody* pleft)
{
   if ( pleft != mpLeft )
   {
      delete mpLeft;
      mpLeft = pleft;
   }
}

bool CollisionContact::hasRight()
{
   return mpRight != NULL;
}

CollisionContactBody& CollisionContact::getRight()
{
   ASSERT_PTR(mpRight);
   return *mpRight;
}

void CollisionContact::setRight(CollisionContactBody* pright)
{
   if ( pright != mpRight )
   {
      delete mpRight;
      mpRight = pright;
   }
}

void CollisionContact::prepare(float timestep)
{
   if ( mpLeft == NULL )
      mpLeft = mpRight;
   
   ASSERT_PTR(mpLeft);

   mpLeft->prepare(mPoint);
   if ( mpRight != NULL )
      mpRight->prepare(mPoint);
}
