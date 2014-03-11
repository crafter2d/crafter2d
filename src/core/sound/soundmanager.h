
#ifndef SOUND_MANAGER_H_
#define SOUND_MANAGER_H_

#include "core/core_base.h"

class String;

namespace c2d
{
   class Sound;

   class CORE_API SoundManager
   {
   public:
      SoundManager();

      // operations
      virtual bool initialize() = 0;
      virtual void destroy() = 0;
      virtual void update() = 0;

      // media player functions
      virtual bool play(const Sound& sound) = 0;

      // factory functions
      virtual Sound* createSound(const String& filename) const = 0;
      virtual Sound* createTrack(const String& filename) const = 0;
   };
}

#endif // SOUND_MANAGER_H_
