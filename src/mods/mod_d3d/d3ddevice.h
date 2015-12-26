
#ifndef D3D_DEVICE_H
#define D3D_DEVICE_H

#include "core/graphics/device.h"

#include <d3d11.h>
#include <d2d1_1.h>
#include <dwrite_1.h>

#include "text/d3dfontcollection.h"
#include "d3drendercontext.h"

namespace Graphics
{
   class D3DDevice : public Device
   {
   public:
      D3DDevice();

    // get/set
      ID3D11Device& getDevice() { return *mpDevice; }

    // overloads
      virtual bool create(GameWindow& window) override;
      virtual void destroy() override;
      virtual void present() override;

      virtual CodePath*       createCodePath() override;
      virtual VertexBuffer*   createVertexBuffer() override;
      virtual IndexBuffer*    createIndexBuffer() override;
      virtual Texture*        createTexture(int width, int height, int bytesperpixel) override;
      virtual Texture*        createTexture(DataStream& imagedata) override;
      virtual RenderTarget*   createRenderTarget() override;
      virtual BlendState*     createBlendState(const BlendStateDesc& desc) override;
      virtual GlyphProvider*  createGlyphProvider(Font& font);
      
   protected:
    // overrides
      virtual RenderContext*  createRenderContext() override;
      virtual Font*           createFont(const String& name) override;

   private:

    // initialization
      bool createD2D();

    // data
      ID3D11Device*           mpDevice;
      ID3D11DeviceContext*    mpContext;
      IDXGISwapChain*         mpSwapChain;
      ID3D11RenderTargetView* mpRenderTargetView;

      ID2D1Factory1*          mpD2DFactory;
      ID2D1Device*            mpD2DDevice;
      ID2D1DeviceContext*     mpD2DContext;
      IDWriteFactory1*        mpDWriteFactory;

      D3DFontCollection*      mpFontCollection;
   };
}

#endif // D3D_DEVICE_H
