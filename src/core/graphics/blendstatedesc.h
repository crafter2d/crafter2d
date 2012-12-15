
#ifndef BLEND_STATE_DESC_H
#define BLEND_STATE_DESC_H

#include "core/core_base.h"

class String;

namespace Graphics
{
   class CORE_API BlendStateDesc
   {
   public:
      enum BlendFactor {
         BS_ONE,
         BS_ZERO,
         BS_SRC_COLOR,
         BS_SRC_INV_COLOR,
         BS_SRC_ALPHA,
         BS_SRC_INV_ALPHA,
         BS_DST_COLOR,
         BS_DST_INV_COLOR,
         BS_DST_ALPHA,
         BS_DST_INV_ALPHA,
         BS_INVALID
      };

      static BlendFactor fromString(const String& factor);

      BlendStateDesc(BlendFactor source, BlendFactor dest, bool enabled);

    // get/set
      BlendFactor getSourceFactor() const;
      BlendFactor getDestFactor() const;

      bool isEnabled() const;

   private:
      BlendFactor mSourceFactor;
      BlendFactor mDestFactor;
      bool        mEnabled;
   };
};

#endif // BLEND_STATE_DESC_H
