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
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <queue>
#include <string>
#include <SDL/SDL.h>
#include "texture.h"
#include "math/vector.h"
#include "scenegraph.h"
#include "resources.h"

class TiXmlDocument;
class State;

/**
@author Jeroen Broekhuizen
\brief Implements a base interface for objects in the game
*/
class Object: public SceneObject
{
public:
   DEFINE_REPLICATABLE(Object)

	                  Object();
	virtual           ~Object();

   virtual void      update(Uint32 tick);
	virtual void      draw();

	virtual Object*   clone ();

   void              addState(State* state);
	void              applyGravity (Uint32 tick);
	void              move (Uint32 tick);
   void              rotate(float deg);
	void              flip ();
	bool              direction() const;
	
	void              setMoveSpeed(int msec);
	void              setPosition(const Vector& vec);
	void              setVelocity(const Vector& vec);
   void              setRotation(const float deg);
	void              setVisible(bool vis = true);

	Vector            getPosition() const;
	Vector            getVelocity() const;
   float             getRadius() const;
   float             getRotation() const;
	int               getMoveSpeed() const;
   Vector            getSize() const;
	bool              isVisible() const;

   const std::string& filename() const;

 // visitor interface
   virtual void      accept(NodeVisitor& nv);

   virtual void      pack(BitStream& stream) const;
   virtual void      unpack(BitStream& stream);

protected:
   virtual bool      load(TiXmlDocument& doc);
	 
   std::string name;
	TexturePtr texture;
   std::queue<State*> states;
	int width, height;
	Vector pos, vel;
	float halfX, halfY;
   float angle;
	bool dir;
	float radius;
	Uint32 moveSpeed;
	Uint32 moveLast;
	bool visible;
};

#ifdef JENGINE_INLINE
#  include "object.inl"
#endif

#endif
