
#include "d3dfont.h"

namespace Graphics
{
   D3DFont::D3DFont(IDWriteFactory* pdwritefactory) :
      Font(),
      mpDWriteFactory(pdwritefactory),
      mpFontFace(NULL)
   {
      mpDWriteFactory->AddRef();
   }

   D3DFont::~D3DFont()
   {
      SafeRelease(&mpDWriteFactory);
   }

   // - Query

   int D3DFont::getBaseLine() const
   {
      return 0;
   }

   // - Sizes

   int D3DFont::getTextWidth(const String& text) const
   {
      return 0;
   }

   int D3DFont::getTextHeight(const String& text) const
   {
      return 0;
   }

   void D3DFont::render(const String& text)
   {
   }

   // - Operations

   bool D3DFont::create(const String& name, int pointsize)
   {
      IDWriteFontFile* pFontFile = NULL;
      HRESULT hr = mpDWriteFactory->CreateFontFileReference(name.constData(), NULL, &pFontFile);
      if ( FAILED(hr) )
      {
         return false;
      }

      IDWriteFontFile* fontFileArray[] = { pFontFile };
      hr = mpDWriteFactory->CreateFontFace(DWRITE_FONT_FACE_TYPE_TRUETYPE,
         1, fontFileArray, 0, DWRITE_FONT_SIMULATIONS_NONE, &mpFontFace);

      SafeRelease(&pFontFile);

      return SUCCEEDED(hr);
   }
}
