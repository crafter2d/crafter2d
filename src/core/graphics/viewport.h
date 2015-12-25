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
      int  getLeft() const;
      void setLeft(int left);

      int  getTop() const;
      void setTop(int top);

      int  getWidth() const;
      void setWidth(int width);

      int  getHeight() const;
      void setHeight(int height);

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
