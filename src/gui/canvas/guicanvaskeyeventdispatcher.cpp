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

#include "guicanvaskeyeventdispatcher.h"

#include "gui/input/keyevent.h"
#include "gui/guifocus.h"

#include "guicanvas.h"

GuiCanvasKeyEventDispatcher::GuiCanvasKeyEventDispatcher(GuiCanvas& canvas):
   KeyEventDispatcher(),
   mCanvas(canvas)
{
}

GuiCanvasKeyEventDispatcher::~GuiCanvasKeyEventDispatcher()
{
}

//-----------------------------------
// - Dispatching
//-----------------------------------

void GuiCanvasKeyEventDispatcher::dispatch(const KeyEvent& keyevent)
{
   if ( GuiFocus::getInstance().hasFocus() )
   {
      GuiWnd& focussed = GuiFocus::getInstance().getFocus();
      focussed.fireKeyEvent(keyevent);
   }

   // for not converted windows
   if ( keyevent.getEventType() == KeyEvent::ePressed )
   {
     mCanvas.onKeyDown(keyevent.getKey(), keyevent.isShiftDown(), keyevent.isCtrlDown(), keyevent.isAltDown());
   }
   else
   {
     mCanvas.onKeyUp(keyevent.getKey());
   }
}
