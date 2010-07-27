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

#include "mouselisteners.h"

#include <algorithm>

#include "mouseevent.h"
#include "mouselistener.h"

MouseListeners::MouseListeners():
   MouseListenersImp()
{
}

MouseListeners::~MouseListeners()
{
}

//-----------------------------------
// - Notifications
//-----------------------------------

void MouseListeners::fireMouseButtonEvent(const MouseEvent& event)
{
   for ( iterator it = begin(); it != end(); ++it )
   {
      MouseListener* plistener = (*it);
      plistener->onMouseButton(event);

      if ( event.isConsumed() )
        break;
   }
}

void MouseListeners::fireMouseClickEvent(const MouseEvent& event)
{
   for ( iterator it = begin(); it != end(); ++it )
   {
      MouseListener* plistener = (*it);
      plistener->onMouseClick(event);

      if ( event.isConsumed() )
        break;
   }
}

void MouseListeners::fireMouseMotionEvent(const MouseEvent& event)
{
   for ( iterator it = begin(); it != end(); ++it )
   {
      MouseListener* plistener = (*it);
      plistener->onMouseMotion(event);

      if ( event.isConsumed() )
        break;
   }
}

void MouseListeners::fireMouseContextEvent(const MouseEvent& event)
{
   for ( iterator it = begin(); it != end(); ++it )
   {
      MouseListener* plistener = (*it);
      plistener->onMouseContext(event);

      if ( event.isConsumed() )
        break;
   }
}

void MouseListeners::fireMouseEnterEvent(const MouseEvent& event)
{
   for ( iterator it = begin(); it != end(); ++it )
   {
      (*it)->onMouseEntered(event);
   }
}

void MouseListeners::fireMouseExitEvent(const MouseEvent& event)
{
   for ( iterator it = begin(); it != end(); ++it )
   {
      (*it)->onMouseExited(event);
   }
}
