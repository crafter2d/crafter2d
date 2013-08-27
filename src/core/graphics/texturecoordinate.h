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
#ifndef _TEXTURE_COORDINATE
#define _TEXTURE_COORDINATE

#include "core/math/vertex.h"

namespace Graphics
{
   class TextureCoordinate
   {
   public:
      static const int TopLeft = 0;
      static const int TopRight = 1;
      static const int BottomRight = 2;
      static const int BottomLeft = 3;

      TextureCoordinate();
      TextureCoordinate(const TextureCoordinate& that);

      void initialize(const Vertex& tl, const Vertex& br);

      const Vertex& get(int index) const;

      const Vertex& getTopLeft() const;
      const Vertex& getTopRight() const;
      const Vertex& getBottomLeft() const;
      const Vertex& getBottomRight() const;

   private:
      Vertex mCoordinates[4];
   };
}

#endif
