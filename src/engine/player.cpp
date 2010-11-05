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
#include "player.h"
#ifndef JENGINE_INLINE
#  include "player.inl"
#endif

#include "net/netconnection.h"
#include "net/events/viewportevent.h"

#include "world/world.h"

#include "client.h"

static const std::string SName = "Player";

Player::Player():
   controler(NULL),
   name(SName),
   client(0),
   _viewport(),
   _worldobserver(*this)
{
}

Player::~Player()
{
   if ( controler != NULL )
   {
      controler->destroy();
      delete controler;
      controler = 0;
   }
}

//////////////////////////////////////////////////////////////////////////
// - initialization
//////////////////////////////////////////////////////////////////////////

void Player::initialize(World& world)
{
   world.attach(_worldobserver);
}

//////////////////////////////////////////////////////////////////////////
// - notifications
//////////////////////////////////////////////////////////////////////////

void Player::notifyScrollChange(const Vector& scrollposition)
{
   ViewportEvent event(_viewport);

   // Game::getInstance().getClient().getConnection()->send(&event);
}
