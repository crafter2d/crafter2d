
#include "particlemodulelifetime.h"

#include "particle.h"
#include "particlesystem.h"

namespace Graphics
{
   ParticleModuleLifetime::ParticleModuleLifetime():
      ParticleModuleSpawn(),
      mLifeTime(5.0f)
   {
   }

   void ParticleModuleLifetime::exec(Particle& particle)
   {
      particle.lifeTime = mLifeTime;
   }
}