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

MouseListeners::MouseListeners()
{
}

MouseListeners::~MouseListeners()
{
}

//=============================================================================
//== container ================================================================
//=============================================================================

void MouseListeners::addListener(MouseListener& listener)
{
  push_back(&listener);
}

void MouseListeners::removeListener(const MouseListener& listener)
{
  iterator it = std::find(begin(), end(), &listener);
  if ( it != end() )
  {
    erase(it);
  }
}

//=============================================================================
//== notifications ============================================================
//=============================================================================

void MouseListeners::fireMouseButtonEvent(const MouseEvent& event)
{
   for ( iterator it = begin(); it != end(); ++it )
   {
      (*it)->onMouseButton(event);
   }
}

void MouseListeners::fireMouseClickEvent(const MouseEvent& event)
{
  for ( iterator it = begin(); it != end(); ++it )
  {
     (*it)->onMouseClick(event);
  }
}
