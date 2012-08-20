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
#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <vector>

#include "core/math/color.h"

#include "engine/window/gamewindow.h"

struct SDL_KeyboardEvent;
struct SDL_MouseMotionEvent;
struct SDL_MouseButtonEvent;
struct SDL_Surface;

class GameWindowListener;
class KeyEventDispatcher;
class MouseEventDispatcher;

class SDLGameWindow : public GameWindow
{
public:
   SDLGameWindow();
   ~SDLGameWindow();

 // get/set
   const Color& getBackgroundColor() const;
   void         setBackgroundColor(const Color& color);

 // query
   int getWidth() const;
   int getHeight() const;

 // operations
   void resize(int width, int height);
   void toggleFullscreen();
   void update();
   void display();

   void takeScreenshot();

protected:
 // creation
   bool doCreate(const String& title, int width, int height, int bitdepth, bool fullscreen);
   void doDestroy();

private:
 // query
   int getWindowFlags(bool fullscreen);

 // event handling
   void handleEvents();

 // callbacks
   void onKeyboardEvent(SDL_KeyboardEvent& event);
   void onMouseMotionEvent(SDL_MouseMotionEvent& event);
   void onMouseButtonEvent(SDL_MouseButtonEvent& event);
   void onQuit();

   SDL_Surface*          mpWindow;
   Color                 mBackgroundColor;
   int                   mBitDepth;
   unsigned int          mFlags;
};

#endif
