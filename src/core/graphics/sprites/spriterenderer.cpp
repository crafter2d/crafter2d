/***************************************************************************
 *   Copyright (C) 2013 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "spriterenderer.h"

#include "core/graphics/device.h"
#include "core/graphics/indexbuffer.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/uniformbuffer.h"
#include "core/graphics/utils.h"
#include "core/graphics/vertexbuffer.h"
#include "core/graphics/vertexinputlayout.h"
#include "core/math/size.h"
#include "core/math/xform.h"

#include "core/graphics/texturecoordinate.h"

#include "sprite.h"

#include "core/math/matrix4.h"
#include "core/math/vector.h"

struct ConstantBuffer
   {
      Matrix4 projection;
      Matrix4 world;
      Matrix4 object;
   };

namespace Graphics
{

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

   if ( !mEffect.load(device, vertexLayout, UTEXT("spriteeffect")) )
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
   if ( mpVB == NULL || !mpVB->create(vertexLayout, batchsize * SpriteVertices, usage) )
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

   ConstantBuffer constants;
   constants.projection.setOrtho(0, 800.0f, 0, 600.0f);
   constants.world.setIdentity();
   constants.object.setIdentity();
   constants.object.translate(Vector(100, 100));

   mpUB->set(context, &constants);
}
   
void SpriteRenderer::endDraw(RenderContext& context)
{
   mSprites.sort();

   PTVertex* pvertices = static_cast<PTVertex*>(mpVB->lock(context));

   for ( int index = 0; index < mSprites.size(); ++index )
   {
      const Sprite& sprite = mSprites[index];

      renderSprite(sprite, pvertices);
   }

   mpVB->unlock(context);

   context.setVertexBuffer(*mpVB);
   context.setIndexBuffer(*mpIB);
   context.setUniformBuffer(*mpUB);

   mEffect.setTexture(0, mSprites[0].getTexture());
   mEffect.render(context, 6);
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

      pbuffer[index].pos = point;
      pbuffer[index].tex = tex;
   }
}

} // namespace Graphics
