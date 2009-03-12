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
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <math.h>

class Color
{
public:
	Color();
	Color(float _r, float _g, float _b, float _a=1.0f);
	~Color();

	void set (float _r, float _g, float _b);

	float r, g, b, a;
};

class Vector
{
public:
   static Vector& zero();

	Vector(void);
	Vector(float _x, float _y);
   Vector(const Vector& v): x(v.x), y(v.y) {}
	~Vector(void);

	float normalize ();
	float length ();

	inline void set (float _x, float _y);
	inline float dot (const Vector& v);

   inline Vector& operator-();
	inline void operator= (const Vector& v);
	inline void operator*= (const float& f);
	inline void operator/= (const float& f);
	inline void operator+= (const Vector& v);
	inline void operator-= (const Vector& v);

	inline Vector operator+ (const Vector& v) const;
	inline Vector operator- (const Vector& v) const;
	inline Vector operator* (const float f) const;
   inline Vector operator/ (const float f) const;

   inline bool operator== (const Vector& v);
   inline bool operator!= (const Vector& v);

	float x, y;
};

class Vector3
{
public:
   float x,y,z;
};

inline float Vector::dot (const Vector& v) {
	return (x * v.x + y * v.y);
}

inline Vector& Vector::operator-()
{
   x = -x;
   y = -y;

   return *this;
}

inline void Vector::operator= (const Vector& v) {
	x = v.x; y = v.y;
}

inline void Vector::operator*= (const float& f) {
	x *= f; y *= f;
}

inline void Vector::operator/= (const float& f) {
	x /= f; y /= f;
}

inline void Vector::operator+= (const Vector& v) {
	x += v.x; y += v.y;
}

inline void Vector::operator-= (const Vector& v) {
	x -= v.x; y -= v.y;
}

inline void Vector::set (float _x, float _y) {
	x = _x; y = _y;
}

inline Vector Vector::operator+ (const Vector& v) const {
	return Vector (x+v.x, y+v.y);
}

inline Vector Vector::operator- (const Vector& v) const {
	return Vector (x-v.x, y-v.y);
}

inline Vector Vector::operator* (const float f) const {
	return Vector (x*f, y*f);
}

inline Vector Vector::operator/ (const float f) const {
	return Vector (x/f, y/f);
}

bool Vector::operator== (const Vector& v) {
   return ((fabs(v.x-x)<0.001) && (fabs(v.y-y)<0.001));
}

bool Vector::operator!= (const Vector& v)
{
   return !operator==(v);
}

#endif
