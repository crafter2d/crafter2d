#pragma once

#include <d3d11_3.h>

#include "core/graphics/rendercontext.h"

namespace Graphics
{
   class D3D11RenderContext : public RenderContext
   {
   public:
      static ID3D11DeviceContext3& asContext(RenderContext& context);

      D3D11RenderContext(ID3D11DeviceContext3* pcontext, ID3D11RenderTargetView1* targetview);
      virtual ~D3D11RenderContext();

      // query
      ID3D11DeviceContext3& getContext();

      virtual void setRenderTarget(RenderTarget& target) override;
      virtual void setCodePath(CodePath& path) override;
      virtual void setBlendState(const BlendState& state) override;
      virtual void setVertexBuffer(const VertexBuffer& buffer) override;
      virtual void setIndexBuffer(const IndexBuffer& buffer) override;
      virtual void setTexture(int stage, const Texture& texture) override;

      // drawing instructions
      virtual void drawPoints(int start, int count) override;
      virtual void drawTriangles(int start, int count) override;
      virtual void drawTriangleFan(int start, int count) override;
      virtual void drawTriangleStrip(int start, int count) override;

      virtual void clear() override;

   protected:
      // notifications
      virtual void onViewportChanged(const Viewport& viewport) override;

   private:

      ID3D11DeviceContext3*	   m_d3dContext;
      ID3D11RenderTargetView1*   m_d3dRenderTargetView;
   };
}
