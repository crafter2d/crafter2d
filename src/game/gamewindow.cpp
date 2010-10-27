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
#include "gamewindow.h"

#include <SDL/SDL.h>

#include <algorithm>

#include "gui/input/keyeventdispatcher.h"
#include "gui/input/keyevent.h"
#include "gui/input/mouseeventdispatcher.h"
#include "gui/input/mouseevent.h"

#include "console.h"
#include "gamewindowlistener.h"

#define CALL(type,array,method) \
  for ( type::iterator it = array.begin(); it != array.end(); ++it ) (*it)->##method()

GameWindow::GameWindow():
   mListeners(),
   mpKeyDispatcher(NULL),
   mpMouseDispatcher(NULL),
   mpWindow(NULL),
   mBackgroundColor(),
   mBitDepth(32),
   mFlags(0)
{
}

GameWindow::~GameWindow()
{
}

bool GameWindow::create(const std::string& title, int width, int height, int bitdepth, bool fullscreen)
{
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

   SDL_WM_SetCaption(title.c_str(), title.c_str());
   SDL_EnableUNICODE(1);

   return true;
}

void GameWindow::destroy()
{
   if ( mpWindow != NULL )
   {
      // release main window
      SDL_FreeSurface (mpWindow);
      mpWindow = NULL;
   }
}

//-----------------------------------
// - Get/set
//-----------------------------------

void GameWindow::setKeyEventDispatcher(KeyEventDispatcher& dispatcher)
{
  mpKeyDispatcher = &dispatcher;
}

void GameWindow::setMouseEventDispatcher(MouseEventDispatcher& dispatcher)
{
  mpMouseDispatcher = &dispatcher;
}

const Color& GameWindow::getBackgroundColor() const
{
   return mBackgroundColor;
}
   
void GameWindow::setBackgroundColor(const Color& color)
{
   mBackgroundColor;
}

//-----------------------------------
// - Query
//-----------------------------------

int GameWindow::getWindowFlags(bool fullscreen)
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

int GameWindow::getWidth() const
{
   return mpWindow->w;
}

int GameWindow::getHeight() const
{
   return mpWindow->h;
}

//-----------------------------------
// - Operations
//-----------------------------------

void GameWindow::resize(int width, int height)
{
  // try to set the new video mode
   mpWindow = SDL_SetVideoMode(width, height, mBitDepth, mFlags);
   if ( mpWindow == NULL )
   {
      Log::getInstance() << "Could not resize window to " << width << "x" << height << ": " << SDL_GetError();
   }

   //CALL(Listeners, mListeners, onWindowResized);
   for ( Listeners::iterator it = mListeners.begin(); it != mListeners.end(); ++it )
      (*it)->onWindowResized();
}

void GameWindow::toggleFullscreen()
{
  // not yet supported
}

void GameWindow::takeScreenshot()
{
  SDL_SaveBMP(mpWindow, "screen1");
}

//-----------------------------------
// - Listeners
//-----------------------------------

void GameWindow::addListener(GameWindowListener& listener)
{
  mListeners.push_back(&listener);
}

void GameWindow::removeListener(GameWindowListener& listener)
{
   Listeners::iterator it = std::find(mListeners.begin(), mListeners.end(), &listener);
   if ( it != mListeners.end() )
   {
      mListeners.erase(it);
   }
}

//-----------------------------------
// - Event Handling
//-----------------------------------

void GameWindow::handleEvents()
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

void GameWindow::onKeyboardEvent(SDL_KeyboardEvent& event)
{
  ASSERT_PTR(mpKeyDispatcher);

  KeyEvent keyevent = KeyEvent::FromSDL(event);
  mpKeyDispatcher->dispatch(keyevent);
}

void GameWindow::onMouseMotionEvent(SDL_MouseMotionEvent& event)
{
  MouseEvent mouseevent = MouseEvent::FromSDL(event);
  mpMouseDispatcher->dispatch(mouseevent);
}

void GameWindow::onMouseButtonEvent(SDL_MouseButtonEvent& event)
{
  MouseEvent mouseevent = MouseEvent::FromSDL(event);
  mpMouseDispatcher->dispatch(mouseevent);
}

void GameWindow::onQuit()
{
  for ( Listeners::iterator it = mListeners.begin(); it != mListeners.end(); ++it )
  {
    if ( !(*it)->onWindowClosing() )
    {
      return;
    }
  }

  //CALL(Listeners, mListeners, onWindowClosed);
  for ( Listeners::iterator it = mListeners.begin(); it != mListeners.end(); ++it )
     (*it)->onWindowClosed();
}
