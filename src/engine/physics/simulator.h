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
#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include "bodies.h"

class Object;
class World;
class SimulatorListener;

/**
@author Jeroen Broekhuizen
\brief A simulator is the engine which performs collision detection and positioning of
all objects in the current game world.

The task of the Simulator is to update the state of the bodies (see class Body). Like the
scenegraph a simulator can have multiple bodies it should take care of. The update takes
place each frame through a call to the run method.

At startup the generateWorldShapes method is called so that the 
*/

class Simulator
{
public:
   Simulator();
   virtual ~Simulator() = 0;

 // listener interface
   bool hasListener() const;
   SimulatorListener& getListener();
   void setListener(SimulatorListener& listener);

 // get/set
   const World& getWorld() const;
   void         setWorld(const World& world);

 // maintenance
   virtual Body& createBody(Object& object) = 0;
   virtual void  removeBody(Body& body);

 // operations
   virtual void run(float timestep) = 0;

protected:
   Bodies& getBodies();
   void    addBody(Body* body);

   virtual void worldChanged();

private:

   const World*       mpWorld;
   Bodies             mBodies;
   SimulatorListener* mpListener;
};

#endif