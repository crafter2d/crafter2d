
#ifndef OGL_TEXTURE_H
#define OGL_TEXTURE_H

#include "core/graphics/texture.h"

#include "GL/gl3w.h"

namespace Graphics
{
   class OGLTexture : public Texture
   {
   public:
      OGLTexture();
      virtual ~OGLTexture();

    // creation
      bool create(int width, int height, int bytes);
      bool create(const TextureDescription& desc);

    // overrides
      virtual void   update(RenderContext& context, const void* pdata, int rowpitch) override;

      virtual void   enable(RenderContext& context, int stage) const override;
      virtual void   disable(RenderContext& context) const override;

      virtual bool   isValid() const override;

   private:
    // operations
      void release();

    // data
      GLuint mID;
      GLenum mTarget;
      GLenum mFormat;
      GLuint mBytes;
      GLuint mSampler;
   };
};

#endif // OGL_TEXTURE_H
