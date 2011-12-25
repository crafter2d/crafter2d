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
#ifndef _SOUND_H_
#define _SOUND_H_

#include <AL/al.h>
#include <AL/alc.h>

#include "core/math/vector.h"

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

/**
 * \author Jeroen Broekhuizen
 * \brief Manager for sound objects.
 *
 * This class manages the sound objects in your game. Loading sounds must be done
 * via the load function. Currently only wave files are supported. An example usage
 * is demonstrated below.
 *
 * @code
 * AutoPtr<Sound> sound = SoundManager::getInstance().load("mysound");
 * sound->play();
 * @endcode
 *
 * \todo Extra support for other sound formats
 */
class SoundManager
{
public:
   
   SoundManager();
   ~SoundManager();

	bool                 initialize();
	void                 destroy();
   
   bool                 isInitialized() const;
   bool                 isEnabled() const;

	Sound*               load(const char* file);
   void                 enable();
   void                 disable();

   void                 setPlayerPosition(const Vector& pos);

private:

   SoundManager(const SoundManager& mgr);
   void operator=( const SoundManager& mgr );

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
