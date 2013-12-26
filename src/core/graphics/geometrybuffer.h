
#ifndef GEOMETRY_BUFFER_H
#define GEOMETRY_BUFFER_H

#include <vector>

#include "abstractgeometrybuffer.h"

namespace Graphics
{
   template <class T>
   class GeometryBuffer : public AbstractGeometryBuffer
   {
   public:
      static GeometryBuffer<T>* createTriangleBuffer(Device& device, const String& effect, int elements);
      static GeometryBuffer<T>* createRectangleBuffer(Device& device, const String& effect, int elements);

      GeometryBuffer();

    // operations
      void addVertex(const T& vertex);
      void addVertices(T* pvertices, int count);

    // drawing
      virtual void draw(RenderContext& context) override;

   private:
      typedef std::vector<T> Vertices;

    // data
      Vertices mVertices;
      bool     mNeedsSync;
   };
}

#include "geometrybuffer.inl"

#endif // GEOMETRY_BUFFER_H
