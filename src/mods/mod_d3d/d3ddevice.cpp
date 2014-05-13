
#include "d3ddevice.h"

#include "core/smartptr/autoptr.h"
#include "core/graphics/textureinfo.h"

#include "texture/d3dtexture.h"
#include "texture/d3dtextureloaderdds.h"

#include "text/d3dfontcollection.h"
#include "text/d3dglyphprovider.h"
#include "text/d3dfont.h"

#include "d3dblendstate.h"
#include "d3dcodepath.h"
#include "d3dindexbuffer.h"
#include "d3drendercontext.h"
#include "d3dvertexbuffer.h"

namespace Graphics
{

D3DDevice::D3DDevice():
   Device(),
   mpDevice(NULL),
   mpContext(NULL),
   mpSwapChain(NULL),
   mpRenderTargetView(NULL),
   mpBlendState(NULL),
   mpD2DFactory(NULL),
   mpD2DDevice(NULL),
   mpD2DContext(NULL),
   mpDWriteFactory(NULL)
{
}

bool D3DDevice::create(int windowhandle, int width, int height)
{
   D3D_FEATURE_LEVEL level;
   DXGI_SWAP_CHAIN_DESC sd;
   ZeroMemory(&sd, sizeof(sd));
   sd.BufferCount = 1;
   sd.BufferDesc.Width = width;
   sd.BufferDesc.Height = height;
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
   creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

   D3D_FEATURE_LEVEL requestedlevel[] = {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
   };

   HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, NULL, 
                                              creationFlags, requestedlevel, 3, D3D11_SDK_VERSION,
                                              &sd, &mpSwapChain, &mpDevice, &level, &mpContext);
   if ( FAILED(hr) )
   {
      return false;
   }

   ID3D11Texture2D* pbackbuffer = NULL;

   // create & set the rendertarget view
   hr = mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pbackbuffer);
   if ( FAILED(hr) )
   {
      return false;
   }
   mpDevice->CreateRenderTargetView(pbackbuffer, NULL, &mpRenderTargetView);
   pbackbuffer->Release();

   // set the buffer view
   mpContext->OMSetRenderTargets(1, &mpRenderTargetView, NULL);

   // set the viewport
   CD3D11_VIEWPORT viewport(pbackbuffer, mpRenderTargetView);
   mpContext->RSSetViewports(1, &viewport);
   
   return createD2D(windowhandle, width, height)
       && Device::create(windowhandle, width, height);
}

bool D3DDevice::createD2D(int windowhandle, int width, int height)
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
   return NULL;
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
      return NULL; // not supported
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

   ID3D11Texture2D *ptexture = NULL;
   mpDevice->CreateTexture2D(&desc, NULL, &ptexture);
   
   // Create the shader-resource view
   D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
   srDesc.Format = desc.Format;
   srDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
   srDesc.Texture2D.MostDetailedMip = 0;
   srDesc.Texture2D.MipLevels = 1;

   ID3D11ShaderResourceView *pShaderResView = NULL;
   mpDevice->CreateShaderResourceView(ptexture, &srDesc, &pShaderResView);

   TextureInfo info;
   info.setWidth(width);
   info.setHeight(height);

   D3DTexture* presult = new D3DTexture(pShaderResView, ptexture);
   presult->create(*this, info);
   return presult;
}

Texture* D3DDevice::createTexture(DataStream& imagedata)
{
   D3DTextureLoaderDDS loader;
   D3DTexture* ptexture = loader.load(*this, imagedata);
   return ptexture;
}

BlendState* D3DDevice::createBlendState(const BlendStateDesc& desc)
{
   D3DBlendState* presult = new D3DBlendState();
   presult->create(*mpDevice, desc);
   return presult;
}

GlyphProvider* D3DDevice::createGlyphProvider(Font& font)
{
   D3DGlyphProvider* pprovider = new D3DGlyphProvider(mpD2DContext, mpDWriteFactory);
   pprovider->initialize(mpFontCollection->getCustomFontCollection(), font);
   return pprovider;
}

Font* D3DDevice::createFont(const String& name)
{
   return mpFontCollection->createFont(name);
}

} // namespace Graphics
