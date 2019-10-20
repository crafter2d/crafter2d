
#include "ogltexture.h"

#include "core/math/math.h"

namespace Graphics
{

OGLTexture::OGLTexture():
   Texture(),
   mID(0),
   mTarget(0),
   mFormat(0),
   mBytes(0),
   mSampler(0)
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
      mFormat = GL_RED;
      mBytes = GL_R8;
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

   glGenSamplers(1, &mSampler);

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

bool OGLTexture::create(const TextureDescription& desc)
{
   create(desc.width, desc.height, 4);

   int blockSize = 0;
   bool bc = false;
   switch ( desc.format )
   {
   case Graphics::eFormat_Luminance:
      mFormat = GL_RED;
      mBytes = 1;
      break;
   case Graphics::eFormat_RG:
      mFormat = GL_RG;
      mBytes = 2;
      break;
   case Graphics::eFormat_RGBA:
      mFormat = GL_RGBA;
      mBytes = 4;
      break;
   case Graphics::eFormat_BC3:
      bc = true;
      blockSize = 16;
      mBytes = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
      break;
   }

   if ( bc )
   {
      size_t size = ((desc.width + 3) / 4) * ((desc.height + 3) / 4) * blockSize;
      glCompressedTexImage2D(mTarget, 0, mBytes, desc.width, desc.height, 0, size, desc.pinitData);
   }
   else
   {
      glTexImage2D(mTarget, 0, mBytes, _actualwidth, _actualheight, 0, mFormat, GL_UNSIGNED_BYTE, desc.pinitData);
   }

   return true;
}

void OGLTexture::update(RenderContext& context, const void* pdata, int rowpitch)
{
   C2D_UNUSED(context);
   C2D_UNUSED(rowpitch);
   
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
   C2D_UNUSED(context);
   
	glActiveTexture (GL_TEXTURE0 + stage);
	glBindTexture(mTarget, mID);
   //glBindSampler(stage, mSampler);
}

void OGLTexture::disable(RenderContext& context) const
{
   C2D_UNUSED(context);
}

// - Query
bool OGLTexture::isValid() const
{
   return mID > 0;
}

};
