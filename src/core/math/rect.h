/***************************************************************************
*   Copyright (C) 2013 by Jeroen Broekhuizen                              *
*   crafter2d@outlook.com                                                 *
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
#ifndef RECT_H
#define RECT_H

#include "core/core_base.h"
#include "core/math/vector.h"

namespace c2d
{
   template<typename T>
   class Rect
   {
   public:
      Rect() :
         left(0),
         right(0),
         top(0),
         bottom(0)
      {
      }

      Rect(T x, T y, T width, T height) :
         left(x),
         right(x + width),
         top(y),
         bottom(y + height)
      {
      }

    // query
      T width() const {
         return right - left;
      }
      T height() const {
         return bottom - top;
      }
      
      Vector center() const {
         return Vector(width() / 2, height() / 2);
      }

      void set(T x, T y, T width, T height) {
         left = x;
         top = y;
         right = x + width;
         bottom = y + height;
      }

      bool contains(const Rect<T>& rect) const {
         return rect.left >= left
             && rect.top >= top
             && rect.right <= right
             && rect.bottom <= bottom;
      }

      bool overlaps(const Rect<T>& other) const {
         if ( other.left > right || left > other.right )
            return false;
         if ( other.top > bottom || top > other.bottom )
            return false;
         return true;
      }

      void shift(T x, T y) {
         left += x;
         top += y;
         right += x;
         bottom += y;
      }

    // data
      T left;
      T right;
      T top;
      T bottom;
   };

   using RectF = Rect<float>;
   using RectI = Rect<int>;

}

#endif // RECT_H