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
#include "pickvisitor.h"
#ifndef JENGINE_INLINE
#  include "pickvisitor.inl"
#endif

#include "core/entity/entity.h"

PickVisitor::PickVisitor(): 
   mpPicked(NULL),
   mPickPos()
{
}

PickVisitor::PickVisitor(const Vector& pos):
   mpPicked(NULL),
   mPickPos(pos)
{
}

/// \fn PickVisitor::visitEntity(Entity* pentity) 
/// \brief Tries to pick the given object.
void PickVisitor::visitEntity(Entity* pentity) 
{
   // move to a component message to the mesh component

   Vector pos  = pentity->getPosition();
   Vector size = Vector(); //(pactor->getSize() / 2);

   if ( (mPickPos.x >= (pos.x-size.x) && mPickPos.x < (pos.x+size.x)) &&
        (mPickPos.y >= (pos.y-size.y) && mPickPos.y < (pos.y+size.y)))
   {
      mpPicked = pentity;
   }
}
