
#ifndef UNIFORM_BUFFER_H
#define UNIFORM_BUFFER_H

#include "core/string/string.h"

namespace Graphics
{
   class Device;
   class RenderContext;

   struct UNIFORM_BUFFER_DESC
   {
      String name;
      int    size;
   };

   class UniformBuffer
   {
   public:

      /// create the buffer
      virtual bool create(Device& device, UNIFORM_BUFFER_DESC* pdescs, int nr) = 0;

      virtual void release() = 0;

      /// set the data into the uniform buffer
      virtual void set(RenderContext& context, const void* pdata) = 0;

      /// enable the uniform buffer for rendering
      virtual void enable(RenderContext& context) const = 0;
   };
}

#endif // UNIFORM_BUFFER_H
