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

#include "collisionresolver.h"

#include "collisiondata.h"
#include "physicsbody.h"
#include "collisioncontactbody.h"

// ----------------------------------
// -- Statics
// ----------------------------------

void CollisionResolver::resolve(CollisionData& collisiondata, float timestep)
{
   CollisionResolver resolver;
   resolver.doResolve(collisiondata, timestep);
}

// ----------------------------------
// -- Resolver
// ----------------------------------

CollisionResolver::CollisionResolver()
{
}

void CollisionResolver::doResolve(CollisionData& collisiondata, float timestep)
{
   collisiondata.prepare(timestep);

   CollisionData::ContactIterator iterator = collisiondata.getIterator();

   while ( iterator.isValid() )
   {
      CollisionContact& contact = (*iterator);

      resolveContact(contact);

      ++iterator;
   }
}

void CollisionResolver::resolveContact(CollisionContact& contact)
{
   float linearInertiaLeft = 0, angularInertiaLeft = 0;
   float linearInertiaRight = 0, angularInertiaRight = 0;

   calculateInertia(dynamic_cast<PhysicsBody&>(contact.getLeft().getBody()), linearInertiaLeft, angularInertiaLeft);

   if ( contact.hasRight() )
      calculateInertia(dynamic_cast<PhysicsBody&>(contact.getRight().getBody()), linearInertiaRight, angularInertiaRight);

   float totalInertia = linearInertiaLeft  + angularInertiaLeft
                      + linearInertiaRight + angularInertiaRight;
   float invIntertia  = 1 / totalInertia;

   float linearMoveLeft  =  contact.mPenetration * linearInertiaLeft * invIntertia;
   float linearMoveRight = -contact.mPenetration * linearInertiaRight * invIntertia;

   {
      Body& body = contact.getLeft().getBody();
      body.setPosition(body.getPosition() + contact.mNormal * linearMoveLeft);
   }

   if ( contact.hasRight() != NULL )
   {
      Body& body = contact.getRight().getBody();
      body.setPosition(body.getPosition() + contact.mNormal * linearMoveRight);
   }
}

void CollisionResolver::calculateInertia(PhysicsBody& body, float& linearInertia, float& angularInertia)
{
   linearInertia = body.getInverseMass();

   // angular to be implemented
}
