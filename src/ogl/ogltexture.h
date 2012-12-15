
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
   virtual bool   create(const TextureInfo& info);
   virtual bool   createNormalizingCube(int size = 128);
   virtual void   release();

 // enable/disable
   virtual void   enable() const;
   virtual void   disable () const;

 // query
   virtual bool   isValid() const;

private:
   static GLenum  getRenderTarget();

   GLuint mID;
   GLenum mTarget;
};

};

#endif // OGL_TEXTURE_H
