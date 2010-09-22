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
#include "animationset.h"
#ifndef JENGINE_INLINE
#  include "animationset.inl"
#endif

#include "animation.h"

AnimationSet::AnimationSet():
   mAnimations()
{
}

AnimationSet::~AnimationSet()
{
   destroy();
}

/**************************************************************
 * Operations
 */

void AnimationSet::destroy()
{
   while (!mAnimations.empty())
      remove(mAnimations[0]);
}

void AnimationSet::remove(Animation* animation)
{
   Animations::iterator it = find(animation);
   if (it != mAnimations.end()) {
      mAnimations.erase(it);
   }
}