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
#include "textureloaderfactory.h"

#include "textureloaderbmp.h"
#include "textureloaderpng.h"
#include "textureloadertma.h"

AbstractTextureLoader* TextureLoaderFactory::constructLoader(const std::string& filename)
{
   AbstractTextureLoader* ploader = NULL;

   if ( filename.rfind("bmp") != filename.npos )
   {
		ploader = new TextureLoaderBmp();
   }
   else if ( filename.rfind("png") != filename.npos )
   {
      ploader = new TextureLoaderPng();
   }
   else if ( filename.rfind("tma") != filename.npos )
   {
		ploader = new TextureLoaderTma();
   }

   return ploader;
}

TextureLoaderFactory::TextureLoaderFactory()
{
}

TextureLoaderFactory::~TextureLoaderFactory()
{
}
