
#include "d3d11device.h"

#include <algorithm>
#include <memory>

#include "core/window/gamewindow.h"

#include "d3dxhelpers.h"
#include "d3d11rendercontext.h"

#include "text/d3dfontcollection.h"
#include "text/d3dglyphprovider.h"
#include "text/d3dfont.h"


#include "d3dblendstate.h"
#include "d3dcodepath.h"
#include "d3dindexbuffer.h"
#include "d3dtexture.h"
#include "d3dvertexbuffer.h"
#include "d3duniformbuffer.h"

namespace Graphics
{
   D3D11Device::D3D11Device() :
      Device(),
      m_d3dFeatureLevel(),
      mpFontCollection(nullptr)
   {
   }

   void D3D11Device::set(ID3D11Device3* pd3dDevice, ID3D11DeviceContext3* pd3dContext, IDXGISwapChain3* pswapChain, float dpi)
   {
      m_d3dDevice = DX::SafeAcquire(pd3dDevice);
      m_d3dContext = DX::SafeAcquire(pd3dContext);
      m_swapChain = DX::SafeAcquire(pswapChain);
      m_dpi = dpi;
   }

   bool D3D11Device::create(GameWindow& window)
   {
      window.initDevice(*this);

      ASSERT_PTR(m_d3dDevice);
      ASSERT_PTR(m_d3dContext);
      ASSERT_PTR(m_swapChain);

      // create and set rasterizer
      D3D11_RASTERIZER_DESC desc;
      ZeroMemory(&desc, sizeof(desc));
      desc.FillMode = D3D11_FILL_SOLID;
      desc.CullMode = D3D11_CULL_NONE;
      desc.DepthClipEnable = FALSE;

      ID3D11RasterizerState* prasterizerstate;
      m_d3dDevice->CreateRasterizerState(&desc, &prasterizerstate);
      m_d3dContext->RSSetState(prasterizerstate);

      if ( !Device::create(window) )
      {
         return false;
      }

      // Initialize Direct2D resources.
      D2D1_FACTORY_OPTIONS options;
      ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
      // If the project is in a debug build, enable Direct2D debugging via SDK Layers.
      options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

      // Initialize the Direct2D Factory.
      HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3), &options, reinterpret_cast<void**>(&m_d2dFactory));
      if ( FAILED(hr) )
      {
         return false;
      }

      // Create the Direct2D device object and a corresponding context.
      IDXGIDevice* dxgiDevice;
      m_d3dDevice->QueryInterface<IDXGIDevice>(&dxgiDevice);
      hr = m_d2dFactory->CreateDevice(dxgiDevice, &m_d2dDevice);
      if ( FAILED(hr) )
      {
         return false;
      }

      hr = m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2dContext);
      if ( FAILED(hr) )
      {
         return false;
      }

      // Initialize the DirectWrite Factory.
      hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory3), reinterpret_cast<IUnknown**>(&m_dwriteFactory));
      if ( FAILED(hr) )
      {
         return false;
      }

      mpFontCollection = new D3DFontCollection();
      if ( !mpFontCollection->initialize(m_dwriteFactory) )
      {
         return false;
      }      
      
      return createWindowSizeDependendResources(window.getWidth(), window.getHeight());
   }

   bool D3D11Device::createWindowSizeDependendResources(int width, int height)
   {
      D3D11RenderContext& context = static_cast<D3D11RenderContext&>(getContext());
      context.release();
      
      // Clear the previous window size specific context.
      ID3D11RenderTargetView* nullViews[] = { nullptr };
      m_d3dContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
      DX::SafeRelease(&m_d3dRenderTargetView);
      m_d2dContext->SetTarget(nullptr);
      DX::SafeRelease(&m_d2dTargetBitmap);
      m_d3dContext->Flush1(D3D11_CONTEXT_TYPE_ALL, nullptr);

      ASSERT(m_swapChain != nullptr);
      HRESULT hr = m_swapChain->ResizeBuffers(2, width, height, DXGI_FORMAT_B8G8R8A8_UNORM, 0);
      if ( FAILED(hr) )
      {
         return false;
      }

      // create & set the rendertarget view
      ID3D11Texture2D* pbackbuffer = NULL;
      hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*) & pbackbuffer);
      if ( FAILED(hr) )
      {
         return false;
      }
      m_d3dDevice->CreateRenderTargetView1(pbackbuffer, NULL, &m_d3dRenderTargetView);
      
      DX::SafeRelease(&pbackbuffer);

      ID3D11RenderTargetView* const targets[1] = { m_d3dRenderTargetView };
      m_d3dContext->OMSetRenderTargets(1, targets, NULL);

      // Set the 3D rendering viewport to target the entire window.
      m_screenViewport = CD3D11_VIEWPORT(0.0f, 0.0f, width, height);
      m_d3dContext->RSSetViewports(1, &m_screenViewport);

      // Create a Direct2D target bitmap associated with the
      // swap chain back buffer and set it as the current target.
      D2D1_BITMAP_PROPERTIES1 bitmapProperties =
         D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
            m_dpi,
            m_dpi
            );

      IDXGISurface2* dxgiBackBuffer;
      m_swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));

      hr = m_d2dContext->CreateBitmapFromDxgiSurface(
         dxgiBackBuffer,
         &bitmapProperties,
         &m_d2dTargetBitmap
         );
      if ( FAILED(hr) )
      {
         return false;
      }

      DX::SafeRelease(&dxgiBackBuffer);

      m_d2dContext->SetTarget(m_d2dTargetBitmap);
      m_d2dContext->SetDpi(m_dpi, m_dpi);

      // Grayscale text anti-aliasing is recommended for all Windows Store apps.
      m_d2dContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

      context.setTargetView(m_d3dRenderTargetView);

      return true;
   }

   void D3D11Device::destroy()
   {
   }

   void D3D11Device::present()
   {
      // The first argument instructs DXGI to block until VSync, putting the application
      // to sleep until the next VSync. This ensures we don't waste any cycles rendering
      // frames that will never be displayed to the screen.
      DXGI_PRESENT_PARAMETERS parameters = { 0 };
      HRESULT hr = m_swapChain->Present1(1, 0, &parameters);

      // Discard the contents of the render target.
      // This is a valid operation only when the existing contents will be entirely
      // overwritten. If dirty or scroll rects are used, this call should be removed.
      m_d3dContext->DiscardView1(m_d3dRenderTargetView, nullptr, 0);
      
      // If the device was removed either by a disconnection or a driver upgrade, we 
      // must recreate all device resources.
      if ( hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET )
      {
         handleDeviceLost();
      }
      else
      {
         //DX::ThrowIfFailed(hr);
      }
   }

   void D3D11Device::resize(int width, int height)
   {
      createWindowSizeDependendResources(width, height);
   }

   CodePath* D3D11Device::createCodePath()
   {
      return new D3DCodePath(*this);
   }

   VertexBuffer* D3D11Device::createVertexBuffer()
   {
      return new D3DVertexBuffer(*this);
   }

   IndexBuffer* D3D11Device::createIndexBuffer()
   {
      return new D3DIndexBuffer(*this);
   }

   Texture* D3D11Device::createTexture(int width, int height, int bytesperpixel)
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
      m_d3dDevice->CreateTexture2D(&desc, NULL, &ptexture);

      // Create the shader-resource view
      D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
      srDesc.Format = desc.Format;
      srDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
      srDesc.Texture2D.MostDetailedMip = 0;
      srDesc.Texture2D.MipLevels = 1;

      ID3D11ShaderResourceView *pShaderResView = NULL;
      m_d3dDevice->CreateShaderResourceView(ptexture, &srDesc, &pShaderResView);

      D3DTexture* presult = new D3DTexture(pShaderResView, ptexture);
      presult->create(*this, width, height);
      return presult;
   }


   Texture* D3D11Device::createTexture(const TextureDescription& texdesc)
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

      ID3D11Texture2D *ptexture = NULL;
      HRESULT hr = m_d3dDevice->CreateTexture2D(&desc, initData.get(), &ptexture);
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

      ID3D11ShaderResourceView *pShaderResView = NULL;
      hr = m_d3dDevice->CreateShaderResourceView(ptexture, &srDesc, &pShaderResView);
      if ( FAILED(hr) )
      {
         ptexture->Release();
         return nullptr;
      }

      D3DTexture* presult = new D3DTexture(pShaderResView, ptexture);
      presult->create(*this, texdesc.width, texdesc.height);
      return presult;
   }

   RenderTarget* D3D11Device::createRenderTarget()
   {
      return NULL;
   }

   BlendState* D3D11Device::createBlendState(const BlendStateDesc& desc)
   {
      D3DBlendState* presult = new D3DBlendState();
      presult->create(getDevice(), desc);
      return presult;
   }

   GlyphProvider* D3D11Device::createGlyphProvider(Font& font)
   {
      D3DGlyphProvider* pprovider = new D3DGlyphProvider(m_d2dFactory, m_d2dContext, m_dwriteFactory);
      pprovider->initialize(mpFontCollection->getCustomFontCollection(), font);
      return pprovider;
   }

   Font* D3D11Device::createFont(const String& name)
   {
      return mpFontCollection->createFont(name);
   }

   RenderContext* D3D11Device::createRenderContext()
   {
      return new D3D11RenderContext(m_d3dContext, m_d3dRenderTargetView, m_dpi);
   }

   // Helpers

   // Recreate all device resources and set them back to the current state.
   void D3D11Device::handleDeviceLost()
   {
      //m_swapChain = nullptr;

      /*
      if ( m_deviceNotify != nullptr )
      {
         m_deviceNotify->OnDeviceLost();
      }
      */

      //createDeviceResources();
      //->SetDpi(m_dpi, m_dpi);
      //createWindowSizeDependentResources();

      /*
      if ( m_deviceNotify != nullptr )
      {
         m_deviceNotify->OnDeviceRestored();
      }
      */
   }
}