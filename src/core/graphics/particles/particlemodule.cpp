
#include "particlemodule.h"

namespace Graphics
{
   ParticleModule::ParticleModule(ModuleKind kind):
      mKind(kind)
   {
   }

   // - Get/set

   ModuleKind ParticleModule::getKind() const
   {
      return mKind;
   }
}
