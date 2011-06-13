/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "texture.h"
#ifndef JENGINE_INLINE
#  include "texture.inl"
#endif

#include <fstream>
#include <cstdlib>
#include <string.h>
#include <math.h>
#include <GL/GLee.h>
#include <GL/glu.h>
#include <SOIL.h>

#include "core/smartptr/autoptr.h"
#include "core/log/log.h"

#include "opengl.h"

#include "texture/abstracttextureloader.h"
#include "texture/textureloaderfactory.h"

/*!
    \fn Texture::Texture()
	 \brief Initialized all internal variables.
 */
Texture::Texture():
   _width(0),
   _height(0),
   _actualwidth(0),
   _actualheight(0),
   _sourceWidth(1.0f),
   _sourceHeight(1.0f),
   tex(0),
   target(0),
	texStage(0)
{
}

/*!
    \fn Texture::~Texture()
	 \brief Releases all internal variables.
 */
Texture::~Texture()
{
	release();
}

/*!
    \fn Texture::release()
	 \brief Releases the texture if not already done so.
	 \return Nothing
 */
void Texture::release()
{
	if (tex != 0) {
		glDeleteTextures (1, &tex);
		tex = 0;
	}
}

/// \fn Texture::load(const std::string& file)
/// \brief Call this function to load a texture from file. Currently only the BMP and TMA file formats are supported.
/// \param[in] filename the complete filename of the texture
/// \retval true if the texture was loaded successfully
/// \retval false not supported file format (see the loadBitmap and loadPicture functions for more information)
bool Texture::load(const std::string& file)
{
   if ( file.empty() )
      return false;

   setName(file);

   AutoPtr<AbstractTextureLoader> ploader = TextureLoaderFactory::constructLoader(file);
   ploader->load(file);

   return generateTexture(ploader->getTextureInfo());
}

bool Texture::generateTexture(const TextureInfo& info)
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
      target = getRenderTarget();

      // generate the GL texture
      glGenTextures (1, &tex);
	   glBindTexture (target, tex);
      glTexImage2D (target, 0, interval, _actualwidth, _actualheight, 0, format, GL_UNSIGNED_BYTE, pdata);

      glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

      glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

      if ( pdata != info.getData() )
         delete[] pdata;

      success = true;
   }

	return success;
}

// - Operations

/*!
    \fn Texture::enable(bool active=true)
	 \brief Use this function to either activate or deactivate this texture.
	 \param[in] active when true the texture is made active, when false it is deactivated
 */
void Texture::enable() const
{
	glActiveTexture (GL_TEXTURE0+texStage);
	glBindTexture (target, tex);
	glEnable (target);
}

void Texture::disable () const
{
	glActiveTexture (GL_TEXTURE0+texStage);
	glDisable (target);
}

GLenum Texture::getRenderTarget()
{
	return GL_TEXTURE_2D;
}

bool Texture::createNormalizingCube(int size)
{
	target = GL_TEXTURE_CUBE_MAP;

	tex = 0;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

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

uchar* Texture::ensureProperSize(int bytes, uchar* pdata, int width, int height)
{
   _actualwidth  = findNextPowerOfTwo(width);
   _actualheight = findNextPowerOfTwo(height);

   if ( _actualwidth > 1024 || _actualheight > 1024 )
   {
      Log::getInstance().error("Image size of %s is too big!", filename.c_str());
      delete[] pdata;
      return NULL;
   }

   // make the texture rectangular
   _actualwidth = _actualheight = MAX(_actualwidth, _actualheight);
   if ( _actualwidth == width && _actualheight == height )
      return pdata;

   // calculate the max width in texture space
   _sourceWidth  = static_cast<float>(width) / _actualwidth;
   _sourceHeight = static_cast<float>(height) / _actualheight;

   int size = _actualwidth * _actualheight * bytes;
   uchar* pnewdata = new uchar[size];
   memset(pnewdata, 0, width);

   int rowwidth    = _actualwidth * bytes;
   int orgrowwidth = width * bytes;

   uchar* pactual  = pnewdata;
   uchar* porginal = pdata;
   for ( int y = 0; y < height; ++y )
   {
      memcpy(pactual, porginal, orgrowwidth);

      pactual  += rowwidth;
      porginal += orgrowwidth;
   }

   return pnewdata;
}

int Texture::findNextPowerOfTwo(int size)
{
   int newsize = 1;
   while ( newsize < size )
      newsize <<= 1;

   return newsize;
}
