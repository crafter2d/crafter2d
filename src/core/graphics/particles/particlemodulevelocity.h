
#ifndef PARTICLE_MODULE_VELOCITY_H
#define PARTICLE_MODULE_VELOCITY_H

#include "core/math/vector.h"

#include "particle.h"

namespace Graphics
{
   class ParticleModuleVelocity
   {
   public:
      ParticleModuleVelocity();

    // operations
      void setConstant(const Vector& velocity);
      void setRange(const Vector& start, const Vector& end);

    // interface
      void exec(Particle& particle, float delta) {
         if ( mRange != Vector::zero() )
         {
            particle.vel += mRange * delta;
         }
      }

   private:

    // get/set
      Vector mStart;
      Vector mRange;
   };
}

#endif // PARTICLE_MODULE_VELOCITY_H
