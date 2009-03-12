/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jeroen@jengine.homedns.org                                            *
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

#include "soundtrack.h"

const int BUFFER_SIZE = 4096 * 16;

SoundTrack::SoundTrack():
   _pfile(NULL),
   _oggStream(),
   _poggInfo(NULL),
   _buffers(),
   _source(),
   _format(0),
   mState(eInvalid),
   _play(false)
{
}

SoundTrack::~SoundTrack()
{
   destroy();
}

//////////////////////////////////////////////////////////////////////////
// - Creation
//////////////////////////////////////////////////////////////////////////

bool SoundTrack::create(const std::string& filename)
{
   _pfile = fopen(filename.c_str(), "rb");
   if ( _pfile == NULL )
      return false;
   
   int result = ov_open(_pfile, &_oggStream, NULL, 0);
   if ( result < 0 )
   {
      fclose(_pfile);
      _pfile = NULL;
      
      return false;
   }
   
   _poggInfo = ov_info(&_oggStream, -1);
   if ( _poggInfo->channels == 1 )
      _format = AL_FORMAT_MONO16;
   else
      _format = AL_FORMAT_STEREO16;
      
   alGenBuffers(4, _buffers);
   alGenSources(1, &_source);
    
   alSource3f(_source, AL_POSITION,        0.0, 0.0, 0.0);
   alSource3f(_source, AL_VELOCITY,        0.0, 0.0, 0.0);
   alSource3f(_source, AL_DIRECTION,       0.0, 0.0, 0.0);
   alSourcef (_source, AL_ROLLOFF_FACTOR,  0.0          );
   alSourcei (_source, AL_SOURCE_RELATIVE, AL_TRUE      );
   
   check();
   
   mState = eCreated;
   
   return true;
}

void SoundTrack::destroy()
{
   stop();
   dequeueAll();
   alDeleteSources(1, &_source);
   alDeleteBuffers(4, _buffers);
   
   ov_clear(&_oggStream);
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

bool SoundTrack::playing()
{
   ALenum state;
    
   alGetSourcei(_source, AL_SOURCE_STATE, &state);
    
   return state == AL_PLAYING;
}

bool SoundTrack::play()
{
   if ( playing() )
      return false;

   mState = ePlaying;
   _play = true;
   
   dequeueAll();

   if ( !stream(_buffers[0]) )
      return false;

   if ( !stream(_buffers[1]) )
      return false;

   if ( !stream(_buffers[2]) )
      return false;

   if ( !stream(_buffers[3]) )
      return false;

   alSourceQueueBuffers(_source, 4, _buffers);
   alSourcePlay(_source);

   check();

   return true;
}

void SoundTrack::stop()
{
   alSourceStop(_source);

   mState = eStopped;
   _play = false;
}

bool SoundTrack::update()
{
   bool active = true;

   if ( mState == ePlaying )
   {
      if ( !playing() )
         play();

      int processed;
      alGetSourcei(_source, AL_BUFFERS_PROCESSED, &processed);

      while( processed-- )
      {
         ALuint buffer;
         alSourceUnqueueBuffers(_source, 1, &buffer);

         active = stream(buffer);

         alSourceQueueBuffers(_source, 1, &buffer);
      }
   }

   return active;
}

bool SoundTrack::stream(ALuint buffer)
{
   char data[BUFFER_SIZE];
   int  size = 0;
   int  section;

   while ( size < BUFFER_SIZE )
   {
      int bytesread = ov_read(&_oggStream, data + size, BUFFER_SIZE - size, 0, 2, 1, &section);
   
      if( bytesread > 0 )
         size += bytesread;
      else if ( bytesread < 0 )
         return false;
      else
         break;
   }
    
   if ( size == 0 )
      return false;
 
   alBufferData(buffer, _format, data, size, _poggInfo->rate);
 
   return check();
}

void SoundTrack::dequeueAll()
{
   int queued = 0;
   
   alGetSourcei(_source, AL_BUFFERS_QUEUED, &queued);
   
   while ( queued-- )
   {
      ALuint buffer;
      alSourceUnqueueBuffers(_source, 1, &buffer);
   }
}

bool SoundTrack::check()
{
   return alGetError() == AL_NO_ERROR;
}
