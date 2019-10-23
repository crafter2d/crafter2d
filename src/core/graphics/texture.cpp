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
#include "texture.h"
#ifndef JENGINE_INLINE
#  include "texture.inl"
#endif

#include <fstream>
#include <cstdlib>
#include <string.h>
#include <math.h>

#include "core/log/log.h"
#include "core/string/string.h"
#include "core/math/math.h"

using namespace Graphics;

/*!
    \fn Texture::Texture()
	 \brief Initialized all internal variables.
 */
Texture::Texture():
   _width(0),
   _height(0),
   _actualwidth(0),
   _actualheight(0),
   _sourceWidth(1.0f),
   _sourceHeight(1.0f)
{
}

/*!
    \fn Texture::~Texture()
	 \brief Releases all internal variables.
 */
Texture::~Texture()
{
}

// - Operations

uint8_t* Texture::ensureProperSize(int bytes, uint8_t* pdata, int width, int height)
{
   _actualwidth  = Math::nextPowerOfTwo(width);
   _actualheight = Math::nextPowerOfTwo(height);

   if ( _actualwidth > 1024 || _actualheight > 1024 )
   {
      Log::getInstance().error("Image size of ... is too big!");
      delete[] pdata;
      return nullptr;
   }

   // make the texture rectangular
   _actualwidth = _actualheight = MAX(_actualwidth, _actualheight);
   if ( _actualwidth == width && _actualheight == height )
      return pdata;

   // calculate the max width in texture space
   _sourceWidth  = static_cast<float>(width) / _actualwidth;
   _sourceHeight = static_cast<float>(height) / _actualheight;

   int size = _actualwidth * _actualheight * bytes;
   uint8_t* pnewdata = new uint8_t[size];
   memset(pnewdata, 0, size);

   int rowwidth    = _actualwidth * bytes;
   int orgrowwidth = width * bytes;

   uint8_t* pactual  = pnewdata;
   uint8_t* porginal = pdata;
   for ( int y = 0; y < height; ++y )
   {
      memcpy(pactual, porginal, orgrowwidth);

      pactual  += rowwidth;
      porginal += orgrowwidth;
   }

   return pnewdata;
}
