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
#ifndef _ACTOR_H_
#define _ACTOR_H_

#include <string>
#include <vector>

#include "core/math/vector.h"

#include "entity.h"
#include "texture.h"

class TiXmlDocument;

class Animator;
class Body;
class Controller;
class NodeVisitor;
class State;

/**
@author Jeroen Broekhuizen
\brief Implements a base interface for objects in the game
*/
class Actor: public Entity
{
public:
   DEFINE_REPLICATABLE(Object)

   enum { ePositionDirty = 2, eAnimationDirty = 4 };

	                  Actor();
   virtual           ~Actor();

   virtual void      destroy();
   virtual Actor*    clone ();

   void              addState(State* state);
   void              rotate(float deg);
   void              flip();
   bool              direction() const;
	
 // get/set interface
   void              setPosition(const Vector& vec);
   void              setVelocity(const Vector& vec);
   void              setRotation(const float deg);
   void              setSize(int width, int height);
   void              setVisible(bool vis = true);
   void              setStatic(bool isstatic);
   void              setTexture(TexturePtr texture);
   void              setAnimator(Animator* panimator);
   void              setBody(Body& body);

   const Vector&     getPosition() const;
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
   Controller&       getController();
   void              setController(Controller* pcontroller);

 // visitor interface
   virtual void      accept(NodeVisitor& nv);

 // simulator interface
   void updateState();

protected:
 // get/set
   Animator&         getAnimator();

 // update & drawing
   virtual void      doUpdate(float delta);
   virtual void      doUpdateClient(float delta);
   virtual void      doDraw() const;

 // streaming
   virtual void      doPack(BitStream& stream) const;
   virtual void      doUnpack(BitStream& stream, int dirtyflag);
	
private:

   TexturePtr     texture;
   Body*          mpBody;
   Animator*      mpAnimator; // owned
   Controller*    mpController;
   Vector         mPos;
   Vector         mVel;
   int            mWidth, mHeight;
   float          halfX, halfY;
   float          angle;
   float          radius;
   bool           visible;
   bool           dir;
   bool           mStatic;
};

#ifdef JENGINE_INLINE
#  include "actor.inl"
#endif

#endif