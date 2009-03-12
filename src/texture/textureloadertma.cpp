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
#include "textureloadertma.h"

#include "../vfs/file.h"

struct PIC_HEADER {
	unsigned short id;
	short version;
	short pixelFormat;
	int width;
	int height;
};

#define PIC_HEADER_ID			0x5F5F
#define PIC_HEADER_VERSION		1

#define PIC_FORMAT_RGB			0x0001
#define PIC_FORMAT_RGBA			0x0002

TextureLoaderTma::TextureLoaderTma(void)
{
}

TextureLoaderTma::~TextureLoaderTma(void)
{
}

bool TextureLoaderTma::virLoad(File& file, TextureInfo& info)
{
   PIC_HEADER ph;

   file.read(&ph, sizeof(PIC_HEADER));
   if (ph.id != PIC_HEADER_ID)
		return false;

   // allocate memory for the picture data	
	unsigned int dataLen = ph.width * ph.height * (ph.pixelFormat == PIC_FORMAT_RGB ? 3 : 4);
	unsigned char* data = new unsigned char[dataLen];
	if ( data == NULL )
		return false;
	
   // now read in the data
	file.read(data, dataLen);

   // fill info structure
   info.setWidth(ph.width);
   info.setHeight(ph.height);
   info.setData(data);
   info.setDataSize(dataLen);
   info.setFormat(ph.pixelFormat == PIC_FORMAT_RGB ? TextureInfo::RGB : TextureInfo::RGBA);

   return true;
}
