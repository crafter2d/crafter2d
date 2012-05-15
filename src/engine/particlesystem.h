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

#include "engine/texture.h"

#include "texture.h"
#include "entity.h"

class CodePath;
class Script;
class VertexBuffer;

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
class ParticleSystem : public Entity
{	
public:
   DEFINE_REPLICATABLE(ParticleSystem)

	               ParticleSystem();
	virtual        ~ParticleSystem();
	
   virtual void   destroy();

	void           setEmitRate(int rate);
	void           setEmitCount(int count);

	int            getEmitRate() const;

 // visitor
   virtual void accept(NodeVisitor& visitor);
	
protected:
   //virtual bool   load(TiXmlDocument& doc);

 // update & drawing
   virtual void   doUpdate(float delta);
	virtual void   doDraw() const;

   bool           prepare();

 // streaming
   virtual void   doPack(DataStream& stream) const;
   virtual void   doUnpack(DataStream& stream);

   Vector    position;
   TexturePtr texture;
	Particle* activeList;
	Particle* freeList;
	uint      maxBufferSize;
	
	int emitRate;
   int emitCount;
	int active;
   int maxActive;
	int lastUpdate;
   int lastInit;

	Script* updateScript;
	
	VertexBuffer* buffer;
	CodePath* path;
};

#ifdef JENGINE_INLINE
#  include "particlesystem.inl"
#endif

#endif
