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
#ifndef BOX2D_SIMULATOR_H_
#define BOX2D_SIMULATOR_H_

#include <vector>

#include "core/physics/simulator.h"

#include "box2dcontactlistener.h"

class b2World;
struct b2Vec2;

class Object;
class String;
class Vector;
class Matrix4;
class BodyDefinition;
class RevoluteJointDefinition;

class Box2DBody;
class Box2DJoint;
class Box2DRevoluteJoint;
class Box2DRevoluteJointDefinition;
class Box2DRopeJoint;
class Box2DRopeJointDefinition;

class Box2DSimulator : public Simulator
{
public:
   static Vector b2ToVector(const b2Vec2& b2);
   static b2Vec2 vectorToB2(const Vector& v);
   //static Matrix4 b2ToMatrix(const b2Transform& tf);

   static const int eBound;
   static const int eObject;

   struct BodyInfo {
      BodyInfo(int type): mType(type) {}

      int mType;
   };

   Box2DSimulator();
   virtual ~Box2DSimulator();

 // query
   virtual const String& getClassName() const;
   virtual bool lineOfSight(const Body& from, const Body& to) const;

 // maintenance
   virtual Body& createBody(const BodyDefinition& definition);
   virtual void  removeBody(Body& body);

   virtual void  createLink(const Body& left, const Body& right, const JointDefinition& definition);

 // update
   virtual void run(float delta);

protected:
 // notifications
   virtual void notifyWorldChanged();

private:
   typedef std::vector<Box2DJoint*> Joints;
   using DeadBodies = std::vector<b2Body*>;

 // creation
   //void createRopeJoint(b2Body& left, b2Body& right, RopeJointDefinition& definition);
   void createRevoluteJoint(b2Body& left, b2Body& right, const RevoluteJointDefinition& definition);

 // operations
   void cleanUp();

 // members
   b2World*             mpb2World;
   Box2DContactListener mContactListener;
   Joints               mJoints;
   DeadBodies           mDeadBodies;


   static const String  sClassName;
};

#endif
