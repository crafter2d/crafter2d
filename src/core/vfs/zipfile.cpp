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

#include <time.h>

ZipFile::ZipFile():
   _zip(NULL)
{
}

ZipFile::ZipFile(const std::string &path):
   _zip(NULL)
{
   open(path);
}

ZipFile::~ZipFile()
{
   zipClose(_zip, NULL);
}

bool ZipFile::open(const std::string& path)
{
   _zip = zipOpen(path.c_str(), APPEND_STATUS_ADDINZIP);

   return _zip != NULL;
}

bool ZipFile::create(const std::string& path)
{
   _zip = zipOpen(path.c_str(), APPEND_STATUS_CREATE);

   return _zip != NULL;
}

void ZipFile::addFile(const std::string& name, void* pdata, int size)
{
   const zip_fileinfo info = constructInfo();

   zipOpenNewFileInZip(_zip, 
                       name.c_str(),
                       &info,
                       NULL,
                       0,
                       NULL,
                       0,
                       NULL,                       // comments
                       0,                          // method (store)
                       Z_DEFAULT_COMPRESSION);     // compression

   zipWriteInFileInZip(_zip, pdata, size);

   zipCloseFileInZip(_zip);
}

zip_fileinfo ZipFile::constructInfo()
{
   time_t rawtime;
   struct tm * timeinfo;

   time(&rawtime);
   timeinfo = localtime(&rawtime);

   zip_fileinfo info;
   info.tmz_date.tm_sec  = timeinfo->tm_sec;
   info.tmz_date.tm_min  = timeinfo->tm_min;
   info.tmz_date.tm_hour = timeinfo->tm_hour;
   info.tmz_date.tm_mday = timeinfo->tm_mday;
   info.tmz_date.tm_mon  = timeinfo->tm_mon;
   info.tmz_date.tm_year = timeinfo->tm_year;

   info.dosDate     = 0;
   info.internal_fa = 0;
   info.external_fa = 0;

   return info;
}
