
#ifndef D3D_DEVICE_H
#define D3D_DEVICE_H

#include "core/graphics/device.h"

#include <d3d11.h>

#include "d3drendercontext.h"

namespace Graphics
{
   class D3DDevice : public Device
   {
   public:
      explicit D3DDevice(ContentManager& contentmanager);

    // get/set
      ID3D11Device& getDevice() { return *mpDevice; }

    // overloads
      virtual bool create(int windowhandle, int width, int height);

      virtual void present();

      virtual RenderContext*  createRenderContext() override;
      virtual CodePath*       createCodePath() override;
      virtual VertexBuffer*   createVertexBuffer() override;
      virtual IndexBuffer*    createIndexBuffer() override;
      virtual Texture*        createTexture(DataStream& imagedata) override;
      virtual RenderTarget*   createRenderTarget() override;
      virtual BlendState*     createBlendState(const BlendStateDesc& desc) override;
      virtual Font*           createFont(const String& name, int pointsize) override;

   private:

      ID3D11Device*           mpDevice;
      ID3D11DeviceContext*    mpContext;
      IDXGISwapChain*         mpSwapChain;
      ID3D11RenderTargetView* mpRenderTargetView;
      ID3D11BlendState*       mpBlendState;
   };
}

#endif // D3D_DEVICE_H
