
#include "soundmodule.h"

#include "core/sound/soundmanager.h"

#include "core/defines.h"

namespace c2d
{
   SoundModule::SoundModule(SoundManager* pmanager):
      Module(eSoundModule, UUID_SoundModule),
      mpSoundManager(pmanager)
   {
      ASSERT_PTR(mpSoundManager);
   }

   SoundModule::~SoundModule()
   {
      delete mpSoundManager;
      mpSoundManager = NULL;
   }

   // - Get/set

   SoundManager& SoundModule::getSoundManager()
   {
      return *mpSoundManager;
   }
}
