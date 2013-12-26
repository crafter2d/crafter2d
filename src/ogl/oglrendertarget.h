
#ifndef OGL_RENDER_TARGET_H
#define OGL_RENDER_TARGET_H

#include <cstdint>

#include "core/graphics/rendertarget.h"

namespace Graphics
{
   class OGLRenderTarget : public RenderTarget
   {
   public:
      OGLRenderTarget();
      virtual ~OGLRenderTarget();

    // get/set
      uint32_t getFBO() const;

    // overrides
      virtual bool create(int width, int height) override;

   private:

    // operations
      void release();

    // data
      uint32_t mFBO;
      uint32_t mTexture;
   };
}

#endif // OGL_RENDER_TARGET_H
