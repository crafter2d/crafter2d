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
#include "unzipfile.h"

#include <string.h>
#include <time.h>

#include "core/string/string.h"

//static
bool UnzipFile::isZip(const String& path)
{
   UnzipFile zip;
   return zip.open(path);
}

UnzipFile::UnzipFile():
   _zip(NULL)
{
}

UnzipFile::UnzipFile(const String &path):
   _zip(NULL)
{
   open(path);
}

UnzipFile::~UnzipFile()
{
   unzClose(_zip);
}

bool UnzipFile::open(const String& path)
{
   std::string p = path.toUtf8();
   _zip = unzOpen(p.c_str());
   return _zip != NULL;
}

bool UnzipFile::contains(const String& name) const
{
   bool result = false;
   if ( _zip != NULL )
   {
      std::string file = name.toUtf8();
      result = unzLocateFile(_zip, file.c_str(), 2) != UNZ_OK;
   }
   return result;
}

bool UnzipFile::readFile(const String& name, void*& pdata, int &size, bool casesensitive)
{
   bool result = false;
   if ( _zip != NULL )
   {
      std::string file = name.toUtf8();
      int result = unzLocateFile(_zip, file.c_str(), casesensitive ? 1 : 2);
      if ( result != UNZ_OK )
      {
         return false;
      }
   
      unz_file_info info;
      if ( unzGetCurrentFileInfo(_zip, &info, NULL, 0, NULL, 0, NULL, 0) != UNZ_OK )
         return false;

      size  = info.uncompressed_size;
      pdata = new char[size + 1];
      memset(pdata, 0, size+1);

      if ( unzOpenCurrentFile(_zip) != UNZ_OK )
         return false;

      size = unzReadCurrentFile(_zip, pdata, info.uncompressed_size);
      if ( size < 0 )
         return false;

      unzCloseCurrentFile(_zip);
   }

   return true;
}

