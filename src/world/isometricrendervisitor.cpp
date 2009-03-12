/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jeroen@jengine.homedns.org                                            *
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

#include "isometricrendervisitor.h"
#ifndef JENGINE_INLINE
#  include "isometricrendervisitor.inl"
#endif

#include "../animatable.h"
#include "../creature.h"

IsometricRenderVisitor::IsometricRenderVisitor():
   RenderVisitor(),
   _objects()
{
}

IsometricRenderVisitor::~IsometricRenderVisitor()
{
}

//////////////////////////////////////////////////////////////////////////
// - Overloads
//////////////////////////////////////////////////////////////////////////

void IsometricRenderVisitor::visitObject(Object* object)
{
   _objects.add(*object);
}

void IsometricRenderVisitor::visitAnimObject(AnimObject* object)
{
   _objects.add(*object);
}

void IsometricRenderVisitor::visitCreature(Creature* object)
{
   _objects.add(*object);
}
