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
#include "player.h"
#ifndef JENGINE_INLINE
#  include "player.inl"
#endif

#include "net/netconnection.h"
#include "net/netevent.h"
#include "net/bitstream.h"

#include "world/world.h"

#include "client.h"
#include "game.h"

Player::Player():
   controler(NULL),
   name(),
   client(0),
   _viewport(),
   _worldobserver(*this)
{
}

Player::~Player()
{
   if (controler)
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
   BitStream stream;
   NetEvent event(scrollEvent);

   // inform the server about the viewport change
   stream << (NetObject*)&event << (int)scrollposition.x << (int)scrollposition.y;
   Game::getInstance().getClient().getConnection()->send(&stream);
}
