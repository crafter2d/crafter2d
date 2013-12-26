
#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

#include "core/core_base.h"

namespace Graphics
{
   class RenderContext;

   class CORE_API RenderTarget
   {
   public:
      RenderTarget();

    // operations
      virtual bool create(int width, int height) = 0;

      //virtual void enable(RenderContext& context) = 0;
      //virtual void disable(RenderContext& context) = 0;
   };
}

#endif // RENDER_TARGET_H
