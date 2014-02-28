/***************************************************************************
 *   Copyright (C) 2010 by Jeroen Broekhuizen                              *
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
#ifndef WORLD_SIMULATOR_LISTENER_H_
#define WORLD_SIMULATOR_LISTENER_H_

#include "core/physics/simulatorlistener.h"

class World;

class WorldSimulatorListener : public SimulatorListener
{
public:
   WorldSimulatorListener(World& world);
   virtual ~WorldSimulatorListener();

   virtual void collideObjectWorld(Entity& object, Bound& bound, int side, bool begin);
   virtual void collideObjectObject(Entity& object, Entity& target, int side, bool begin);

private:
   World& mWorld;
};

#endif
