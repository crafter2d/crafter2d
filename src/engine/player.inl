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
#include "core/defines.h"

INLINE int Player::getClientId() const
{
   return mClientId;
}

INLINE void Player::setClientId(int clientid)
{
   mClientId = clientid;
}

INLINE const std::string& Player::getName() const
{
   return mName;
}

INLINE void Player::setName(const std::string& name)
{
   mName = name;
}

INLINE Viewport& Player::getViewport()
{
   return mViewport;
}

INLINE bool Player::hasController() const
{
   return mpController != NULL;
}

INLINE Actor& Player::getController()
{
   ASSERT_PTR(mpController)
   return *mpController;
}

INLINE void Player::setController(Actor& actor)
{
   mpController = &actor;
}
