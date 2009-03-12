//
// C++ Interface: soundtrack
//
// Description: 
//
//
// Author: Jeroen Broekhuizen <jeroen@jengine.homedns.org>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
