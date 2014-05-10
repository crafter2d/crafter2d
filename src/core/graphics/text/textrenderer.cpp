
#include "textrenderer.h"

#include <vector>
#include <algorithm>

#include "core/defines.h"
#include "core/system/exception.h"
#include "core/graphics/device.h"
#include "core/graphics/font.h"
#include "core/graphics/effect.h"
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

namespace Graphics
{
   TextRenderer::TextRenderer():
      mpProvider(NULL)
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

   bool glyphSort(uint32_t i, uint32_t j) { return (i >> 16) < (j >> 16); }

   void TextRenderer::draw(RenderContext& context, const Vector& position, Font& font, float fontsize, const String& text)
   {
      TextLayout layout;
      if ( layout.create(context, position, font, fontsize, text) )
      {
         draw(context, layout);
      }
   }

   void TextRenderer::draw(RenderContext& context, const TextLayout& layout)
   {
      mpEffect->enable(context);
      context.setUniformBuffer(*mpUB);
      context.setIndexBuffer(*mpIB);

      GlyphVertex* pvertices = (GlyphVertex*)mpVB->lock(context);

      int start = 0;
      int nrindices = 0;
      const Texture* ptex = &atlas.getGlyphTexture(indices[0]);
      for ( uint32_t index = 0; index < indices.size(); ++index )
      {
         uint32_t glyphindex = indices[index];
         const Texture& texture = atlas.getGlyphTexture(glyphindex);
         if ( &texture != ptex )
         {
            context.setTexture(0, texture);
            context.drawTriangles(start, nrindices);

            start += nrindices;
            nrindices = 0;
            ptex = &texture;
         }
         
         nrindices += 6;
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
