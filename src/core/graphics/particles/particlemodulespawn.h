
#ifndef PARTICLE_MODULE_SPAWN_H
#define PARTICLE_MODULE_SPAWN_H

#include "particlemodule.h"

namespace Graphics
{
   class Particle;

   class ParticleModuleSpawn : public ParticleModule
   {
   public:
      ParticleModuleSpawn();

    // interface
      virtual void exec(Particle& particle) = 0;

   private:
   };
}

#endif // PARTICLE_MODULE_SPAWN_H
