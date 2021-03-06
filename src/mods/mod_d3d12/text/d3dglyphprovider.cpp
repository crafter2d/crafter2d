
#include "d3dglyphprovider.h"

#include <memory>
#include <stdio.h>
#include <string.h>

#include "core/defines.h"
#include "core/graphics/text/glyph.h"
#include "core/math/size.h"

#include "../d3dxhelpers.h"

#include "d3dfont.h"
#include "d3dtextrenderer.h"

namespace Graphics
{
   D3DGlyphProvider::D3DGlyphProvider(ID2D1Factory1* pfactory, ID2D1DeviceContext* pd2dcontext, IDWriteFactory* pdwfactory) :
      mpD2DFactory(pfactory),
      mpD2DContext(pd2dcontext),
      mpTextFormat(NULL),
      mpDWriteFactory(pdwfactory),
      mpTextRenderer(NULL),
      mWidth(512),
      mHeight(512)
   {
      mpD2DFactory->AddRef();
      mpD2DContext->AddRef();
      mpDWriteFactory->AddRef();
   }

   D3DGlyphProvider::~D3DGlyphProvider()
   {
      DX::SafeRelease(&mpD2DContext);
      DX::SafeRelease(&mpDWriteFactory);
   }

   bool D3DGlyphProvider::initialize(IDWriteFontCollection* pcollection, Font& font)
   {
      D3DFont& d3dfont = static_cast<D3DFont&>(font);
      String family = d3dfont.getFamilyName();

      HRESULT hr;
      hr = mpDWriteFactory->CreateTextFormat(
         family.c_str(),
         pcollection, 
         DWRITE_FONT_WEIGHT_NORMAL, 
         DWRITE_FONT_STYLE_NORMAL,
         DWRITE_FONT_STRETCH_NORMAL,
         24.0f,
         L"",
         &mpTextFormat);
      if ( FAILED(hr) )
      {
         return false;
      }

      mpTextRenderer = new D3DTextRenderer();
      mpTextRenderer->AddRef();
      hr = mpTextRenderer->initialize(mpD2DFactory, mpD2DContext, 512, 512);
      if ( FAILED(hr) )
      {
         return false;
      }

      return true;
   }
   
   Glyph* D3DGlyphProvider::getGlyph(UChar ch, float emsize)
   {
      HRESULT hr = S_OK;
      IDWriteTextLayout* ptextlayout = NULL;

      ASSERT_PTR(mpTextFormat);

      hr = mpDWriteFactory->CreateTextLayout(&ch, 1, mpTextFormat, 512, 512, &ptextlayout);
      if ( SUCCEEDED(hr) )
      {
         DWRITE_TEXT_RANGE range = { 0, 1 };
         ptextlayout->SetFontSize(emsize, range);

         auto glyph = std::make_unique<Glyph>();
         hr = ptextlayout->Draw(glyph.get(), mpTextRenderer, 0, 0);
         if ( SUCCEEDED(hr) )
         {
            return glyph.release();
         }
      }

      return NULL;
   }
}
