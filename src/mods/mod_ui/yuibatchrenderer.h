#ifndef YUI_BATCH_RENDERER_H
#define YUI_BATCH_RENDERER_H

#include "core/math/matrix4.h"
#include "core/math/vertex.h"

namespace Graphics
{
   class Device;
   class Effect;
   class UniformBuffer;
   class VertexBuffer;
   class IndexBuffer;
   class RenderContext;
   class Viewport;
}

namespace c2d
{
   class Rect;
   class TileAtlas;

   class YuiBatchRenderer
   {
   public:

      bool create(Graphics::Device& device);
      void viewportChanged(Graphics::RenderContext& context, const Graphics::Viewport& viewport);

      void beginRendering(Graphics::RenderContext& context, const TileAtlas& atlas);
      void endRendering();

      void renderRect(const Rect& rect, int tile);

   private:
      struct PTVertex
      {
         Vertex pos;
         Vertex tex;
      };

      struct ConstantBuffer
      {
         Matrix4 projection;
         Matrix4 world;
         Matrix4 object;
      };

    // members
      ConstantBuffer mConstants;

      Graphics::Effect*        mpEffect;
      Graphics::UniformBuffer* mpUB;
      Graphics::VertexBuffer*  mpVB;
      Graphics::IndexBuffer*   mpIB;

      Graphics::RenderContext* mpContext;
      const TileAtlas* mpAtlas;
      PTVertex* mpData;
      int       mIndices;
   };
}

#endif // YUI_BATCH_RENDERER_H
