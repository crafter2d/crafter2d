
#ifndef PARTICLE_MODULE_INIT_SIZE_H
#define PARTICLE_MODULE_INIT_SIZE_H

#include "particle.h"

namespace Graphics
{
   class Particle;

   class ParticleModuleInitSize
   {
   public:
      ParticleModuleInitSize();

      inline void exec(Particle& particle) {
         particle.size = mSize;
      }

   private:

    // data
      float mSize;
   };
}

#endif // PARTICLE_MODULE_INIT_SIZE_H
