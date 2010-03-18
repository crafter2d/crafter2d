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

#include "guicanvasmouseeventdispatcher.h"

#include "system/timer.h"

#include "guicanvas.h"
#include "guifocus.h"

float GuiCanvasMouseEventDispatcher::sClickSpeed = 0.8f;

GuiCanvasMouseEventDispatcher::GuiCanvasMouseEventDispatcher(GuiCanvas& canvas):
   MouseEventDispatcher(),
   mCanvas(canvas)
{
}

GuiCanvasMouseEventDispatcher::~GuiCanvasMouseEventDispatcher()
{
}

void GuiCanvasMouseEventDispatcher::dispatch(const MouseEvent& event)
{
  GuiWnd& wnd = GuiFocus::getInstance().getFocus();

   switch ( event.getEventType() )
   {
      case MouseEvent::ePressed:
         dispatchButtonPressed(wnd, event);
         break;
      case MouseEvent::eReleased:
         dispatchButtonReleased(wnd, event);
         break;
      case MouseEvent::eMotion:
         dispatchMouseMotion(wnd, event);
         break;
   }
}

void GuiCanvasMouseEventDispatcher::dispatchButtonPressed(GuiWnd& wnd, const MouseEvent& event)
{
   wnd.fireMouseButtonEvent(event);

   mClickTimer = TIMER.getTick();

   // convenience calls as long as not all controls have been converted
   GuiPoint point = GuiPoint(event.getLocation());
   if ( event.isLeftButtonDown() )
      mCanvas.onLButtonDown(point, event.getModifiers());
   else if ( event.isRightButtonDown() )
      mCanvas.onRButtonDown(point, event.getModifiers());
}

void GuiCanvasMouseEventDispatcher::dispatchButtonReleased(GuiWnd& wnd, const MouseEvent& event)
{
   GuiPoint point = GuiPoint(event.getLocation());

   if ( event.isWheelDown() || event.isWheelUp() )
   {
      wnd.fireMouseWheelEvent(event);
   }
   else
   {
      wnd.fireMouseButtonEvent(event);
   }

   if ( TIMER.getTick() - mClickTimer < sClickSpeed )
   {
      // click event
      wnd.fireMouseClickEvent(event);
   }

   // convenience calls as long as not all controls have been converted
   if ( event.isLeftButtonDown() )
      mCanvas.onLButtonUp(point, event.getModifiers());
   else if ( event.isWheelUp() )
      mCanvas.onMouseWheel(point, -1, event.getModifiers());
   else if ( event.isWheelDown() )
      mCanvas.onMouseWheel(point, 1, event.getModifiers());
}

void GuiCanvasMouseEventDispatcher::dispatchMouseMotion(GuiWnd& wnd, const MouseEvent& event)
{
   GuiPoint point = GuiPoint(event.getLocation());
   GuiPoint rel = GuiPoint(event.getRelative());

   mCanvas.onMouseMove(point, rel, event.getModifiers());
}