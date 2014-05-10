
#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include "core/core_base.h"
#include "core/math/color.h"

#include "viewport.h"
#include "sprites/spriterenderer.h"
#include "text/textrenderer.h"

class Vector;
class XForm;

namespace Graphics
{
   class BlendState;
   class CodePath;
   class Effect;
   class Sprite;
   class RenderTarget;
   class IndexBuffer;
   class VertexBuffer;
   class UniformBuffer;
   class Texture;

   class CORE_API RenderContext
   {
   public:
      RenderContext();

    // get/set
      const Color& getClearColor() const;
      void         setClearColor(const Color& color);

      const Viewport& getViewport() const;
      void            setViewport(const Viewport& viewport);

      TextRenderer& getTextRenderer();

    // initialization
      void initialize(Device& device);

    // operations
      virtual void setRenderTarget(RenderTarget& target) = 0;
      virtual void setCodePath(CodePath& path) = 0;
      virtual void setBlendState(const BlendState& state) = 0;
      virtual void setVertexBuffer(const VertexBuffer& buffer) = 0;
      virtual void setIndexBuffer(const IndexBuffer& buffer) = 0;
      virtual void setUniformBuffer(const UniformBuffer& buffer) = 0;
      virtual void setTexture(int stage, const Texture& texture) = 0;

    // drawing instructions
      virtual void drawTriangles(int start, int count) = 0;
      virtual void drawTriangleFan(int start, int count) = 0;
      virtual void drawTriangleStrip(int start, int count) = 0;
              void drawText(const Vector& position, Font& font, float fontsizeem, const String& text);
     
      virtual void clear() = 0;

    // drawing sprites
      void beginDraw();
      void endDraw();

      void setSpriteOffset(const Vector& offset);
      void drawSprite(const Sprite& sprite);

   protected:
    // notifications
      virtual void onViewportChanged(const Viewport& viewport) = 0;

   private:

    // data
      SpriteRenderer mSpriteRenderer;
      TextRenderer   mTextRenderer;
      Viewport       mViewport;
      Color          mClearColor;
   };
};

#include "rendercontext.inl"

#endif // RENDER_CONTEXT_H
