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
#include "guiheadercontrolmouselistener.h"

#include "gui/input/mouseevent.h"

#include "guiheadercontrol.h"
#include "guiheadercolumn.h"

GuiHeaderControlMouseListener::GuiHeaderControlMouseListener(GuiHeaderCtrl& header):
   MouseListener(),
   mHeader(header),
   mColumn(-1)
{
}

// - Notifications

void GuiHeaderControlMouseListener::onMouseButton(const MouseEvent& event)
{
   if ( !event.isLeftButtonDown() )
   {
      return;
   }

   if ( event.getEventType() == MouseEvent::ePressed )
   {
      Point point = event.getLocation();

      mHeader.windowToClient(point);
      mColumn = findColumn(point);
   }
   else
   {
      mColumn = -1;
   }
}

void GuiHeaderControlMouseListener::onMouseMotion(const MouseEvent& event)
{
   if ( mColumn > -1 )
   {
      GuiHeaderColumn& column = *mHeader.getColumns()[mColumn];
      column.resize(event.getRelative().x());

      for ( int index = mColumn+1; index < mHeader.getColumns().size(); index++ )
      {
         GuiHeaderColumn& column = *mHeader.getColumns()[index];
         column.rect().offset(event.getRelative().x(), 0);
      }
   }
}

// - Utility functions

int GuiHeaderControlMouseListener::findColumn(const Point& point) const
{
   const GuiHeaderCtrl::Columns& columns = mHeader.getColumns();
   for ( int n = 0, x = 0; n < columns.size(); ++n )
   {
      const GuiHeaderColumn& column = *columns[n];
      x += column.width();

      if ( abs(point.x() - x) < 4 )
      {
         return n;
      }
   }

   return -1;
}
