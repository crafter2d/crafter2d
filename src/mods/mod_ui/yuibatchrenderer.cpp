
#include "yuibatchrenderer.h"

#include "core/graphics/device.h"
#include "core/graphics/effect.h"
#include "core/graphics/indexbuffer.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/uniformbuffer.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/utils.h"
#include "core/graphics/vertexbuffer.h"
#include "core/graphics/viewport.h"
#include "core/math/rect.h"

using namespace Graphics;

namespace c2d
{
   bool YuiBatchRenderer::create(Graphics::Device& device)
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
      mpVB = mpEffect->createVertexBuffer(device, batchsize * 32, usage);
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

   void YuiBatchRenderer::viewportChanged(Graphics::RenderContext& context, const Graphics::Viewport& viewport)
   {
      mConstants.projection.setOrtho(viewport.getWidth(), viewport.getHeight(), -1, 1);

      mpUB->set(context, &mConstants, sizeof(mConstants));
   }

   void YuiBatchRenderer::beginRendering(Graphics::RenderContext& context, const TileAtlas& atlas)
   {
      mpContext = &context;
      mpAtlas = &atlas;

      

      mpData = reinterpret_cast<PTVertex*>(mpVB->lock(context));
   }

   void YuiBatchRenderer::endRendering()
   {
      if ( mIndices > 0 )
      {
         mpVB->unlock(*mpContext);

         mpEffect->enable(*mpContext);
         mpEffect->setConstantBuffer(*mpContext, 0, *mpUB);

         mpAtlas->bind(*mpContext, 0);

         mpContext->setIndexBuffer(*mpIB);
         mpContext->setVertexBuffer(*mpVB);

         mpContext->drawTriangles(0, mIndices);
      }

      mpContext = nullptr;
   }

   void YuiBatchRenderer::renderRect(const Rect& rect, int tile)
   {
      auto& tileinfo = mpAtlas->getTile(tile);

      mpData[0].pos.set(rect.left, rect.top);
      mpData[0].tex.set(tileinfo.coord.left, tileinfo.coord.top);
      mpData[1].pos.set(rect.right, rect.top);
      mpData[1].tex.set(tileinfo.coord.right, tileinfo.coord.top);
      mpData[2].pos.set(rect.right, rect.bottom);
      mpData[2].tex.set(tileinfo.coord.right, tileinfo.coord.bottom);
      mpData[3].pos.set(rect.left, rect.bottom);
      mpData[3].tex.set(tileinfo.coord.left, tileinfo.coord.bottom);

      mpData += 4;
      mIndices += 6;
      /*
      if ( mIndices >= 6 )
      {
         mpContext->setVertexBuffer(*mpVB);
         mpContext->drawTriangles(0, 6);

         mpData = reinterpret_cast<PTVertex*>(mpVB->lock(*mpContext));
      }
      */
   }
}
