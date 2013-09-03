/***************************************************************************
 *   Copyright (C) 2011 by Jeroen Broekhuizen                              *
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
#ifndef GAME_WINDOW_H_
#define GAME_WINDOW_H_

#include "engine/engine_base.h"

#include <vector>

namespace Input
{
   class MouseEventDispatcher;
   class MouseEvent;
   class KeyEventDispatcher;
   class KeyEvent;
}

class Driver;
class GameWindowListener;
class String;

class ENGINE_API GameWindow
{
   typedef std::vector<GameWindowListener*> Listeners;

public:
   GameWindow();
   virtual ~GameWindow() = 0;

 // creation
   bool create(const String& title, int width, int height, int bitdepth, bool fullscreen);
   void destroy();

 // query
   virtual Driver* loadDriver() = 0;
   virtual int getHandle() const = 0;
   virtual int getWidth() const;
   virtual int getHeight() const;

 // operations
   virtual void resize(int width, int height) = 0;
   virtual void toggleFullscreen() = 0;
   virtual void update() = 0;
   virtual void display() = 0;

 // dispatchers
   void setKeyEventDispatcher(Input::KeyEventDispatcher& dispatcher);
   void setMouseEventDispatcher(Input::MouseEventDispatcher& dispatcher);

 // listeners
   void addListener(GameWindowListener& listener);
   void removeListener(GameWindowListener& listener);

protected:
 // creation
   virtual bool doCreate(const String& title, int width, int height, int bitdepth, bool fullscreen);
   virtual void doDestroy();

   Driver* doLoadDriver(const String& driver);

 // listener notification
   void fireWindowClosed();
   void fireWindowResized();

 // dispatch
   void dispatch(const Input::MouseEvent& mouseevent);
   void dispatch(const Input::KeyEvent& keyevent);

private:

 // members
   Listeners                     mListeners;
   Input::KeyEventDispatcher*    mpKeyDispatcher;
   Input::MouseEventDispatcher*  mpMouseDispatcher;
};

#endif // GAME_WINDOW_H_
