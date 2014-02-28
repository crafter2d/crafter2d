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

#include "physicsfactory.h"

#include "physicsbody.h"
#include "physicssimulator.h"

static const String sFactoryName = UTEXT("physics");

PhysicsFactory::PhysicsFactory():
   SimulationFactory()
{
}

PhysicsFactory::~PhysicsFactory()
{
}

const String& PhysicsFactory::getName() const
{
   return sFactoryName;
}

Simulator* PhysicsFactory::createSimulator()
{
   return new PhysicsSimulator();
}

bool PhysicsFactory::isCreatedBy(const Simulator& simulator) const
{
   const PhysicsSimulator* pbox2dsim = dynamic_cast<const PhysicsSimulator*>(&simulator);
   return pbox2dsim != NULL;
}
