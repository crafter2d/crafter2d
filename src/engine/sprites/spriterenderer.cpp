
#include "spriterenderer.h"

#include "core/graphics/device.h"
#include "core/graphics/indexbuffer.h"
#include "core/graphics/uniformbuffer.h"
#include "core/graphics/utils.h"
#include "core/graphics/vertexbuffer.h"
#include "core/graphics/vertexinputlayout.h"
#include "core/math/size.h"
#include "core/math/xform.h"

#include "engine/texturecoordinate.h"

#include "sprite.h"

using namespace Graphics;

SpriteRenderer::SpriteRenderer():
   mSprites(),
   mEffect(),
   mpUB(NULL),
   mpVB(NULL),
   mpIB(NULL)
{
}

static const int SpriteVertices = 4;
static const int SpriteIndices  = 6;

bool SpriteRenderer::create(Device& device)
{
   Graphics::VertexInputLayout vertexLayout(Graphics::INPUT_XY | Graphics::INPUT_Tex0);
   uint usage = VertexBuffer::eDynamic | VertexBuffer::eWriteOnly;

   if ( !mEffect.load(device, vertexLayout, UTEXT("spriteeffect.xml")) )
   {
      return false;
   }

   UNIFORM_BUFFER_DESC descs[] = {
      { UTEXT("proj"), sizeof(float) * 16 },
      { UTEXT("world"), sizeof(float) * 16 },
      { UTEXT("object"), sizeof(float) * 16 },
   };

   mpUB = mEffect.getUniformBuffer(UTEXT("mpv"));
   if ( !mpUB->create(device, descs, 3) )
   {
      return false;
   }

   const int batchsize = 256;

   mpVB = device.createVertexBuffer();
   if ( mpVB == NULL || mpVB->create(vertexLayout, batchsize * SpriteVertices, usage) )
   {
      return false;
   }

   mpIB = Utils::createIndexBuffer(device, batchsize, 4, 6);
   if ( mpIB == NULL )
   {
      return false;
   }

   return true;
}

void SpriteRenderer::beginDraw(RenderContext& context)
{
   mSprites.clear();
}
   
void SpriteRenderer::endDraw(RenderContext& context)
{
   mSprites.sort();

   PTVertex* pvertices = (PTVertex*) mpVB->lock(context);

   for ( int index = 0; index < mSprites.size(); ++index )
   {
      const Sprite& sprite = mSprites[index];

      renderSprite(sprite, pvertices);
   }

   mpVB->unlock(context);
}

void SpriteRenderer::draw(const Sprite& sprite)
{
   mSprites.add(sprite);
}

void SpriteRenderer::renderSprite(const Sprite& sprite, PTVertex* pbuffer)
{
   const TextureCoordinate& coordinate = sprite.getTextureCoordinate();
   const XForm& xform = sprite.getTransform();
   const Size& size = sprite.getHalfSize();

   int offsets[4][2] = { {-1, -1}, {1, -1}, {1, 1}, {-1, 1} };
   int texindex[] = { TextureCoordinate::TopLeft, TextureCoordinate::TopRight, TextureCoordinate::BottomRight, TextureCoordinate::BottomLeft };

   // create the vertices

   Vector point;
   for ( int index = 0; index < 4; ++index )
   {
      point.x = size.width * offsets[index][0];
      point.y = size.height * offsets[index][1];
      point = xform.transform(point);

      const Vertex& tex = coordinate.get(texindex[index]);

      pbuffer->pos = point;
      pbuffer->tex = tex;
   }
}
