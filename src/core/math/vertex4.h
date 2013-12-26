
#ifndef VERTEX4_H
#define VERTEX4_H

struct Vertex4
{
   float x;
   float y;
   float z;
   float w;

   Vertex4();
   Vertex4(const Vertex4& other);

   void set(float _x, float _y, float _z, float _w);
};

inline 
Vertex4::Vertex4() :
   x(0),
   y(0),
   z(0),
   w(0)
{
}

inline
Vertex4::Vertex4(const Vertex4& other):
   x(other.x),
   y(other.y),
   z(other.z),
   w(other.w)
{
}

inline
void Vertex4::set(float _x, float _y, float _z, float _w)
{
   x = _x;
   y = _y;
   z = _z;
   w = _w;
}

#endif // VERTEX4_H
