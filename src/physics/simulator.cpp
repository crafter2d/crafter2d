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
#include "simulator.h"

#include "../defines.h"

#include "collisionshape.h"

Simulator::Simulator():
   mBodies(),
   mWorldShapes()
{
}

Simulator::~Simulator()
{
   destroyWorldShapes();
}

// ----------------------------------
// -- Body interface
// ----------------------------------

Bodies& Simulator::getBodies()
{
   return mBodies;
}

void Simulator::addBody(Body& body)
{
   mBodies.add(body);
}

void Simulator::removeBody(Body& body)
{
   mBodies.remove(body);
}

// ----------------------------------
// -- World body interface
// ----------------------------------

void Simulator::generateWorldShapes(const World& world)
{
}

void Simulator::addWorldShape(CollisionShape* pshape)
{
   mWorldShapes.push_back(pshape);
}

void Simulator::destroyWorldShapes()
{
   mWorldShapes.removeAll();
}

const CollisionShapes& Simulator::getWorldShapes() const
{
   return mWorldShapes;
}

// ----------------------------------
// -- Run
// ----------------------------------

void Simulator::run(float timestep)
{
   PURE_VIRTUAL;
}
