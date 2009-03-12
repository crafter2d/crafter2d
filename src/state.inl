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

INLINE void State::initialize()
{
}

INLINE void State::start(Uint32 start)
{
   _start = start;
}

INLINE Uint32 State::start() const
{
   return _start;
}

INLINE void State::duration(Uint32 dur)
{
   _duration = dur;
}

INLINE Uint32 State::duration() const
{
   return _duration;
}

INLINE void State::object(Object* obj)
{
   _object = obj;
   initialize();
}

INLINE Object* State::object() const
{
   return _object;
}

INLINE void LocationState::location(const Vector& loc)
{
   _location = loc;
}

INLINE Vector LocationState::location() const
{
   return _location;
}

INLINE void MoveState::destination(const Vector& dest)
{
   _destination = dest;
}

INLINE Vector MoveState::destination() const
{
   return _destination;
}

INLINE void RotateState::rotation(float rot)
{
   _rotation = rot;
}

INLINE float RotateState::rotation() const
{
   return _rotation;
}
