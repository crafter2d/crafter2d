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
#include "core/content/contentmanager.h"
#include "core/log/log.h"
#include "core/streams/datastream.h"
#include "core/system/timer.h"
#include "core/graphics/device.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/texture.h"
#include "core/math/color.h"
#include "core/math/vector3.h"

#include "particle.h"
#include "particlemoduleinitsize.h"
#include "particlemoduleinitvel.h"
#include "particlemodulelifetime.h"
#include "particlemodulesize.h"
#include "particlemodulevelocity.h"

namespace Graphics
{

   class ParticleSystem::InitPolicy
   {
   public:

      inline void operator()(Particle& particle)
      {
         mSize.exec(particle);
         mVel.exec(particle);
         mLifetime.exec(particle);
      }

      ParticleModuleInitSize mSize;
      ParticleModuleInitVel  mVel;
      ParticleModuleLifetime mLifetime;
   };

   class ParticleSystem::UpdatePolicy
   {
   public:

      void init()
      {
         mVel.setRange(Vector(0, -15), Vector(0, -20));
      }

      inline void operator()(Particle& particle, float delta)
      {
         mSize.exec(particle, delta);
         mVel.exec(particle, delta);
      }

      ParticleModuleSize mSize;
      ParticleModuleVelocity mVel;
   };

// - ParticleSystem

/// \fn ParticleSystem::ParticleSystem()
/// \brief Initializes member variables.
ParticleSystem::ParticleSystem():
   mPosition(),
	mActiveList(NULL),
   mFreeList(NULL),
   mpTexture(NULL),
   dirty(false),
   emittime(0.0f),
   updatetime(0.0f),
   emitRate(0),
   emitCount(0),
   active(0),
   maxActive(2000)
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
   mpTexture = device.getContentManager().loadContent<Graphics::Texture>(UTEXT("images/particle"));
   if ( mpTexture == NULL )
   {
      return false;
   }

   // add some default modules

   mpInitPolicy = new InitPolicy();
   mpUpdatePolicy = new UpdatePolicy();
   mpUpdatePolicy->init();

   emittime = 0.0f;
   updatetime = 0.0f;
   emitRate = 1.0f;
   emitCount = 100;

	srand(TIMER.getTick() * 1000);
	return true;
}

/// \fn ParticleSystem::destroy()
/// \brief Release all dynamic objects inside this particle system.
void ParticleSystem::destroy()
{
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
   static const float step = 1.0f / 60.0f;

	updatetime += delta;
   if ( updatetime >= step )
   {
      Particle* prev = NULL;
		Particle* part = mActiveList;
		while ( part != NULL)
      {
			part->activeTime += delta;

			if ( part->activeTime >= part->lifeTime )
         {
            (*mpUpdatePolicy)(*part, delta);

            part->pos += part->vel * delta;
            prev = part;
				part = part->next;
			}
			else
         {
            // particle's time is up
				// add it to the free list
				Particle* pcur = part;
            part = part->next;
            if ( pcur == mActiveList )
               mActiveList = part;
            else
               prev->next = part;
            
            pcur->next = mFreeList;
				mFreeList = pcur;

				active--;
			}
		}

      dirty = true;
      updatetime = 0.0f;
	}

   emittime += delta;
	if ( emittime > emitRate && active < maxActive )
   {
      int amount = MIN(emitCount, maxActive - active);
		for ( int i = 0; i < amount; ++i )
      {
			// fetch a particle from the free list
			Particle *part = mFreeList;
			if ( part != NULL )
				mFreeList = mFreeList->next;
			else
            part = new Particle();
			
			part->next = mActiveList;
			mActiveList = part;

			// initialize the particle
			part->pos = mPosition;
         part->vel.set(0, 0);
			part->color.set(0, 0, 1);
			part->activeTime = 0;
			part->state = 0;
			part->size = 0;

         (*mpInitPolicy)(*part);

			active++;
		}

      dirty = true;
      emittime = 0.0f;
	}
}

/// \fn ParticleSystem::draw ()
/// \brief Draw the particles to the screen buffer via a vertex buffer and
/// vertex shader for rendering speed.
void ParticleSystem::draw(RenderContext& context) const
{
   context.drawParticles(*this);
}

} // namespace Graphics
