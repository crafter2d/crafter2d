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
#include "soundloaderogg.h"

#include <AL/alc.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include "oggfile.h"

SoundLoaderOgg::SoundLoaderOgg():
   SoundLoader()
{
}

SoundLoaderOgg::~SoundLoaderOgg()
{
}

ALuint SoundLoaderOgg::load(const std::string& filename)
{
   CharBuffer data;
   ALenum format;
   ALsizei frequency;

   if ( buffer(filename, data, format, frequency) )
   {
      ALuint buffer;
      alGenBuffers(1, &buffer);
      alBufferData(buffer, format, &data[0], static_cast<ALsizei>(data.size()), frequency);

      return buffer;
   }
   else
      return 0;
}

bool SoundLoaderOgg::buffer(const std::string& filename, CharBuffer& buffer, ALenum &format, ALsizei &freq)
{
   static const int BUFFER_SIZE = 32768;    // 32 KB buffers
   OggFile file;

   if ( file.open(filename) )
   {
      format = file.getFormat();
      freq   = file.getFrequency();

      long bytes;
      char array[BUFFER_SIZE];

      // store the data in the buffer
      do 
      {
         bytes = file.read(array, BUFFER_SIZE);
         buffer.insert(buffer.end(), array, array + bytes);
      }
      while ( bytes > 0 );

      return true;
   }

   return false;
}
