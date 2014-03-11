
#ifndef FMOD_SOUND_MANAGER_H
#define FMOD_SOUND_MANAGER_H

#include "core/sound/soundmanager.h"

namespace FMOD
{
   class System;
}

class String;

namespace c2d
{
   class Sound;

   class FModSoundManager : public SoundManager
   {
   public:
      FModSoundManager();

    // operations
      virtual bool initialize();
      virtual void destroy();
      virtual void update();

    // media player functions
      virtual bool play(const Sound& sound);

    // factory functions
      virtual Sound* createSound(const String& filename) const;
      virtual Sound* createTrack(const String& filename) const;

   private:

    // data
      FMOD::System* mpSystem;
   };
}

#endif // FMOD_SOUND_MANAGER_H
