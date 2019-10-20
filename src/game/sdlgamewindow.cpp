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
#include "sdlgamewindow.h"

#include <SDL2/SDL.h>

#include <algorithm>

#include "core/string/string.h"
#include "core/input/keyeventdispatcher.h"
#include "core/input/keyevent.h"
#include "core/input/mouseeventdispatcher.h"
#include "core/input/mouseevent.h"

#include "console.h"

using namespace Input;

static int getModifiers()
{
   int modifiers = InputEvent::eNone;

   SDL_Keymod mode = SDL_GetModState();
   if ( mode & KMOD_SHIFT )
      SET_FLAG(modifiers, InputEvent::eShift);

   if ( mode & KMOD_CTRL )
      SET_FLAG(modifiers, InputEvent::eCtrl);

   if ( mode & KMOD_ALT )
      SET_FLAG(modifiers, InputEvent::eAlt);

   return modifiers;
}

static MouseEvent::Button toMouseEventButton(int sdlbutton)
{
   /*
   switch ( sdlbutton )
   {
      case SDL_BUTTON_LEFT:      return MouseEvent::eLeft;
      case SDL_BUTTON_RIGHT:     return MouseEvent::eRight;
      case SDL_BUTTON_MIDDLE:    return MouseEvent::eMiddle;
      case SDL_BUTTON_WHEELUP:   return MouseEvent::eWheelUp;
      case SDL_BUTTON_WHEELDOWN: return MouseEvent::eWheelDown;
   }
   */

   return MouseEvent::eInvalid;
}

struct SDLGameWindow::SDL_Info
{
   SDL_Window* mpWindow;
   SDL_GLContext mGLContext;

   SDL_Info() :
      mpWindow(nullptr),
      mGLContext(nullptr)
   {
   }
};

SDLGameWindow::SDLGameWindow():
   GameWindow(),
   mpInfo(nullptr),
   mBackgroundColor(),
   mBitDepth(32),
   mWidth(0),
   mHeight(0),
   mFlags(0)
{
}

SDLGameWindow::~SDLGameWindow()
{
}

bool SDLGameWindow::doCreate(const String& title, int width, int height, int bitdepth, bool fullscreen)
{
   if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
   {
      //log << "Couldn't initialize the SDL library!";
      return false;
   }

   mWidth = width;
   mHeight = height;
   mpInfo = new SDL_Info();

   //mFlags = getWindowFlags(fullscreen);
   mpInfo->mpWindow = SDL_CreateWindow(title.toUtf8().c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
   if ( mpInfo->mpWindow == nullptr )
   {
      Log::getInstance().error("Error creating window: %s", SDL_GetError());
   }

   atexit(SDL_Quit);

   mpInfo->mGLContext = SDL_GL_CreateContext(mpInfo->mpWindow);

   /*
   // make sure that the bitdepth is valid
   bitdepth = SDL_VideoModeOK(width, height, bitdepth, mFlags);
   if (bitdepth == 0)
   {
      Log::getInstance() << "Bitdepth is not supported with resolution " << width << "x" << height << "\n";
      return false;
   }
   else
   {
      Log::getInstance() << "Screen resolution:\t" << width << "x" << height << "x" << bitdepth << "\n";
   }

   resize(width, height);

   SDL_WM_SetCaption(title.toUtf8().c_str(), title.toUtf8().c_str());
   SDL_EnableUNICODE(1);
   */

   return true;
}

void SDLGameWindow::doDestroy()
{
   if ( mpInfo )
   {
      // release main window
      if ( mpInfo->mGLContext )
      {
         SDL_GL_DeleteContext(mpInfo->mGLContext);
      }

      if ( mpInfo->mpWindow )
      {
         SDL_DestroyWindow(mpInfo->mpWindow);
      }

      delete mpInfo;
   }
}

void SDLGameWindow::update()
{
   handleEvents();
}

void SDLGameWindow::display()
{
   SDL_GL_SwapWindow(mpInfo->mpWindow);
}

//-----------------------------------
// - Get/set
//-----------------------------------

const Color& SDLGameWindow::getBackgroundColor() const
{
   return mBackgroundColor;
}
   
void SDLGameWindow::setBackgroundColor(const Color& color)
{
   mBackgroundColor;
}

//-----------------------------------
// - Query
//-----------------------------------

int SDLGameWindow::getHandle() const
{
   return -1;
}

int SDLGameWindow::getWidth() const
{
   return mWidth;
}

int SDLGameWindow::getHeight() const
{
   return mHeight;
}

//-----------------------------------
// - Operations
//-----------------------------------

void SDLGameWindow::resize(int width, int height)
{
   // try to set the new video mode
   mWidth = width;
   mHeight = height;

   fireWindowResized();
}

void SDLGameWindow::toggleFullscreen()
{
  // not yet supported
}

void SDLGameWindow::takeScreenshot()
{
   SDL_Surface* psurface = SDL_GetWindowSurface(mpInfo->mpWindow);
   if ( psurface )
   {
      SDL_SaveBMP(psurface, "screen1");
   }
}

//-----------------------------------
// - Event Handling
//-----------------------------------

void SDLGameWindow::handleEvents()
{
   SDL_Event event;
   SDL_PumpEvents();
   while (SDL_PollEvent (&event))
   {
      switch (event.type)
      {
         case SDL_WINDOWEVENT_RESIZED:
            resize(event.window.data1, event.window.data2);
            break;
         case SDL_KEYDOWN:
         case SDL_KEYUP:
            onKeyboardEvent(event.key);
            break;
         case SDL_MOUSEMOTION:
            onMouseMotionEvent(event.motion);
            break;
         case SDL_MOUSEBUTTONDOWN:
         case SDL_MOUSEBUTTONUP:
            onMouseButtonEvent (event.button);
            break;
         case SDL_QUIT:
            onQuit();
            break;
         default:
            break;
      }
   }
}

void SDLGameWindow::onKeyboardEvent(SDL_KeyboardEvent& event)
{
   int key       = event.keysym.sym;
   int modifiers = getModifiers();

   KeyEvent::EventType type = (event.type == SDL_KEYDOWN) ? KeyEvent::ePressed : KeyEvent::eReleased;
   KeyEvent keyevent(key, type, modifiers);

   dispatch(keyevent);
}

void SDLGameWindow::onMouseMotionEvent(SDL_MouseMotionEvent& event)
{
   /*
   int buttons = 0;
   if ( event.state & SDL_BUTTON(SDL_BUTTON_LEFT) )
      SET_FLAG(buttons, MouseEvent::eLeft);
   if ( event.state & SDL_BUTTON(SDL_BUTTON_RIGHT) )
      SET_FLAG(buttons, MouseEvent::eRight);
   if ( event.state & SDL_BUTTON(SDL_BUTTON_MIDDLE) )
      SET_FLAG(buttons, MouseEvent::eMiddle);

   int keymodifiers = getModifiers();
   Point location(event.x, event.y);
   Point relative(event.xrel, event.yrel);

   MouseEvent mouseevent(buttons, MouseEvent::eMotion, keymodifiers, location, relative);

   if ( mpMouseDispatcher != nullptr )
      mpMouseDispatcher->dispatch(mouseevent);
      */
}

void SDLGameWindow::onMouseButtonEvent(SDL_MouseButtonEvent& event)
{
   c2d::Point location(event.x, event.y);
   MouseEvent::Button button = toMouseEventButton(event.button);
   int keymodifiers = getModifiers();

   MouseEvent::EventType eventtype;
   switch ( event.type )
   {
      case SDL_MOUSEBUTTONDOWN:
         eventtype = MouseEvent::ePressed;
         break;
      case SDL_MOUSEBUTTONUP:
         eventtype = MouseEvent::eReleased;
         break;
   }

   MouseEvent mouseevent(button, eventtype, keymodifiers, location);

   dispatch(mouseevent);
}

void SDLGameWindow::onQuit()
{
  /*for ( Listeners::iterator it = mListeners.begin(); it != mListeners.end(); ++it )
  {
    if ( !(*it)->onWindowClosing() )
    {
      return;
    }
  }*/

  fireWindowClosed();
}
