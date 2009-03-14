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
#include "gridcontrol.h"
#ifndef JENGINE_INLINE
#  include "gridcontrol.inl"
#endif

#include "../guicontrol.h"

#include "gridcolumn.h"
#include "gridrow.h"
#include "gridaxes.h"

GridControl::GridControl(GuiControl& control):
   mControl(control),
   mpStartColumn(NULL),
   mpEndColumn(NULL),
   mpStartRow(NULL),
   mpEndRow(NULL)
{
}

GridControl::~GridControl()
{
}

//////////////////////////////////////////////////////////////////////////
// - Get/set
//////////////////////////////////////////////////////////////////////////

void GridControl::setSingle(GridAxis& axis)
{
   setRange(axis, axis);
}

void GridControl::setRange(GridAxis& start, GridAxis& end)
{
   setStart(start);
   setEnd(end);

   start.getOwner().registerControl(*this, start, end);
}

void GridControl::setStart(GridAxis& axis)
{
   if ( axis.isColumn() )
      mpStartColumn = &axis.asColumn();
   else
      mpStartRow    = &axis.asRow();
}

void GridControl::setEnd(GridAxis& axis)
{
   if ( axis.isColumn() )
      mpEndColumn = &axis.asColumn();
   else
      mpEndRow    = &axis.asRow();
}

//////////////////////////////////////////////////////////////////////////
// - Querying
//////////////////////////////////////////////////////////////////////////

bool GridControl::isHorizontalSpanning() const
{
   return mpStartColumn != mpEndColumn;
}

bool GridControl::isVerticalSpanning() const
{
   return mpStartRow != mpEndRow;
}

float GridControl::getPreferredWidth() const
{
   return mControl.getPreferredWidth();
}

float GridControl::getPreferredHeight() const
{
   return mControl.getPreferredHeight();
}

//////////////////////////////////////////////////////////////////////////
// - Layouting
//////////////////////////////////////////////////////////////////////////

void GridControl::update()
{
   GuiRect bounds(
      mpStartColumn->getOrigin(),
      mpStartRow->getOrigin(),
      mpStartColumn->getOwner().getSize(*mpStartColumn, *mpEndColumn),
      mpStartRow->getOwner().getSize(*mpStartRow, *mpEndRow));

   mControl.setWindowRect(bounds);
}

void GridControl::updateSpanningAxes()
{
   if ( isHorizontalSpanning() )
   {
      mpStartColumn->getOwner().updateSizes(getPreferredWidth(), *mpStartColumn, *mpEndColumn);
   }
}
