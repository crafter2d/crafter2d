
#ifndef VERTEX2_H
#define VERTEX2_H

struct Vertex
{
   float x;
   float y;

   Vertex();
   Vertex(const Vertex& that);
   Vertex(float _x, float _y);

   void set(float _x, float _y);
};

inline
Vertex::Vertex():
   x(0),
   y(0)
{
}

inline
Vertex::Vertex(const Vertex& that):
   x(that.x),
   y(that.y)
{
}

inline
Vertex::Vertex(float _x, float _y):
   x(_x),
   y(_y)
{
}

inline
void Vertex::set(float _x, float _y)
{
   x = _x;
   y = _y;
}

#endif // VERTEX2_H
