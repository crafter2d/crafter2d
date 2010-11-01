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

INLINE void State::initialize()
{
}

INLINE void State::start(float start)
{
   _start = start;
}

INLINE float State::start() const
{
   return _start;
}

INLINE void State::duration(float dur)
{
   _duration = dur;
}

INLINE float State::duration() const
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
