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
#include "oggfile.h"
#ifndef JENGINE_INLINE
#  include "oggfile.inl"
#endif

OggFile::OggFile():
   _oggFile(),
   _format(0),
   _freq(0)
{
}

OggFile::OggFile(const std::string& filename):
   _oggFile(),
   _format(0),
   _freq(0)
{
   open(filename);
}

OggFile::~OggFile()
{
   close();
}

bool OggFile::open(const std::string& filename)
{
   FILE *pfile;

   pfile = fopen(filename.c_str(), "rb");
   if ( pfile == NULL )
      return false;

   // open the oggfile
   int result = ov_open(pfile, &_oggFile, NULL, 0);
   if ( result < 0 )
   {
      fclose(pfile);
      return false;
   }

   // read the information
   readInfo();
   
   return true;
}

void OggFile::close()
{
   ov_clear(&_oggFile);
}

long OggFile::read(char* pbuffer, int size)
{
   int bitStream;

   long bytes = ov_read(&_oggFile, pbuffer, size, 0, 2, 1, &bitStream);

   return bytes;
}

void OggFile::readInfo()
{
   // get the OGG file information
   vorbis_info *pinfo = ov_info(&_oggFile, -1);
   if ( pinfo != NULL )
   {
      if ( pinfo->channels == 1 )
         _format = AL_FORMAT_MONO16;
      else
         _format = AL_FORMAT_STEREO16;

      _freq = pinfo->rate;
   }
}
