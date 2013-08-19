
#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

namespace FMOD
{
   class Sound;
   class System;
}

class String;

namespace JEngineSSE
{
   class Sound;

   class SoundManager
   {
   public:
      SoundManager();

    // operations
      bool initialize();
      void destroy();

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

#endif // SOUND_MANAGER_H
