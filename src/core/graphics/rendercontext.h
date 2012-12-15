
#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include "core/core_base.h"
#include "core/math/color.h"

#include "viewport.h"

namespace Graphics
{
   class BlendState;
   class IndexBuffer;
   class VertexBuffer;

   class CORE_API RenderContext
   {
   public:
      RenderContext();

    // get/set
      const Color& getClearColor() const;
      void         setClearColor(const Color& color);

      const Viewport getViewport() const;

    // operations

      /// Set the current viewport dimensions
      virtual void setViewport(const Viewport& viewport) = 0;
      virtual void setBlendState(const BlendState& state) = 0;
      virtual void setVertexBuffer(const VertexBuffer& buffer) = 0;
      virtual void setIndexBuffer(const IndexBuffer& buffer) = 0;

      /// Sets the projection matrix to orthographic using the current viewport
      virtual void setOrthoProjection() = 0;

      virtual void setIdentityViewMatrix() = 0;
      virtual void setIdentityWorldMatrix() = 0;

      virtual void clear() = 0;

      virtual void drawTriangles(int start, int count) = 0;
      virtual void drawTriangleStrip(int start, int count) = 0;

   private:

      Viewport mViewport;
      Color    mClearColor;
   };
};

#include "rendercontext.inl"

#endif // RENDER_CONTEXT_H
