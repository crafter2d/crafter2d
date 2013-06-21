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
#ifndef XFORM_H_
#define XFORM_H_

#include "core/core_base.h"

#include "vector.h"

class Matrix4;

class CORE_API XForm
{
public:
   static XForm& identity();

   XForm();
   XForm(const Vector& position, float angle);
   ~XForm();

   const XForm& operator=(const XForm& that);

   XForm operator*(const XForm& that) const;

 // get/set
   const Vector& getPosition() const;
   void          setPosition(const Vector& pos);
   void          setPosition(float x, float y);

   float         getAngle() const;
   void          setAngle(float angle);
   
   void setIdentity();
   void set(const Vector& position, float angle);

 // conversion
   void asMatrix(Matrix4& mat) const;
   void asOpenGL(float mat[16]) const;

private:
   static XForm sIdentity;

   Vector    mPosition;
   float     mAngle;
};

#endif
