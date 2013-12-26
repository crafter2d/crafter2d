
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
   virtual bool   create(Device& device, const TextureInfo& info) override;
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
};

};

#endif // OGL_TEXTURE_H
