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
#include "unzipfile.h"
#include "zipfile.h"

// static
bool CompressedFile::isCompressedFile(const std::string& file)
{
   int dot = file.find('.');
   int slash = file.find('/');

   if ( dot < slash )
   {
      std::string zip = file.substr(0, dot+3);
      return UnzipFile::isZip(zip);
   }

   return false;
}

// static
bool CompressedFile::exists(const std::string& path, const std::string& file)
{
    UnzipFile zipfile(path);
    return zipfile.contains(file);
}

CompressedFile::CompressedFile():
   File(),
   mZipFile(),
   mFile()
{
}

CompressedFile::~CompressedFile()
{
   virClose();
}

bool CompressedFile::virOpen(const std::string& filename, int modus)
{
   decode(filename);

   if ( IS_SET(modus, File::ERead) )
   {
      char* pdata = NULL;
      int size = 0;

      UnzipFile zipfile(mZipFile);
      zipfile.readFile(mFile, (void*&)pdata, size);

      setBuffer(Buffer::fromMemory(pdata, size));

      delete[] pdata;
   }
   else
   {
      setBuffer(new MemoryBuffer());
   }

   return true;
}

void CompressedFile::virClose()
{
   if ( getBuffer().isWritting() )
   {
      MemoryBuffer& buffer = getBuffer().asMemoryBuffer();

      ZipFile zipfile(mZipFile);
      zipfile.addFile(mFile, buffer.getData(), buffer.getDataSize());
   }

   File::virClose();
}

bool CompressedFile::isValid() const
{
   return true;
}

void CompressedFile::decode(const std::string& filename)
{
   int dot = filename.find('.');
   int len = dot + 4;

   mZipFile = filename.substr(0, len);

   len = filename.length() - len - 1;

   mFile = filename.substr(dot + 5, len);
}
