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

#include <Box2D.h>
#include <tinyxml.h>

#include "box2dsimulator.h"

static const std::string sBODYELEMENT       = "body";
static const std::string sSHAPEELEMENT      = "shape";
static const std::string sTYPE              = "type";

Box2DBody::Box2DBody(Object& object, b2Body& body):
   Body(object),
   mBody(body),
   mHalfWidth(0),
   mHalfHeight(0),
   mpBottomSensor(NULL),
   mpLeftSensor(NULL),
   mpRightSensor(NULL)
{
   mBody.SetUserData(this);
}

Box2DBody::~Box2DBody()
{
   if ( mBody.GetWorld() != NULL )
   {
      mBody.GetWorld()->DestroyBody(&mBody);
   }
}

// get/set

b2Body& Box2DBody::getBody()
{
   return mBody;
}

// query
   
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

// loading

void Box2DBody::load(const TiXmlElement& element)
{
   Body::load(element);

   const TiXmlElement* pphysics = dynamic_cast<const TiXmlElement*>(element.FirstChild(sBODYELEMENT));
   if ( pphysics == NULL )
   {
      return;
   }

   int isstatic = 0;
   pphysics->QueryIntAttribute("static", &isstatic);
   if ( isstatic > 0 )
      mBody.SetType(b2_staticBody);

   int rotate = 1;
   if ( pphysics->QueryIntAttribute("rotate", &rotate) == TIXML_SUCCESS && rotate == 0 )
      mBody.SetFixedRotation(true);

   const TiXmlElement* pshapeelement = dynamic_cast<const TiXmlElement*>(pphysics->FirstChild(sSHAPEELEMENT));
   if ( pshapeelement != NULL )
   {
      const std::string* pshapetype = pshapeelement->Attribute(sTYPE);
      if ( pshapetype != NULL )
      {
         if ( pshapetype->compare("box") == 0 )
         {
            pshapeelement->QueryFloatAttribute("halfx", &mHalfWidth);
            pshapeelement->QueryFloatAttribute("halfy", &mHalfHeight);

            mHalfWidth /= 30.0f;
            mHalfHeight /= 30.0f;

            b2PolygonShape shape;
            shape.SetAsBox(mHalfWidth, mHalfHeight);

            b2FixtureDef fixturedef;
            fixturedef.density  = 1;
            fixturedef.friction = 0.3f;
            fixturedef.shape    = &shape;
            fixturedef.userData = (void*)Box2DSimulator::eObject;
            
            mBody.CreateFixture(&fixturedef);
         }
         else if ( pshapetype->compare("circle") == 0 )
         {
            float radius = 0.0f;
            pshapeelement->QueryFloatAttribute("radius", &radius);
            
            b2CircleShape shape;
            shape.m_radius = radius / 30;

            b2FixtureDef fixturedef;
            fixturedef.density  = 1;
            fixturedef.friction = 1;
            fixturedef.shape    = &shape;
            fixturedef.userData = (void*)Box2DSimulator::eObject;

            mBody.CreateFixture(&fixturedef);
         }
      }
   }
}

// operations
   
void Box2DBody::generateSensors()
{
   mpBottomSensor = createSensor(mHalfWidth, 0.1f, b2Vec2(0, mHalfHeight));
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
   sensordef.userData = (void*)Box2DSimulator::eObject;

   return mBody.CreateFixture(&sensordef);
}

// forces
  
void Box2DBody::applyForce(const Vector& force)
{
   mBody.ApplyForce(b2Vec2(force.x, force.y), mBody.GetWorldCenter());
}

void Box2DBody::applyForce(const Vector& force, const Vector& pos)
{
   mBody.ApplyForce(b2Vec2(force.x, force.y), Box2DSimulator::vectorToB2(pos));
}

void Box2DBody::applyImpulse(const Vector& impulse)
{
   mBody.ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), mBody.GetWorldCenter());
}

// integration

void Box2DBody::integrate(float timestep)
{
   getForceGenerators().applyForces(*this);
}

#define PI 3.14159265358979323846
#define RAD2DEG(rad)(rad * 180 / PI)


void Box2DBody::finalize()
{
   setPosition(Box2DSimulator::b2ToVector(mBody.GetPosition()));
   setAngle(RAD2DEG(mBody.GetAngle()));

   Body::finalize();
}