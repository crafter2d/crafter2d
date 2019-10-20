#ifndef DEBUG_GLYPHSHEET_RENDERER_H
#define DEBUG_GLYPHSHEET_RENDERER_H

#include "core/math/matrix4.h"
#include "core/math/vertex.h"

namespace Graphics
{
   class Device;
   class Effect;
   class GlyphAtlas;
   class IndexBuffer;
   class RenderContext;
   class UniformBuffer;
   class VertexBuffer;
   class Viewport;

   class DebugGlyphSheetRenderer
   {
   public:
      DebugGlyphSheetRenderer(const GlyphAtlas& atlas);

      bool create(Device& device);
      void render(RenderContext& context);

      void viewportChanged(RenderContext& context, const Viewport& viewport);

   private:
      struct ConstantBuffer
      {
         Matrix4 projection;
         Matrix4 world;
         Matrix4 object;
      };

      struct pv
      {
         Vertex pos;
         Vertex tex;
      };

      void update(RenderContext& context);

      const GlyphAtlas& mAtlas;
      ConstantBuffer    mConstants;
      Effect*           mpEffect;
      VertexBuffer*     mpVB;
      IndexBuffer*      mpIB;
      UniformBuffer*    mpUB;
   };
}

#endif // DEBUG_GLYPHSHEET_RENDERER_H
