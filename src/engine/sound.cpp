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
#include "sound.h"
#ifndef JENGINE_INLINE
#  include "sound.inl"
#endif

#include <AL/alut.h>

#include "core/log/log.h"

#include "sound/soundloaderfactory.h"
#include "sound/soundloader.h"

/// \fn Sound::Sound(ALuint buf,ALuint src)
/// \brief initializes internal member variables. Remember to load the sounds via the
/// \param buf the buffer that contains the sound
/// \param src the source object that contains the sound object
/// SoundManager::load function (consult the OpenAL documentation for more information).
Sound::Sound(ALuint buf,ALuint src):
   buffer(buf),
   source(src)
{
}

/// \fn Sound::clone()
/// \brief Creates a clone of this sound object. In case you need multiple instances
/// of a sound, it's best to clone a sound object to minimize allocated resources (the
/// sound data is in memory only once after calling clone, while loading a sound twice
/// would result in the data being twice in memory).
/// \returns  clone of this sound object that is ready to be played
Sound* Sound::clone()
{
	ALuint osource;
	alGenSources(1, &osource);
	if( alGetError() != AL_NO_ERROR )
   {
      Log::getInstance().error("Sound.clone: Can not generate a source object.");
		return NULL;
	}

	alSourcei(osource, AL_BUFFER, buffer);
	alSourcef(osource, AL_PITCH,1.0f);
	alSourcef(osource, AL_GAIN, 1.0f);
	alSourcei(osource, AL_LOOPING, AL_FALSE);
	alSourcef(osource, AL_REFERENCE_DISTANCE, 75);
	alSourcef(osource, AL_MAX_DISTANCE, 100);

	return new Sound (buffer, osource);
}

/// \fn Sound::setPosition(const Vector& pos)
/// \brief Set the position of this sound object in 2D world space.
/// \param pos the position in 2D world space
void Sound::setPosition (const Vector& pos)
{
   // set the position of the sound
   alSource3f (source, AL_POSITION, pos.x, pos.y, 0.0f);
}

/// \fn Sound::play(bool loop = false)
/// \brief Plays the sound either continuously or once depending on the loop parameter.
/// \param loop true if it should loop continuously, false otherwise (default)
void Sound::play (bool loop) const
{
   // play the source
   alSourcei( source, AL_LOOPING, loop );
   alSourcePlay( source );
}

/// \fn Sound::stop()
/// \brief Stops playing the sound in case it is still active.
void Sound::stop() const
{
   // stop the sound
   alSourceStop( source );
}

/// \fn SoundManager::SoundManager(void)
/// \brief initializes internal member variables.
SoundManager::SoundManager():
   device(0),
   context(NULL),
   _initialized(false),
   _enabled(true)
{
}

/// \fn SoundManager::~SoundManager(void)
/// \brief releases internal member variables.
SoundManager::~SoundManager()
{
	if ( context != NULL )
	{
		alcDestroyContext (context);
		context = NULL;
	}
}

/// \fn SoundManager::setPlayerPosition(const Vector& pos)
/// \brief Sets the listener position of OpenAL. This position is used for sounds that have
/// a fixed position in world space.
/// \param pos the position of the player in world coordinates.
void SoundManager::setPlayerPosition (const Vector& pos)
{
   // set the listener position
   alListener3f (AL_POSITION, pos.x, pos.y, 0.0f);
}

/// \fn SoundManager::initialize()
/// \brief Initializes the sound system via the opensource OpenAL sound system. Details
/// about eventual failure are listed in the log file. The manager uses the default sound
/// device of the machine it runs on.
/// \retval true the sound system was initialized properly
/// \retval false failed to initialize the sound system, see log file for reason
bool SoundManager::initialize ()
{
   Log& log = Log::getInstance();

	// try to open the device
	device = alcOpenDevice(NULL);
	if ( device == NULL )
   {
      log.error("OpenAL: Could not open default device.");
		return false;
	}
	else 
   {
		const char* spec = (const char*)alcGetString (device, ALC_DEVICE_SPECIFIER);
      log.info("OpenAL: using %s", spec);
	}

	// try to create a context
	context = alcCreateContext (device, NULL);
	if ( context == NULL )
   {
      log.error("OpenAL: Could not create a context.");
		return false;
	}

	// activate the context
	alcMakeContextCurrent (context);
	if ( alGetError () != AL_NO_ERROR ) 
   {
      log.error("OpenAL: Could not make the context current.");
		return false;
	}

	// set the angle for the player
	static ALfloat angle[] = { 0, 0, -1, 0, 1, 0 };
	alListenerfv (AL_ORIENTATION, angle);
	alListener3f (AL_VELOCITY, 0, 0, 0);
	alListenerf (AL_GAIN, 1.0);

	// set the distance model
	alDistanceModel (AL_INVERSE_DISTANCE);

      // create the dictionary table
   if (!dict.create (256))
      return false;

   _initialized = true;

	return true;
}

/// \fn SoundManager::release()
/// \brief Releases the sound system. After calling this function any calls to other functions
/// of the sound manager can fail except the initialize function.
void SoundManager::destroy ()
{
   if ( context != NULL )
   {
      alcDestroyContext (context);
      alcCloseDevice (device);
   }
}

/// \fn SoundManager::load(const char* file)
/// \brief If the sound file was already loaded, a clone of the original object will be returned.
/// Otherwise the sound file will be loaded into memory.
/// \param file character string with the name of the sound file to load.
/// \returns a pointer to the loaded sound object, NULL if failed.
Sound* SoundManager::load (const char* file)
{
   if ( isEnabled() )
   {
      Sound *snd = static_cast<Sound*>(dict.lookup ((char*)file));
      if (snd != NULL) {
         // return a clone of this sound
         return snd->clone ();
      }
      else {
         ALuint buffer, source;
         alGetError();

         alGenSources(1, &source);
         if( alGetError() != AL_NO_ERROR )
         {
            Log::getInstance().error("OpenAL: Can not generate a source object.");
            return NULL;
         }

         // load the sound
         SoundLoader& loader = SoundLoaderFactory::getInstance().resolveLoader(file);
         buffer = loader.load(file);
         if( buffer == 0 )
         {
            Log::getInstance().error("OpenAL: could not load %s\n", file);
            return 0;
         }

         // set static source properties
         alSourcei( source, AL_BUFFER, buffer );
         alSourcef( source, AL_PITCH,1.0f );
         alSourcef( source, AL_GAIN, 1.0f );
         alSourcei( source, AL_LOOPING, AL_FALSE );
         //alSourcef( source, AL_REFERENCE_DISTANCE, 75 );
         //alSourcef( source, AL_MAX_DISTANCE, 100 );
         alSourcef (source, AL_ROLLOFF_FACTOR,  0.0          );
         alSourcei (source, AL_SOURCE_RELATIVE, AL_TRUE      );

         Sound* sound = new Sound( buffer, source );
         dict.insert( file, sound );
         return sound;
      }
   }
   else
      return NULL;
}
