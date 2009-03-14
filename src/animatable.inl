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
#include "defines.h"

/**************************************************************
 * AnimationSet class
 */

/// \fn Animation::add(int id)
/// \brief Add a new item to the sequence
INLINE void Animation::add(int id)
{
   sequence.push_back(id);
}

/// \fn Animation::operator[](int index) const
/// \brief Return element 'index' from the sequence.
INLINE int Animation::operator[](int index) const
{
   return sequence[index];
}

INLINE int Animation::size() const
{
   return sequence.size();
}

/**************************************************************
 * AnimationSet class
 */

INLINE const Animation& AnimationSet::operator[](int index) const 
{
   return *animations[index];
}

INLINE int AnimationSet::size() const
{
   return static_cast<int>(animations.size());
}

INLINE void AnimationSet::add(Animation* anim)
{
	animations.push_back (anim);
}

INLINE AnimationSet::Animations::iterator AnimationSet::find(Animation* animation)
{
   Animations::iterator it = animations.begin();
   for ( ; it != animations.end(); ++it) {
      if ((*it) == animation)
         return it;
   }

   return animations.end();
}

/**************************************************************
 * AnimObject class
 */

INLINE int AnimObject::getAnimation() const
{
   return animCur;
}

INLINE AnimationSet& AnimObject::getAnimations()
{
   return *_animations;
}
