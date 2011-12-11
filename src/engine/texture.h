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
#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/GLee.h>
#include <string>

#include "core/defines.h"

#include "engine/resource/resource.h"
#include "engine/resource/resourceptr.h"

class TextureInfo;

/**
@author Jeroen Broekhuizen
\brief Wrapper class around an OpenGL texture. Use this class to load and display textures on your objects.
*/
class Texture : public Resource
{
public:
   explicit       Texture();
   virtual        ~Texture();

   bool           createNormalizingCube(int size = 128);
   virtual void   release();

   virtual bool   load(const std::string& file);

   void           enable() const;
   void           disable () const;

   void           setStage (int stage);

   int            getWidth() const;
   int            getHeight() const;
   float          getSourceWidth() const;
   float          getSourceHeight() const;
   bool           isValid() const;

   GLuint getId() const { return tex; }

protected:
   bool           generateTexture(const TextureInfo& info);
   uchar*         ensureProperSize(int bytes, uchar* data, int width, int height);
   void           flipIt(unsigned char* buffer);
   int            findNextPowerOfTwo(int size);

   static GLenum  getRenderTarget();

protected:
   int _width;
   int _height;
   int _actualwidth;
   int _actualheight;
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

typedef ResourcePtr<Texture> TexturePtr;

#endif
