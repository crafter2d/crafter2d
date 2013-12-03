
#include "d3ddevice.h"

#include "texture/d3dtexture.h"
#include "texture/d3dtextureloaderdds.h"

#include "d3dblendstate.h"
#include "d3dcodepath.h"
#include "d3dindexbuffer.h"
#include "d3drendercontext.h"
#include "d3dvertexbuffer.h"

namespace Graphics
{

D3DDevice::D3DDevice(ContentManager& contentmanager):
   Device(contentmanager),
   mpDevice(NULL),
   mpContext(NULL),
   mpSwapChain(NULL),
   mpRenderTargetView(NULL),
   mpBlendState(NULL)
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

   HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, 0, D3D11_SDK_VERSION,
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

Font* D3DDevice::createFont(const String& name, int pointsize)
{
   return NULL;
}

} // namespace Graphics
