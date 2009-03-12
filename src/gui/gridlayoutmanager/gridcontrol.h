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
#ifndef GRID_CONTROL_H_
#define GRID_CONTROL_H_

class GuiControl;
class GridAxis;
class GridColumn;
class GridRow;

class GridControl
{
public:
   explicit GridControl(GuiControl& control);
   ~GridControl();

   GuiControl&  getControl();

 // get/set
   void setSingle(GridAxis& axis);
   void setRange(GridAxis& start, GridAxis& end);

 // querying
   bool isHorizontalSpanning() const;
   bool isVerticalSpanning() const;

   float getPreferredWidth() const;
   float getPreferredHeight() const;

 // layouting
   void update();
   void updateSpanningAxes();

private:
   void setStart(GridAxis& axis);
   void setEnd(GridAxis& axis);

   GuiControl& mControl;

   GridColumn* mpStartColumn;
   GridColumn* mpEndColumn;
   GridRow*    mpStartRow;
   GridRow*    mpEndRow;
};

#ifdef JENGINE_INLINE
#  include "gridcontrol.inl"
#endif

#endif // GRID_CONTROL_H_
