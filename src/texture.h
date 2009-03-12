/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL/SDL.h>
#include <GL/GLee.h>
#include <string>

#include "refcount.h"

class TextureInfo;

/**
@author Jeroen Broekhuizen
\brief Wrapper class around an OpenGL texture. Use this class to load and display textures on your objects.
*/
class Texture : public RefCount
{
public:
   explicit       Texture();
   virtual        ~Texture();

   bool           createNormalizingCube(Uint32 size = 128);
   virtual void   release();

   virtual bool   load(const std::string& file);

   void           enable() const;
   void           disable () const;

   void           setStage (int stage);

   Uint32         getWidth() const;
   Uint32         getHeight() const;
   float          getSourceWidth() const;
   float          getSourceHeight() const;
   std::string    getFileName() const;
   bool           isValid() const;

protected:
   bool           generateTexture(const TextureInfo& info);
   Uint8*         ensureProperSize(int bytes, Uint8* data, Uint32 width, Uint32 height);
   void           flipIt(unsigned char* buffer);
   Uint32         findNextPowerOfTwo(Uint32 size);

   static GLenum  getRenderTarget();

protected:
   Uint32 _width;
   Uint32 _height;
   Uint32 _actualwidth;
   Uint32 _actualheight;
   float  _sourceWidth;
   float  _sourceHeight;
   GLuint tex;
   GLenum target;
   int texStage;
   std::string filename;
};

#ifdef JENGINE_INLINE
#  include "texture.inl"
#endif

#endif
