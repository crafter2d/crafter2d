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

#include "core/content/contentmanager.h"
#include "core/graphics/device.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/texture.h"
#include "core/math/color.h"
#include "core/math/vector3.h"

#include "particle.h"

namespace Graphics
{

/// \fn ParticleSystem::ParticleSystem()
/// \brief Initializes member variables.
ParticleSystem::ParticleSystem():
   mPosition(),
   mParticles(),
	mpTexture(nullptr),
   mInitAreaRange(),
   mInitSize(),
   mInitSizeRange(),
   mInitLifeTime(),
   mInitLifeTimeRange(),
   mInitVelocity(),
   mInitVelocityRange(),
   emittime(0.0f),
   updatetime(0.0f),
   mEmitCount(10),
   mEmitRate(0.0f),
   mGravity(0.0f),
   active(0),
   maxActive(0)
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
   if ( mpTexture == nullptr )
   {
      return false;
   }

   emittime = 0.0f;
   updatetime = 0.0f;
   active = 0;
   mParticles.initialize(maxActive);

	return true;
}

/// \fn ParticleSystem::destroy()
/// \brief Release all dynamic objects inside this particle system.
void ParticleSystem::destroy()
{
   delete mpTexture;
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
		for ( int index = 0; index < active; )
      {
         Particle& particle = mParticles[index];
         updateParticle(particle, delta, index);			
		}

      updatetime = 0.0f;
	}

   emittime += delta;
   if ( emittime > mEmitRate )
   {
      for ( int i = 0; i < mEmitCount && active < maxActive; ++i )
      {
         // fetch a particle from the free list
         Particle& particle = mParticles[active++];
         initParticle(particle);
      }
      emittime -= mEmitRate;
   }
}

void ParticleSystem::initParticle(Particle& particle)
{
   // initialize the particle
   particle.pos = mPosition + mInitAreaRange.getRandomVector();
   particle.color.set(0, 0, 1);
   particle.activeTime = 0;
   particle.size = mInitSize + mInitSizeRange.getRandom();
   particle.vel = mInitVelocity + mInitVelocityRange.getRandomVector();
   particle.lifeTime = mInitLifeTime + mInitLifeTimeRange.getRandom();
}

void ParticleSystem::updateParticle(Particle& particle, float delta, int& index)
{
   particle.activeTime += delta;

	if ( particle.activeTime < particle.lifeTime )
   {
      Vector forces(0, mGravity);

      particle.vel += (forces * delta);
      particle.pos += (particle.vel * delta);
            
      index++;
	}
	else
   {
      // particle's time is up
		// add it to the free list
      active--;
      mParticles.swap(index, active);      
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
