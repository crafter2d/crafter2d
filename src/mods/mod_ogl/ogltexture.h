
#ifndef OGL_TEXTURE_H
#define OGL_TEXTURE_H

#include "core/graphics/texture.h"

#include <GL/GLee.h>

namespace Graphics
{
   class OGLTexture : public Texture
   {
   public:
      OGLTexture();

    // creation
              bool   create(int width, int height, int bytes);
      virtual bool   create(Device& device, const TextureInfo& info) override;
      virtual void   update(RenderContext& context, const void* pdata, int rowpitch) override;
      virtual void   release() override;

    // enable/disable
      virtual void   enable(RenderContext& context, int stage) const override;
      virtual void   disable(RenderContext& context) const override;

    // query
      virtual bool   isValid() const override;

   private:
      static GLenum  getRenderTarget();

      GLuint mID;
      GLenum mTarget;
      GLenum mFormat;
      GLuint mBytes;
   };
};

#endif // OGL_TEXTURE_H
