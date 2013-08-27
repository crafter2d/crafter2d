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
#ifndef TEXTURE_INFO_H_
#define TEXTURE_INFO_H_

#include "core/core_base.h"
#include "core/defines.h"

namespace Graphics
{
   class CORE_API TextureInfo
   {
   public:
      enum Format {Alpha, RGBA, RGB, BGR};

      TextureInfo();
      ~TextureInfo();

      int      getWidth() const;
      void     setWidth(int width);

      int      getHeight() const;
      void     setHeight(int height);

      uchar*   getData() const;
      void     setData(uchar* pdata);

      int      getDataSize() const;
      void     setDataSize(int size);

      Format   getFormat() const;
      void     setFormat(Format format);

   private:
      int      mWidth;
      int      mHeight;
      uchar*   mpData;
      int      mDataSize;
      Format   mFormat;
   };
};

#ifdef JENGINE_INLINE
#  include "textureinfo.inl"
#endif

#endif // TEXTURE_INFO_H_
