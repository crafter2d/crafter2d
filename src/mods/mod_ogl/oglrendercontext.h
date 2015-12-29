
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

    // overrides
      virtual void setRenderTarget(RenderTarget& target) override;
      virtual void setCodePath(CodePath& path) override;
      virtual void setBlendState(const BlendState& state) override;
      virtual void setVertexBuffer(const VertexBuffer& buffer) override;
      virtual void setIndexBuffer(const IndexBuffer& buffer) override;
      virtual void setTexture(int stage, const Texture& texture) override;

      virtual void clear();

      virtual void drawPoints(int start, int count);
      virtual void drawTriangles(int start, int count);
      virtual void drawTriangleFan(int start, int count);
      virtual void drawTriangleStrip(int start, int count);
      
   private:

    // data
      const Effect*          mpEffect;
            CodePath*        mpCodePath;
      const OGLVertexBuffer* mpVertexBuffer;
      const OGLIndexBuffer*  mpIndexBuffer;
   };

};

#endif // OGL_RENDER_CONTEXT_H
