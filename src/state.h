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
#ifndef _STATE_H_
#define _STATE_H_

#include <SDL/SDL.h>
#include "math/vector.h"

class Object;

/// @author Jeroen Broekhuizen
/// \brief State in which object remains
///
/// This class implements the state in which the object currently remains.
/// Possible usage is a movement state, in which an object will be moved to
/// a certain location.
class State
{
public:
                  State();
   explicit       State( int duration );
   virtual        ~State();

   virtual void   initialize();
   virtual bool   update(float timestep) = 0;

   void           object(Object* obj);
   Object*        object() const;

   void           start(Uint32 start);
   Uint32         start() const;

   void           duration(Uint32 t);
   Uint32         duration() const;

protected:
   Object* _object;
   Uint32 _start;
   Uint32 _duration;
};

class LocationState: public State
{
public:
                  LocationState();
   explicit       LocationState( const Vector& location );

   virtual bool   update(float timestep);

   void           location(const Vector& loc);
   Vector         location() const;

protected:
   Vector _location;
};

class MoveState: public State
{
public:
                  MoveState();
                  MoveState( int duration, const Vector& dest );

   virtual void   initialize();
   virtual bool   update(float timestep);

   void           destination(const Vector& d);
   Vector         destination() const;

protected:
   Vector _destination;
};

class RotateState: public State
{
public:
                  RotateState();
                  RotateState(int duration, float rotation);

   virtual void   initialize();
   virtual bool   update(float timestep);

   void           rotation(float r);
   float          rotation() const;

protected:
   float _rotation;
   float finalRotation;
   float lastupdate;
};

#ifdef JENGINE_INLINE
#  include "state.inl"
#endif

#endif
