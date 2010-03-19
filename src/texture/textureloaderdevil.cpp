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
#include "textureloaderdevil.h"

#include <IL/il.h>
#include <IL/ilu.h>

#include "../vfs/file.h"
#include "../vfs/buffer.h"

#include "../autoptr.h"

// -- statics

static ILHANDLE ILAPIENTRY openProc(ILconst_string file)
{
  return 0;
}

static void ILAPIENTRY closeProc(ILHANDLE handle)
{
}

static ILboolean ILAPIENTRY eofProc(ILHANDLE handle)
{
  Buffer* pbuffer = (Buffer*)handle;
  return pbuffer->eof();
}

static ILint ILAPIENTRY getcProc(ILHANDLE handle)
{
  Buffer* pbuffer = (Buffer*)handle;
  return pbuffer->getc();
}

static ILint ILAPIENTRY readProc(void* buffer, ILuint typesize, ILuint datasize, ILHANDLE handle)
{
  ILuint readsize = typesize * datasize;
  Buffer* pbuffer = (Buffer*)handle;
  return pbuffer->read(buffer, readsize);
}

static ILint ILAPIENTRY seekProc(ILHANDLE handle, ILint skip, ILint mode)
{
  Buffer* pbuffer = (Buffer*)handle;
  pbuffer->seek(skip, mode);
  return 0;
}

static ILint ILAPIENTRY tellProc(ILHANDLE handle)
{
  Buffer* pbuffer = (Buffer*)handle;
  return pbuffer->tell();
}

// -- loader

TextureLoaderDevil::TextureLoaderDevil()
{
}

TextureLoaderDevil::~TextureLoaderDevil()
{
}

bool TextureLoaderDevil::virLoad(File& file, TextureInfo& info)
{
   ilSetRead(openProc, closeProc, eofProc, getcProc, readProc, seekProc, tellProc);

   ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
   ilEnable(IL_ORIGIN_SET);

   ILuint handle = ilGenImage();
   ilBindImage(handle);

   ILHANDLE bufferHandle = (ILHANDLE)&file.getBuffer();

   ILenum type = ilDetermineTypeF(bufferHandle);
   if ( !ilLoadF(type, bufferHandle) )
   {
     ilDeleteImage(handle);
     return false;
   }

   ILinfo ilinfo;
   iluGetImageInfo(&ilinfo);

   AutoPtr<Uint8> pdata = new Uint8[ilinfo.SizeOfData];
   memcpy(pdata.getPointer(), ilinfo.Data, ilinfo.SizeOfData);

   TextureInfo::Format format;
   switch ( ilinfo.Format )
   {
   case IL_ALPHA: format = TextureInfo::Alpha;  break;
   case IL_RGB:   format = TextureInfo::RGB;    break;
   case IL_RGBA:  format = TextureInfo::RGBA;   break;
   case IL_BGR:   format = TextureInfo::BGR;    break;
   }

   // fill info structure
   info.setWidth(ilinfo.Width);
   info.setHeight(ilinfo.Height);
   info.setData(pdata.release());
   info.setDataSize(ilinfo.SizeOfData);
   info.setFormat(format);

   ilDeleteImage(handle);

   return true;
}