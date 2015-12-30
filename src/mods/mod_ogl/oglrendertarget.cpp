
#include "oglrendertarget.h"

#include <GL/glew.h>

namespace Graphics
{
   OGLRenderTarget::OGLRenderTarget():
      RenderTarget(),
      mFBO(0),
      mTexture(0)
   {
   }

   OGLRenderTarget::~OGLRenderTarget()
   {
      release();
   }

   // - Get/set

   uint32_t OGLRenderTarget::getFBO() const
   {
      return mFBO;
   }

   // - Operations

   bool OGLRenderTarget::create(int width, int height)
   {
      glGenFramebuffers(1, &mFBO);
      glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

      glGenTextures(1, &mTexture);
      glBindTexture(GL_TEXTURE_2D, mTexture);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_INT, 0);

      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);
      GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      return status == GL_FRAMEBUFFER_COMPLETE;
   }

   void OGLRenderTarget::release()
   {
      glBindTexture(GL_TEXTURE_2D, 0);
      glDeleteTextures(1, &mTexture);

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glDeleteFramebuffers(1, &mFBO);
   }

   /*
   void OGLRenderTarget::enable()
   {
      glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
   }

   void OGLRenderTarget::disable()
   {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
   }
   */

} // namespace Graphics
