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
#ifndef BODY_H_
#define BODY_H_

#include "math\xform.h"

#include "collisionshapes.h"
#include "forcegenerators.h"

class Object;
class CollisionShape;
class TiXmlElement;
class ForceGenerator;

class Body
{
public:
   static bool hasInfo(const TiXmlElement& element);

   explicit Body(Object& object);
   virtual ~Body();

   const Object& getObject() const;
         Object& getObject();

   const Vector& getPosition() const;
   void          setPosition(const Vector& pos);

   float         getAngle() const;
   void          setAngle(float angle);
   
   ForceGenerators& getForceGenerators();

 // loading
   virtual void load(const TiXmlElement& element);

 // shapes
   void  addShape(CollisionShape* pshape);
   const CollisionShapes& getShapes() const;

 // generators
   void  addForceGenerator(ForceGenerator* pgenerator);

   virtual void applyForce(const Vector& force);
   virtual void applyForce(const Vector& force, const Vector& pos);

   virtual void applyImpulse(const Vector& impulse);

 // integration
   virtual void integrate(float timestep) = 0;
   virtual void finalize() = 0;

 // space conversion
   Vector localToWorld(const Vector& vector) const;

protected:
   void calculateDerivedData();

   Vector   mPosition;
   float    mAngle;

private:
   Object&           mObject;
   XForm             mTransform;
   CollisionShapes   mShapes;
   ForceGenerators   mForceGenerators;
};

#ifdef JENGINE_INLINE
#  include "body.inl"
#endif

#endif
