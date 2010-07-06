/***************************************************************************
 *   Copyright (C) 2010 by Jeroen Broekhuizen                              *
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

#include "guidesignselectionmouselistener.h"

#include "gui/input/mouseevent.h"

#include "gui/guidesignselection.h"

GuiDesignSelectionMouseListener::GuiDesignSelectionMouseListener(GuiDesignSelection& selection):
   MouseListener(),
   mSelector(selection)
{
}

// - Notifications

void GuiDesignSelectionMouseListener::onMouseButton(const MouseEvent& event)
{
   if ( !event.isLeftButtonDown() )
   {
     return;
   }

   if ( event.getEventType() == MouseEvent::ePressed )
   {
      Point point = event.getLocation();

      mSelector.windowToClient(point);
      mSelector.mBorders = determineBorder(point);

      if ( mSelector.mBorders != GuiDesignSelection::eNone )
      {
         event.consume();
      }
   }
   else
   {
      mSelector.mBorders = GuiDesignSelection::eNone;
   }
}

// - Operations

int GuiDesignSelectionMouseListener::determineBorder(const Point& point) const
{
   const GuiRect& rect = mSelector.getWindowRect();
   int borders = GuiDesignSelection::eNone;

   if ( point.x() == rect.left() || point.x() - rect.left() <= 2 )
   {
      borders = GuiDesignSelection::eLeft;
   }
   else if ( point.x() == rect.right() || abs(rect.right() - point.x())  <= 2 )
   {
      borders = GuiDesignSelection::eRight;
   }
   
   if ( point.y() == rect.top() || point.y() - rect.top() <= 2 )
   {
      SET_FLAG(borders, GuiDesignSelection::eTop);
   }
   else if ( point.y() == rect.bottom() || abs(rect.bottom() - point.y()) <= 2 )
   {
      SET_FLAG(borders, GuiDesignSelection::eBottom);
   }
   
   return borders;
}
