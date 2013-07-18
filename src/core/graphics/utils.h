
#ifndef GRAPHICS_UTILS_H
#define GRAPHICS_UTILS_H

#include "core/core_base.h"

namespace Graphics
{
   class Device;
   class IndexBuffer;

   namespace Utils
   {
      CORE_API IndexBuffer* createIndexBuffer(Device& device, int batchsize, int vertsPerElement, int indicesPerElement);
   }
}

#endif // GRAPHICS_UTILS_H
