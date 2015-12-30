
#ifndef OGL_TEXTURE_H
#define OGL_TEXTURE_H

#include "core/graphics/texture.h"

#include <GL/glew.h>

namespace Graphics
{
   class OGLTexture : public Texture
   {
   public:
      OGLTexture();

    // creation
      bool   create(int width, int height, int bytes);

    // overrides
      virtual void   update(RenderContext& context, const void* pdata, int rowpitch) override;
      virtual void   release() override;

      virtual void   enable(RenderContext& context, int stage) const override;
      virtual void   disable(RenderContext& context) const override;

      virtual bool   isValid() const override;

   private:

    // data
      GLuint mID;
      GLenum mTarget;
      GLenum mFormat;
      GLuint mBytes;
   };
};

#endif // OGL_TEXTURE_H
