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
#ifndef BOX2D_BODY_H_
#define BOX2D_BODY_H_

#include "engine/physics/body.h"

class b2Body;
class b2Fixture;
struct b2Vec2;

class Box2DBody : public Body
{
public:
   explicit Box2DBody(Simulator& simulator, b2Body& body, Actor& actor);
   virtual ~Box2DBody();

 // get/set
   b2Body& getBody();

 // query
   int getSide(const b2Fixture& sensor) const;

 // loading
   virtual void load(const TiXmlElement& element);

 // operations
   void generateSensors();

 // forces
   virtual void applyForce(const Vector& force);
   virtual void applyForce(const Vector& force, const Vector& pos);

   virtual void applyImpulse(const Vector& impulse);

 // integration
   virtual void integrate(float timestep);
   virtual void finalize();

protected:
 // notifications
   virtual void notifyPositionChanged();

private:
   b2Fixture* createSensor(float halfx, float halfy, const b2Vec2& center);

   b2Body& mBody;
   float   mHalfWidth;
   float   mHalfHeight;

   b2Fixture* mpBottomSensor;
   b2Fixture* mpLeftSensor;
   b2Fixture* mpRightSensor;
};

#endif
