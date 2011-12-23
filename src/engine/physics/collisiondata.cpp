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
#include "collisiondata.h"

#include "core/defines.h"
#include "core/containers/listalgorithms.h"
#include "core/containers/listiterator.h"

CollisionData::CollisionData():
   mContacts()
{
}

CollisionData::~CollisionData()
{
   ListAlgorithms::Flush<CollisionContact>(mContacts);
}

// maintenance

void CollisionData::addContact(CollisionContact* pcontact)
{
   mContacts.addTail(pcontact);
}

CollisionData::ContactIterator CollisionData::getIterator()
{
   return ListIterator<CollisionContact>(mContacts);
}

// resolve preparation

void CollisionData::prepare(float timestep)
{
   ContactIterator it(mContacts);
   while ( it.isValid() )
   {
      CollisionContact& contact = *it;
      contact.prepare(timestep);
      ++it;
   }
}

void CollisionData::adjustPositions(float timestep)
{
   int iterations = 1;

   /*
   int usediterations = 0;
   while ( usediterations < iterations )
   {
      max = positionEpsilon;
      index = numContacts;
      for ( int i=0; i<numContacts; i++ )
      {
         if (c[i].penetration > max)
         {
             max = c[i].penetration;
             index = i;
         }
      }
      if (index == numContacts) break;
   }*/
}

void CollisionData::adjustVelocities(float timestep)
{
}
