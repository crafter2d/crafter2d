
#ifndef PARTICLE_MODULE_INIT_VEL_H
#define PARTICLE_MODULE_INIT_VEL_H

#include "core/math/vector.h"

#include "particlemodulespawn.h"

namespace Graphics
{
   class ParticleModuleInitVel : public ParticleModuleSpawn
   {
   public:
      ParticleModuleInitVel();

      virtual void exec(Particle& particle) override;

   private:

    // data
      Vector mInitVel;
   };
}

#endif // PARTICLE_MODULE_INIT_VEL_H
