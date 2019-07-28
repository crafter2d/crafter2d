
#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include "core/core_base.h"
#include "core/math/color.h"

#include "particles/particlesystemrenderer.h"
#include "sprites/spriterenderer.h"
#include "tiles/tileatlas.h"
#include "text/textrenderer.h"

class Vector;
class XForm;

namespace Graphics
{
   class BlendState;
   class CodePath;
   class Effect;
   class ParticleSystem;
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
      virtual ~RenderContext();

    // get/set
      const Color& getClearColor() const {
         return mClearColor;
      }

      void setClearColor(const Color& color) {
         mClearColor = color;
      }
      
      float getDpi() const {
         return mDpi;
      }

      void setDpi(float dpi) {
         mDpi = dpi;
      }

      TextRenderer& getTextRenderer() {
         return mTextRenderer;
      }

      c2d::TileAtlas& getSpriteAtlas() {
         return *mpTileAtlas;
      }

      void setSpriteAtlas(c2d::TileAtlas* patlas) {
         mpTileAtlas = patlas;
      }
      
    // initialization
      bool initialize(Device& device);
      void destroy();
      
    // operations
      virtual void setRenderTarget(RenderTarget& target) = 0;
      virtual void setCodePath(CodePath& path) = 0;
      virtual void setBlendState(const BlendState& state) = 0;
      virtual void setVertexBuffer(const VertexBuffer& buffer) = 0;
      virtual void setIndexBuffer(const IndexBuffer& buffer) = 0;
      virtual void setTexture(int stage, const Texture& texture) = 0;

    // drawing instructions
      virtual void drawPoints(int start, int count) = 0;
      virtual void drawTriangles(int start, int count) = 0;
      virtual void drawTriangleFan(int start, int count) = 0;
      virtual void drawTriangleStrip(int start, int count) = 0;
     
      virtual void clear() = 0;

    // drawing systems
      void beginDraw();
      void endDraw();

      void setSpriteOffset(const Vector& offset);
      void drawSprite(const c2d::Sprite& sprite);
      void drawParticles(const ParticleSystem& particles);
      void drawText(const TextLayout& textlayout);

   // notifications
      void viewportChanged(const Viewport& viewport);

   private:

    // data
      c2d::TileAtlas*        mpTileAtlas;
      c2d::SpriteRenderer    mSpriteRenderer;
      ParticleSystemRenderer mParticleRenderer;
      TextRenderer           mTextRenderer;
      Color                  mClearColor;
      float                  mDpi;
   };
};

#endif // RENDER_CONTEXT_H
