#pragma once

#include "core/graphics/device.h"

#include "mod_d3d12.h"

#include <dxgi1_4.h>
#include <d3d11_3.h>
#include <d2d1_3.h>
#include <dwrite_3.h>

class D3DFontCollection;

namespace Graphics
{

   class MOD_API D3D11Device : public Device
   {
   public:
      D3D11Device();

    // get/set
      ID3D11Device3& getDevice() { return *m_d3dDevice; }

      void set(ID3D11Device3* pd3dDevice, ID3D11DeviceContext3* pd3dContext, IDXGISwapChain3* pswapChain, float dpi);

      virtual bool create(GameWindow& window) override;
      virtual void destroy() override;
      virtual void present() override;
      virtual void resize(int width, int height) override;

      virtual CodePath*       createCodePath() override;
      virtual VertexBuffer*   createVertexBuffer() override;
      virtual IndexBuffer*    createIndexBuffer() override;
      virtual Texture*        createTexture(int width, int height, int bytesperpixel) override;
      virtual Texture*        createTexture(const TextureDescription& desc) override;
      virtual RenderTarget*   createRenderTarget() override;
      virtual BlendState*     createBlendState(const BlendStateDesc& desc) override;
      virtual GlyphProvider*  createGlyphProvider(Font& font) override;

   protected:
      virtual Font*           createFont(const String& name) override;
      virtual RenderContext*  createRenderContext() override;

   private:

    // helpers
      bool createWindowSizeDependendResources(int width, int height);

      void handleDeviceLost();

    // members
      ID3D11Device3*        m_d3dDevice;
      ID3D11DeviceContext3* m_d3dContext;
      IDXGISwapChain3*      m_swapChain;
      D3D_FEATURE_LEVEL     m_d3dFeatureLevel;

      ID3D11RenderTargetView1* m_d3dRenderTargetView;
      D3D11_VIEWPORT           m_screenViewport;

      // Direct2D drawing components.
      ID2D1Factory3*		   m_d2dFactory;
      ID2D1Device2*		   m_d2dDevice;
      ID2D1DeviceContext2*	m_d2dContext;
      ID2D1Bitmap1*		   m_d2dTargetBitmap;

      // DirectWrite drawing components.
      IDWriteFactory3*		m_dwriteFactory;
      D3DFontCollection*   mpFontCollection;

      float m_dpi;
   };
}
