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

#include <Box2D/Box2D.h>

#include "core/math/matrix4.h"
#include "core/math/size.h"
#include "core/defines.h"
#include "core/physics/bodydefinition.h"
#include "core/physics/revolutejointdefinition.h"
#include "core/world/world.h"
#include "core/world/bound.h"
#include "core/world/bounds.h"
#include "core/system/exception.h"

#include "box2dbody.h"
#include "box2draycastcallback.h"
#include "box2drevolutejoint.h"
#include "box2dropejoint.h"

const String Box2DSimulator::sClassName = UTEXT("box2d.Box2DSimulator");

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

Matrix4 Box2DSimulator::b2ToMatrix(const b2Transform& tf)
{
   /*
   return Matrix4(
      tf.q.c     , tf.q.s     , 0, 0,
      -tf.q.s    , tf.q.c     , 0, 0,
      0          , 0          , 1, 0,
      tf.p.x * 30, tf.p.y * 30, 0, 1
   );
   */
   return Matrix4();
}

Box2DSimulator::Box2DSimulator():
   Simulator(),
   mpb2World(NULL),
   mContactListener(*this),
   mJoints(),
   mDelta(0.0f)
{
}

Box2DSimulator::~Box2DSimulator()
{
   cleanUp();
}

// - Query

const String& Box2DSimulator::getClassName() const
{
   return sClassName;
}

bool Box2DSimulator::lineOfSight(const Body& from, const Body& to) const
{
   Box2DRayCastCallback callback;
   b2Vec2 point1 = vectorToB2(from.getPosition());
   b2Vec2 point2 = vectorToB2(to.getPosition());
   mpb2World->RayCast(&callback, point1, point2);
   return callback.hasCollision();
}

// - Maintenance

void Box2DSimulator::cleanUp()
{
   for ( std::size_t index = 0; index < mJoints.size(); index++ )
   {
      Box2DJoint* pjoint = mJoints[index];
      delete pjoint;
   }
   mJoints.clear();

   delete mpb2World;
   mpb2World = NULL;
}

Body& Box2DSimulator::createBody(const BodyDefinition& definition)
{
   b2BodyDef bodydef;
   bodydef.position      = b2Vec2();
   bodydef.angle         = 0.0f;
   bodydef.type          = definition.isStatic() ? b2_staticBody : b2_dynamicBody;
   bodydef.fixedRotation = definition.isFixedRotation();

   b2Body* pboxbody = mpb2World->CreateBody(&bodydef);

   switch ( definition.getShapeType() )
   {
      case BodyDefinition::eBox:
         {
            b2PolygonShape shape;
            shape.SetAsBox(definition.getWidth(), definition.getHeight());

            b2FixtureDef fixturedef;
            fixturedef.density  = 1;
            fixturedef.friction = 0.3f;
            fixturedef.restitution = 0.2f;
            fixturedef.shape    = &shape;
            fixturedef.userData = (void*)Box2DSimulator::eObject;

            pboxbody->CreateFixture(&fixturedef);
         }
         break;
      case BodyDefinition::eCircle:
         {
            b2CircleShape shape;
            shape.m_radius = definition.getRadius() / 30;

            b2FixtureDef fixturedef;
            fixturedef.density  = 1;
            fixturedef.friction = 1;
            fixturedef.shape    = &shape;
            fixturedef.userData = (void*)Box2DSimulator::eObject;

            pboxbody->CreateFixture(&fixturedef);
         }
      default:
         throw new c2d::Exception(UTEXT("invalid body shape."));
   }

   Box2DBody* pbody = new Box2DBody(*this, *pboxbody);
   pbody->setHalfSize(Size(definition.getWidth(), definition.getHeight()));
   pbody->createSensors();
   addBody(pbody);

   return *pbody;
}

void Box2DSimulator::removeBody(Body& body)
{
}

void Box2DSimulator::createLink(const Body& left, const Body& right, const JointDefinition& definition)
{
   b2Body& leftbody = static_cast<Box2DBody&>(const_cast<Body&>(left)).getBox2DBody();
   b2Body& rightbody = static_cast<Box2DBody&>(const_cast<Body&>(right)).getBox2DBody();

   switch ( definition.getKind() )
   {
   case JointDefinition::eRevolute:
      {
         const RevoluteJointDefinition& revolutedef = static_cast<const RevoluteJointDefinition&>(definition);
         createRevoluteJoint(leftbody, rightbody, revolutedef);
      }
      break;
   }
}

void Box2DSimulator::createRevoluteJoint(b2Body& left, b2Body& right, const RevoluteJointDefinition& definition)
{
   b2RevoluteJointDef jd;
   jd.Initialize(&left, &right, vectorToB2(definition.anchor));

   b2RevoluteJoint* pb2joint = static_cast<b2RevoluteJoint*>(mpb2World->CreateJoint(&jd));

   Box2DRevoluteJoint* pjoint = new Box2DRevoluteJoint(*pb2joint);

   mJoints.push_back(pjoint);
}

/*
void Box2DSimulator::createRopeJoint(Box2DRopeJointDefinition& definition)
{
   b2RopeJointDef jd;
   jd.bodyA = &definition.pleft->getBody();
   jd.bodyB = &definition.pright->getBody();
   jd.localAnchorA = vectorToB2(definition.anchorLeft);
   jd.localAnchorB = vectorToB2(definition.anchorRight);
   jd.maxLength = 0.06f; // == 2 pixels (> b2_linearSlop)

   b2RopeJoint* pb2joint = static_cast<b2RopeJoint*>(mpb2World->CreateJoint(&jd));

   Box2DRopeJoint* pjoint = new Box2DRopeJoint(*pb2joint);

   mJoints.push_back(pjoint);

   return *pjoint;
}
*/

// - Notifications

void Box2DSimulator::notifyWorldChanged()
{
   b2Vec2 gravity(0, 9);
   mpb2World = new b2World(gravity);
   mpb2World->SetContactListener(&mContactListener);

   const Bounds& bounds = getWorld().getBounds();

   for ( Bounds::size_type index = 0; index < bounds.size(); ++index )
   {
      const Bound& bound = *bounds[index];

      b2BodyDef def;
      def.position.Set(0,0);
      def.type     = b2_staticBody;
      def.userData = const_cast<Bound*>(&bound);

      b2EdgeShape ground;
      ground.Set(vectorToB2(bound.getLeft()), vectorToB2(bound.getRight()));
      /*
      b2EdgeShape ground;
      ground.Set(vectorToB2(bound.getLeft()), vectorToB2(bound.getRight()));*/

      b2FixtureDef fixturedef;
      fixturedef.shape    = &ground;
      fixturedef.density  = 1;
      fixturedef.friction = 0.3f;
      fixturedef.userData = (void*)eBound;

      b2Body* pbody = mpb2World->CreateBody(&def);
      pbody->CreateFixture(&fixturedef);
   }
}

void Box2DSimulator::run(float timestep)
{
   static const float step = 1.0f / 60.0f;

   mDelta += timestep;
   if ( mDelta >= step )
   {
      static const int velocityIterations = 8;
      static const int positionIterations = 3;

      getBodies().integrate(step);

      mpb2World->Step(step, velocityIterations, positionIterations);
      //mpb2World->ClearForces();

      getBodies().finalize();

      mDelta = 0.0f;
   }
}
