
#ifndef PARTICLE_MODULE_SIZE_H
#define PARTICLE_MODULE_SIZE_H

#include "particle.h"

namespace Graphics
{
   class ParticleModuleSize
   {
   public:
      ParticleModuleSize();

      void init() {

      }

    // overrides
      inline void exec(Particle& particle, float delta) {
         //float incrementPerSec = (mEndSize - mBeginSize) / particle.lifeTime;
         //particle.size += delta * incrementPerSec;

         particle.size = mBeginSize + (mEndSize - mBeginSize) * (particle.activeTime / particle.lifeTime);
      }

   private:

    // data
      float mBeginSize;
      float mEndSize;
      float mIncrementPerSec;
   };
}
#endif // PARTICLE_MODULE_SIZE_H
