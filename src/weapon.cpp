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

#include "weapon.h"
#ifndef JENGINE_INLINE
#  include "weapon.inl"
#endif

Weapon::Weapon(Object* owner):
   owner(owner),
   target(0),
   endTime(0)
{
}

void Weapon::fireAt(Object* t, Uint32 duration)
{
   target = t;
   endTime = SDL_GetTicks() + duration;
}

void Weapon::draw()
{
   Uint32 tick = SDL_GetTicks();
   if (tick < endTime) {
      onDraw(tick);
   }
   else
      target = 0;
}
