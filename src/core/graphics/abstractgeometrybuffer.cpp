
#include "geometrybuffer.h"

#include "core/defines.h"

#include "device.h"
#include "effect.h"
#include "rendercontext.h"
#include "vertexbuffer.h"
#include "utils.h"

namespace Graphics
{
   AbstractGeometryBuffer::AbstractGeometryBuffer() :
      mpEffect(NULL),
      mpVB(NULL),
      mpIB(NULL)
   {
   }

   // - Operations
   
   bool AbstractGeometryBuffer::create(Device& device, const String& effect, int maxelements, int vertsperelement)
   {
      mpEffect = device.createEffect(effect);
      if ( mpEffect == NULL )
      {
         return false;
      }

      uint length = maxelements * vertsperelement;
      uint usage = VertexBuffer::eDynamic | VertexBuffer::eWriteOnly;
      mpVB = mpEffect->createVertexBuffer(device, length, usage);
      if ( mpVB == NULL )
      {
         return false;
      }

      mpIB = Utils::createIndexBuffer(device, maxelements, vertsperelement);
      if ( mpIB == NULL )
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
      std::memcpy(pvbdata, pdata, size);
      mpVB->unlock(context);
   }
}