
#ifndef PARTICLE_MODULE_LIFETIME_H
#define PARTICLE_MODULE_LIFETIME_H

#include "particle.h"

namespace Graphics
{
   class ParticleModuleLifetime
   {
   public:
      ParticleModuleLifetime();

      inline void exec(Particle& particle) {
         particle.lifeTime = mLifeTime;
      }

   private:

    // data
      float mLifeTime;
   };
}

#endif // PARTICLE_MODULE_LIFETIME_H
