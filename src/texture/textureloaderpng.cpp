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
#include "textureloaderpng.h"

#include <SDL/SDL.h>
#include <png.h>

#include "../vfs/file.h"

#define PNG_BYTES_TO_CHECK 8

static void readPng(png_structp ctx, png_bytep area, png_size_t size)
{
   File* pfile = reinterpret_cast<File*>(png_get_io_ptr(ctx));
   pfile->read(area, size);
}

TextureLoaderPng::TextureLoaderPng():
   AbstractTextureLoader()
{
}

TextureLoaderPng::~TextureLoaderPng()
{
}

bool TextureLoaderPng::virLoad(File& file, TextureInfo& info)
{
   // make sure we are dealing with PNG
   char buf[PNG_BYTES_TO_CHECK];
   if ( file.read(buf, PNG_BYTES_TO_CHECK) != PNG_BYTES_TO_CHECK )
      return false;

   if ( png_sig_cmp((png_bytep)buf, (png_size_t)0, PNG_BYTES_TO_CHECK) != 0 )
      return false;

   // allocate required structures
   png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,NULL,NULL);
   if ( png_ptr == NULL )
      return false;

   png_infop info_ptr = png_create_info_struct(png_ptr);
   if ( info_ptr == NULL )
   {
      png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
      return false;
   }

   if ( setjmp(png_jmpbuf(png_ptr)) )
   {
      /* Free all of the memory associated with the png_ptr and info_ptr */
      png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
      return false;
   }

   png_set_read_fn(png_ptr, (void*)&file, readPng);
   png_set_sig_bytes(png_ptr, PNG_BYTES_TO_CHECK);

   // read the info from the file
   png_read_info(png_ptr, info_ptr);

   int bit_depth;
   int color_type;
   png_uint_32 width = 0;
   png_uint_32 height = 0;

   png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);

   if (bit_depth == 16)
      png_set_strip_16(png_ptr);

   bool transAlpha = false;
   if ( png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS) )
   {
      // convert transparent parts of the picture to alpha values
      png_set_expand(png_ptr);
      transAlpha = true;
   }

   // make sure we are dealing with a 24 bits image
   if ( color_type == PNG_COLOR_TYPE_PALETTE ) 
   {
      png_set_expand(png_ptr);
      info.setFormat(transAlpha ? TextureInfo::RGBA : TextureInfo::RGB);
   }
   else if ( color_type == PNG_COLOR_TYPE_GRAY )
   {
      png_set_expand(png_ptr);
      info.setFormat(TextureInfo::Alpha);
   }
   else if ( color_type == PNG_COLOR_TYPE_GRAY_ALPHA )
   {
      png_set_expand(png_ptr);
      png_set_gray_to_rgb(png_ptr);
      info.setFormat(TextureInfo::RGBA);
   }
   else if ( color_type == PNG_COLOR_TYPE_RGB )
   {
      png_set_expand(png_ptr);
      info.setFormat(transAlpha ? TextureInfo::RGBA : TextureInfo::RGB);
   }
   else if ( color_type == PNG_COLOR_TYPE_RGB_ALPHA )
   {
      png_set_expand(png_ptr);
      info.setFormat(TextureInfo::RGBA);
   }

   png_read_update_info(png_ptr, info_ptr);

   // get row length in bytes
   png_uint_32 row_bytes = png_get_rowbytes(png_ptr, info_ptr);
   png_uint_32 datasize = height * row_bytes;

   Uint8* pdata    = new Uint8[datasize];

   // read in the image
   const Uint32 MAX_ROWS = 2048;
   png_bytep row_pointers[MAX_ROWS];
   png_bytep base = pdata;
   for ( Uint32 row = 0; row < height; ++row )
      row_pointers[row] = base + (row * row_bytes);

   png_read_image(png_ptr, row_pointers);
   png_read_end(png_ptr, info_ptr);

   // fill the info structure
   info.setWidth(width);
   info.setHeight(height);
   info.setData(pdata);
   info.setDataSize(datasize);

   // free up memory & close the file
   png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

   return true;
}
