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

#include "world/world.h"
#include "world/bound.h"

#include "object.h"

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
   mpWorld(NULL),
   mJoints()
{
}

Box2DSimulator::~Box2DSimulator()
{
   delete mpWorld;
   mpWorld = NULL;
}

Body& Box2DSimulator::createBody(Object& object)
{
   b2BodyDef bodydef;
   bodydef.position = vectorToB2(object.getPosition());
   bodydef.angle = object.getRotation();
   bodydef.type = b2_dynamicBody;

   b2Body* pboxbody = mpWorld->CreateBody(&bodydef);

   Box2DBody* pbody = new Box2DBody(object, *pboxbody);

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

   b2RevoluteJoint* pb2joint = static_cast<b2RevoluteJoint*>(mpWorld->CreateJoint(&jd));

   Box2DRevoluteJoint* pjoint = new Box2DRevoluteJoint(*pb2joint);

   mJoints.push_back(pjoint);

   return *pjoint;
}

void Box2DSimulator::generateWorldShapes(const World& world)
{
   b2Vec2 gravity(0, 9);
   mpWorld = new b2World(gravity, true);

   b2BodyDef def;
   def.position.Set(0,0);
   b2Body* pbody = mpWorld->CreateBody(&def);

   for ( int index = 0; index < world.getBoundCount(); ++index )
   {
      const Bound& bound = world.getBound(index);

      b2PolygonShape ground;
      ground.SetAsEdge(vectorToB2(bound.getLeft()), vectorToB2(bound.getRight()));

      b2Fixture* pfixture = pbody->CreateFixture(&ground, 0);
      pfixture->SetFriction(.3f);
   }
}

void Box2DSimulator::run(float timestep)
{
   int velocityIterations = 6;
   int positionIterations = 2;

   getBodies().integrate(timestep);

   mpWorld->Step(timestep, velocityIterations, positionIterations);
   mpWorld->ClearForces();

   getBodies().finalize();
}
