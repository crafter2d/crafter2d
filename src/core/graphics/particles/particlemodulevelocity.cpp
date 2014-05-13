
#include "particlemodulevelocity.h"

namespace Graphics
{
   ParticleModuleVelocity::ParticleModuleVelocity():
      mStart(),
      mRange()
   {
   }
   
   // - Operations

   void ParticleModuleVelocity::setConstant(const Vector& velocity)
   {
      setRange(velocity, velocity);
   }

   void ParticleModuleVelocity::setRange(const Vector& start, const Vector& end)
   {
      mStart = start;
      mRange = end - start;
   }
}
