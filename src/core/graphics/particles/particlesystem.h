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

#include <vector>

#include "core/entity/components/components.h"
#include "core/math/range.h"
#include "core/math/vector.h"
#include "core/math/vectorrange.h"
#include "core/defines.h"
#include "core/core_base.h"

#include "particles.h"

namespace Graphics
{
   class BlendState;
   class CodePath;
   class Device;
   class Effect;
   class Particle;
   class IndexBuffer;
   class VertexBuffer;
   class RenderContext;
   class Texture;

   /**
   @author Jeroen Broekhuizen
   */
   class CORE_API ParticleSystem
   {	
   public:
      ParticleSystem();
	   ~ParticleSystem();
	
    // operations
      bool create(Device& device);
      void destroy();

    // get/set
	   void  setEmitRate(float rate);
      void  setGravity(float gravity);

      float       getInitSize() const;
      void        setInitSize(float size);
      Range       getInitSizeRange() const;
      void        setInitSizeRange(const Range& range);
      float       getInitLifeTime() const;
      void        setInitLifeTime(float lifetime);
      Range       getInitLifeTimeRange() const;
      void        setInitLifeTimeRange(const Range& range);
      const Vector& getInitVelocity() const;
      void        setInitVelocity(const Vector& vel);
      const VectorRange& getInitVelocityRange() const;
      void        setInitVelocityRange(const VectorRange& range);

      void setPosition(const Vector& pos);

      int getActiveParticleCount() const;
      const Particles& getActiveParticles() const;
      const Texture& getTexture() const;
            
    // painting
      void update(float delta);
	   void draw(RenderContext& context) const;

   private:

    // Operations
      void initParticle(Particle& particle);
      void updateParticle(Particle& particle, float delta, int& index);

    // data
      Vector            mPosition;
      Particles         mParticles;      
      Texture*          mpTexture;

      float       mInitSize;
      Range       mInitSizeRange;
      float       mInitLifeTime;
      Range       mInitLifeTimeRange;
      Vector      mInitVelocity;
      VectorRange mInitVelocityRange;
      	
      float emittime;
      float updatetime;
	   float mEmitRate;
      float mGravity;

      int active;
      int maxActive;
   };
}
#ifdef JENGINE_INLINE
#  include "particlesystem.inl"
#endif

#endif
