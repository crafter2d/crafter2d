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
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <string>
#include <vector>

#include "core/math/vector.h"

#include "texture.h"
#include "scenegraph.h"

class TiXmlDocument;

class Animator;
class Controller;
class State;

/**
@author Jeroen Broekhuizen
\brief Implements a base interface for objects in the game
*/
class Object: public SceneObject
{
public:
   DEFINE_REPLICATABLE(Object)

   enum { ePositionDirty = 2, eAnimationDirty = 4 };

	                  Object();
   virtual           ~Object();

   virtual void      destroy();
   virtual Object*   clone ();

   void              addState(State* state);
   void              move(float tick);
   void              rotate(float deg);
   void              flip();
   bool              direction() const;
	
 // get/set interface
   void              setPosition(const Vector& vec);
   void              setVelocity(const Vector& vec);
   void              setRotation(const float deg);
   void              setVisible(bool vis = true);

   virtual const Vector& getPosition() const;

   const Vector&     getVelocity() const;
   float             getRadius() const;
   float             getRotation() const;
   Vector            getSize() const;
   bool              isVisible() const;
   bool              isStatic() const;
   const Texture&    getTexture() const;

   bool              hasBody() const;
   Body&             getBody();

   int               getAnimation() const;
   void              setAnimation(int anim);

 // modifier interface
   Controller& getController();
   void              setController(Controller* pcontroller);

 // visitor interface
   virtual void      accept(NodeVisitor& nv);

 // streaming
   virtual void      pack(BitStream& stream) const;
   virtual void      unpack(BitStream& stream);

 // simulator interface
   void updateState();

protected:
   Animator&         getAnimator();

   virtual bool      load(TiXmlDocument& doc);

   virtual void      doUpdate(float delta);
   virtual void      doUpdateClient(float delta);
   virtual void      doDraw();

   virtual void      parentChanged();
	
   TexturePtr     texture;
   Body*          mpBody;
   Animator*      mpAnimator;
   Controller*    mpController;
   Vector         mPos;
   Vector         mVel;
   int            width, height;
   float          halfX, halfY;
   float          angle;
   float          radius;
   bool           visible;
   bool           dir;
   bool           mStatic;
};

#ifdef JENGINE_INLINE
#  include "object.inl"
#endif

#endif
