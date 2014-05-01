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
#include "particlesystem.h"
#ifndef JENGINE_INLINE
#  include "particlesystem.inl"
#endif

#include "core/containers/listalgorithms.h"
#include "core/log/log.h"
#include "core/streams/datastream.h"
#include "core/system/timer.h"
#include "core/graphics/vertexbuffer.h"
#include "core/graphics/device.h"
#include "core/graphics/codepath.h"
#include "core/graphics/rendercontext.h"
#include "core/resource/resourcemanager.h"
#include "core/script/scriptobject.h"
#include "core/world/nodevisitor.h"

#include "particle.h"
#include "particlemodule.h"
#include "particlemodulespawn.h"

struct ParticleVertex {
	Vector pos;
	Color diffuse;
	Vector tex;
	Vector offset;
};

namespace Graphics
{

// - ParticleSystem

/// \fn ParticleSystem::ParticleSystem()
/// \brief Initializes member variables.
ParticleSystem::ParticleSystem():
   mPosition(),
	mActiveList(0),
   mFreeList(0),
   mpEffect(NULL),
   mGeometryBufferSize(0),
   mGeometryBuffer(NULL),
   mSpawnModules(),
   emitRate(0),
   emitCount(0),
   active(0),
   maxActive(70),
	lastUpdate(0),
   lastInit(0)
{
}

ParticleSystem::~ParticleSystem()
{
   destroy();
}

// - Operations

/// \fn ParticleSystem::create()
/// \brief Creates and initializes the particle system components neccessary for rendering.
bool ParticleSystem::create(Device& device)
{
   // load effect
   mpEffect = device.createEffect(UTEXT("shaders/basic"));
   if ( mpEffect == NULL )
   {
      return false;
   }

   mGeometryBufferSize = 256;
   int size  = mGeometryBufferSize * 4;
   int usage = VertexBuffer::eStream | VertexBuffer::eWriteOnly;

	// generate the vertex buffer
	mGeometryBuffer = mpEffect->createVertexBuffer(device, size, usage);
   if ( mGeometryBuffer == NULL )
	{
      return false;
   }

	srand(TIMER.getTick() * 1000);
	return true;
}

/// \fn ParticleSystem::destroy()
/// \brief Release all dynamic objects inside this particle system.
void ParticleSystem::destroy()
{
   if ( mpEffect != NULL )
   {
      delete mpEffect;
      mpEffect = NULL;
   }

   if ( mGeometryBuffer != NULL )
   {
      delete mGeometryBuffer;
      mGeometryBuffer = NULL;
   }

   // release the particle lists of this particle system
   while ( mActiveList != NULL )
   {
      Particle* temp = mActiveList;
      mActiveList = mActiveList->next;
      delete temp;
   }
   while ( mFreeList != NULL )
   {
      Particle* temp = mFreeList;
      mFreeList = mFreeList->next;
      delete temp;
   }
}

/// \fn ParticleSystem::doUpdate (float delta)
/// \brief Fires the particle update script for every particle. After that the system
/// checks if there are still enough particles alive. If not, new particles are initialized
/// and put in the active list.
void ParticleSystem::update(float delta)
{
	if ( delta > 100 )
   {
		Particle** part = &mActiveList;
		while ( *part != NULL)
      {
			Particle *curpart = *part;
			float lifetime = curpart->activeTime;

			if (lifetime >= curpart->lifeTime)
         {
				// particle's time is up
				// now add it to the free list
				*part = curpart->next;
				curpart->next = mFreeList;
				mFreeList = curpart;

				active--;
			}
			else
         {
            curpart->activeTime += delta;
				curpart->pos += curpart->vel;

				part = &curpart->next;
			}
		}
	}

	if ( delta > emitRate && active < maxActive)
   {
		for ( int i = 0; i < emitCount; i++ )
      {
			// fetch a particle from the free list
			Particle *part = mFreeList;
			if (!part)
				part = new Particle();
			else
				mFreeList = mFreeList->next;
			part->next = mActiveList;
			mActiveList = part;

			// initialize the particle
			part->pos = mPosition;
			part->pos.x += rand()%6;
			part->pos.y += rand()%4;
			part->vel = Vector (0, -1.0f-rand()%2);
			part->color = Color(1,1,0);
			part->activeTime = 0;
         part->lifeTime = (rand()%2000) / 1000.0f;
			part->state = 0;
			part->size = 20;

         auto fnc = [this,part](ParticleModule* pmodule) { static_cast<ParticleModuleSpawn*>(pmodule)->exec(*part); };
         ListIterator<ParticleModule*> it = mSpawnModules.getFront();
         ListAlgorithms::foreach(it, fnc);

			active++;
		}
	}
}

/// \fn ParticleSystem::draw ()
/// \brief Draw the particles to the screen buffer via a vertex buffer and
/// vertex shader for rendering speed.
void ParticleSystem::draw(RenderContext& context) const
{
	uint32_t num = 0;
	Particle* part = mActiveList;
   ParticleVertex* verts = reinterpret_cast<ParticleVertex*>(mGeometryBuffer->lock(context));

   mpEffect->enable(context);

	while ( part != NULL )
   {
		float halfSize = part->size * 0.5f;

		// create the rectangle
		verts[0].pos = part->pos;
		verts[0].diffuse = part->color;
		verts[0].tex.set (0,0);
		verts[0].offset.set (-halfSize,-halfSize);

		verts[1].pos = part->pos;
		verts[1].diffuse = part->color;
		verts[1].tex.set (0,1);
		verts[1].offset.set (-halfSize,halfSize);

		verts[2].pos = part->pos;
		verts[2].diffuse = part->color;
		verts[2].tex.set (1,1);
		verts[2].offset.set (halfSize,halfSize);

		verts[3].pos = part->pos;
		verts[3].diffuse = part->color;
		verts[3].tex.set (1,0);
		verts[3].offset.set (halfSize,-halfSize);

		verts += 4;

		// check buffer limit
      if ( ++num >= mGeometryBufferSize )
      {
			// buffer full -> render contents and continue
			mGeometryBuffer->unlock(context);

			mGeometryBuffer->enable(context);
         context.drawTriangles(0, num * 4);
			mGeometryBuffer->disable(context);

			verts = reinterpret_cast<ParticleVertex*>(mGeometryBuffer->lock (context));
			num = 0;
		}

		part = part->next;
	}

	// render any remaining particles
	mGeometryBuffer->unlock(context);
	if ( num != 0 )
   {
		mGeometryBuffer->enable(context);
		context.drawTriangles(0, num * 4);
		mGeometryBuffer->disable(context);
	}
}

void ParticleSystem::registerModule(ParticleModule* pmodule)
{
   switch ( pmodule->getKind() )
   {
   case eSpawnModule:
      mSpawnModules.add(pmodule);
      break;
   }
}

} // namespace Graphics
