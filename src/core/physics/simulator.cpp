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

#include "core/defines.h"

Simulator::Simulator():
   mpWorld(nullptr),
   mBodies(),
   mpListener(nullptr)
{
}

Simulator::~Simulator()
{
}

// ----------------------------------
// - Body interface
// ----------------------------------

bool Simulator::hasListener() const
{
   return mpListener != nullptr;
}

SimulatorListener& Simulator::getListener()
{
   ASSERT_PTR(mpListener);
   return *mpListener;
}

void Simulator::setListener(SimulatorListener& listener)
{
   mpListener = &listener;
}

// ----------------------------------
// - Body interface
// ----------------------------------

Bodies& Simulator::getBodies()
{
   return mBodies;
}

void Simulator::addBody(Body* pbody)
{
   mBodies.push_back(pbody);
}

void Simulator::removeBody(Body& body)
{
   mBodies.remove(body);
}

// ----------------------------------
// - World body interface
// ----------------------------------

const World& Simulator::getWorld() const
{
   ASSERT_PTR(mpWorld);
   return *mpWorld;
}

void Simulator::setWorld(const World& world)
{
   if ( mpWorld != &world )
   {
      mpWorld = &world;

      notifyWorldChanged();
   }
}

// ----------------------------------
// - Notifications
// ----------------------------------

void Simulator::notifyWorldChanged()
{
}
