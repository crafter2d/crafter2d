/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jeroen@jengine.homedns.org                                            *
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
#include "textureloaderbmp.h"

#include "../vfs/file.h"
#include "../vfs/buffer.h"

TextureLoaderBmp::TextureLoaderBmp(void)
{
}

TextureLoaderBmp::~TextureLoaderBmp(void)
{
}

bool TextureLoaderBmp::virLoad(File& file, TextureInfo& info)
{
   SDL_RWops* prw = file.getBuffer().asRWops();

   // load the bitmap
   SDL_Surface* psurface = SDL_LoadBMP_RW(prw, 0);
	if ( psurface == NULL )
		return false;

   // copy bitmap data
   Uint32 datasize = psurface->w * psurface->h * 3;
   Uint8* pdata = new Uint8[datasize];
   memcpy(pdata, psurface->pixels, datasize);

   // fill info structure
   info.setWidth(psurface->w);
   info.setHeight(psurface->h);
   info.setData(pdata);
   info.setDataSize(datasize);
   info.setFormat(TextureInfo::BGR);

   // free up the surface
   SDL_FreeSurface(psurface);
   SDL_FreeRW(prw);

   return true;
}