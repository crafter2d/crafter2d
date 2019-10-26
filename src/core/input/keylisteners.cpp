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

#include "keylisteners.h"

#include <algorithm>

#include "keyevent.h"
#include "keylistener.h"

namespace Input
{

KeyListeners::KeyListeners():
   Listeners<KeyListener>()
{
}

KeyListeners::~KeyListeners()
{
}

//=============================================================================
//== notifications ============================================================
//=============================================================================

void KeyListeners::fireKeyEvent(const KeyEvent& event)
{
  if ( event.getEventType() == KeyEvent::ePressed )
  {
    fireKeyPressed(event);
  }
  else
  {
    fireKeyReleased(event);
  }
}

void KeyListeners::fireKeyPressed(const KeyEvent& event)
{
  for ( iterator it = begin(); it != end(); ++it )
  {
    (*it)->onKeyPressed(event);
  }
}

void KeyListeners::fireKeyReleased(const KeyEvent& event)
{
  for ( iterator it = begin(); it != end(); ++it )
  {
    (*it)->onKeyReleased(event);
  }
}

} // namespace Input
