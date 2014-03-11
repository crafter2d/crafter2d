
#ifndef SOUND_H
#define SOUND_H

#include "core/core_base.h"

class Vector;

namespace c2d
{
   class CORE_API Sound
   {
   public:
      virtual Sound*         clone() = 0;

      virtual void           pause() = 0;
      virtual void           resume() = 0;
      virtual void           stop() const = 0;

      virtual void           setPosition(const Vector& pos) = 0;
   };
}
#endif // SOUND_H
