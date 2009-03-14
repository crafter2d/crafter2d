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

#include "object.h"
#include "animatable.h"
#include "creature.h"

PickVisitor::PickVisitor(): 
   picked(0) 
{
}

PickVisitor::PickVisitor(const Vector& pos):
   picked(0),
   pickPos(pos)
{
}

/// \fn PickVisitor::apply(Object* object) 
/// \brief Tries to pick the given object.
void PickVisitor::visitObject(Object* object) 
{
   Vector pos  = object->getPosition();
   Vector size = (object->getSize() / 2);

   if ((pickPos.x >= (pos.x-size.x) && pickPos.x < (pos.x+size.x)) &&
      (pickPos.y >= (pos.y-size.y) && pickPos.y < (pos.y+size.y))) {
      picked = object;
   }
}

/// \fn PickVisitor::visitAnimObject(AnimObject* object) 
/// \brief Tries to pick the given object.
void PickVisitor::visitAnimObject(AnimObject* object)
{
   visitObject(object);
}

/// \fn PickVisitor::visitCreature(Creature* object) 
/// \brief Tries to pick the given object.
void PickVisitor::visitCreature(Creature* object)
{
   visitObject(object);
}