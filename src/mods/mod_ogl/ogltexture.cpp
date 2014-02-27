
#include "ogltexture.h"

#include "core/graphics/textureinfo.h"

namespace Graphics
{

OGLTexture::OGLTexture():
   Texture(),
   mID(0),
   mTarget(0)
{
}

// - Creation

bool OGLTexture::create(Device& device, const TextureInfo& info)
{
   bool success = false;

   _width  = info.getWidth();
   _height = info.getHeight();

   GLenum format = GL_RGBA;
   GLint  interval = 4;

   switch ( info.getFormat() )
   {
   case TextureInfo::Alpha:
      // not implemented yet
      break;
   case TextureInfo::RGB:
      format   = GL_RGB;
      interval = 3;
      break;
   case TextureInfo::BGR:
      format   = GL_BGR;
      interval = 3;
      break;
   case TextureInfo::RGBA:
   default:
      break;
   }

   uint8_t* pdata = ensureProperSize(interval, info.getData(), _width, _height);
   if ( pdata != NULL )
   {
      mTarget = GL_TEXTURE_2D;

      // generate the GL texture
      glGenTextures (1, &mID);
	   glBindTexture (mTarget, mID);
      glTexImage2D (mTarget, 0, interval, _actualwidth, _actualheight, 0, format, GL_UNSIGNED_BYTE, pdata);

      glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

      glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

      if ( pdata != info.getData() )
         delete[] pdata;

      success = true;
   }

	return success;
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