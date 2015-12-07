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

#include <SDL/SDL.h>

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
   int mode = SDL_GetModState();
   int modifiers = InputEvent::eNone;

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
   switch ( sdlbutton )
   {
      case SDL_BUTTON_LEFT:      return MouseEvent::eLeft;
      case SDL_BUTTON_RIGHT:     return MouseEvent::eRight;
      case SDL_BUTTON_MIDDLE:    return MouseEvent::eMiddle;
      case SDL_BUTTON_WHEELUP:   return MouseEvent::eWheelUp;
      case SDL_BUTTON_WHEELDOWN: return MouseEvent::eWheelDown;
   }

   return MouseEvent::eInvalid;
}

SDLGameWindow::SDLGameWindow():
   GameWindow(),
   mpWindow(NULL),
   mBackgroundColor(),
   mBitDepth(32),
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

   mFlags = getWindowFlags(fullscreen);

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

   return true;
}

void SDLGameWindow::doDestroy()
{
   if ( mpWindow != NULL )
   {
      // release main window
      SDL_FreeSurface (mpWindow);
      mpWindow = NULL;
   }

   if ( SDL_WasInit(SDL_INIT_VIDEO) )
   {
	   SDL_Quit ();
   }
}

void SDLGameWindow::update()
{
   handleEvents();
}

void SDLGameWindow::display()
{
   SDL_GL_SwapBuffers();
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

int SDLGameWindow::getWindowFlags(bool fullscreen)
{
   Log& log = Log::getInstance();

   const SDL_VideoInfo *videoInfo;
   videoInfo = SDL_GetVideoInfo();

   if (videoInfo == NULL)
   {
      log << "Can't get video information about graphics card.";
      return 0;
   }

   // set up initial flags
   int flags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER;
   if ( fullscreen )
      flags |= SDL_FULLSCREEN;

   // try to use hardware surface (in videomemory)
   if (videoInfo->hw_available)
   {
      flags |= SDL_HWSURFACE;
      log << "Hardware rendering is supported\n";
   }
   else
   {
      log << "Switching to software rendering mode\n";
      flags |= SDL_SWSURFACE;
   }

   // see if hardware blitting is supported on the platform
   if (videoInfo->blit_hw)
      flags |= SDL_HWACCEL;

  return flags;
}

Driver* SDLGameWindow::loadDriver()
{
   return doLoadDriver(UTEXT("OGLd.dll"));
}

int SDLGameWindow::getHandle() const
{
   return -1;
}

int SDLGameWindow::getWidth() const
{
   return mpWindow->w;
}

int SDLGameWindow::getHeight() const
{
   return mpWindow->h;
}

//-----------------------------------
// - Operations
//-----------------------------------

void SDLGameWindow::resize(int width, int height)
{
  // try to set the new video mode
   mpWindow = SDL_SetVideoMode(width, height, mBitDepth, mFlags);
   if ( mpWindow == NULL )
   {
      Log::getInstance() << "Could not resize window to " << width << "x" << height << ": " << SDL_GetError();
   }

   fireWindowResized();
}

void SDLGameWindow::toggleFullscreen()
{
  // not yet supported
}

void SDLGameWindow::takeScreenshot()
{
  SDL_SaveBMP(mpWindow, "screen1");
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
         case SDL_VIDEORESIZE:
            resize(event.resize.w, event.resize.h);
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
   int key       = event.keysym.unicode == 0 ? event.keysym.sym : event.keysym.unicode & 0x7F;
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

   if ( mpMouseDispatcher != NULL )
      mpMouseDispatcher->dispatch(mouseevent);
      */
}

void SDLGameWindow::onMouseButtonEvent(SDL_MouseButtonEvent& event)
{
   Point location(event.x, event.y);
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
