#ifndef PARTICLE_MODULE_INIT_COLOR_H
#define PARTICLE_MODULE_INIT_COLOR_H

#include "core/math/color.h"

namespace Graphics
{
   class Particle;

   class ParticleModuleInitColor
   {
   public:
      explicit ParticleModuleInitColor(const Color& color = Color::white());

    // get/set
      void setColor(const Color& color);

    // interface
      void exec(Particle& particle);

   private:

    // data
      Color mColor;
   };
}

#endif // PARTICLE_MODULE_INIT_COLOR_H
