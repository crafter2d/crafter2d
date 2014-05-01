/***************************************************************************
 *   Copyright (C) 2014 by Jeroen Broekhuizen                              *
 *   crafter2d@outlook.com                                                 *
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
#ifndef PARTICLE_H
#define PARTICLE_H

#include "core/math/color.h"
#include "core/math/vector.h"

namespace Graphics
{
   class Particle
   {
   public:
	   Particle();

	   void setColor (float r, float g, float b) {
		   color.set (r,g,b);
	   }

	   Vector pos, vel;
	   Color color;
	   float activeTime;
      float lifeTime;
      float state;
	   float size;
	   Particle *next;
   };
}

#endif // PARTICLE_H
