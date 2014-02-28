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
#include "box2dcontactlistener.h"

#include "core/physics/simulatorlistener.h"
#include "core/world/bound.h"

#include "box2dbody.h"
#include "box2dsimulator.h"

Box2DContactListener::Box2DContactListener(Box2DSimulator& simulator):
   b2ContactListener(),
   mSimulator(simulator)
{
}

void Box2DContactListener::BeginContact(b2Contact* pcontact)
{
   collision(pcontact, true);
}

void Box2DContactListener::EndContact(b2Contact* pcontact)
{
   collision(pcontact, false);
}

void Box2DContactListener::collision(b2Contact* pcontact, bool begin)
{
   b2Fixture* pa = pcontact->GetFixtureA();
   b2Fixture* pb = pcontact->GetFixtureB();

   int typeA = (int)pa->GetUserData();
   int typeB = (int)pb->GetUserData();

   if ( pa->IsSensor() && typeB == Box2DSimulator::eObject )
   {
      collisionObjectObject(pcontact, *pa, *pb, begin);
   }
   else if ( pa->IsSensor() && typeB == Box2DSimulator::eBound )
   {
      collisionObjectWorld(pcontact, *pa, *pb, begin);
   }
   else if ( pb->IsSensor() && typeA == Box2DSimulator::eBound )
   {
      collisionObjectWorld(pcontact, *pb, *pa, begin);
   }
}

void Box2DContactListener::collisionObjectWorld(b2Contact* pcontact, b2Fixture& sensor, b2Fixture& bound, bool begin)
{
   // collision between a object(A) && bound(B)

   Box2DBody* pbody = static_cast<Box2DBody*>(sensor.GetBody()->GetUserData());
   int side = pbody->getSide(sensor);
   
   if ( side > 0 && mSimulator.hasListener() )
   {
      mSimulator.getListener().collideObjectWorld(pbody->getEntity(), *(Bound*)bound.GetBody()->GetUserData(), side, begin);
   }
}

void Box2DContactListener::collisionObjectObject(b2Contact* pcontact, b2Fixture& sensor, b2Fixture& object, bool begin)
{
   // collision between object(A) && object(B)

   Box2DBody* pbody = static_cast<Box2DBody*>(sensor.GetBody()->GetUserData());
   int side = pbody->getSide(sensor);
   
   if ( side > 0 && mSimulator.hasListener() )
   {
      Box2DBody* pobjectbody = static_cast<Box2DBody*>(object.GetBody()->GetUserData());
      mSimulator.getListener().collideObjectObject(pbody->getEntity(), pobjectbody->getEntity(), side, begin);
   }
}
