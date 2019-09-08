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

namespace Graphics
{
   INLINE void ParticleSystem::setGravity(float gravity)
   {
      mGravity = gravity;
   }

   INLINE void ParticleSystem::setEmitCount(int count)
   {
      mEmitCount = count;
   }

   INLINE void ParticleSystem::setEmitRate(float rate)
   {
      mEmitRate = 1.0f / rate;
   }

   INLINE void ParticleSystem::setMaxActive(int max)
   {
      maxActive = max;
   }

   INLINE void ParticleSystem::setPosition(const Vector& pos)
   {
      mPosition = pos;
   }

   INLINE int ParticleSystem::getActiveParticleCount() const
   {
      return active;
   }

   INLINE const Particles& ParticleSystem::getActiveParticles() const
   {
      return mParticles;
   }

   INLINE const Texture& ParticleSystem::getTexture() const
   {
      return *mpTexture;
   }

   INLINE void ParticleSystem::setInitAreaRange(const VectorRange& range)
   {
      mInitAreaRange = range;
   }

   INLINE float ParticleSystem::getInitSize() const
   {
      return mInitSize;
   }

   INLINE void ParticleSystem::setInitSize(float size)
   {
      mInitSize = size;
   }

   INLINE Range ParticleSystem::getInitSizeRange() const
   {
      return mInitSizeRange;
   }

   INLINE void ParticleSystem::setInitSizeRange(const Range& range)
   {
      mInitSizeRange = range;
   }

   INLINE float ParticleSystem::getInitLifeTime() const
   {
      return mInitLifeTime;
   }

   INLINE void ParticleSystem::setInitLifeTime(float lifetime)
   {
      mInitLifeTime = lifetime;
   }

   INLINE Range ParticleSystem::getInitLifeTimeRange() const
   {
      return mInitLifeTimeRange;
   }

   INLINE void ParticleSystem::setInitLifeTimeRange(const Range& range)
   {
      mInitLifeTimeRange = range;
   }

   INLINE const Vector& ParticleSystem::getInitVelocity() const
   {
      return mInitVelocity;
   }

   INLINE void ParticleSystem::setInitVelocity(const Vector& vel)
   {
      mInitVelocity = vel;
   }

   INLINE const VectorRange& ParticleSystem::getInitVelocityRange() const
   {
      return mInitVelocityRange;
   }
      
   INLINE void ParticleSystem::setInitVelocityRange(const VectorRange& range)
   {
      mInitVelocityRange = range;
   }
}