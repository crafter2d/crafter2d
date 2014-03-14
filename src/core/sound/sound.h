
#ifndef SOUND_H
#define SOUND_H

#include "core/content/content.h"
#include "core/core_base.h"

class Vector;

namespace c2d
{
   class CORE_API Sound : public IContent
   {
   public:
      virtual Sound*         clone() = 0;

      virtual void           pause() = 0;
      virtual void           resume() = 0;
      virtual void           stop() const = 0;

      /// use this method to make the sound looping (e.g. a track)
      virtual void           setLooping() = 0;

      virtual void           setPosition(const Vector& pos) = 0;
   };
}
#endif // SOUND_H
