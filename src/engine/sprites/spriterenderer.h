
#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <vector>

#include "core/graphics/effect.h"
#include "core/math/vertex.h"

#include "spritebatch.h"

namespace Graphics
{
   class Device;
   class RenderContext;
   class UniformBuffer;
   class VertexBuffer;
   class IndexBuffer;
}

class Sprite;

class SpriteRenderer
{
public:
   SpriteRenderer();

   bool create(Graphics::Device& device);

   void beginDraw(Graphics::RenderContext& context);
   void endDraw(Graphics::RenderContext& context);

   void draw(const Sprite& sprite);

private:   
   struct PTVertex
   {
      Vertex pos;
      Vertex tex;
   };

 // rendering
   void renderSprite(const Sprite& sprite, PTVertex* pbuffer);

 // data
   SpriteBatch                   mSprites;
   Graphics::Effect              mEffect;
   Graphics::UniformBuffer*      mpUB;
   Graphics::VertexBuffer*       mpVB;
   Graphics::IndexBuffer*        mpIB;
};

#endif // SPRITE_RENDERER_H
