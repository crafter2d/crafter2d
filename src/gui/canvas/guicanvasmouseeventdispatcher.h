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
#ifndef GUICANVAS_MOUSEEVENT_DISPATCHER_H
#define GUICANVAS_MOUSEEVENT_DISPATCHER_H

#include "core/input/mouseeventdispatcher.h"

#include "gui/guiwndreference.h"

class GuiCanvas;
class GuiWnd;
class MouseEvent;

class GuiCanvasMouseEventDispatcher : public MouseEventDispatcher
{
public:
   static float sClickSpeed;

   explicit GuiCanvasMouseEventDispatcher(GuiCanvas& canvas);
   virtual ~GuiCanvasMouseEventDispatcher();

   virtual void dispatch(const MouseEvent& event);

private:
   void dispatchButtonPressed(const MouseEvent& event);
   void dispatchButtonReleased(const MouseEvent& event);
   void dispatchMouseMotion(const MouseEvent& event);

   GuiCanvas& mCanvas;
   GuiWndReference    mWindow;
   float      mClickTimer;
   int        mClickButton;
   GuiWndReference mClickWindow;
};

#endif
