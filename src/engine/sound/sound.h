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

namespace FMOD
{
   class Channel;
   class Sound;
}

#include "core/math/vector.h"

namespace JEngineSSE
{
   /*!
   @author Jeroen Broekhuizen
   \brief Interface of a sound object.

   The sound class represents a sound object that can be positioned and played somewhere 
   in your world. To load a sound the SoundManager::load function must be used.
    */
   class Sound
   {
   public:
	   Sound*         clone();

	   void           pause();
      void           resume();
	   void           stop() const;

	   void           setPosition( const Vector& pos );

   private:
      friend class SoundManager;

    // constructors
      explicit       Sound(FMOD::Sound* psound);

    // data
      FMOD::Sound*   mpSound;
      mutable FMOD::Channel* mpChannel;
   };
}

#ifdef JENGINE_INLINE
#  include "sound.inl"
#endif

#endif
