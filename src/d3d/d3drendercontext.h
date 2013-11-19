
#ifndef D3D_RENDER_CONTEXT_H
#define D3D_RENDER_CONTEXT_H

#include <d3d11.h>

#include "core/graphics/rendercontext.h"

namespace Graphics
{
   class D3DRenderContext : public RenderContext
   {
   public:
      static ID3D11DeviceContext& asContext(RenderContext& context);

      D3DRenderContext(ID3D11DeviceContext* pcontext, ID3D11RenderTargetView* targetview);

    // query
      ID3D11DeviceContext& getContext();

    // operations
      virtual void setCodePath(CodePath& path) override;
      virtual void setBlendState(const BlendState& state);
      virtual void setVertexBuffer(const VertexBuffer& buffer);
      virtual void setIndexBuffer(const IndexBuffer& buffer);
      virtual void setUniformBuffer(const UniformBuffer& buffer);
      virtual void setTexture(int stage, const Texture& texture) override;

      // drawing instructions
      virtual void drawTriangles(int start, int count);
      virtual void drawTriangleFan(int start, int count);
      virtual void drawTriangleStrip(int start, int count);
      virtual void clear();

   protected:
    // notifications
      virtual void onViewportChanged(const Viewport& viewport);

   private:

      ID3D11DeviceContext*    mpContext;
      ID3D11RenderTargetView* mpRenderTargetView;
   };
}

#endif // D3D_RENDER_CONTEXT_H
