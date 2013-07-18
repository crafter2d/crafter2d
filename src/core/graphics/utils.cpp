
#include "utils.h"

#include <vector>

#include "device.h"
#include "indexbuffer.h"

namespace Graphics
{

namespace Utils
{

   IndexBuffer* createIndexBuffer(Device& device, int batchsize, int vertsPerElement, int indicesPerElement)
   {
      std::vector<short> indices;
      indices.reserve(batchsize * indicesPerElement);
      for ( int index = 0; index < batchsize * vertsPerElement; index += vertsPerElement )
      {
         indices.push_back(index);
         indices.push_back(index + 1);
         indices.push_back(index + 2);
         indices.push_back(index + 0);
         indices.push_back(index + 2);
         indices.push_back(index + 3);
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
