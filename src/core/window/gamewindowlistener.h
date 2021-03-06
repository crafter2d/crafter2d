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
#ifndef GAME_WINDOW_LISTENER_H
#define GAME_WINDOW_LISTENER_H

#include "core/core_base.h"

class CORE_API GameWindowListener
{
public:
   GameWindowListener();
   virtual ~GameWindowListener() = 0;

   /// \fn GameWindowListener::onWindowCreated()
   /// Called when the window is created
   virtual void onWindowCreated();

   /// \fn GameWindowListener::onWindowResized()
   /// Called when game window is resized
   virtual void onWindowResized();

   /// \fn GameWindowListener::onWindowClosing()
   /// Called when user is closing the game window
   /// \return true when game may close, false otherwise
   virtual bool onWindowClosing();

   /// \fn GameWindowListener::onWindowClosed
   /// Called when the game window is closed.
   virtual void onWindowClosed();
};

#endif
