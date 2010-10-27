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
#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <string>
#include <vector>

#include "core/math/color.h"

struct SDL_KeyboardEvent;
struct SDL_MouseMotionEvent;
struct SDL_MouseButtonEvent;
struct SDL_Surface;

class GameWindowListener;
class KeyEventDispatcher;
class MouseEventDispatcher;

class GameWindow
{
public:
   GameWindow();
   ~GameWindow();

  // creation
   bool create(const std::string& title, int width, int height, int bitdepth, bool fullscreen);
   void destroy();

  // get/set
   void setKeyEventDispatcher(KeyEventDispatcher& dispatcher);
   void setMouseEventDispatcher(MouseEventDispatcher& dispatcher);

   const Color& getBackgroundColor() const;
   void         setBackgroundColor(const Color& color);

  // query
   int getWidth() const;
   int getHeight() const;

  // event handling
   void handleEvents();

  // operations
   void resize(int width, int height);
   void toggleFullscreen();

   void takeScreenshot();

  // listeners
   void addListener(GameWindowListener& listener);
   void removeListener(GameWindowListener& listener);

private:
   typedef std::vector<GameWindowListener*> Listeners;

  // query
   int getWindowFlags(bool fullscreen);

  // callbacks
   void onKeyboardEvent(SDL_KeyboardEvent& event);
   void onMouseMotionEvent(SDL_MouseMotionEvent& event);
   void onMouseButtonEvent(SDL_MouseButtonEvent& event);
   void onQuit();

   Listeners             mListeners;
   KeyEventDispatcher*   mpKeyDispatcher;
   MouseEventDispatcher* mpMouseDispatcher;
   SDL_Surface*          mpWindow;
   Color                 mBackgroundColor;
   int                   mBitDepth;
   int                   mFlags;
};

#endif
