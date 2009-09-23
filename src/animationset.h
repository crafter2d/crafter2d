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
#ifndef ANIMATIONSET_H_
#define ANIMATIONSET_H_

#include <vector>
#include "refcount.h"

class Animation;

/**
@author Jeroen Broekhuizen
\brief Interface for an animation set. It includes one animation.

You can use this class for your objects that have a form of animation in it.
*/
class AnimationSet: public RefCount
{
public:
   typedef std::vector<Animation*> Animations;

                     AnimationSet();
	virtual           ~AnimationSet();

   void              destroy();

   void              add(Animation* anim);
   void              remove(Animation* anim);

	int               size() const;
	const Animation&  operator[](int index) const;

private:
   Animations::iterator find(Animation* animation);

   Animations mAnimations;
};

#ifdef JENGINE_INLINE
#  include "animationset.inl"
#endif

#endif
