
#include "ogltexture.h"

#include "core/math/math.h"

namespace Graphics
{

OGLTexture::OGLTexture():
   Texture(),
   mID(0),
   mTarget(0)
{
}

// - Creation

bool OGLTexture::create(int width, int height, int bytes)
{
   mTarget = GL_TEXTURE_2D;
   mBytes = bytes;

   _width = width;
   _height = height;

   switch ( bytes )
   {
   case 1:
      mFormat = GL_LUMINANCE;
      break;
   case 3:
      mFormat = GL_RGB;
      break;
   case 4:
      mFormat = GL_RGBA;
      break;
   }

   _actualwidth = Math::nextPowerOfTwo(width);
   _actualheight = Math::nextPowerOfTwo(height);

   glGenTextures(1, &mID);
   glBindTexture(mTarget, mID);
   if ( mID == 0 )
   {
      return false;
   }

   glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

   return true;
}

void OGLTexture::update(RenderContext& context, const void* pdata, int rowpitch)
{
   glBindTexture(mTarget, mID);
   glTexImage2D(mTarget, 0, mBytes, _actualwidth, _actualheight, 0, mFormat, GL_UNSIGNED_BYTE, pdata);
}

/// \fn Texture::release()
/// \brief Releases the texture if not already done so.
/// \return Nothing
void OGLTexture::release()
{
	if ( mID != 0 )
   {
		glDeleteTextures(1, &mID);
		mID = 0;
	}
}

// - Enable disable

/// \fn Texture::enable(bool active=true)
/// \brief Use this function to either activate or deactivate this texture.
/// \param[in] active when true the texture is made active, when false it is deactivated
void OGLTexture::enable(RenderContext& context, int stage) const
{
	glActiveTexture (GL_TEXTURE0 + stage);
	glBindTexture(mTarget, mID);
}

void OGLTexture::disable(RenderContext& context) const
{
}

// - Query
bool OGLTexture::isValid() const
{
   return mID > 0;
}

};