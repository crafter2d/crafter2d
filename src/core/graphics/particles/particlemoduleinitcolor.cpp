
#include "particlemoduleinitcolor.h"

#include "particle.h"

namespace Graphics
{
   ParticleModuleInitColor::ParticleModuleInitColor(const Color& color):
      mColor(color)
   {
   }

   // - Get/set

   void ParticleModuleInitColor::setColor(const Color& color)
   {
      mColor = color;
   }

   // - Interface

   void ParticleModuleInitColor::exec(Particle& particle)
   {
      particle.color = mColor;
   }
}
