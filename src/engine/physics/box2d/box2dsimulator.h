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

#include "../simulator.h"

#include "box2dcontactlistener.h"

class b2World;
struct b2Vec2;

class Object;
class Vector;

class Box2DRevoluteJoint;
class Box2DRevoluteJointDefinition;

class Box2DSimulator : public Simulator
{
public:
   static Vector b2ToVector(const b2Vec2& b2);
   static b2Vec2 vectorToB2(const Vector& v);

   enum BodyType { eBound, eObject };

   struct BodyInfo {
      BodyInfo(BodyType type): mType(type) {}

      BodyType mType;
   };

   Box2DSimulator();
   virtual ~Box2DSimulator();

 // maintenance
   virtual Body& createBody(Actor& actor);
   virtual void  removeBody(Body& body);

   Box2DRevoluteJoint& createRevoluteJoint(Box2DRevoluteJointDefinition& definition);

   virtual void run(float timestep);

protected:
 // notifications
   virtual void notifyWorldChanged();

private:
   typedef std::vector<Box2DRevoluteJoint*> Joints;

   b2World*             mpb2World;
   Box2DContactListener mContactListener;
   Joints               mJoints;
};

#endif
