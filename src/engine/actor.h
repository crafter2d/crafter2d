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

#include "entity.h"

class TiXmlDocument;
class Controller;
class NodeVisitor;
class Vector;

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

 // maintenance
   virtual void      destroy();
   virtual Actor*    clone ();
   
   void              flip();
   bool              direction() const;
	
 // get/set interface
   void              setPosition(const Vector& vec);
   const Vector&     getPosition() const;

   float             getRotation() const;
   void              setRotation(const float deg);
  
   bool              isVisible() const;
   void              setVisible(bool vis = true);
  
   int               getAnimation() const;
   void              setAnimation(int anim);

 // query interface
   bool hasLineOfSight(const Actor& that) const;

 // modifier interface
   Controller&       getController();
   void              setController(Controller* pcontroller);

 // visitor interface
   virtual void      accept(NodeVisitor& nv);

 // simulator interface
   void updateState();

protected:

 // update & drawing
   virtual void      doUpdate(float delta);
   virtual void      doUpdateClient(float delta);
   virtual void      doDraw(Graphics::RenderContext& context) const override;

 // streaming
   virtual void      doPack(DataStream& stream) const;
   virtual void      doUnpack(DataStream& stream);
	
private:

   Controller*    mpController;
   int            mWidth, mHeight;
   bool           visible;
   bool           dir;
};

#ifdef JENGINE_INLINE
#  include "actor.inl"
#endif

#endif
