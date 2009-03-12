/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef ANIMATABLE_H
#define ANIMATABLE_H

#include <vector>
#include "object.h"
#include "refcount.h"
#include "texturecoordlookup.h"

class Animation
{
public:
   typedef std::vector<int> Sequence;

   void     add(int id);

   int      size() const;
	int      operator[](int index) const;

protected:
   Sequence sequence;
};

/**
@author Jeroen Broekhuizen
\brief Interface for an animation set. It includes one animation.

You can use this class for your objects that have a form of animation in it.
*/
class AnimationSet: public RefCount
{
public:
   typedef std::vector<Animation*> Animations;

	virtual           ~AnimationSet();

   void              destroy();

   void              add(Animation* anim);
   void              remove(Animation* anim);

	int               size() const;
	const Animation&  operator[](int index) const;

private:
   Animations::iterator find(Animation* animation);

protected:
   Animations animations;
};

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
	 
   virtual void   update(Uint32 tick);
	virtual void   draw();

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
	void           parseAnimation(const char* sequence, Animation *animation);
   void           determineFrameCount();

	RefPointer<AnimationSet> _animations;
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
