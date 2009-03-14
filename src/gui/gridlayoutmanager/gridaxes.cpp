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
#include "gridaxes.h"

#include <exception>

#include "../guicontrol.h"

#include "gridcontrol.h"
#include "gridaxis.h"

GridAxes::GridAxes():
   mAxes()
{
}

GridAxes::~GridAxes()
{
}

//////////////////////////////////////////////////////////////////////////
// - Maintenance
//////////////////////////////////////////////////////////////////////////

void GridAxes::insert(int pos, GridAxis* paxis)
{
   Axes::iterator it = mAxes.begin() + pos;
   mAxes.insert(it, paxis);

   renumber();
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

GridAxis& GridAxes::get(int index)
{
   return *(mAxes[index]);
}

void GridAxes::insert(GridControl& control, float position, float size, bool grows)
{
   GuiControl& guicontrol = control.getControl();
   const GuiRect& rect    = guicontrol.getWindowRect();

   GridAxis& axis = resolveByPosition(position);

   if ( !grows )
   {
      GridAxis* pnewaxis = axis.split(size);
      insert(axis.getIndex() + 1, pnewaxis);
   }

   control.setSingle(axis);
}

void GridAxes::renumber()
{
   for ( Axes::size_type index = 0; index < mAxes.size(); index++ )
   {
      GridAxis& axis = *(mAxes[index]);
      axis.setIndex(index);
   }
}

void GridAxes::registerControl(GridControl& control, GridAxis& start, GridAxis& end)
{
   for ( Axes::size_type idx = start.getIndex(); idx < end.getIndex(); ++idx )
   {
      GridAxis& axis = get(idx);
      axis.registerControl(control);
   }
}

void GridAxes::updatePosition()
{
   float position = 0;
   for ( Axes::size_type idx = 0; idx < mAxes.size(); ++idx )
   {
      GridAxis& axis = get(idx);
      axis.setOrigin(position);
      position += axis.getSize();
   }
}

void GridAxes::updateSize()
{
   for ( Axes::size_type idx = 0; idx < mAxes.size(); ++idx )
   {
      GridAxis& axis = get(idx);
      axis.calculatePreferredSize();
   }
}

void GridAxes::updateSizes(float preferredsize, GridAxis& start, GridAxis& end)
{
   float size = getSize(start, end);
   if ( size < preferredsize )
   {
      Axes axes;

      getGrowables(axes, start, end);

      if ( axes.empty() )
         getRange(axes, start, end);

      float add = (preferredsize - size) / axes.size();
      for ( Axes::size_type idx = 0; idx < axes.size(); ++idx )
      {
         GridAxis& axis = *(axes[idx]);
         axis.setSize(axis.getSize() + add);
      }
   }
}

float GridAxes::getSize()
{
   if ( mAxes.empty() )
      return 0;
   else
      return getSize(*mAxes.front(), *mAxes.back());
}

float GridAxes::getSize(GridAxis& start, GridAxis& end)
{
   float result = 0;
   for ( Axes::size_type idx = start.getIndex(); idx < end.getIndex(); ++idx )
   {
      GridAxis& axis = get(idx);
      result += axis.getSize();
   }

   return result;
}

void GridAxes::getRange(Axes& axes, GridAxis& start, GridAxis& end)
{
   for ( Axes::size_type idx = start.getIndex(); idx < end.getIndex(); ++idx )
   {
      GridAxis* paxis = mAxes[idx];
      axes.push_back(paxis);
   }
}

void GridAxes::getGrowables(Axes& growables, GridAxis& start, GridAxis& end)
{
   for ( Axes::size_type idx = start.getIndex(); idx < end.getIndex(); ++idx )
   {
      GridAxis& axis = get(idx);
      if ( axis.grows() )
      {
         growables.push_back(&axis);
      }
   }
}

//////////////////////////////////////////////////////////////////////////
// - Searching
//////////////////////////////////////////////////////////////////////////

GridAxis& GridAxes::resolveByPosition(float position)
{
   for ( Axes::size_type idx = 0; idx < mAxes.size(); ++idx )
   {
      GridAxis& axis = *(mAxes[idx]);
      if ( position < axis.getEnd() )
      {
         return axis;
      }
   }

   throw std::exception();
}
