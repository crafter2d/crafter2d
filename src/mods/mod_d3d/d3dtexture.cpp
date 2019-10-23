
#include "d3dtexture.h"

#include "d3ddevice.h"
#include "d3dhelpers.h"
#include "d3drendercontext.h"

namespace Graphics
{

   D3DTexture::D3DTexture(ID3D11ShaderResourceView* presource, ID3D11Resource* ptexture) :
      Texture(),
      mpTexture(ptexture),
      mpResource(presource),
      mpSampler(nullptr)
   {
   }

   D3DTexture::~D3DTexture()
   {
      release();
   }

   // - Operations

   bool D3DTexture::create(Device& device, int width, int height)
   {
      _width = width;
      _height = height;

      _sourceWidth = 1.0f;
      _sourceHeight = 1.0f;

      D3D11_SAMPLER_DESC sampDesc;
      ZeroMemory(&sampDesc, sizeof(sampDesc));

      sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
      sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
      sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
      sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
      sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
      sampDesc.MinLOD = 0;
      sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
   
      D3DDevice& d3ddevice = static_cast<D3DDevice&>(device);
      HRESULT hr = d3ddevice.getDevice().CreateSamplerState( &sampDesc, &mpSampler);
      return SUCCEEDED(hr);
   }

   void D3DTexture::release()
   {
      SafeRelease(&mpSampler);
      SafeRelease(&mpTexture);
      SafeRelease(&mpResource);
   }

   void D3DTexture::update(RenderContext& context, const void* pdata, int rowpitch)
   {
      ID3D11DeviceContext& d3dcontext = D3DRenderContext::asContext(context);
      d3dcontext.UpdateSubresource(mpTexture, D3D11CalcSubresource(0, 0, 1), nullptr, pdata, rowpitch, 0);
   }

   void D3DTexture::enable(RenderContext& context, int stage) const
   {
      ID3D11DeviceContext& d3dcontext = D3DRenderContext::asContext(context);
      d3dcontext.PSSetShaderResources(stage, 1, &mpResource);
      d3dcontext.PSSetSamplers(stage, 1, &mpSampler);
   }

   void D3DTexture::disable(RenderContext& context) const
   {
   }

   bool D3DTexture::isValid() const
   {
      return mpResource != nullptr;
   }

} // namespace Graphics
