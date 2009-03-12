/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "defines.h"

INLINE bool Game::designing() const
{
   return mDesigning;
}

/// \fn Game::designing(bool designing)
/// \brief Sets the desiging flag.
/// \param designing if true then the designer is started, closed otherwise
INLINE void Game::designing(bool designing)
{
   mDesigning = designing;
}

INLINE const GameConfiguration& Game::getConfiguration() const
{
   return *mpConfiguration;
}

/// \fn Game::setActive(bool act)
/// \brief Set the activity flag of the application.
INLINE void Game::setActive(bool act)
{
   active = act;
}

/// \fn Game::isActive() const
/// \brief Returns the active flag.
INLINE bool Game::isActive() const
{
   return active;
}

/// \fn Game::getServer() 
/// \brief Returns a reference to the current server.
INLINE Server& Game::getServer()
{
   return server;
}

/// \fn Game::getClient()
/// \brief Returns a reference to the local client.
INLINE Client& Game::getClient()
{
   return client;
}

/// \fn Game::getCanvas()
/// \brief Returns a reference to the canvas.
INLINE GuiCanvas& Game::getCanvas()
{
   return canvas;
}

INLINE int Game::getScreenWidth() const
{
   return width;
}

INLINE int Game::getScreenHeight() const
{
   return height;
}

INLINE GuiDesigner& Game::designer()
{
   ASSERT_PTR(mpDesigner)
   return *mpDesigner;
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
