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
#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <SDL/SDL.h>
#include "texture.h"
#include "math/vector.h"
#include "object.h"

class CodePath;
class Script;
class VertexBuffer;

/**
@author Jeroen Broekhuizen
*/
class Particle 
{
public:
	Particle(): next(NULL) {}

	void setColor (float r, float g, float b) {
		color.set (r,g,b);
	}

	Vector pos, vel;
	Color color;
	Uint32 initTime, life, state;
	float size;
	Particle *next;
};

/**
@author Jeroen Broekhuizen
*/
class ParticleSystem: public Object
{	
public:
   DEFINE_REPLICATABLE(ParticleSystem)

	               ParticleSystem();
	virtual        ~ParticleSystem();
	
   virtual void   destroy();

	virtual void   update(Uint32 tick);
	virtual void   draw();

	void           setEmitRate(int rate);
	void           setEmitCount(int count);

	int            getEmitRate() const;

   virtual void   pack(BitStream& stream) const;
   virtual void   unpack(BitStream& stream);
	
protected:
   virtual bool   load(TiXmlDocument& doc);

   bool           prepare();

	Particle* activeList;
	Particle* freeList;
	Uint32 maxBufferSize;
	
	int emitRate, emitCount;
	int active, maxActive;
	int lastUpdate, lastInit;

	Script* updateScript;
	
	VertexBuffer* buffer;
	//ShaderObject object;
	//VertexProgram vp;
	CodePath* path;
};

#ifdef JENGINE_INLINE
#  include "particlesystem.inl"
#endif

#endif
