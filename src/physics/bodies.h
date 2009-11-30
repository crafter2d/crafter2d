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
#ifndef BODIES_H_
#define BODIES_H_

#include <vector>

class Body;
class CollisionData;
class CollisionShapes;

class Bodies : public std::vector<Body*>
{
   typedef std::vector<Body*> BodiesImp;

public:
   Bodies();
   ~Bodies();

   void add(Body& body);
   void remove(Body& body);

   void integrate(float timestep);
   void collectContactData(CollisionData& data, const CollisionShapes& worldshapes) const;

private:
   iterator find(Body* pbody);
};

#endif
