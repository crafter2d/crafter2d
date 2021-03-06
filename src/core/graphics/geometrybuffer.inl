
#include "core/defines.h"

#include <memory>

namespace Graphics
{
   // static 
   template <class T>
   GeometryBuffer<T>* GeometryBuffer<T>::createTriangleBuffer(Device& device, const String& effect, int elements)
   {
      std::unique_ptr<GeometryBuffer<T>> buffer(new GeometryBuffer<T>());
      if ( buffer->create(device, effect, elements, 3) )
      {
         return buffer.release();
      }
      return nullptr;
   }

   // static 
   template <class T>
   GeometryBuffer<T>* GeometryBuffer<T>::createRectangleBuffer(Device& device, const String& effect, int maxrects)
   {
      std::unique_ptr<GeometryBuffer<T>> buffer(new GeometryBuffer<T>());
      if ( buffer->create(device, effect, maxrects, 4) )
      {
         return buffer.release();
      }
      return nullptr;
   }

   // - Class implementation

   template <class T>
   GeometryBuffer<T>::GeometryBuffer() :
      AbstractGeometryBuffer(),
      mVertices(),
      mNeedsSync(false)
   {
   }

   // - Operations

   template <class T>
   void GeometryBuffer<T>::addVertex(const T& vertex)
   {
      addVertices(&vertex, 1);
   }

   template <class T>
   void GeometryBuffer<T>::addVertices(T* pvertices, int count)
   {
      for ( int index = 0; index < count; ++index )
      {
         mVertices.push_back(pvertices[index]);
      }

      mNeedsSync = true;
   }

   // - Drawing

   template <class T>
   void GeometryBuffer<T>::draw(RenderContext& context)
   {
      if ( mNeedsSync )
      {
         syncVertexBuffer(context, &mVertices[0], sizeof(T)* mVertices.size());
         mNeedsSync = false;
      }
   }
}
