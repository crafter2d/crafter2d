/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public Licen5se as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* This file contains the prototypes for the toLua program to bind these
 * classes to Lua. GUI part of the classes.
 */

$#include "physics/physicssimulator.h"
$#include "physics/physicsbody.h"
$#include "physics/gravityforcegenerator.h"
$#include "physics/inputforcegenerator.h"
$#include "physics/box2d/box2dsimulator.h"
$#include "physics/box2d/box2dbody.h"
$#include "physics/box2d/box2drevolutejoint.h"

class PhysicsSimulator
{
};

class Body
{
   void addForceGenerator(ForceGenerator* pgenerator);
};

class PhysicsBody
{
};

class ForceGenerator
{
};

class GravityForceGenerator : public ForceGenerator
{
   GravityForceGenerator(const Vector& gravity);
};

class InputForceGenerator : public ForceGenerator
{
   InputForceGenerator();

   const Vector& getVelocity() const;
   void setVelocity(const Vector& vel);
   
   void setImpulse(const Vector& vel);
};

class Box2DSimulator : public Simulator
{
   Box2DRevoluteJoint& createRevoluteJoint(Box2DRevoluteJointDefinition& definition);
};

class Box2DBody : public Body
{
};

class Box2DRevoluteJoint
{
};

class Box2DRevoluteJointDefinition
{
   Box2DBody* pleft;
   Box2DBody* pright;
   Vector anchor;
};