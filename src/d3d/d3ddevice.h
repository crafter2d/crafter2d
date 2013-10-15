
#ifndef D3D_DEVICE_H
#define D3D_DEVICE_H

#include "core/graphics/device.h"
#include "d3d_base.h"

#include <d3d11_1.h>

#include "d3drendercontext.h"

namespace Graphics
{
   class D3D_API D3DDevice : public Device
   {
   public:
      D3DDevice();

    // get/set
      ID3D11Device1& getDevice() { return *mpDevice; }

      void setSwapChain(IDXGISwapChain1* pswapchain);

    // overloads
      virtual bool create(int windowhandle, int width, int height);

      virtual void present();

      virtual RenderContext*  createRenderContext();
      virtual CodePath*       createCodePath();
      virtual VertexBuffer*   createVertexBuffer();
      virtual IndexBuffer*    createIndexBuffer();
      virtual Texture*        createTexture(const String& filename);
      virtual BlendState*     createBlendState(const BlendStateDesc& desc);
      virtual Font*           createFont(const String& name, int pointsize);

   private:

    // data
      ID3D11Device1*           mpDevice;
      ID3D11DeviceContext1*    mpContext;
      IDXGISwapChain1*         mpSwapChain;
      ID3D11RenderTargetView*  mpRenderTargetView;
      ID3D11BlendState*        mpBlendState;
   };
}

#endif // D3D_DEVICE_H
