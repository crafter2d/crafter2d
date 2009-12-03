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
#include "forcegenerators.h"

#include "forcegenerator.h"

ForceGenerators::ForceGenerators():
   ForceGeneratorsImp()
{
}

ForceGenerators::~ForceGenerators()
{
}

void ForceGenerators::applyForces(PhysicsBody& body) const
{
  for ( ForceGeneratorsImp::size_type index = 0; index < size(); ++index )
  {
    const ForceGenerator* pgenerator = operator[](index);
    pgenerator->applyForce(body);
  }
}
