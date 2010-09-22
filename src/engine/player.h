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
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <string>
#include "object.h"
#include "playerworldobserver.h"
#include "viewport.h"
#include "net/netconnection.h"

/// @author Jeroen Broekhuizen
/// 
/// A player is represented by this class on the server.
class Player
{
public:
   Player();
   ~Player();

 // get/set interface
   Viewport&   getViewport();

 // initialization
   void  initialize(World& world);

 // notifications
   void  notifyScrollChange(const Vector& scrollposition);

   Object*     controler;
   std::string name;
   int         client;

private:
   Viewport             _viewport;
   PlayerWorldObserver  _worldobserver;
};

#ifdef JENGINE_INLINE
#  include "player.inl"
#endif

#endif
