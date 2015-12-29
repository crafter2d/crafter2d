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
#ifndef VIEWPORT_H_
#define VIEWPORT_H_

#include "core/core_base.h"

namespace Graphics
{

   class CORE_API Viewport
   {
   public:
      Viewport();
      Viewport(int left, int top, int width, int height);
      ~Viewport();

      Viewport& operator=(const Viewport& that);

    // get/set
      int  getLeft() const { return mLeft; }
      void setLeft(int left) { mLeft = left; }

      int  getTop() const { return mTop; }
      void setTop(int top) { mTop = top; }

      int  getWidth() const { return mWidth; }
      void setWidth(int width) { mWidth = width; }

      int  getHeight() const { return mHeight; }
      void setHeight(int height) { mHeight = height; }

    // operations
      void resize(int width, int height) {
         mWidth = width;
         mHeight = height;
      }

   private:
      int mLeft;
      int mTop;
      int mWidth;
      int mHeight;
   };

};

#ifdef JENGINE_INLINE
#  include "viewport.inl"
#endif

#endif
