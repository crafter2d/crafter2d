
#include "particlemodules.h"

#include "particlemodule.h"

namespace Graphics
{
   ParticleModules::ParticleModules():
      mModules()
   {
   }

   // - Iterators

   ListIterator<ParticleModule*> ParticleModules::getFront()
   {
      return mModules.getFront();
   }

   // - Operations

   void ParticleModules::add(ParticleModule* pmodule)
   {
      mModules.addFront(pmodule);
   }
}
