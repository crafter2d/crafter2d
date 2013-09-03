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

#include "core/math/vector.h"
#include "core/math/xform.h"
#include "core/script/iscriptable.h"

#include "collisionshapes.h"
#include "forcegenerators.h"

class IBodyListener;
class CollisionShape;
class TiXmlElement;
class ForceGenerator;
class Simulator;
class Entity;

class Body : public IScriptable
{
public:
   static bool hasInfo(const TiXmlElement& element);

   explicit Body(Simulator& simulator);
   virtual ~Body();

 // get/set
   const Simulator& getSimulator() const;
         Simulator& getSimulator();

   const Entity& getEntity() const;
         Entity& getEntity();
   void          setEntity(Entity& entity);

   const Vector& getPosition() const;
   void          setPosition(const Vector& pos);

   float getAngle() const;
   void  setAngle(float angle);

   const XForm& getTransform() const;
   void         setTransform(const XForm& transform);

   ForceGenerators& getForceGenerators();

 // query
   bool hasLineOfSight(const Body& that) const;

 // maintenance
   void addListener(IBodyListener* plistener);

 // loading
   virtual void load(const TiXmlElement& element);

 // generators
   void  addForceGenerator(ForceGenerator* pgenerator);

   virtual void applyForce(const Vector& force);
   virtual void applyForce(const Vector& force, const Vector& pos);

   virtual void applyImpulse(const Vector& impulse);

 // integration
   virtual void integrate(float timestep) = 0;
   virtual void finalize() = 0;

protected:
 // operations
   void updateTransform(const XForm& xform);

 // notification
   virtual void notifyPositionChanged();

 // listener notifications
   void firePositionChanged();

private:
 // operations
   void cleanUp();

   Simulator&        mSimulator;
   XForm             mTransform;
   Entity*           mpEntity;
   IBodyListener*    mpListener;
   ForceGenerators   mForceGenerators;
};

#ifdef JENGINE_INLINE
#  include "body.inl"
#endif

#endif
