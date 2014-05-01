
#include "particle.h"

#include "core/defines.h"

namespace Graphics
{
   Particle::Particle():
      pos(),
      vel(),
      color(),
      activeTime(0),
      lifeTime(0),
      state(0),
      size(0),
      next(NULL)
   {
   }
}
