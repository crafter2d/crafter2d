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
#include "box2dbody.h"

#include <Box2D/Box2D.h>

#include "core/defines.h"
#include "core/math/xform.h"
#include "core/math/size.h"

#include "box2dsimulator.h"

static const std::string sBODYELEMENT       = "body";
static const std::string sSHAPEELEMENT      = "shape";
static const std::string sTYPE              = "type";

String Box2DBody::sClassName = UTEXT("box2d.Box2DBody");

Box2DBody::Box2DBody(Simulator& simulator, b2Body& body):
   Body(simulator),
   mpBody(&body),
   mHalfWidth(0),
   mHalfHeight(0),
   mpBottomSensor(nullptr),
   mpLeftSensor(nullptr),
   mpRightSensor(nullptr)
{
   mpBody->SetUserData(this);
}

Box2DBody::~Box2DBody()
{
   if ( mpBody && mpBody->GetWorld() != nullptr )
   {
      mpBody->GetWorld()->DestroyBody(mpBody);
   }
}

// get/set

b2Body& Box2DBody::getBox2DBody()
{
   ASSERT_PTR(mpBody);
   return *mpBody;
}

void Box2DBody::setHalfSize(const Size& size)
{
   mHalfWidth = size.width;
   mHalfHeight = size.height;
}

float Box2DBody::getMass() const
{
   return mpBody->GetMass();
}

Vector Box2DBody::getLinearVelocity() const
{
   return Vector(mpBody->GetLinearVelocity().x, mpBody->GetLinearVelocity().y);
}

// query

const String& Box2DBody::getClassName() const
{
   return sClassName;
}
   
int Box2DBody::getSide(const b2Fixture& sensor) const
{
   if ( &sensor == mpBottomSensor )
      return 1;
   else if ( &sensor == mpLeftSensor )
      return 2;
   else if ( &sensor == mpRightSensor )
      return 3;
   else
      return 0;
}

// operations
   
void Box2DBody::createSensors()
{
   mpBottomSensor = createSensor(mHalfWidth, 0.2f, b2Vec2(0, mHalfHeight));
   mpLeftSensor   = createSensor(0.1f, mHalfHeight, b2Vec2(-mHalfWidth, 0));
   mpRightSensor  = createSensor(0.1f, mHalfHeight, b2Vec2(mHalfWidth, 0));
}

b2Fixture* Box2DBody::createSensor(float halfx, float halfy, const b2Vec2& center)
{
   b2PolygonShape sensor;
   sensor.SetAsBox(halfx, halfy, center, 0);

   b2FixtureDef sensordef;
   sensordef.isSensor = true;
   sensordef.shape    = &sensor;
   sensordef.userData = (void*)& Box2DSimulator::eObject;

   return mpBody->CreateFixture(&sensordef);
}

// forces
  
void Box2DBody::applyForce(const Vector& force)
{
   // mBody.ApplyForce(b2Vec2(force.x, force.y), mBody.GetWorldCenter());
   b2Vec2 v = Box2DSimulator::vectorToB2(force);
   b2Vec2 vel = mpBody->GetLinearVelocity();
   vel.x = v.x;
   mpBody->SetLinearVelocity(vel);
}

void Box2DBody::applyForce(const Vector& force, const Vector& pos)
{
   mpBody->ApplyForce(b2Vec2(force.x, force.y), Box2DSimulator::vectorToB2(pos), true);
}

void Box2DBody::applyImpulse(const Vector& impulse)
{
   mpBody->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), mpBody->GetWorldCenter(), true);
}

// - Notifications

void Box2DBody::notifyPositionChanged()
{
   mpBody->SetTransform(Box2DSimulator::vectorToB2(getPosition()), getAngle());
}

// integration

void Box2DBody::integrate(float timestep)
{
   getForceGenerators().applyForces(*this);
}

void Box2DBody::finalize()
{
   XForm transform(Box2DSimulator::b2ToVector(mpBody->GetPosition()), mpBody->GetAngle());
   updateTransform(transform);

   firePositionChanged();
   
   Body::finalize();
}
