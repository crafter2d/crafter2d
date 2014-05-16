
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
      inline void exec(Particle& particle) {
         particle.size = mBegin + mRange * (particle.activeTime / particle.lifeTime);
      }

   private:

    // data
      float mBegin;
      float mRange;
   };
}
#endif // PARTICLE_MODULE_SIZE_H
