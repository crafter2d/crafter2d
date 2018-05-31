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
#include "core/graphics/viewport.h"
#include "core/graphics/tiles/tileatlas.h"
#include "core/math/size.h"
#include "core/math/xform.h"

#include "core/graphics/texturecoordinate.h"

#include "sprite.h"

#include "core/math/matrix4.h"
#include "core/math/vector.h"

using namespace Graphics;

static const int SpriteVertices = 4;
static const int SpriteIndices = 6;

namespace c2d
{

   c2d::SpriteRenderer::SpriteRenderer() :
      mSprites(),
      mConstants(),
      mOffset(),
      mpEffect(nullptr),
      mpUB(nullptr),
      mpVB(nullptr),
      mpIB(nullptr)
   {
   }

   c2d::SpriteRenderer::~SpriteRenderer()
   {
      release();
   }

   bool SpriteRenderer::create(Device& device)
   {
      mpEffect = device.createEffect(UTEXT("shaders/basic"));
      if ( mpEffect == nullptr )
      {
         return false;
      }

      UNIFORM_BUFFER_DESC descs[] = {
         { UTEXT("proj"), sizeof(float) * 16 },
         { UTEXT("world"), sizeof(float) * 16 },
         { UTEXT("object"), sizeof(float) * 16 },
      };

      mpUB = mpEffect->createUniformBuffer(UTEXT("mpv"));
      if ( !mpUB->create(device, descs, 3) )
      {
         return false;
      }

      mConstants.projection.setIdentity();
      mConstants.world.setIdentity();
      mConstants.object.setIdentity();

      const int batchsize = 256;

      int usage = VertexBuffer::eDynamic | VertexBuffer::eWriteOnly;
      mpVB = mpEffect->createVertexBuffer(device, batchsize * SpriteVertices, usage);
      if ( mpVB == nullptr )
      {
         return false;
      }

      mpIB = Utils::createIndexBuffer(device, batchsize, 4);
      if ( mpIB == nullptr )
      {
         return false;
      }

      return true;
   }

   void SpriteRenderer::release()
   {
      delete mpVB;
      delete mpIB;
      delete mpUB;
      delete mpEffect;
   }

   void SpriteRenderer::setOffset(RenderContext& context, const Vector& offset)
   {
      if ( mOffset != offset )
      {
         mConstants.world.translate(-(offset - mOffset));
         mpUB->set(context, &mConstants, sizeof(mConstants));

         mOffset = offset;
      }
   }

   void SpriteRenderer::viewportChanged(RenderContext& context, const Viewport& viewport)
   {
      mConstants.projection.setOrtho(viewport.getWidth(), viewport.getHeight(), -1, 1);

      mpUB->set(context, &mConstants, sizeof(mConstants));
   }

   // - Drawing

   void SpriteRenderer::beginDraw(RenderContext& context)
   {
      C2D_UNUSED(context);

      mSprites.clear();
   }

   void SpriteRenderer::endDraw(RenderContext& context)
   {
      if ( mSprites.size() > 0 )
      {
         renderSprites(context);

         mpEffect->enable(context);
         mpEffect->setConstantBuffer(context, 0, *mpUB);

         context.setVertexBuffer(*mpVB);
         context.setIndexBuffer(*mpIB);

         auto& atlas = context.getSpriteAtlas();

         int start = 0;
         int indices = 6;
         int sheet = mSprites.front()->getTile() >> 16;
         for ( auto psprite : mSprites )
         {
            int thissheet = psprite->getTile() >> 16;
            if ( sheet != thissheet )
            {
               atlas.bind(context, sheet);
               context.drawTriangles(start, indices);

               start += indices;
               indices = 0;
               sheet = thissheet;
            }

            indices += 6;
         }

         if ( indices > 0 )
         {
            atlas.bind(context, sheet);
            context.drawTriangles(start, indices);
         }

         mpVB->disable(context);
         mpIB->disable(context);
      }
   }

   void SpriteRenderer::draw(const Sprite& sprite)
   {
      mSprites.add(sprite);
   }

   void SpriteRenderer::renderSprites(RenderContext& context)
   {
      PTVertex* pvertices = static_cast<PTVertex*>(mpVB->lock(context));
      TileAtlas& atlas = context.getSpriteAtlas();

      mSprites.sort();
      for ( auto psprite : mSprites )
      {
         renderSprite(atlas, *psprite, pvertices);

         pvertices += 4;
      }

      mpVB->unlock(context);
   }

   void SpriteRenderer::renderSprite(const TileAtlas& atlas, const Sprite& sprite, PTVertex* pbuffer)
   {
      const XForm& xform = sprite.getTransform();
      const Size& halfsize = sprite.getHalfSize();
      const TileSheet::Tile& tileinfo = atlas.getTile(sprite.getTile());

      // create the vertices
      pbuffer[0].pos = xform.transform(-halfsize.width, -halfsize.height);
      pbuffer[1].pos = xform.transform(halfsize.width, -halfsize.height);
      pbuffer[2].pos = xform.transform(halfsize.width, halfsize.height);
      pbuffer[3].pos = xform.transform(-halfsize.width, halfsize.height);
      

      if ( sprite.isFlipped() )
      {
         pbuffer[0].tex.set(tileinfo.coord.right, tileinfo.coord.top);
         pbuffer[1].tex.set(tileinfo.coord.left, tileinfo.coord.top);
         pbuffer[2].tex.set(tileinfo.coord.left, tileinfo.coord.bottom);
         pbuffer[3].tex.set(tileinfo.coord.right, tileinfo.coord.bottom);
      }
      else
      {
         pbuffer[0].tex.set(tileinfo.coord.left, tileinfo.coord.top);
         pbuffer[1].tex.set(tileinfo.coord.right, tileinfo.coord.top);
         pbuffer[2].tex.set(tileinfo.coord.right, tileinfo.coord.bottom);
         pbuffer[3].tex.set(tileinfo.coord.left, tileinfo.coord.bottom);
      }
   }

} // namespace c2d