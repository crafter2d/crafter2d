
#include "textrenderer.h"

#include <vector>
#include <algorithm>

#include "core/defines.h"
#include "core/system/exception.h"
#include "core/graphics/device.h"
#include "core/graphics/font.h"
#include "core/graphics/effect.h"
#include "core/graphics/blendstatedesc.h"
#include "core/graphics/indexbuffer.h"
#include "core/graphics/vertexbuffer.h"
#include "core/graphics/uniformbuffer.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/utils.h"
#include "core/math/vector.h"

#include "glyphatlas.h"
#include "glyphprovider.h"
#include "glyphvertexdata.h"
#include "textlayout.h"

struct GlyphVertex
{
   Vector pos;
   Vector tex;
};

namespace Graphics
{
   TextRenderer::TextRenderer():
      mConstants(),
      mpDevice(NULL),
      mpEffect(NULL),
      mpVB(NULL),
      mpIB(NULL),
      mpUB(NULL),
      mpProvider(NULL),
      mFonts()
   {
   }

   bool TextRenderer::initialize(Device& device)
   {
      mpDevice = &device;

      mpEffect = device.createEffect(UTEXT("shaders/basic"));
      if ( mpEffect == NULL )
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

      BlendStateDesc desc(BlendStateDesc::BS_SRC_ALPHA, BlendStateDesc::BS_ONE, true);
      BlendState* pblendstate = mpDevice->createBlendState(desc);
      mpEffect->setBlendState(pblendstate);

      int batchsize = 256;

      int usage = VertexBuffer::eDynamic | VertexBuffer::eWriteOnly;
      mpVB = mpEffect->createVertexBuffer(device, batchsize * 4, usage);
      if ( mpVB == NULL )
      {
         return false;
      }

      mpIB = Utils::createIndexBuffer(device, batchsize, 4);
      if ( mpIB == NULL )
      {
         return false;
      }

      return true;
   }

   void TextRenderer::viewportChanged(RenderContext& context, const Viewport& viewport)
   {
      mConstants.projection.setOrtho(viewport.getWidth(), viewport.getHeight(), -1, 1);

      mpUB->set(context, &mConstants, sizeof(mConstants));
   }

   void TextRenderer::draw(RenderContext& context, const Vector& position, Font& font, float fontsize, const String& text)
   {
      float dpisize = (fontsize / 72.0f) * 96.0f;

      TextLayout layout;
      if ( layout.create(context, position, font, dpisize, text) )
      {
         draw(context, layout);
      }
   }

   void TextRenderer::draw(RenderContext& context, TextLayout& layout)
   {
      mpEffect->enable(context);
      mpEffect->setConstantBuffer(context, 0, *mpUB);

      context.setVertexBuffer(*mpVB);
      context.setIndexBuffer(*mpIB);

      Font& font = layout.getFont();

      TextLayoutInfo info;
      layout.fill(info);

      uint32_t vertex = 0;
      
      GlyphAtlas& atlas = font.getGlyphAtlas();
      for ( uint32_t index = 0; index < info.sheetCount; ++index )
      {
         uint32_t amount = info.indicesPerSheet[index];
         if ( amount > 0 )
         {
            const Texture& texture = atlas.getGlyphTexture(info.data[vertex].glyphindex);
            context.setTexture(0, texture);

            GlyphVertex* pvertices = (GlyphVertex*)mpVB->lock(context);
            int nrindices = 0;

            for ( uint32_t d = 0; d < amount; ++d )
            {
               TextLayoutData& data = info.data[vertex++];
               const GlyphVertexData& vertexdata = atlas.getGlyphVertexData(data.glyphindex);

               pvertices[0].pos.x = data.pos.x;
               pvertices[0].pos.y = data.pos.y;
               pvertices[0].tex.x = vertexdata.mTexturePos.x;
               pvertices[0].tex.y = vertexdata.mTexturePos.y;

               pvertices[1].pos.x = data.pos.x + vertexdata.mGlyphSize.x;
               pvertices[1].pos.y = data.pos.y;
               pvertices[1].tex.x = vertexdata.mTexturePos.x + vertexdata.mTextureDim.x;
               pvertices[1].tex.y = vertexdata.mTexturePos.y;

               pvertices[2].pos.x = data.pos.x + vertexdata.mGlyphSize.x;
               pvertices[2].pos.y = data.pos.y + vertexdata.mGlyphSize.y;
               pvertices[2].tex.x = vertexdata.mTexturePos.x + vertexdata.mTextureDim.x;
               pvertices[2].tex.y = vertexdata.mTexturePos.y + vertexdata.mTextureDim.y;

               pvertices[3].pos.x = data.pos.x;
               pvertices[3].pos.y = data.pos.y + vertexdata.mGlyphSize.y;
               pvertices[3].tex.x = vertexdata.mTexturePos.x;
               pvertices[3].tex.y = vertexdata.mTexturePos.y + vertexdata.mTextureDim.y;

               pvertices += 4;
               nrindices += 6;
            }

            mpVB->unlock(context);

            context.drawTriangles(0, nrindices);
         }
      }
   }

   // - Font

   bool TextRenderer::hasFont(const String& name)
   {
      return mFonts.find(name) != mFonts.end();
   }

   Font& TextRenderer::getFont(const String& name)
   {
      Font* pfont = NULL;
      Fonts::iterator it = mFonts.find(name);
      if ( it == mFonts.end() )
      {
         pfont = mpDevice->createFont(name);

         GlyphProvider* pprovider = mpDevice->createGlyphProvider(*pfont);
         if ( pprovider == NULL )
         {
            throw new c2d::Exception(UTEXT("Could not create glyph provider!"));
         }

         GlyphAtlas* patlas = new GlyphAtlas(*mpDevice, pprovider, 100);
         pfont->setGlyphAtlas(patlas);
      }
      else
      {
         pfont = it->second;
      }

      return *pfont;
   }

   void TextRenderer::addFont(Font* pfont)
   {
      mFonts.insert(std::make_pair(pfont->getFamilyName(), pfont));
   }
}
