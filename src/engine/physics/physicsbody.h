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
#ifndef PHYSICS_BODY_H_
#define PHYSICS_BODY_H_

#include "core/math/xform.h"

#include "body.h"
#include "forcegenerators.h"

class PhysicsBody : public Body
{
public:
   PhysicsBody(Simulator& simulator);
   virtual ~PhysicsBody();

  // get/set
   float getInverseMass() const;
   float getMass() const;
   void  setMass(float mass);

   float getInverseInertia() const;
   float getInertia() const;
   void  setInertia(float inertia);

   const Vector& getVelocity() const;

   const Vector& getAcceleration() const;
   void setAcceleration(const Vector& acc);

   const Vector& getLastFrameAcceleration() const;

 // query
   const String& PhysicsBody::getClassName() const;

 // shapes
   void  addShape(CollisionShape* pshape);
   const CollisionShapes& getShapes() const;

  // loading
   virtual void load(const TiXmlElement& element);

  // forces
   virtual void  addForce(const Vector& force);
   virtual void  addForce(const Vector& force, const Vector& location);

   void  addWorldForce(const Vector& force, const Vector& location);
   void  addTorque(float torque);

 // space conversion
   Vector localToWorld(const Vector& vector) const;

  // integration
   virtual void integrate(float timestep);
   virtual void finalize();

private:
   void  calculateDerivedData();
   void  clearAccumulates();

   XForm             mTransform;
   CollisionShapes   mShapes;

   Vector   mLinearVelocity;
   float    mAngularVelocity;

   Vector   mAccumForce;
   float    mAccumTorque;

   Vector   mAcceleration;
   Vector   mLastFrameAcceleration;

   Vector   mPosition;
   float    mAngle;

   float    mLinearDamping;
   float    mAngularDamping;

   float    mInverseInertia;
   float    mInverseMass;

   static String sClassName;
};

#endif
