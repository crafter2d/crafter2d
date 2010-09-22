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
#ifndef SOUNDTRACK_H
#define SOUNDTRACK_H

#include <string>

#include <AL/al.h>
#include <AL/alc.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

/**
@author Jeroen Broekhuizen
*/
class SoundTrack
{
public:
   enum State { eCreated, ePlaying, eStopped, eInvalid };

   SoundTrack();
   ~SoundTrack();
   
   bool create(const std::string& filename);
   void destroy();
   
   bool playing();
   
   bool play();
   void stop();
   bool update();
   
protected:
   bool stream(ALuint buffer);
   void dequeueAll();
   
   bool check();
   
private:
   FILE*             _pfile;
   OggVorbis_File    _oggStream;
   vorbis_info*      _poggInfo;

   ALuint            _buffers[4];
   ALuint            _source;
   ALenum            _format;

   State             mState;
   bool              _play;
};

#endif
