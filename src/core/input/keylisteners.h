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
#ifndef KEYBOARD_LISTENER_H
#define KEYBOARD_LISTENER_H

#include "listeners.h"

namespace Input
{
   class KeyEvent;
   class KeyListener;

   class KeyListeners : public Listeners<KeyListener>
   {
   public:
      KeyListeners();
      ~KeyListeners();

     // notifications
      void fireKeyEvent(const KeyEvent& event);

   protected:
     // notifications
      void fireKeyPressed(const KeyEvent& event);
      void fireKeyReleased(const KeyEvent& event);

   private:

    // listeners
      Listeners mListeners;
   };
}

#endif