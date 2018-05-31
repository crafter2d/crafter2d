
#include "geometrybuffer.h"

#include <string.h>

#include "core/defines.h"

#include "device.h"
#include "effect.h"
#include "rendercontext.h"
#include "vertexbuffer.h"
#include "utils.h"

namespace Graphics
{
   AbstractGeometryBuffer::AbstractGeometryBuffer() :
      mpEffect(nullptr),
      mpVB(nullptr),
      mpIB(nullptr)
   {
   }

   // - Operations
   
   bool AbstractGeometryBuffer::create(Device& device, const String& effect, int maxelements, int vertsperelement)
   {
      mpEffect = device.createEffect(effect);
      if ( mpEffect == nullptr )
      {
         return false;
      }

      int length = maxelements * vertsperelement;
      int usage = VertexBuffer::eDynamic | VertexBuffer::eWriteOnly;
      mpVB = mpEffect->createVertexBuffer(device, length, usage);
      if ( mpVB == nullptr )
      {
         return false;
      }

      mpIB = Utils::createIndexBuffer(device, maxelements, vertsperelement);
      if ( mpIB == nullptr )
      {
         return false;
      }

      return true;
   }

   void AbstractGeometryBuffer::draw(RenderContext& context)
   {
      mpEffect->enable(context);

      context.setVertexBuffer(*mpVB);
      context.setIndexBuffer(*mpIB);
      
      context.drawTriangles(0, 0);
   }

   void AbstractGeometryBuffer::syncVertexBuffer(RenderContext& context, void* pdata, uint32_t size)
   {
      // if ( mVertices.size() > mpVB->size() ) does not exist yet :(
      void* pvbdata = mpVB->lock(context);
      memcpy(pvbdata, pdata, size);
      mpVB->unlock(context);
   }
}