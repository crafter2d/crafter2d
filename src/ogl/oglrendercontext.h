
#ifndef OGL_RENDER_CONTEXT_H
#define OGL_RENDER_CONTEXT_H

#include "core/graphics/rendercontext.h"

#include "core/math/xform.h"

namespace Graphics
{
   class OGLIndexBuffer;
   class OGLVertexBuffer;

   class OGLRenderContext : public RenderContext
   {
   public:
      OGLRenderContext();

      virtual void setBlendState(const BlendState& state);
      virtual void setEffect(const Effect& effect);
      virtual void setVertexBuffer(const VertexBuffer& buffer);
      virtual void setIndexBuffer(const IndexBuffer& buffer);

      virtual void setOrthoProjection();
      
      virtual void setObjectMatrix(const XForm& matrix);
      virtual void setWorldMatrix(const XForm& matrix);

      virtual void clear();

      virtual void drawTriangles(int start, int count);
      virtual void drawTriangleFan(int start, int count);
      virtual void drawTriangleStrip(int start, int count);

   protected:
    // notifications
      virtual void onViewportChanged(const Viewport& viewport);

   private:
      void updateViewMatrix();

      const Effect*          mpEffect;
      const OGLVertexBuffer* mpVertexBuffer;
      const OGLIndexBuffer*  mpIndexBuffer;

      XForm mObjectMatrix;
      XForm mWorldMatrix;

      float matogl[16];
   };

};

#endif // OGL_RENDER_CONTEXT_H
