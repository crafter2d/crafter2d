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
#include "gridaxis.h"
#ifndef JENGINE_INLINE
#  include "gridaxis.inl"
#endif

#include <exception>

#include "../../autoptr.h"

GridAxis::GridAxis(GridAxes& owner):
   mControls(),
   mOwner(owner),
   mIndex(0),
   mOrigin(0),
   mSize(0),
   mPreferredSize(0),
   mGrowWeight(0)
{
}

GridAxis::~GridAxis()
{
}

//////////////////////////////////////////////////////////////////////////
// - Downcast
//////////////////////////////////////////////////////////////////////////

bool GridAxis::isColumn() const
{
   return false;
}

GridColumn& GridAxis::asColumn()
{
   throw new std::exception();
}

bool GridAxis::isRow() const
{
   return false;
}
  
GridRow& GridAxis::asRow()
{
   throw new std::exception();
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

GridAxis* GridAxis::virConstruct()
{
   throw std::exception();
}

void GridAxis::registerControl(GridControl& control)
{
   mControls.add(control);
}

GridAxis* GridAxis::split(float size)
{
   AutoPtr<GridAxis> axis = virConstruct();

   axis->setOrigin(getOrigin() + size);
   axis->setSize(getSize() - size);

   setSize(size);

   return axis.release();
}

void GridAxis::calculatePreferredSize()
{
   mPreferredSize = virCalculatePreferredSize();
}

float GridAxis::virCalculatePreferredSize() const
{
   return 0;
}
