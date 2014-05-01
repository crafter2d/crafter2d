
#include "particlemoduleinitvel.h"

#include "particle.h"

namespace Graphics
{
   ParticleModuleInitVel::ParticleModuleInitVel():
      ParticleModuleSpawn(),
      mInitVel(0, 5)
   {
   }

   void ParticleModuleInitVel::exec(Particle& particle)
   {
      particle.vel = mInitVel;
   }
}
