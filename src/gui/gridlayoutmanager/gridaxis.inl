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
#include "../../defines.h"

INLINE GridAxes& GridAxis::getOwner()
{
   return mOwner;
}

int GridAxis::getIndex() const
{
   return mIndex;
}

void GridAxis::setIndex(int index)
{
   mIndex = index;
}

float GridAxis::getOrigin() const
{
   return mOrigin;
}

void GridAxis::setOrigin(float origin)
{
   mOrigin = origin;
}

float GridAxis::getSize() const
{
   return mSize;
}

void GridAxis::setSize(float size)
{
   mSize = size;
}

float GridAxis::getEnd() const
{
   return mOrigin + mSize;
}

GridAxisControls& GridAxis::getControls()
{
   return mControls;
}

const GridAxisControls& GridAxis::getControls() const
{
   return mControls;
}

int GridAxis::getGrowWeight() const
{
   return mGrowWeight;
}

void GridAxis::setGrowWeight(int weight)
{
   mGrowWeight = weight;
}

bool GridAxis::grows() const
{
   return mGrowWeight > 0;
}
