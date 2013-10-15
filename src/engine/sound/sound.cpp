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

#include <fmod.hpp>

#include "core/log/log.h"

namespace JEngineSSE
{

/// \fn Sound::Sound(FMOD::Sound* psound)
/// \brief initializes internal member variables. Remember to load the sounds via the
/// \param psound a FMOD sound object
   Sound::Sound(FMOD::Sound* psound):
   mpSound(psound),
   mpChannel(NULL)
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
	Sound* presult = new Sound(mpSound);
   return presult;
}

/// \fn Sound::setPosition(const Vector& pos)
/// \brief Set the position of this sound object in 2D world space.
/// \param pos the position in 2D world space
void Sound::setPosition (const Vector& pos)
{
   // set the position of the sound
}

void Sound::pause()
{
   mpChannel->setPaused(true);
}

void Sound::resume()
{
   mpChannel->setPaused(false);
}

/// \fn Sound::stop()
/// \brief Stops playing the sound in case it is still active.
void Sound::stop() const
{
   ASSERT_PTR(mpChannel);
   mpChannel->stop();
}

}