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

#include "buttonmouselistener.h"

#include "gui/input/mouseevent.h"

#include "guibutton.h"

ButtonMouseListener::ButtonMouseListener(GuiButton& button):
   MouseListener(),
   mButton(button),
   mClicking(false)
{
}

//-----------------------------------
// - Notifications
//-----------------------------------

void ButtonMouseListener::onMouseButton(const MouseEvent& event)
{
   if ( event.getEventType() == MouseEvent::ePressed )
   {
      mButton.pressed(true);

      mClicking = true;
   }
}

void ButtonMouseListener::onMouseClick(const MouseEvent& event)
{
   if ( event.getButtons() == MouseEvent::eLeft )
   {
      mButton.pressed(false);
      mButton.click();

      mClicking = false;

      event.consume();
   }
}

void ButtonMouseListener::onMouseEntered(const MouseEvent& event)
{
   if ( mClicking )
   {
      mButton.pressed(true);
   }
   else
   {
      mButton.setHoover(true);
   }
}

void ButtonMouseListener::onMouseExited(const MouseEvent& event)
{
   if ( mClicking )
   {
      mButton.pressed(false);
   }
   else
   {
      mButton.setHoover(false);
   }
}
