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
#include "collisionshapes.h"

class CollisionShape;

class Simulator
{
public:
   Simulator();
   virtual ~Simulator() = 0;

   void addBody(Body& body);
   void removeBody(Body& body);

   void addWorldShape(CollisionShape* pshape);

   virtual void run(float timestep) = 0;

protected:
         Bodies&           getBodies();
   const CollisionShapes&  getWorldShapes() const;

private:
   void destroyWorldShapes();

   Bodies          mBodies;
   CollisionShapes mWorldShapes; // owned
};

#endif
