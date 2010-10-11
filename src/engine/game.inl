/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
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
#include "defines.h"

INLINE const GameConfiguration& Game::getConfiguration() const
{
   return *mpConfiguration;
}

INLINE GameWindow& Game::getGameWindow()
{
  return mWindow;
}

/// \fn Game::setActive(bool act)
/// \brief Set the activity flag of the application.
INLINE void Game::setActive(bool act)
{
   mActive = act;
}

/// \fn Game::isActive() const
/// \brief Returns the active flag.
INLINE bool Game::isActive() const
{
   return mActive;
}

INLINE TimerData& Game::getTimerData()
{
   ASSERT_PTR(mpTimerData);
   return *mpTimerData;
}

INLINE int Game::getScreenWidth() const
{
   return mWindow.getWidth();
}

INLINE int Game::getScreenHeight() const
{
   return mWindow.getHeight();
}

/// \fn Game::setTitle(const char* _title)
/// \brief Set the new title of the game application.
INLINE void Game::setTitle(const std::string& title)
{
   mTitle = title;
}

/// \fn const char* Game::getTitle() const
/// \brief Returns a const pointer to the current application title.
INLINE const std::string& Game::getTitle() const
{
   return mTitle;
}
