
#ifndef FMOD_SOUND_MANAGER_H
#define FMOD_SOUND_MANAGER_H

namespace FMOD
{
   class System;
}

class String;

namespace c2d
{
   class Sound;

   class SoundManager
   {
   public:
      SoundManager();

    // operations
      bool initialize();
      void destroy();
      void update();

    // media player functions
      bool play(const Sound& sound);

    // factory functions
      Sound* createSound(const String& filename) const;
      Sound* createTrack(const String& filename) const;

   private:

    // data
      FMOD::System* mpSystem;
   };
}

#endif // FMOD_SOUND_MANAGER_H
