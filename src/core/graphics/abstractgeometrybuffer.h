
#ifndef ABSTRACT_GEOMETRY_BUFFER_H
#define ABSTRACT_GEOMETRY_BUFFER_H

#include <cstdint>

#include "core/core_base.h"

class String;

namespace Graphics
{
   class Device;
   class Effect;
   class VertexBuffer;
   class IndexBuffer;
   class RenderContext;

   class CORE_API AbstractGeometryBuffer
   {
   public:
      AbstractGeometryBuffer();

    // operations
      bool create(Device& device, const String& effect, int maxvertices, int vertsperelement);

      virtual void draw(RenderContext& context) = 0;

   protected:

    // operations
      void syncVertexBuffer(RenderContext& context, void* pdata, uint32_t size);

   private:

    // data
      Effect* mpEffect;
      VertexBuffer* mpVB;
      IndexBuffer* mpIB;
   };
}

#endif // ABSTRACT_GEOMETRY_BUFFER_H
