/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
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
#include "simulationfiller.h"

#include "../world/world.h"
#include "../world/bound.h"

#include "../object.h"

#include "collisionplane.h"
#include "simulator.h"

// static 
void SimulationFiller::add(Simulator& simulator, SceneObject& object)
{
   SimulationFiller filler(simulator);
   filler.traverse(object);
}

SimulationFiller::SimulationFiller(Simulator& simulator):
   NodeVisitor(),
   mSimulator(simulator)
{
}

SimulationFiller::~SimulationFiller()
{
}

void SimulationFiller::visitWorld(World* pworld)
{
   ASSERT_PTR(pworld)
   int bounds = pworld->getBoundCount();
   for ( int index = 0; index < bounds; index++ )
   {
      Bound& bound = pworld->getBound(index);
      CollisionPlane* pplane = CollisionPlane::construct(bound.getLeft(), bound.getRight());
      mSimulator.addWorldShape(pplane);
   }
}

void SimulationFiller::visitObject(Object* pobject)
{
   ASSERT_PTR(pobject)
   mSimulator.addBody(pobject->getBody());
}
