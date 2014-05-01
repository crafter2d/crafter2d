
#ifndef PARTICLE_MODULES_H
#define PARTICLE_MODULES_H

#include "core/containers/list.h"

namespace Graphics
{
   class ParticleModule;

   class ParticleModules
   {
   public:
      ParticleModules();

    // iterators
      ListIterator<ParticleModule*> getFront();

    // operations
      void add(ParticleModule* pmodule);

   private:
      typedef List<ParticleModule*> Modules;

    // data
      Modules mModules;
   };
}

#endif // PARTICLE_MODULES_H
