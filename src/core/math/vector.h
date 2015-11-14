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
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "core/core_base.h"

#include <math.h>

class DataStream;

class CORE_API Vector
{
public:
   static Vector& zero();

   Vector() :
      x(0),
      y(0)
   {
   }

   Vector(float _x, float _y) :
      x(_x),
      y(_y)
   {
   }

   Vector(const Vector& v):
      x(v.x),
      y(v.y) 
   {
   }

   ~Vector() {
   }

   bool operator== (const Vector& v) const {
      return (fabs(v.x - x) < 0.001) && (fabs(v.y - y) < 0.001);
   }
   
   bool operator!= (const Vector& v) const {
      return (fabs(v.x - x) >= 0.001) && (fabs(v.y - y) >= 0.001);
   }

 // - Operations
   
   float normalize()
   {
      float len = length();
      if ( len == 0 )
         return 0;

      // normalize
      *this /= len;
      return len;
   }

	float length()
   {
      float len = dot(*this);
      return (float) sqrt(len);
   }

   float distance(const Vector& that) const
   {
      float nx = that.x - x;
      float ny = that.y - y;

      return (float) sqrt((nx * nx) + (ny * ny));
   }

   void set(float _x, float _y) {
      x = _x;
      y = _y;
   }

   void addScaled(const Vector& v, float scale) {
      x += v.x * scale;
      y += v.y * scale;
   }
	
   float dot (const Vector& v) const {
      return (x * v.x + y * v.y);
   }

   float cross(const Vector& v) const {
      return (x * v.y) - (y * v.x);
   }

 // Operators

   Vector operator-() {
      return Vector(-x, -y);
   }

   Vector& operator= (const Vector& v) {
      x = v.x;
      y = v.y;
      return *this;
   }

   __forceinline
   void operator*= (float f) {
      x *= f;
      y *= f;
   }

   void operator/= (float f) {
      x /= f;
      y /= f;
   }

   void operator+= (const Vector& v) {
      x += v.x;
      y += v.y;
   }

   void operator-= (const Vector& v) {
      x -= v.x;
      y -= v.y;
   }

   Vector operator+ (const Vector& v) const {
      return Vector(x + v.x, y + v.y);
   }

   Vector operator- (const Vector& v) const {
      return Vector(x - v.x, y - v.y);
   }

   __forceinline
   Vector operator* (const Vector& v) const {
      return Vector(x * v.x, y * v.y);
   }

   __forceinline
   Vector operator* (const float f) const {
      return Vector(x * f, y * f);
   }

   Vector operator/ (const float f) const {
      return Vector(x / f, y / f);
   }   

   friend Vector operator*(float f, const Vector& v) {
      return Vector(v.x * f, v.y * f);
   }

   friend CORE_API DataStream& operator<<(DataStream& stream, const Vector& v);
   friend CORE_API DataStream& operator>>(DataStream& stream, Vector& v);

	float x, y;
};

#endif
