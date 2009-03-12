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

INLINE bool SoundManager::isInitialized() const
{
   return _initialized;
}

INLINE bool SoundManager::isEnabled() const
{
   return isInitialized() && _enabled;
}

INLINE void SoundManager::enable()
{
   _enabled = true;
}

INLINE void SoundManager::disable()
{
   _enabled = false;
}

/// \fn SoundManager::operator=(const SoundManager& mgr)
/// \brief Copy constructor must not be used, use the getInstance function to get a reference
/// to the sound manager.
INLINE void SoundManager::operator= (const SoundManager& mgr)
{
}
