
#include "utils.h"

#include <vector>

#include "core/defines.h"

#include "device.h"
#include "indexbuffer.h"

namespace Graphics
{

namespace Utils
{

   IndexBuffer* createIndexBuffer(Device& device, int batchsize, int vertsPerElement)
   {
      ASSERT(vertsPerElement > 3);

      int triangles = vertsPerElement - 2;
      int indicesPerElement = 3 + (vertsPerElement - 3) * 3;

      std::vector<short> indices;
      indices.reserve(batchsize * indicesPerElement);
      int v0 = 0;
      for ( int loop = 0; loop < batchsize; ++loop )
      {
         int offset = 1;
         for ( int index = 0; index < triangles; ++index )
         {
            indices.push_back(v0);
            indices.push_back(v0 + offset);
            indices.push_back(v0 + offset + 1);
            offset++;
         }

         v0 += vertsPerElement;
      }

      IndexBuffer* presult = device.createIndexBuffer();
      if ( presult == NULL || !presult->create(IndexBuffer::eShort, batchsize * indicesPerElement, &indices.front()) )
      {
         return NULL;
      }

      return presult;
   }

}
}
