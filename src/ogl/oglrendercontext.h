
#ifndef OGL_RENDER_CONTEXT_H
#define OGL_RENDER_CONTEXT_H

#include "core/graphics/rendercontext.h"

namespace Graphics
{
   class OGLIndexBuffer;
   class OGLVertexBuffer;

   class OGLRenderContext : public RenderContext
   {
   public:
      OGLRenderContext();

      virtual void setViewport(const Viewport& viewport);
      virtual void setBlendState(const BlendState& state);
      virtual void setVertexBuffer(const VertexBuffer& buffer);
      virtual void setIndexBuffer(const IndexBuffer& buffer);

      virtual void setOrthoProjection();

      virtual void setIdentityViewMatrix();
      virtual void setIdentityWorldMatrix();
      virtual void setWorldMatrix(const Matrix4& matrix);

      virtual void clear();

      virtual void drawTriangles(int start, int count);
      virtual void drawTriangleStrip(int start, int count);

   private:
      const OGLVertexBuffer* mpVertexBuffer;
      const OGLIndexBuffer*  mpIndexBuffer;
   };

};

#endif // OGL_RENDER_CONTEXT_H
