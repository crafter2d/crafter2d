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

#include "gui/input/mouseevent.h"
#include "gui/guifocus.h"

#include "guicanvas.h"

float GuiCanvasMouseEventDispatcher::sClickSpeed = 0.8f;

GuiCanvasMouseEventDispatcher::GuiCanvasMouseEventDispatcher(GuiCanvas& canvas):
   MouseEventDispatcher(),
   mCanvas(canvas),
   mWindow(),
   mClickTimer(0.0f),
   mClickButton(MouseEvent::eInvalid),
   mClickWindow()
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
   else if ( mWindow.isAlive() )
   {
      mWindow->setFocus();
      mWindow->getMouseListeners().fireMouseButtonEvent(event);

      mClickButton = event.getButtons();
      mClickWindow = mWindow;
   }
}

void GuiCanvasMouseEventDispatcher::dispatchButtonReleased(const MouseEvent& event)
{
   if ( mWindow.isAlive() )
   {
      bool fireclick = (mWindow == mClickWindow);
      if ( fireclick )
      {
         mClickWindow.clear();
      }

      MouseListeners& listeners = mWindow->getMouseListeners();
      listeners.fireMouseButtonEvent(event);

      if ( fireclick )
      {
         if ( event.isRightButtonDown() )
         {
            listeners.fireMouseContextEvent(event);
         }
         listeners.fireMouseClickEvent(event);
      }
   }
}

void GuiCanvasMouseEventDispatcher::dispatchMouseMotion(const MouseEvent& event)
{
   GuiPoint point = GuiPoint(event.getLocation());
   GuiPoint rel = GuiPoint(event.getRelative());

   GuiWnd* pwindow = mCanvas.findWindowAtLocation(event.getLocation());
   if ( mWindow.instancePtr() != pwindow )
   {
      if ( mClickWindow.isAlive() )
      {
         // when clicking, only send enter/exit events to click window
         if ( mClickWindow == *pwindow )
         {
            mClickWindow->getMouseListeners().fireMouseExitEvent(event);
            mWindow.clear();
         }
         else if ( mWindow.isAlive() )
         {
           mClickWindow->getMouseListeners().fireMouseEnterEvent(event);
           mWindow = mClickWindow;
         }
      }
      else
      {
         // send enter/exit events to the windows
         if ( mWindow.isAlive() )
            mWindow->getMouseListeners().fireMouseExitEvent(event);

         mWindow = pwindow;

         if ( mWindow.isAlive() )
            mWindow->getMouseListeners().fireMouseEnterEvent(event);
      }
   }

   if ( mWindow.isAlive() )
   {
      mWindow->getMouseListeners().fireMouseMotionEvent(event);
   }
}