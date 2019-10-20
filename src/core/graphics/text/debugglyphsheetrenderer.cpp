/***************************************************************************
 *   Copyright (C) 2019 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "debugglyphsheetrenderer.h"

#include "core/graphics/device.h"
#include "core/graphics/effect.h"
#include "core/graphics/indexbuffer.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/uniformbuffer.h"
#include "core/graphics/utils.h"
#include "core/graphics/viewport.h"
#include "core/graphics/vertexbuffer.h"
#include "core/log/log.h"
#include "core/string/string.h"

namespace Graphics
{

DebugGlyphSheetRenderer::DebugGlyphSheetRenderer(const GlyphAtlas& atlas):
   mAtlas(atlas),
   mConstants(),
   mpEffect(nullptr),
   mpVB(nullptr),
   mpIB(nullptr),
   mpUB(nullptr)
{
}

bool DebugGlyphSheetRenderer::create(Device& device)
{
   mpEffect = device.createEffect(UTEXT("shaders/basic"));
   if ( mpEffect == nullptr )
   {
      return false;
   }

   int usage = VertexBuffer::eWriteOnly | VertexBuffer::eStatic;

   int size = 4; // two layers, each tile is 4 vertices
   mpVB = mpEffect->createVertexBuffer(device, size, usage);
   if ( mpVB == nullptr )
   {
      Log::getInstance().error("Could not create the vertex buffer for debug renderer.");
      return false;
   }

   mpIB = Utils::createIndexBuffer(device, 1, 4);
   if ( mpIB == nullptr )
   {
      return false;
   }

   UNIFORM_BUFFER_DESC descs[] = {
      { UTEXT("proj"), sizeof(float) * 16 },
      { UTEXT("world"), sizeof(float) * 16 },
      { UTEXT("object"), sizeof(float) * 16 },
   };

   mpUB = mpEffect->createUniformBuffer(UTEXT("mpv"));
   if ( mpUB == nullptr || !mpUB->create(device, descs, 3) )
   {
      // could not create the uniform buffer object
      // perhaps wrong name??
      return false;
   }

   return true;
}

void DebugGlyphSheetRenderer::update(RenderContext& context)
{
   pv* pvertices = (pv*)mpVB->lock(context);
   pvertices[0].pos.x = 0;
   pvertices[0].pos.y = 0;
   pvertices[0].tex.x = 0;
   pvertices[0].tex.y = 0;

   pvertices[1].pos.x = 200;
   pvertices[1].pos.y = 0;
   pvertices[1].tex.x = 1;
   pvertices[1].tex.y = 0;

   pvertices[2].pos.x = 200;
   pvertices[2].pos.y = 200;
   pvertices[2].tex.x = 1;
   pvertices[2].tex.y = 1;

   pvertices[3].pos.x = 0;
   pvertices[3].pos.y = 200;
   pvertices[3].tex.x = 0;
   pvertices[3].tex.y = 1;
   mpVB->unlock(context);
}

void DebugGlyphSheetRenderer::render(RenderContext& context)
{
   mpEffect->enable(context);
   mpEffect->setConstantBuffer(context, 0, *mpUB);

   mAtlas.bind(context, 0);

   context.setVertexBuffer(*mpVB);
   context.setIndexBuffer(*mpIB);
   context.drawTriangles(0, 6);

   mpVB->disable(context);
   mpIB->disable(context);
}

void DebugGlyphSheetRenderer::viewportChanged(RenderContext& context, const Viewport& viewport)
{
   mConstants.projection.setOrtho(viewport.getWidth(), viewport.getHeight(), -1.0f, 1.0f);
   mpUB->set(context, &mConstants, sizeof(mConstants));
   update(context);
}

}
