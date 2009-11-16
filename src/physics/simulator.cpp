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
   mpWorldShapes(NULL)
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

void Simulator::addWorldShape(CollisionShape* pshape)
{
   if ( mpWorldShapes != NULL )
   {
      pshape->setNext(mpWorldShapes);
   }

   mpWorldShapes = pshape;
}

void Simulator::destroyWorldShapes()
{
   while ( mpWorldShapes != NULL )
   {
      CollisionShape* pshape = mpWorldShapes->getNext();
      delete mpWorldShapes;
      mpWorldShapes = pshape;
   }
}

// ----------------------------------
// -- Run
// ----------------------------------

void Simulator::run(float timestep)
{
   PURE_VIRTUAL;
}
