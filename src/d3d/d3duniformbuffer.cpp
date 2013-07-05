
#include "d3duniformbuffer.h"

#include <d3d11.h>

#include "core/defines.h"

#include "d3ddevice.h"

#include "d3drendercontext.h"

namespace Graphics
{

   D3DUniformBuffer::D3DUniformBuffer():
      mpBuffer(NULL)
   {
   }

   bool D3DUniformBuffer::create(Device& device, int bytes)
   {
      D3DDevice& d3ddevice = static_cast<D3DDevice&>(device);

      D3D11_BUFFER_DESC constantBufferDesc = {0};
      constantBufferDesc.ByteWidth = bytes;
      constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
      constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
      constantBufferDesc.CPUAccessFlags = 0;
      constantBufferDesc.MiscFlags = 0;
      constantBufferDesc.StructureByteStride = 0;

      HRESULT hr = d3ddevice.getDevice().CreateBuffer(&constantBufferDesc, NULL, &mpBuffer);
      if ( FAILED(hr) )
         return false;

      return true;
   }

   void D3DUniformBuffer::set(RenderContext& context, const void* pdata)
   {
      D3DRenderContext& d3dcontext = static_cast<D3DRenderContext&>(context);
      d3dcontext.getContext().UpdateSubresource(mpBuffer, 0, NULL, pdata, 0, 0);
   }

   void D3DUniformBuffer::enable(RenderContext& context) const
   {
      D3DRenderContext& d3dcontext = static_cast<D3DRenderContext&>(context);
      d3dcontext.getContext().VSSetConstantBuffers(0, 1, &mpBuffer);
   }

} // namespace Graphics
