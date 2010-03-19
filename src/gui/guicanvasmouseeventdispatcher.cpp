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
   mCanvas(canvas),
   mClickTimer(0.0f),
   mClickButton(MouseEvent::eInvalid),
   mpWindow(NULL)
{
}

GuiCanvasMouseEventDispatcher::~GuiCanvasMouseEventDispatcher()
{
}

void GuiCanvasMouseEventDispatcher::dispatch(const MouseEvent& event)
{
   switch ( event.getEventType() )
   {
      case MouseEvent::ePressed:
         dispatchButtonPressed(event);
         break;
      case MouseEvent::eReleased:
         dispatchButtonReleased(event);
         break;
      case MouseEvent::eMotion:
         dispatchMouseMotion(event);
         break;
   }
}

void GuiCanvasMouseEventDispatcher::dispatchButtonPressed(const MouseEvent& event)
{
   if ( event.isWheelDown() || event.isWheelUp() )
   {
      if ( GuiFocus::getInstance().hasFocus() )
      {
         GuiFocus::getInstance().getFocus().fireMouseWheelEvent(event);
      }
   }
   else
   {
      mpWindow = mCanvas.findWindowAtLocation(event.getLocation());
      ASSERT_PTR(mpWindow);

      mpWindow->setFocus();
      mpWindow->fireMouseButtonEvent(event);
   }
   
   // convenience calls as long as not all controls have been converted
   GuiPoint point = GuiPoint(event.getLocation());
   if ( event.isLeftButtonDown() )
      mCanvas.onLButtonDown(point, event.getModifiers());
   else if ( event.isRightButtonDown() )
      mCanvas.onRButtonDown(point, event.getModifiers());
}

void GuiCanvasMouseEventDispatcher::dispatchButtonReleased(const MouseEvent& event)
{
   GuiPoint point = GuiPoint(event.getLocation());

   GuiWnd* pwindow = mCanvas.findWindowAtLocation(event.getLocation());
   ASSERT_PTR(pwindow);

   pwindow->fireMouseButtonEvent(event);

   if ( pwindow == mpWindow )
   {
      // same window --> click
      pwindow->fireMouseClickEvent(event);
   }
   
   // convenience calls as long as not all controls have been converted
   if ( event.isLeftButtonDown() )
      mCanvas.onLButtonUp(point, event.getModifiers());
   else if ( event.isWheelUp() )
      mCanvas.onMouseWheel(point, -1, event.getModifiers());
   else if ( event.isWheelDown() )
      mCanvas.onMouseWheel(point, 1, event.getModifiers());
}

void GuiCanvasMouseEventDispatcher::dispatchMouseMotion(const MouseEvent& event)
{
   GuiPoint point = GuiPoint(event.getLocation());
   GuiPoint rel = GuiPoint(event.getRelative());

   GuiWnd* pwindow = mCanvas.findWindowAtLocation(event.getLocation());
   ASSERT_PTR(pwindow);

   pwindow->fireMouseMotionEvent(event);

   mCanvas.onMouseMove(point, rel, event.getModifiers());
}