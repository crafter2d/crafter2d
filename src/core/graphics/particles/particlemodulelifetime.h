
#ifndef PARTICLE_MODULE_LIFETIME_H
#define PARTICLE_MODULE_LIFETIME_H

#include "particlemodulespawn.h"

namespace Graphics
{
   class ParticleModuleLifetime : public ParticleModuleSpawn
   {
   public:
      ParticleModuleLifetime();

      virtual void exec(Particle& particle) override;

   private:

    // data
      float mLifeTime;
   };
}

#endif // PARTICLE_MODULE_LIFETIME_H
