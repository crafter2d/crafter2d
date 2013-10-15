
#include "d3ddevice.h"

#include "core/defines.h"

#include "texture/d3dtexture.h"
#include "texture/d3dtextureloaderdds.h"

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
   mpBlendState(NULL)
{
}

// - Get/set

void D3DDevice::setSwapChain(IDXGISwapChain1* pswapchain)
{
   mpSwapChain = pswapchain;
}

// - Operations

bool D3DDevice::create(int windowhandle, int width, int height)
{
   ID3D11Device* pdevice;
   ID3D11DeviceContext* pcontext;

   UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
   // If the project is in a debug build, enable debugging via SDK Layers with this flag.
   creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

   D3D_FEATURE_LEVEL level;
   D3D_FEATURE_LEVEL featureLevels[] = 
   {
	   D3D_FEATURE_LEVEL_11_1,
	   D3D_FEATURE_LEVEL_11_0,
	   D3D_FEATURE_LEVEL_10_1,
	   D3D_FEATURE_LEVEL_10_0,
	   D3D_FEATURE_LEVEL_9_3
   };

   HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, // Set set debug and Direct2D compatibility flags.
			featureLevels, // List of feature levels this app can support.
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION, // Always set this to D3D11_SDK_VERSION.
			&pdevice, // Returns the Direct3D device created.
			&level, // Returns feature level of device created.
			&pcontext // Returns the device immediate context.
			);

   if ( FAILED(hr) )
   {
      return false;
   }

   if ( FAILED(pdevice->QueryInterface(__uuidof(ID3D11Device1), (void**)&mpDevice))
     || FAILED(pcontext->QueryInterface(__uuidof(ID3D11DeviceContext1), (void**)&mpContext)) )
   {
      pdevice->Release();
      pcontext->Release();
      return false;
   }
   
   ASSERT(hasDeviceInfo());
   IDeviceInfo& info = getDeviceInfo();
   if ( info.needsCustomDevice() && !info.buildCustomDevice(*this) )
   {
      return false;
   }

   ID3D11Texture2D* pbackbuffer = NULL;

   // create & set the rendertarget view
   ASSERT_PTR(mpSwapChain);
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

Texture* D3DDevice::createTexture(const String& filename)
{
   D3DTextureLoaderDDS loader;
   D3DTexture* ptexture = loader.load(*this, filename);
   return ptexture;
}

BlendState* D3DDevice::createBlendState(const BlendStateDesc& desc)
{
   D3DBlendState* presult = new D3DBlendState();
   presult->create(*mpDevice, desc);
   return presult;
}

Font* D3DDevice::createFont(const String& name, int pointsize)
{
   return NULL;
}

} // namespace Graphics
