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
#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "core/math/vector.h"
#include "core/math/color.h"
#include "core/graphics/effect.h"
#include "core/graphics/texture.h"

namespace Graphics
{
   class CodePath;
   class Device;
   class VertexBuffer;
};

namespace c2d
{
   class ScriptObject;
}

/**
@author Jeroen Broekhuizen
*/
class Particle 
{
public:
	Particle();

	void setColor (float r, float g, float b) {
		color.set (r,g,b);
	}

	Vector pos, vel;
	Color color;
	float initTime, life, state;
	float size;
	Particle *next;
};

/**
@author Jeroen Broekhuizen
*/
class ParticleSystem
{	
public:
   ParticleSystem();
	~ParticleSystem();
	
 // operations
   bool create(Graphics::Device& device);
   void destroy();

 // get/set
   int  getEmitRate() const;
	void setEmitRate(int rate);
	void setEmitCount(int count);

 // painting
   void update(float delta);
	void draw(Graphics::RenderContext& context) const;

private:
   //virtual bool   load(TiXmlDocument& doc);

 // data
   Vector         position;
   Particle*      activeList;
	Particle*      freeList;
	uint32_t       mGeometryBufferSize;

   c2d::ScriptObject*  updateScript;

   Graphics::Effect*        mpEffect;
   Graphics::VertexBuffer*  mGeometryBuffer;
	
	int emitRate;
   int emitCount;
	int active;
   int maxActive;
	int lastUpdate;
   int lastInit;
};

#ifdef JENGINE_INLINE
#  include "particlesystem.inl"
#endif

#endif
