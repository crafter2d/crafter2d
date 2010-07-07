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
#include "guidialogmouselistener.h"

#include "gui/input/mouseevent.h"

#include "guidialog.h"

GuiDialogMouseListener::GuiDialogMouseListener(GuiDialog& dialog):
   MouseListener(),
   mDialog(dialog),
   mDragging(false)
{
}

// notifications

void GuiDialogMouseListener::onMouseButton(const MouseEvent& event)
{
   if ( event.isLeftButtonDown() )
   {
      Point location = event.getLocation();
      mDialog.windowToClient(location);

      switch ( event.getEventType() )
      { 
      case MouseEvent::ePressed:
         if ( mDialog.isAboveTitleBar(location) && !mDialog.isAboveCloseButton(location) )
         {
            mDragging = true;
            event.consume();
         }
         break;
      case MouseEvent::eReleased:
         mDragging = false;
         event.consume();
         break;
      default:
         break;
      }
   }
}

void GuiDialogMouseListener::onMouseClick(const MouseEvent& event)
{
   Point point = event.getLocation();
   mDialog.windowToClient(point);

   if ( event.isLeftButtonDown() && mDialog.isAboveCloseButton(point) )
   {
      mDialog.close(false);

      event.consume();
   }
}

void GuiDialogMouseListener::onMouseMotion(const MouseEvent& event)
{
   if ( mDragging )
   {
      mDialog.moveWindow(event.getRelative().x(), event.getRelative().y());
   }
   else
   {
      Point point = event.getLocation();

      mDialog.windowToClient(point);
      mDialog.setHoverCloseButton(mDialog.isAboveCloseButton(point));
   }
}
