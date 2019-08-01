
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
#include "core/graphics/viewport.h"
#include "core/graphics/utils.h"
#include "core/math/vector.h"

#include "glyphatlas.h"
#include "glyphprovider.h"
#include "glyphvertexdata.h"
#include "textlayout.h"
#include "glyphmap.h"

struct GlyphVertex
{
   Vector pos;
   Vector tex;
   uint32_t matidx;
};

namespace Graphics
{
   TextRenderer::TextRenderer():
      mFonts(),
      mTexts(),
      mConstants(),
      mpDevice(nullptr),
      mAtlas(),
      mpEffect(nullptr),
      mpVB(nullptr),
      mpIB(nullptr),
      mpUB(nullptr),
      mpTransformUB(nullptr)
   {
   }

   TextRenderer::~TextRenderer()
   {
      delete mpVB;
      delete mpIB;
      delete mpUB;
      delete mpTransformUB;
      delete mpEffect;
   }

   bool TextRenderer::initialize(Device& device)
   {
      mpDevice = &device;

      mpEffect = device.createEffect(UTEXT("shaders/font"));
      if ( mpEffect == nullptr )
      {
         return false;
      }

      UNIFORM_BUFFER_DESC descs[] = {
            { UTEXT("proj"), sizeof(float) * 16 },
      };

      UNIFORM_BUFFER_DESC transformDesc[] = {
         { UTEXT("transforms"), sizeof(float) * 16 * 92 }
      };

      mpUB = mpEffect->createUniformBuffer(UTEXT("mpv"));
      if ( !mpUB->create(device, descs, 1) )
      {
         return false;
      }

      mpTransformUB = mpEffect->createUniformBuffer(UTEXT("TransformVars"));
      if ( !mpTransformUB->create(device, transformDesc, 1) )
      {
         return false;
      }

      mConstants.projection.setIdentity();

      BlendStateDesc desc(BlendStateDesc::BS_SRC_ALPHA, BlendStateDesc::BS_ONE, true);
      BlendState* pblendstate = mpDevice->createBlendState(desc);
      mpEffect->setBlendState(pblendstate);

      int batchsize = 256;
      int usage = VertexBuffer::eDynamic | VertexBuffer::eWriteOnly;
      mpVB = mpEffect->createVertexBuffer(device, batchsize * 4, usage);
      if ( mpVB == nullptr )
      {
         return false;
      }

      mpIB = Utils::createIndexBuffer(device, batchsize, 4);
      if ( mpIB == nullptr )
      {
         return false;
      }

      mAtlas = std::make_unique<GlyphAtlas>(device);

      return true;
   }

   void TextRenderer::viewportChanged(RenderContext& context, const Viewport& viewport)
   {
      mConstants.projection.setOrtho(viewport.getWidth(), viewport.getHeight(), -1.0f, 1.0f);

      mpUB->set(context, &mConstants, sizeof(mConstants));
   }
   
   void TextRenderer::draw(const TextLayout& layout)
   {
      mTexts.push_back(&layout);
   }

   void TextRenderer::render(RenderContext& context)
   {
      if ( !mTexts.empty() )
      {
         mAtlas->flush(context);

         mpEffect->enable(context);
         mpEffect->setConstantBuffer(context, 0, *mpUB);
         mpEffect->setConstantBuffer(context, 1, *mpTransformUB);

         context.setVertexBuffer(*mpVB);
         context.setIndexBuffer(*mpIB);

         Matrix4 mats[92];
         int index = 0;

         GlyphVertex* pvertices = (GlyphVertex*)mpVB->lock(context);
         int nrindices = 0;

         for (const TextLayout* playout : mTexts)
         {
            const auto& glyphdata = playout->getGlyphData();
            const Vector& pos = playout->getPosition();
            mats[index].setTranslation(pos.x, pos.y, 0.0f);

            for (const auto& pair : glyphdata)
            {
               mAtlas->bind(context, pair.first);

               for (const TextLayoutData& data : pair.second)
               {
                  const GlyphVertexData& vertexdata = mAtlas->getGlyphVertexData(data.glyphindex);

                  pvertices[0].pos.x = data.pos.x;
                  pvertices[0].pos.y = data.pos.y;
                  pvertices[0].tex.x = vertexdata.mTexturePos.x;
                  pvertices[0].tex.y = vertexdata.mTexturePos.y;
                  pvertices[0].matidx = index;

                  pvertices[1].pos.x = data.pos.x + vertexdata.mGlyphSize.x;
                  pvertices[1].pos.y = data.pos.y;
                  pvertices[1].tex.x = vertexdata.mTexturePos.x + vertexdata.mTextureDim.x;
                  pvertices[1].tex.y = vertexdata.mTexturePos.y;
                  pvertices[1].matidx = index;

                  pvertices[2].pos.x = data.pos.x + vertexdata.mGlyphSize.x;
                  pvertices[2].pos.y = data.pos.y + vertexdata.mGlyphSize.y;
                  pvertices[2].tex.x = vertexdata.mTexturePos.x + vertexdata.mTextureDim.x;
                  pvertices[2].tex.y = vertexdata.mTexturePos.y + vertexdata.mTextureDim.y;
                  pvertices[2].matidx = index;

                  pvertices[3].pos.x = data.pos.x;
                  pvertices[3].pos.y = data.pos.y + vertexdata.mGlyphSize.y;
                  pvertices[3].tex.x = vertexdata.mTexturePos.x;
                  pvertices[3].tex.y = vertexdata.mTexturePos.y + vertexdata.mTextureDim.y;
                  pvertices[3].matidx = index;

                  pvertices += 4;
                  nrindices += 6;
               }
            }

            mpVB->unlock(context);

            mpTransformUB->set(context, &mats, sizeof(Matrix4) * 92);

            context.drawTriangles(0, nrindices);

            index++;
         }

         
      }

      mTexts.clear();
   }

   // - Font

   bool TextRenderer::hasFont(const String& name)
   {
      return mFonts.find(name) != mFonts.end();
   }

   Font& TextRenderer::getFont(const String& name)
   {
      Fonts::iterator it = mFonts.find(name);
      if (it != mFonts.end())
      {
         return *it->second;
      }
    
      Font* pfont = mpDevice->createFont(name);
      mFonts.emplace(name, pfont);

      return *pfont;
   }

   void TextRenderer::addFont(Font* pfont)
   {
      mFonts.insert(std::make_pair(pfont->getFamilyName(), pfont));
   }
}
