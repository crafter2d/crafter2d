
#ifndef BLEND_STATE_H
#define BLEND_STATE_H

#include "core/core_base.h"

namespace Graphics
{
   class CORE_API BlendState
   {
   public:
      explicit BlendState(bool enabled = false);
      virtual ~BlendState();

    // get/set
      bool isEnabled() const;
      void setEnabled(bool enabled);

   private:

    // data
      bool mEnabled;
   };
};

#endif // BLEND_STATE_H
