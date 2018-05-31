
#include "d3dblendstate.h"

#include <d3d11.h>

#include "core/defines.h"

namespace Graphics
{
   D3DBlendState::D3DBlendState():
      mpBlendState(nullptr)
   {
   }

   bool D3DBlendState::create(ID3D11Device& device, const BlendStateDesc& desc)
   {
      D3D11_BLEND_DESC blendDesc;
      ZeroMemory(&blendDesc, sizeof(blendDesc));
      blendDesc.AlphaToCoverageEnable = false;
      blendDesc.IndependentBlendEnable = false;
      blendDesc.RenderTarget[0].BlendEnable = true;
      blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
      blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
      blendDesc.RenderTarget[0].SrcBlend = toD3DBlend(desc.getSourceFactor());
      blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
      blendDesc.RenderTarget[0].DestBlend = toD3DBlend(desc.getDestFactor());
      blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
      blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

      return SUCCEEDED(device.CreateBlendState(&blendDesc, &mpBlendState));
   }

   void D3DBlendState::enable(ID3D11DeviceContext& context) const
   {
      context.OMSetBlendState(mpBlendState, nullptr, 0xffffffff);
   }

   // - Helpers

   D3D11_BLEND D3DBlendState::toD3DBlend(BlendStateDesc::BlendFactor factor)
   {
      switch ( factor )
      {
         case BlendStateDesc::BS_ONE:
            return D3D11_BLEND_ONE;
         case BlendStateDesc::BS_ZERO:
            return D3D11_BLEND_ZERO;

         case BlendStateDesc::BS_SRC_COLOR:
            return D3D11_BLEND_SRC_COLOR;
         case BlendStateDesc::BS_SRC_ALPHA:
            return D3D11_BLEND_SRC_ALPHA;
         case BlendStateDesc::BS_SRC_INV_COLOR:
            return D3D11_BLEND_INV_SRC_COLOR;
         case BlendStateDesc::BS_SRC_INV_ALPHA:
            return D3D11_BLEND_INV_SRC_ALPHA;
         
         case BlendStateDesc::BS_DST_COLOR:
            return D3D11_BLEND_DEST_COLOR;
         case BlendStateDesc::BS_DST_ALPHA:
            return D3D11_BLEND_DEST_ALPHA;
         case BlendStateDesc::BS_DST_INV_COLOR:
            return D3D11_BLEND_INV_DEST_COLOR;
         case BlendStateDesc::BS_DST_INV_ALPHA:
            return D3D11_BLEND_INV_DEST_ALPHA;
      }

      UNREACHABLE("Wrong blend state!");
      return D3D11_BLEND_ZERO;
   }
   
} // namespace Graphics
