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
#include "state.h"
#ifndef JENGINE_INLINE
#  include "state.inl"
#endif

#include "system/timer.h" 

#include "object.h"

/*********************************************************
 * State class
 */

State::State():
   _object(0),
   _start(0),
   _duration(0)
{
}

State::State(int duration):
   _object(0),
   _duration(duration),
   _start(0)
{
}

State::~State()
{
   _object = 0;
}

LocationState::LocationState():
   State()
{
}

/*********************************************************
 * LocationState class
 */

LocationState::LocationState(const Vector& location):
   State(),
   _location(location)
{
}

bool LocationState::update(float timestep)
{
   object()->setPosition(location());
   return true;
}

/*********************************************************
 * MoveState class
 */

MoveState::MoveState():
   State(0)
{
}

MoveState::MoveState(int duration, const Vector& dest):
   State( duration ),
   _destination(dest)
{
}

void MoveState::initialize()
{
   Vector diff( destination() );
   Vector pos( object()->getPosition() );

   diff /= (duration() / (float)object()->getMoveSpeed());
   object()->setVelocity(diff);
   object()->setPosition(pos + diff);

   destination(destination() + pos);
}

bool MoveState::update(float timestep)
{
   if (object()->getPosition() == destination())
   {
      // we made it, so stop moving the object
      object()->setVelocity(Vector(0,0));
      object()->setPosition(destination());
      return true;
   }
   else
   {
      object()->move(timestep);
   }
   return false;
}

/*********************************************************
 * RotateState class
 */

RotateState::RotateState():
   _rotation(0),
   finalRotation(0),
   lastupdate(0)
{
}

RotateState::RotateState(int duration, float rotation):
   State(duration),
   _rotation(rotation),
   finalRotation(0),
   lastupdate(0)
{
}

void RotateState::initialize()
{
   finalRotation = object()->getRotation() + rotation();
   if (finalRotation < 0)
      finalRotation += 360;
   else if (finalRotation >= 360)
      finalRotation -= 360;

   float steps = duration() / (float)object()->getMoveSpeed();
   rotation(rotation() / steps);
   lastupdate = Timer::getInstance().getTick();

   object()->rotate(rotation());
}

bool RotateState::update(float timestep)
{
   float rot = object()->getRotation();
   if (fabs( finalRotation-rot ) < 0.001)
   {
      // we reached the final rotation angle
      object()->setRotation(finalRotation);
      return true;
   }
   else
   {
      // update the rotation angle at normal speed
      if (timestep - lastupdate >= object()->getMoveSpeed())
      {
         object()->rotate(rotation());
         lastupdate = timestep;
      }
   }
   return false;
}
