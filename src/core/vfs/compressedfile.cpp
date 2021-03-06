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
#include "compressedfile.h"
#ifndef JENGINE_INLINE
#  include "compressedfile.inl"
#endif

#include "buffer.h"
#include "memorybuffer.h"
#include "zipfile.h"

// static
bool CompressedFile::isCompressedFile(const String& file)
{
   std::size_t dot = file.indexOf('.');
   std::size_t slash = file.lastIndexOf('/');

   if ( dot < slash )
   {
      String zip = file.subStr(0, dot+3);
      return ZipFile::isZip(zip);
   }

   return false;
}

// static
bool CompressedFile::exists(const String& path, const String& file)
{
    ZipFile zipfile(path);
    return zipfile.contains(file);
}

CompressedFile::CompressedFile(ZipFile& zipFile):
   File(),
   mZipFile(zipFile),
   mFile()
{
}

CompressedFile::~CompressedFile()
{
   virClose();
}

bool CompressedFile::virOpen(const String& filename, int modus)
{
   ASSERT(IS_SET(modus, File::ERead))
   
   char* pdata = nullptr;
   int size = 0;

   mZipFile.readFile(filename, (void*&)pdata, size, false);

   setBuffer(Buffer::fromMemory(pdata, size));

   delete[] pdata;
  
   return true;
}

bool CompressedFile::isValid() const
{
   return true;
}

