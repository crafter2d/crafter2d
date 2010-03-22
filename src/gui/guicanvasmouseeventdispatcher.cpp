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

#include "input/mouseevent.h"

#include "guicanvas.h"
#include "guifocus.h"

float GuiCanvasMouseEventDispatcher::sClickSpeed = 0.8f;

GuiCanvasMouseEventDispatcher::GuiCanvasMouseEventDispatcher(GuiCanvas& canvas):
   MouseEventDispatcher(),
   mCanvas(canvas),
   mpWindow(NULL),
   mClickTimer(0.0f),
   mClickButton(MouseEvent::eInvalid),
   mpClickWindow(NULL)
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
   else if ( mpWindow != NULL )
   {
      mpWindow->setFocus();
      mpWindow->getMouseListeners().fireMouseButtonEvent(event);

      mClickButton  = event.getButtons();
      mpClickWindow = mpWindow;
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
   if ( mpWindow != NULL )
   {
      MouseListeners& listeners = mpWindow->getMouseListeners();
      listeners.fireMouseButtonEvent(event);

      if ( mpWindow == mpClickWindow )
      {
         listeners.fireMouseClickEvent(event);

         mpClickWindow = NULL;
      }
   }

   // convenience calls as long as not all controls have been converted
   GuiPoint point = GuiPoint(event.getLocation());
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
   if ( mpWindow != pwindow )
   {
      if ( mpClickWindow != NULL )
      {
         // when clicking, only send enter/exit events to click window
         if ( pwindow != mpClickWindow )
         {
            mpClickWindow->getMouseListeners().fireMouseExitEvent(event);
            mpWindow = NULL;
         }
         else if ( mpWindow == NULL )
         {
           mpClickWindow->getMouseListeners().fireMouseEnterEvent(event);
           mpWindow = mpClickWindow;
         }
      }
      else
      {
         // send enter/exit events to the windows
         if ( mpWindow != NULL )
            mpWindow->getMouseListeners().fireMouseExitEvent(event);
         if ( pwindow != NULL )
            pwindow->getMouseListeners().fireMouseEnterEvent(event);

         mpWindow = pwindow;
      }
   }

   if ( mpWindow != NULL )
   {
      mpWindow->fireMouseMotionEvent(event);
   }

   // convenience call during conversion
   mCanvas.onMouseMove(point, rel, event.getModifiers());
}