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
#include "gamewindow.h"

#include <algorithm>

#include "core/input/mouseeventdispatcher.h"
#include "core/input/keyeventdispatcher.h"
#include "core/system/platform.h"
#include "core/string/string.h"
#include "core/defines.h"

#include "gamewindowlistener.h"

GameWindow::GameWindow():
   mListeners(),
   mpKeyDispatcher(NULL),
   mpMouseDispatcher(NULL)
{
}

GameWindow::~GameWindow()
{
}

// - Creation

bool GameWindow::create(const String& title, int width, int height, int bitdepth, bool fullscreen)
{
   return doCreate(title, width, height, bitdepth, fullscreen);
}

void GameWindow::destroy()
{
   doDestroy();
}

void GameWindow::doDestroy()
{
}

bool GameWindow::initDevice(Graphics::Device& device)
{
   C2D_UNUSED(device);
   return true;
}

// - Query
   
int GameWindow::getWidth() const
{
   return 0;
}

int GameWindow::getHeight() const
{
   return 0;
}

// - Dispatchers

void GameWindow::setKeyEventDispatcher(Input::KeyEventDispatcher& dispatcher)
{
  mpKeyDispatcher = &dispatcher;
}

void GameWindow::setMouseEventDispatcher(Input::MouseEventDispatcher& dispatcher)
{
  mpMouseDispatcher = &dispatcher;
}

void GameWindow::dispatch(const Input::MouseEvent& mouseevent)
{
   if ( mpMouseDispatcher != NULL )
   {
      mpMouseDispatcher->dispatch(mouseevent);
   }
}

void GameWindow::dispatch(const Input::KeyEvent& keyevent)
{
   if ( mpKeyDispatcher != NULL )
   {
      mpKeyDispatcher->dispatch(keyevent);
   }
}

// - Listeners
   
void GameWindow::addListener(GameWindowListener& listener)
{
   if ( std::find(mListeners.begin(), mListeners.end(), &listener) == mListeners.end() )
   {
      mListeners.push_back(&listener);
   }
}

void GameWindow::removeListener(GameWindowListener& listener)
{
   Listeners::iterator it = std::find(mListeners.begin(), mListeners.end(), &listener);
   if ( it != mListeners.end() )
   {
      mListeners.erase(it);
   }
}

// - Listener notifications

void GameWindow::fireWindowClosed()
{
   for ( Listeners::iterator it = mListeners.begin(); it != mListeners.end(); ++it )
   {
      GameWindowListener* plistener = (*it);
      plistener->onWindowClosed();
   }
}

void GameWindow::fireWindowResized()
{
   for ( Listeners::iterator it = mListeners.begin(); it != mListeners.end(); ++it )
   {
      GameWindowListener* plistener = (*it);
      plistener->onWindowResized();
   }
}
