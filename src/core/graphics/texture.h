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

#include "core/core_base.h"
#include "core/content/content.h"
#include "core/string/string.h"
#include "core/defines.h"

namespace Graphics
{
   class Device;
   class RenderContext;

   enum TextureFormat {
      eFormat_Luminance,
      eFormat_RG,
      eFormat_RGBA,
      eFormat_DTX1,
      eFormat_DTX3,
      eFormat_DTX5,
   };

   struct TextureDescription
   {
      int width;
      int height;
      TextureFormat format;
      const void* pinitData;
   };

   /**
   @author Jeroen Broekhuizen
   \brief Wrapper class around an OpenGL texture. Use this class to load and display textures on your objects.
   */
   class CORE_API Texture : public IContent
   {
   public:
      explicit       Texture();
      virtual        ~Texture();

      virtual void   release();

      virtual void   update(RenderContext& context, const void* pdata, int rowpitch) = 0;

      virtual void   enable(RenderContext& context, int stage) const = 0;
      virtual void   disable(RenderContext& context) const = 0;
      
    // get/set
      const String& getName() const;
      void          setName(const String& name);

      const String& getUniform() const;
      void          setUniform(const String& uniform);

      virtual bool   isValid() const = 0;
      int            getWidth() const;
      int            getHeight() const;
      float          getSourceWidth() const;
      float          getSourceHeight() const;

   protected:
    // operations
      uint8_t*       ensureProperSize(int bytes, uint8_t* data, int width, int height);
      void           flipIt(unsigned char* buffer);

      String mName;
      String mUniform;
      int   _width;
      int   _height;
      int   _actualwidth;
      int   _actualheight;
      float _sourceWidth;
      float _sourceHeight;
   };
};

#ifdef JENGINE_INLINE
#  include "texture.inl"
#endif

#endif
