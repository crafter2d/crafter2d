
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

bool OGLTexture::create(const TextureInfo& info)
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

   uchar* pdata = ensureProperSize(interval, info.getData(), _width, _height);
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

bool OGLTexture::createNormalizingCube(int size)
{
	mTarget = GL_TEXTURE_CUBE_MAP;

	mID = 0;
	glGenTextures(1, &mID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	float fX    = 0.0,
		  fY    = 0.0,
		  fZ    = 0.0,
		  oolen = 1.0;
	int x            =    0,
        y            =    0,
        z            =    0,
        mapSize      = size,
        halfMapSize  = mapSize/2;

  	GLubyte *dataContainer = new GLubyte[mapSize*mapSize*3];

	for(y=0;y<mapSize;y++) {
		for(z=0;z<mapSize;z++) {
			fX    = (float)halfMapSize;
			fY    = (float)halfMapSize-y;
			fZ    = (float)halfMapSize-z;
			oolen = 1.0f/sqrt(fX*fX+fY*fY+fZ*fZ);

			fX*=oolen;
			fY*=oolen;
			fZ*=oolen;

			dataContainer[y*3*mapSize+z*3+0] = GLubyte((((fX)+1.0f)/2.0f)*255.0f);
			dataContainer[y*3*mapSize+z*3+1] = GLubyte((((fY)+1.0f)/2.0f)*255.0f);
			dataContainer[y*3*mapSize+z*3+2] = GLubyte((((fZ)+1.0f)/2.0f)*255.0f);
		}
	}

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0,
                0, GL_RGB8, mapSize, mapSize,
                0, GL_RGB, GL_UNSIGNED_BYTE, dataContainer);

	for(y=0;y<mapSize;y++) {
		for(z=0;z<mapSize;z++) {
			fX    =(float)-halfMapSize;
			fY    =(float)halfMapSize-y;
			fZ    =(float)z-halfMapSize;
			oolen = 1.0f/sqrt(fX*fX+fY*fY+fZ*fZ);

			fX*=oolen;
			fY*=oolen;
			fZ*=oolen;
			dataContainer[y*3*mapSize+z*3+0] = GLubyte((((fX)+1.0f)/2.0f)*255.0f);
			dataContainer[y*3*mapSize+z*3+1] = GLubyte((((fY)+1.0f)/2.0f)*255.0f);
			dataContainer[y*3*mapSize+z*3+2] = GLubyte((((fZ)+1.0f)/2.0f)*255.0f);
		}
	}

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1,
                0, GL_RGB8, mapSize, mapSize,
                0, GL_RGB, GL_UNSIGNED_BYTE, dataContainer);

	for(z=0;z<mapSize;z++)
	{
		for(x=0;x<mapSize;x++)
		{
			fX    = (float)x-halfMapSize;
			fY    = (float)halfMapSize;
			fZ    = (float)z-halfMapSize;
			oolen = 1.0f/sqrt(fX*fX+fY*fY+fZ*fZ);

			fX*=oolen;
			fY*=oolen;
			fZ*=oolen;
			dataContainer[z*3*mapSize+x*3+0] = GLubyte((((fX)+1.0f)/2.0f)*255.0f);
			dataContainer[z*3*mapSize+x*3+1] = GLubyte((((fY)+1.0f)/2.0f)*255.0f);
			dataContainer[z*3*mapSize+x*3+2] = GLubyte((((fZ)+1.0f)/2.0f)*255.0f);
		}
	}

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2,
                0, GL_RGB8, mapSize, mapSize,
                0, GL_RGB, GL_UNSIGNED_BYTE, dataContainer);

	for(z=0;z<mapSize;z++)
	{
		for(x=0;x<mapSize;x++)
		{
			fX    = (float)x-halfMapSize;
			fY    = (float)-halfMapSize;
			fZ    = (float)halfMapSize-z;
			oolen = 1.0f/sqrt(fX*fX+fY*fY+fZ*fZ);

			fX*=oolen;
			fY*=oolen;
			fZ*=oolen;

			dataContainer[z*3*mapSize+x*3+0] = GLubyte((((fX)+1.0f)/2.0f)*255.0f);
			dataContainer[z*3*mapSize+x*3+1] = GLubyte((((fY)+1.0f)/2.0f)*255.0f);
			dataContainer[z*3*mapSize+x*3+2] = GLubyte((((fZ)+1.0f)/2.0f)*255.0f);
		}
	}

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3,
                0, GL_RGB8, mapSize, mapSize,
                0, GL_RGB, GL_UNSIGNED_BYTE, dataContainer);

	for(y=0;y<mapSize;y++)
	{
		for(x=0;x<mapSize;x++)
		{
			fX    = (float)x-halfMapSize;
			fY    = (float)halfMapSize - y;
			fZ    = (float)halfMapSize;
			oolen = 1.0f/sqrt(fX*fX+fY*fY+fZ*fZ);

			fX*=oolen;
			fY*=oolen;
			fZ*=oolen;

			dataContainer[y*3*mapSize+x*3+0] = GLubyte((((fX)+1.0f)/2.0f)*255.0f);
		    dataContainer[y*3*mapSize+x*3+1] = GLubyte((((fY)+1.0f)/2.0f)*255.0f);
			dataContainer[y*3*mapSize+x*3+2] = GLubyte((((fZ)+1.0f)/2.0f)*255.0f);
		}
	}

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4,
                0, GL_RGB8, mapSize, mapSize,
                0, GL_RGB, GL_UNSIGNED_BYTE, dataContainer);

	for(y=0;y<mapSize;y++)
	{
		for(x=0;x<mapSize;x++)
		{
			fX    = (float)halfMapSize - x;
			fY    = (float)halfMapSize - y;
			fZ    = (float)-halfMapSize;
			oolen = 1.0f/sqrt(fX*fX+fY*fY+fZ*fZ);

			fX*=oolen;
			fY*=oolen;
			fZ*=oolen;

			dataContainer[y*3*mapSize+x*3+0] = GLubyte((((fX)+1.0f)/2.0f)*255.0f);
			dataContainer[y*3*mapSize+x*3+1] = GLubyte((((fY)+1.0f)/2.0f)*255.0f);
			dataContainer[y*3*mapSize+x*3+2] = GLubyte((((fZ)+1.0f)/2.0f)*255.0f);
		}
	}
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5,
                0, GL_RGB8, mapSize, mapSize,
                0, GL_RGB, GL_UNSIGNED_BYTE, dataContainer);

	delete[] dataContainer;

	_height = size;
	_width  = size;
	return true;
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
void OGLTexture::enable() const
{
	glActiveTexture (GL_TEXTURE0 + mTexStage);
	glBindTexture(mTarget, mID);
}

void OGLTexture::disable () const
{
}

// - Query
bool OGLTexture::isValid() const
{
   return mID > 0;
}

};