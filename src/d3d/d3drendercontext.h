
#ifndef D3D_RENDER_CONTEXT_H
#define D3D_RENDER_CONTEXT_H

#include <d3d11.h>

#include "core/graphics/rendercontext.h"

namespace Graphics
{
   class D3DRenderContext : public RenderContext
   {
   public:
      explicit D3DRenderContext(ID3D11DeviceContext* mpContext);

    // query
      ID3D11DeviceContext& getContext();

    // operations
      virtual void setBlendState(const BlendState& state);
      virtual void setEffect(const Effect& effect);
      virtual void setVertexBuffer(const VertexBuffer& buffer);
      virtual void setIndexBuffer(const IndexBuffer& buffer);
      virtual void setUniformBuffer(const UniformBuffer& buffer);

      /// Sets the projection matrix to orthographic using the current viewport
      virtual void setOrthoProjection();

     // Matrix operations
      virtual void setObjectMatrix(const XForm& matrix);
      virtual void setWorldMatrix(const XForm& matrix);

      virtual void clear();

      // drawing instructions
      virtual void drawTriangles(int start, int count);
      virtual void drawTriangleFan(int start, int count);
      virtual void drawTriangleStrip(int start, int count);

   protected:
    // notifications
      virtual void onViewportChanged(const Viewport& viewport);

   private:

      ID3D11DeviceContext* mpContext;
   };
}

#endif // D3D_RENDER_CONTEXT_H
