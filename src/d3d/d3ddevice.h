
#ifndef D3D_DEVICE_H
#define D3D_DEVICE_H

#include "core/graphics/device.h"

#include <d3d11.h>

namespace Graphics
{
   class D3DDevice : public Device
   {
   public:
      D3DDevice();

    // get/set
      ID3D11Device& getDevice() { return *mpDevice; }

    // overloads
      virtual bool create(int windowhandle, int width, int height);

      virtual RenderContext*  createRenderContext();
      virtual CodePath*       createCodePath(CodePath::PathType type);
      virtual VertexBuffer*   createVertexBuffer(VertexInputLayout& layout);
      virtual IndexBuffer*    createIndexBuffer();
      virtual Texture*        createTexture(const String& filename);
      virtual BlendState*     createBlendState(const BlendStateDesc& desc);
      virtual Font*           createFont(const String& name, int pointsize);

   private:
      ID3D11Device*           mpDevice;
      ID3D11DeviceContext*    mpContext;
      IDXGISwapChain*         mpSwapChain;
      ID3D11RenderTargetView* mpRenderTargetView;
   };
}

#endif // D3D_DEVICE_H
