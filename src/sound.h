#ifndef _SOUND_H_
#define _SOUND_H

#include <AL/al.h>
#include <AL/alc.h>
#include "math/vector.h"
#include "hashtable.h"

/*!
@author Jeroen Broekhuizen
\brief Interface of a sound object.

The sound class represents a sound object that can be positioned and played somewhere 
in your world. To load a sound the SoundManager::load function must be used.
 */
class Sound
{
public:
                  Sound( ALuint buf, ALuint src );

	Sound*         clone();

	void           play( bool loop = false ) const;
	void           stop() const;

	void           setPosition( const Vector& pos );

private:
	ALuint buffer, source;
};

/*!
@author Jeroen Broekhuizen
\brief Manager for sound objects.

This class manages the sound objects in your game. Loading sounds must be done
via the load function. Currently only wave files are supported.

\todo Extra support for other sound formats
 */
class SoundManager
{
public:
   static SoundManager& getInstance();
   static void          setPlayerPosition(const Vector& pos);
   
	                     ~SoundManager();

	bool                 initialize();
	void                 destroy();
   
   bool                 isInitialized() const;
   bool                 isEnabled() const;

	Sound*               load(const char* file);
   void                 enable();
   void                 disable();

private:
	                     SoundManager();
   void                 operator=( const SoundManager& mgr );

private:
	ALCdevice *device;
	ALCcontext *context;
	HashTable dict;
   bool      _initialized;
   bool      _enabled; 
};

#ifdef JENGINE_INLINE
#  include "sound.inl"
#endif

#endif
