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

#include "core/core_base.h"

#include "core/math/vertex.h"

namespace c2d
{
   class CORE_API TextureCoordinate
   {
   public:
      TextureCoordinate();
      TextureCoordinate(const TextureCoordinate& that);
      TextureCoordinate& operator=(const TextureCoordinate& that);
      
    // operations
      void flip();

    // values      
      float left;
      float top;
      float right;
      float bottom;
   };
}

#endif
