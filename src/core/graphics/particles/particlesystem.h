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
#include "core/math/vector.h"
#include "core/defines.h"

#include "particlemodules.h"

namespace Graphics
{
   class CodePath;
   class Device;
   class Effect;
   class Particle;
   class ParticleModule;
   class VertexBuffer;   
   class RenderContext;

   /**
   @author Jeroen Broekhuizen
   */
   class ParticleSystem
   {	
   public:
      ParticleSystem();
	   ~ParticleSystem();
	
    // operations
      bool create(Device& device);
      void destroy();

    // get/set
      int  getEmitRate() const;
	   void setEmitRate(int rate);
	   void setEmitCount(int count);

      Particle* getAliveParticles();

      void registerModule(ParticleModule* pmodule);

    // painting
      void update(float delta);
	   void draw(RenderContext& context) const;

   private:

    // data
      Vector            mPosition;
      Particle*         mActiveList;
	   Particle*         mFreeList;
      Effect*           mpEffect;
	   uint32_t          mGeometryBufferSize;
      VertexBuffer*     mGeometryBuffer;
      ParticleModules   mSpawnModules;
      	
	   int emitRate;
      int emitCount;
	   int active;
      int maxActive;
	   int lastUpdate;
      int lastInit;
   };
}
#ifdef JENGINE_INLINE
#  include "particlesystem.inl"
#endif

#endif
