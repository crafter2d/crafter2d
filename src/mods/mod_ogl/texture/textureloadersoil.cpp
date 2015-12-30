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

#include "textureloadersoil.h"

#include <SOIL.h>

#include "core/vfs/file.h"
#include "core/smartptr/autoptr.h"
#include "core/streams/datastream.h"

namespace Graphics
{

TextureLoaderSoil::TextureLoaderSoil():
   AbstractTextureLoader()
{
}

TextureLoaderSoil::~TextureLoaderSoil()
{
}

bool TextureLoaderSoil::virLoad(DataStream& data, TextureInfo& info)
{
   int width, height, channels;
   unsigned char* pimage = SOIL_load_image_from_memory((unsigned char*)data.getData(), data.getDataSize(), &width, &height, &channels, SOIL_LOAD_AUTO);
   if ( pimage == NULL )
   {
     return false;
   }

   info.setChannels(channels);
   info.setData(pimage);
   info.setDataSize(width * height * channels);
   info.setWidth(width);
   info.setHeight(height);

   return true;
}

};
