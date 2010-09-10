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
#include "physicssimulator.h"

#include "collisiondata.h"
#include "collisiondetector.h"
#include "collisionresolver.h"
#include "collisionplane.h"
#include "physicsbody.h"

#include "world/bound.h"
#include "world/world.h"

PhysicsSimulator::PhysicsSimulator():
   Simulator(),
   mWorldShapes()
{
   CollisionDetector::initRegistry();
}

PhysicsSimulator::~PhysicsSimulator()
{
   mWorldShapes.removeAll();
}

Body& PhysicsSimulator::createBody(Object& object)
{
   PhysicsBody* pbody = new PhysicsBody(object);
   addBody(pbody);
   return *pbody;
}

void PhysicsSimulator::worldChanged()
{
   const Bounds& bounds = getWorld().getBounds();
   for ( Bounds::size_type index = 0; index < bounds.size(); ++index )
   {
      const Bound& bound = *bounds[index];

      mWorldShapes.push_back(CollisionPlane::construct(bound.getLeft(), bound.getRight()));
   }
}

void PhysicsSimulator::run(float timestep)
{
   Bodies& bodies = getBodies();
   bodies.integrate(timestep);

   CollisionData data;
   bodies.collectContactData(data, mWorldShapes);

   CollisionResolverInfo info;
   CollisionResolver::resolve(data, info, timestep);
}
