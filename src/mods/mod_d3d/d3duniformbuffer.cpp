
#include "d3duniformbuffer.h"

#include "core/defines.h"

#include "d3ddevice.h"
#include "d3dhelpers.h"
#include "d3drendercontext.h"

namespace Graphics
{

   D3DUniformBuffer::D3DUniformBuffer():
      mpBuffer(nullptr)
   {
   }

   D3DUniformBuffer::~D3DUniformBuffer()
   {
      release();
   }

   // - Get/set

   ID3D11Buffer* D3DUniformBuffer::getBuffer() const
   {
      return mpBuffer;
   }

   // - Operations

   bool D3DUniformBuffer::create(Device& device, UNIFORM_BUFFER_DESC* pdescs, int nr)
   {
      D3DDevice& d3ddevice = static_cast<D3DDevice&>(device);

      int bytes = determineSize(pdescs, nr);

      D3D11_BUFFER_DESC constantBufferDesc = {0};
      constantBufferDesc.ByteWidth = bytes;
      constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
      constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
      constantBufferDesc.CPUAccessFlags = 0;
      constantBufferDesc.MiscFlags = 0;
      constantBufferDesc.StructureByteStride = 0;

      HRESULT hr = d3ddevice.getDevice().CreateBuffer(&constantBufferDesc, nullptr, &mpBuffer);
      if ( FAILED(hr) )
         return false;

      return true;
   }

   void D3DUniformBuffer::release()
   {
      SafeRelease(&mpBuffer);
   }

   void D3DUniformBuffer::set(RenderContext& context, const void* pdata, const int size)
   {
      D3DRenderContext& d3dcontext = static_cast<D3DRenderContext&>(context);
      d3dcontext.getContext().UpdateSubresource(mpBuffer, 0, nullptr, pdata, 0, 0);
   }
   
   // - Helpers

   int D3DUniformBuffer::determineSize(UNIFORM_BUFFER_DESC* pdescs, int nr)
   {
      float result = 0;
      for ( int index = 0; index < nr; ++index )
      {
         UNIFORM_BUFFER_DESC& desc = pdescs[index];
         result += desc.size;
      }

      result = ceilf(result / 16.0f) * 16.0f;

      return (int) result;
   }

} // namespace Graphics
