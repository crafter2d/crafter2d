
#ifndef PARTICLE_MODULE_INIT_VEL_H
#define PARTICLE_MODULE_INIT_VEL_H

#include "core/math/vector.h"

#include "particle.h"

namespace Graphics
{
   class ParticleModuleInitVel
   {
   public:
      ParticleModuleInitVel();

      inline void exec(Particle& particle) {
         particle.vel = mInitVel;
      }

   private:

    // data
      Vector mInitVel;
   };
}

#endif // PARTICLE_MODULE_INIT_VEL_H
