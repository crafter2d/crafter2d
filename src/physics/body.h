/***************************************************************************
 *   Copyright (C) 2009 by Jeroen Broekhuizen                              *
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
#ifndef BODY_H_
#define BODY_H_

#include "..\math\xform.h"

#include "collisionshapes.h"

class CollisionShape;
class TiXmlElement;

class Body
{
public:
   static bool hasInfo(const TiXmlElement& element);

   Body();
   virtual ~Body();

   const Vector& getPosition() const;
   void          setPosition(const Vector& position);

   float getAngle() const;
   void  setAngle(float angle);

  // loading
   virtual void load(const TiXmlElement& element);

  // shapes
   void  addShape(CollisionShape* pshape);
   const CollisionShapes& getShapes() const;

  // integration
   virtual void integrate(float timestep) = 0;

  // space conversion
   Vector localToWorld(const Vector& vector) const;

protected:
   Vector   mPosition;
   float    mAngle;

private:
   void calculateDerivedData();

   XForm             mTransform;
   CollisionShapes   mShapes;
};

#ifdef JENGINE_INLINE
#  include "body.inl"
#endif

#endif
