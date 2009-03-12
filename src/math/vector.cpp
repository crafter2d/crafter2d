#include "../math/vector.h"
#include <math.h>

Color::Color()
{
	r = g = b = 0.0f;
	a = 1.0f;
}

Color::Color(float _r, float _g, float _b, float _a): r(_r), g(_g), b(_b), a(_a)
{
}

Color::~Color()
{
}

void Color::set (float _r, float _g, float _b) {
	r = _r; g = _g; b = _b;
}

Vector::Vector(void)
{
	x = y = 0.0f;
}

Vector& Vector::zero()
{
   static Vector _zero;
   return _zero;
}

Vector::Vector(float _x, float _y): x(_x), y(_y)
{
}

Vector::~Vector(void)
{
}

float Vector::length ()
{
	float len = dot (*this);
	return (float)sqrt (len);
}

float Vector::normalize ()
{
	float len = length ();
	if (len == 0)
		return 0;

	// normalize
	*this /= len;
	return len;
}
