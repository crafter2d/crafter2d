
#ifndef SOUND_MODULES_H
#define SOUND_MODULES_H

#include "module.h"

namespace c2d
{
   class SoundManager;

   class CORE_API SoundModule : public Module
   {
   public:
      explicit SoundModule(SoundManager* psoundmanager);
      virtual ~SoundModule();

    // get/set
      SoundManager& getSoundManager();

   private:

    // data
      SoundManager* mpSoundManager;
   };
}

#endif // SOUND_MODULES_H
