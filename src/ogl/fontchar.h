/***************************************************************************
 *   Copyright (C) 2011 by Jeroen Broekhuizen                              *
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
#ifndef FONT_CHAR_H_
#define FONT_CHAR_H_

struct FT_Bitmap_;

namespace Graphics
{
   class FontChar
   {
   public:
      FontChar(int left, int top, int width, int height, int advancex, int advancey);
      ~FontChar();

    // get/set
      int getAdvanceX() const;
      int getAdvanceY() const;

      int getLeft() const;
      int getTop() const;
      int getWidth() const;
      int getHeight() const;

    // operations
      bool fromBitmap(FT_Bitmap_& bitmap);

    // rendering
      void render();

   private:
      friend class UIFont;

      int            mLeft;
      int            mTop;
      int            mWidth;
      int            mHeight;
      int            mAdvanceX;
      int            mAdvanceY;
      unsigned int   mTexture;
      unsigned int   mList;
   };
};

#ifdef JENGINE_INLINE
#  include "fontchar.inl"
#endif

#endif // FONT_CHAR_H_
