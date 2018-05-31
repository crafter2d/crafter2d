
#include "d3ddevice.h"

#include <algorithm>
#include <memory>

#include "core/window/gamewindow.h"

#include "text/d3dfontcollection.h"
#include "text/d3dglyphprovider.h"
#include "text/d3dfont.h"

#include "d3dblendstate.h"
#include "d3dcodepath.h"
#include "d3dhelpers.h"
#include "d3dindexbuffer.h"
#include "d3dtexture.h"
#include "d3drendercontext.h"
#include "d3dvertexbuffer.h"

namespace Graphics
{

D3DDevice::D3DDevice():
   Device(),
   mpDevice(nullptr),
   mpContext(nullptr),
   mpSwapChain(nullptr),
   mpRenderTargetView(nullptr),
   mpD2DFactory(nullptr),
   mpD2DDevice(nullptr),
   mpD2DContext(nullptr),
   mpDWriteFactory(nullptr)
{
}

bool D3DDevice::create(GameWindow& window)
{
   int windowhandle = window.getHandle();

   D3D_FEATURE_LEVEL level;
   DXGI_SWAP_CHAIN_DESC sd;
   ZeroMemory(&sd, sizeof(sd));
   sd.BufferCount = 1;
   sd.BufferDesc.Width = window.getWidth();
   sd.BufferDesc.Height = window.getHeight();
   sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
   sd.BufferDesc.RefreshRate.Numerator = 60;
   sd.BufferDesc.RefreshRate.Denominator = 1;
   sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
   sd.OutputWindow = (HWND)windowhandle;
   sd.SampleDesc.Count = 1;
   sd.SampleDesc.Quality = 0;
   sd.Windowed = true;

   UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
   // If the project is in a debug build, enable debugging via SDK Layers with this flag.
   //creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

   D3D_FEATURE_LEVEL requestedlevel[] = {
      D3D_FEATURE_LEVEL_11_1,
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
   };

   HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, nullptr, 
                                              creationFlags, requestedlevel, 3, D3D11_SDK_VERSION,
                                              &sd, &mpSwapChain, &mpDevice, &level, &mpContext);
   if ( FAILED(hr) )
   {
      return false;
   }

   ID3D11Texture2D* pbackbuffer = nullptr;

   // create & set the rendertarget view
   hr = mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pbackbuffer);
   if ( FAILED(hr) )
   {
      return false;
   }
   mpDevice->CreateRenderTargetView(pbackbuffer, nullptr, &mpRenderTargetView);
   pbackbuffer->Release();

   D3D11_RASTERIZER_DESC desc;
   ZeroMemory(&desc, sizeof(desc));
   desc.FillMode = D3D11_FILL_SOLID;
   desc.CullMode = D3D11_CULL_NONE;
   desc.DepthClipEnable = FALSE;

   ID3D11RasterizerState* prasterizerstate;
   mpDevice->CreateRasterizerState(&desc, &prasterizerstate);
   mpContext->RSSetState(prasterizerstate);

   // set the buffer view
   ID3D11RenderTargetView* const targets[1] = { mpRenderTargetView };
   mpContext->OMSetRenderTargets(1, targets, nullptr);

   // set the viewport
   CD3D11_VIEWPORT viewport(pbackbuffer, mpRenderTargetView);
   mpContext->RSSetViewports(1, &viewport);
   
   return createD2D()
       && Device::create(window);
}

bool D3DDevice::createD2D()
{
   HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &mpD2DFactory);
   if ( FAILED(hr) )
   {
      return false;
   }

   IDXGIDevice* dxgiDevice;
   mpDevice->QueryInterface<IDXGIDevice>(&dxgiDevice);
   hr = mpD2DFactory->CreateDevice(dxgiDevice, &mpD2DDevice);
   dxgiDevice->Release();
   if ( FAILED(hr) )
   {
      return false;
   }

   hr = mpD2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &mpD2DContext);
   if (  FAILED(hr) )
   {
      return false;
   }

   hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory1), reinterpret_cast<IUnknown**>(&mpDWriteFactory));
   if ( FAILED(hr) )
   {
      return false;
   }

   mpFontCollection = new D3DFontCollection();
   if ( !mpFontCollection->initialize(mpDWriteFactory) )
   {
      return false;
   }

   return true;
}

void D3DDevice::destroy()
{
   Device::destroy();

   delete mpFontCollection;
   
   SafeRelease(&mpDWriteFactory);
   SafeRelease(&mpD2DContext);
   SafeRelease(&mpD2DDevice);
   SafeRelease(&mpD2DFactory);
   SafeRelease(&mpSwapChain);
   SafeRelease(&mpRenderTargetView);
   SafeRelease(&mpContext);
   SafeRelease(&mpDevice);
}

void D3DDevice::present()
{
   // The first argument instructs DXGI to block until VSync, putting the application
   // to sleep until the next VSync. This ensures we don't waste any cycles rendering
   // frames that will never be displayed to the screen.
   HRESULT hr = mpSwapChain->Present(0, 0);

   if ( FAILED(hr) )
   {
      // error!
   }
}

void D3DDevice::resize(int width, int height)
{
}

RenderContext* D3DDevice::createRenderContext()
{
   return new D3DRenderContext(mpContext, mpRenderTargetView);
}

CodePath* D3DDevice::createCodePath()
{
   return new D3DCodePath(*this);
}

VertexBuffer* D3DDevice::createVertexBuffer()
{
   return new D3DVertexBuffer(*this);
}

IndexBuffer* D3DDevice::createIndexBuffer()
{
   return new D3DIndexBuffer(*this);
}

RenderTarget* D3DDevice::createRenderTarget()
{
   return nullptr;
}

Texture* D3DDevice::createTexture(int width, int height, int bytesperpixel)
{
   DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
   switch ( bytesperpixel )
   {
   case 1:
      format = DXGI_FORMAT_R8_UNORM;
      break;
   case 2:
      format = DXGI_FORMAT_R8G8_UNORM;
      break;
   case 3:
      return nullptr; // not supported
   }

   // Create the render target texture
   D3D11_TEXTURE2D_DESC desc;
   ZeroMemory(&desc, sizeof(desc));
   desc.Width = width;
   desc.Height = height;
   desc.MipLevels = 1;
   desc.ArraySize = 1;
   desc.Format = format;
   desc.SampleDesc.Count = 1;
   desc.Usage = D3D11_USAGE_DEFAULT;
   desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
   
   ID3D11Texture2D *ptexture = nullptr;
   HRESULT hr = mpDevice->CreateTexture2D(&desc, nullptr, &ptexture);
   if ( FAILED(hr) )
   {
      return nullptr;
   }
   
   // Create the shader-resource view
   D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
   memset(&srDesc, 0, sizeof(srDesc));
   srDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
   srDesc.Format = format;
   srDesc.Texture2D.MostDetailedMip = 0;
   srDesc.Texture2D.MipLevels = 1;

   ID3D11ShaderResourceView *pShaderResView = nullptr;
   hr = mpDevice->CreateShaderResourceView(ptexture, &srDesc, &pShaderResView);
   if ( FAILED(hr) )
   {
      ptexture->Release();
      return nullptr;
   }
   
   D3DTexture* presult = new D3DTexture(pShaderResView, ptexture);
   presult->create(*this, width, height);
   return presult;
}

Texture* D3DDevice::createTexture(const TextureDescription& texdesc)
{
   bool bc = false;
   DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
   int blockSize = 0;

   switch ( texdesc.format )
   {
   case eFormat_Luminance:
      format = DXGI_FORMAT_R8_UNORM;
      blockSize = 1;
      break;
   case eFormat_RG:
      format = DXGI_FORMAT_R8G8_UNORM;
      blockSize = 2;
      break;
   case eFormat_RGBA:
      format = DXGI_FORMAT_R8G8B8A8_UNORM;
      blockSize = 4;
      break;
   case eFormat_BC1:
      bc = true;
      format = DXGI_FORMAT_BC1_UNORM;
      blockSize = 8;
      break;
   case eFormat_BC2:
      bc = true;
      format = DXGI_FORMAT_BC2_UNORM;
      blockSize = 16;
      break;
   case eFormat_BC3:
      bc = true;
      format = DXGI_FORMAT_BC3_UNORM;
      blockSize = 16;
      break;
   }

   std::unique_ptr<D3D11_SUBRESOURCE_DATA> initData;
   if ( texdesc.pinitData != nullptr )
   {
      initData.reset(new D3D11_SUBRESOURCE_DATA);
      initData->pSysMem = texdesc.pinitData;
      if ( bc )
      {
         size_t numBlocksWide = std::max<size_t>(1, (texdesc.width + 3) / 4);
         initData->SysMemPitch = numBlocksWide * blockSize;
      }
      else
      {
         initData->SysMemPitch = texdesc.width * blockSize;
      }
   }

   // Create the render target texture
   D3D11_TEXTURE2D_DESC desc;
   ZeroMemory(&desc, sizeof(desc));
   desc.Width = texdesc.width;
   desc.Height = texdesc.height;
   desc.MipLevels = 1;
   desc.ArraySize = 1;
   desc.Format = format;
   desc.SampleDesc.Count = 1;
   desc.Usage = D3D11_USAGE_DEFAULT;
   desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

   ID3D11Texture2D *ptexture = nullptr;
   HRESULT hr = mpDevice->CreateTexture2D(&desc, initData.get(), &ptexture);
   if ( FAILED(hr) )
   {
      return nullptr;
   }

   // Create the shader-resource view
   D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
   memset(&srDesc, 0, sizeof(srDesc));
   srDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
   srDesc.Format = format;
   srDesc.Texture2D.MostDetailedMip = 0;
   srDesc.Texture2D.MipLevels = 1;

   ID3D11ShaderResourceView *pShaderResView = nullptr;
   hr = mpDevice->CreateShaderResourceView(ptexture, &srDesc, &pShaderResView);
   if ( FAILED(hr) )
   {
      ptexture->Release();
      return nullptr;
   }

   D3DTexture* presult = new D3DTexture(pShaderResView, ptexture);
   presult->create(*this, texdesc.width, texdesc.height);
   return presult;
}

BlendState* D3DDevice::createBlendState(const BlendStateDesc& desc)
{
   D3DBlendState* presult = new D3DBlendState();
   presult->create(*mpDevice, desc);
   return presult;
}

Font* D3DDevice::createFont(const String& name)
{
   D3DGlyphProvider* pprovider = new D3DGlyphProvider(mpD2DContext, mpDWriteFactory, getContext().getTextRenderer().getGlyphAtlas());
   return mpFontCollection->createFont(name, pprovider);

   
   //pprovider->initialize(mpFontCollection->getCustomFontCollection(), *font);
   //font->setGlyphProvider(pprovider);
   //return font.release();
}

} // namespace Graphics
