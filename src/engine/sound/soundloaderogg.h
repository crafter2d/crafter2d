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
#ifndef OGG_SOUND_LOADER_H_
#define OGG_SOUND_LOADER_H_

#include <vector>
#include "soundloader.h"

class SoundLoaderOgg : public SoundLoader
{
public:
 // typedefs
   typedef std::vector<char> CharBuffer;

   SoundLoaderOgg();
   virtual ~SoundLoaderOgg();

   virtual ALuint load(const std::string& filename);

private:
   bool buffer(const std::string& filename, CharBuffer& buffer, ALenum &format, ALsizei &freq);
};

#endif