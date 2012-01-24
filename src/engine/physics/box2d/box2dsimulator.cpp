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
#include "box2dsimulator.h"

#include <Box2D.h>

#include "engine/world/world.h"
#include "engine/world/bound.h"
#include "engine/world/bounds.h"
#include "engine/actor.h"

#include "box2dbody.h"
#include "box2drevolutejoint.h"

// static 
Vector Box2DSimulator::b2ToVector(const b2Vec2& b2)
{
   return Vector(b2.x * 30, b2.y * 30);
}

// static 
b2Vec2 Box2DSimulator::vectorToB2(const Vector& v)
{
   return b2Vec2(v.x / 30, v.y / 30);
}

Box2DSimulator::Box2DSimulator():
   Simulator(),
   mpb2World(NULL),
   mContactListener(*this),
   mJoints()
{
}

Box2DSimulator::~Box2DSimulator()
{
   delete mpb2World;
   mpb2World = NULL;
}

Body& Box2DSimulator::createBody(Actor& actor)
{
   b2BodyDef bodydef;
   bodydef.position = vectorToB2(actor.getPosition());
   bodydef.angle    = actor.getRotation();
   bodydef.type     = b2_dynamicBody;

   b2Body* pboxbody = mpb2World->CreateBody(&bodydef);

   Box2DBody* pbody = new Box2DBody(actor, *pboxbody);

   addBody(pbody);

   return *pbody;
}

void Box2DSimulator::removeBody(Body& body)
{
}

Box2DRevoluteJoint& Box2DSimulator::createRevoluteJoint(Box2DRevoluteJointDefinition& definition)
{
   b2RevoluteJointDef jd;
   jd.Initialize(&definition.pleft->getBody(), &definition.pright->getBody(), vectorToB2(definition.anchor));

   b2RevoluteJoint* pb2joint = static_cast<b2RevoluteJoint*>(mpb2World->CreateJoint(&jd));

   Box2DRevoluteJoint* pjoint = new Box2DRevoluteJoint(*pb2joint);

   mJoints.push_back(pjoint);

   return *pjoint;
}

// - Notifications

void Box2DSimulator::notifyWorldChanged()
{
   b2Vec2 gravity(0, 9);
   mpb2World = new b2World(gravity, true);
   mpb2World->SetContactListener(&mContactListener);

   const Bounds& bounds = getWorld().getBounds();

   for ( Bounds::size_type index = 0; index < bounds.size(); ++index )
   {
      const Bound& bound = *bounds[index];

      b2BodyDef def;
      def.position.Set(0,0);
      def.type     = b2_staticBody;
      def.userData = const_cast<Bound*>(&bound);
      b2Body* pbody = mpb2World->CreateBody(&def);

      b2PolygonShape ground;
      ground.SetAsEdge(vectorToB2(bound.getLeft()), vectorToB2(bound.getRight()));

      b2FixtureDef fixturedef;
      fixturedef.shape    = &ground;
      fixturedef.density  = 1;
      fixturedef.friction = 0.3f;
      fixturedef.userData = (void*)eBound;

      pbody->CreateFixture(&fixturedef);
   }
}

void Box2DSimulator::run(float timestep)
{
   int velocityIterations = 6;
   int positionIterations = 2;

   getBodies().integrate(timestep);

   mpb2World->Step(timestep, velocityIterations, positionIterations);
   mpb2World->ClearForces();

   getBodies().finalize();
}
