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
#include "zipfile.h"

#include <memory>
#include <string.h>
#include <zip.h>

#include "core/string/string.h"
#include "core/defines.h"

using ZipPtr = std::unique_ptr<zip_t, int(*)(zip_t*)>;

class ZipFile::ZipImpl
{
public:
   ZipImpl():
      zipptr(nullptr, nullptr)
   {
   }

   ZipPtr zipptr;
};

bool ZipFile::isZip(const String& path)
{
   ZipFile zip;
   return zip.open(path);
}

ZipFile::ZipFile():
   mImpl(new ZipImpl()),
   mError()
{
}

ZipFile::ZipFile(const String &path):
   ZipFile()
{
   open(path, 0);
}

ZipFile::~ZipFile()
{
   delete mImpl;
}

bool ZipFile::open(const String& path)
{
   return open(path, 0);
}

bool ZipFile::create(const String& path)
{
   return open(path, ZIP_CREATE);
}

bool ZipFile::open(const String& path, int flags)
{
   int error = 0;
   std::string p = path.toUtf8();
   auto pzip = zip_open(p.c_str(), flags, &error);

   if ( pzip == nullptr )
   {
      char buf[128]{};
      zip_error_to_str(buf, sizeof(buf), error, errno);
      mError = buf;

      return false;
   }

   mImpl->zipptr = { pzip, zip_close };

   return true;
}

// zip file always has the / path separator
std::string toZipPath(const String& path)
{
   String internalpath(path);
   internalpath.replace(L'\\', L'/');
   return internalpath.toUtf8();
}

bool ZipFile::contains(const String& name) const
{
   std::string file = toZipPath(name);
   auto index = zip_name_locate(mImpl->zipptr.get(), file.c_str(), 0);
   return index >= 0;
}

void ZipFile::addFile(const String& name, const void* pdata, int size)
{
   struct zip_source* psource = zip_source_buffer(mImpl->zipptr.get(), pdata, size, 0);

   std::string file = toZipPath(name);
   if ( zip_file_add(mImpl->zipptr.get(), file.c_str(), psource, ZIP_FL_ENC_UTF_8) < 0 )
   {
      zip_source_free(psource);
      mError = zip_strerror(mImpl->zipptr.get());
   }
}

bool ZipFile::readFile(const String& name, void*& pdata, int &size, bool casesensitive)
{
   C2D_UNUSED(casesensitive);
   
   zip_t* pzip = mImpl->zipptr.get();
   std::string file = toZipPath(name);
   auto index = zip_name_locate(pzip, file.c_str(), 0);
   if ( index < 0 )
   {
      return false;
   }

   struct zip_stat stat;
   if ( zip_stat_index(pzip, index, 0, &stat) != 0 )
   {
      mError = zip_strerror(pzip);
      return false;
   }

   struct zip_file* pfile = zip_fopen(pzip, file.c_str(), 0);

   if ( pfile != nullptr )
   {
      size = stat.size;
      std::unique_ptr<char[]> result(new char[size + 1]);
      memset(result.get(), 0, size + 1);

      auto read = zip_fread(pfile, result.get(), size);
      if ( read < 0 )
      {
         size = 0;
         return false;
      }

      zip_fclose(pfile);
      
      pdata = result.release();

      return true;
   }

   return false;
}
