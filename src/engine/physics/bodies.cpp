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
#include "bodies.h"

#include <algorithm>

#include "body.h"
#include "collisiondetector.h"

Bodies::Bodies():
   BodiesImp()
{
}

Bodies::~Bodies()
{
}

void Bodies::add(Body& body)
{
   push_back(&body);
}

void Bodies::remove(Body& body)
{
   Bodies::iterator it = find(&body);
   if ( it != end() )
   {
      erase(it);
   }
}

Bodies::iterator Bodies::find(Body* pbody)
{
   return std::find(begin(), end(), pbody);
}

void Bodies::integrate(float timestep)
{
   for ( size_type index = 0; index < size(); ++index )
   {
      Body* pbody = operator[](index);
      pbody->integrate(timestep);
   }
}

void Bodies::finalize()
{
   for ( size_type index = 0; index < size(); ++index )
   {
      Body* pbody = operator[](index);
      pbody->finalize();
   }
}

/*
void Bodies::collectContactData(CollisionData& data, const CollisionShapes& worldshapes) const
{
   CollisionDetector detector(data);

   for ( size_type index = 0; index < size(); ++index )
   {
      const Body& body = *(operator[](index));

      detector.collectContactData(body, worldshapes);

      for ( size_type other = index+1; other < size(); ++other )
      {
         const Body& otherbody = *(operator[](index));

         detector.collectContactData(body, otherbody);
      }
   }
}
*/
