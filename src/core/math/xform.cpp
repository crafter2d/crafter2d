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
#include "xform.h"

#include "matrix4.h"

XForm XForm::sIdentity;

// static 
XForm& XForm::identity()
{
   return sIdentity;
}

XForm::XForm():
   mPosition(),
   mAngle(0.0f)
{
}

XForm::XForm(const Vector& position, float angle):
   mPosition(position),
   mAngle(angle)
{
}

XForm::~XForm()
{
}

const XForm& XForm::operator=(const XForm& that)
{
   mPosition = that.mPosition;
   mAngle = that.mAngle;
   return *this;
}

XForm XForm::operator*(const XForm& that) const
{
   XForm result;

   result.setPosition(mPosition.x + that.mPosition.x, mPosition.y + that.mPosition.y);

   return result;
}

// - Get/set

const Vector& XForm::getPosition() const
{
   return mPosition;
}

void XForm::setPosition(const Vector& pos)
{
   mPosition = pos;
}

void XForm::setPosition(float x, float y)
{
   mPosition.set(x, y);
}
   
float XForm::getAngle() const
{
   return mAngle;
}

void XForm::setAngle(float angle)
{
   mAngle = angle;
}

void XForm::setIdentity()
{
   mPosition.set(0, 0);
   mAngle = 0.0f;
}

void XForm::set(const Vector& position, float angle)
{
   mPosition = position;
   mAngle = angle;
}

// - Conversion
   
void XForm::asMatrix(Matrix4& mat) const
{
   mat.translate(mPosition);
}

Vector XForm::transform(float x, float y) const
{
   float sin = sinf(mAngle);
   float cos = cosf(mAngle);

   // 3x3 Matrix
   // cos  -sin   p.x
   // sin   cos   p.y
   // 0    0      1

   return Vector(cos * x + -sin * y + mPosition.x,
                 sin * x +  cos * y + mPosition.y);
}

Vector XForm::transform(const Vector& pos) const
{
   float sin = sinf(mAngle);
   float cos = cosf(mAngle);

   // 3x3 Matrix
   // cos  -sin   p.x
   // sin   cos   p.y
   // 0    0      1

   return Vector(cos * pos.x + -sin * pos.y + mPosition.x,
                 sin * pos.x +  cos * pos.y + mPosition.y);
}
