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
#ifndef ANIMATABLE_H
#define ANIMATABLE_H

#include <vector>
#include "animationset.h"
#include "object.h"
#include "refcount.h"
#include "texturecoordlookup.h"

/**
@author Jeroen Broekhuizen
\brief Interface for animatable objects.

You can use this class for your objects that have a form of animation in it.
*/
class AnimObject : public Object
{
public:
   DEFINE_REPLICATABLE(AnimObject)

                  AnimObject();
   virtual        ~AnimObject();

	AnimObject*    clone();
   int            getAnimation() const;
	void           setAnimation(int anim);
   void           animate(Uint32 tick);
   void           nextFrame();

   AnimationSet&  getAnimations();

 // visitor interface
   virtual void   accept(NodeVisitor& nv);

   virtual void   pack(BitStream& stream) const;
   virtual void   unpack(BitStream& stream);

protected:
   virtual bool   load(TiXmlDocument& doc);
   void           determineFrameCount();

   virtual void   doUpdate(DirtySet& dirtyset, float delta);
	virtual void   doDraw();

	RefPointer<AnimationSet> mAnimations;
   TextureCoordLookup _textureCoords;
	Uint32 animSpeed;
	Uint32 animLast;
   bool   _animChanged;
   float  _animFrameWidth;
   int    _animFrameCount;
	int animCur;
	int animIndex;
	int animFrame;
};

#ifdef JENGINE_INLINE
#  include "animatable.inl"
#endif

#endif
