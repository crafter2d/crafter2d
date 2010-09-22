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
#ifndef OGG_FILE_H_
#define OGG_FILE_H_

#include <string>
#include <AL/al.h>
#include <vorbis/vorbisfile.h>

class OggFile
{
public:
             OggFile();
   explicit OggFile(const std::string& filename);
            ~OggFile();

 // get/set interface
   ALenum   getFormat() const;
   ALsizei  getFrequency() const;

 // operations
   bool open(const std::string& filename);
   void close();

   long read(char* pbuffer, int size);

private:
   void readInfo();

   OggVorbis_File _oggFile;
   ALenum         _format;
   ALsizei        _freq;
};

#ifdef JENGINE_INLINE
#  include "oggfile.inl"
#endif

#endif
