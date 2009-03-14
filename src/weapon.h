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
#ifndef WEAPON_H_
#define WEAPON_H_

#include <SDL/SDL.h>
#include "net/netobject.h"

class Creature;
class Object;

class Weapon : public NetObject
{
public:
   explicit       Weapon(Object* owner);

   void           draw();
   
   void           fireAt(Object* target, Uint32 duration);

   void           setOwner(Object* o);
   const Object*  getOwner() const;

protected:
   virtual void   onDraw(Uint32 tick);

protected:
   Object* owner;
   Object* target;
   Uint32 endTime;
};

#ifdef JENGINE_INLINE
#  include "weapon.inl"
#endif

#endif
