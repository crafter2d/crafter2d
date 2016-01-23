
#ifndef D3D_GLYPH_PROVIDER_H
#define D3D_GLYPH_PROVIDER_H

#include <d2d1_1.h>
#include <dwrite.h>

#include "core/graphics/text/glyphprovider.h"

class D3DTextRenderer;

namespace Graphics
{
   class D3DGlyphProvider : public GlyphProvider
   {
   public:
      D3DGlyphProvider(ID2D1Factory1* pfactory, ID2D1DeviceContext* pcontext, IDWriteFactory* pdwfactory);
      virtual ~D3DGlyphProvider();

      bool initialize(IDWriteFontCollection* pcollection, Font& font);

      virtual Glyph* getGlyph(UChar ch, float emsize) override;

   private:
      

    // data
      ID2D1Factory1*        mpD2DFactory;
      ID2D1DeviceContext*   mpD2DContext;
      IDWriteFactory*       mpDWriteFactory;
      IDWriteTextFormat*    mpTextFormat;
      D3DTextRenderer*      mpTextRenderer;
      int                   mWidth;
      int                   mHeight;
   };
}

#endif // D3D_GLYPH_PROVIDER_H
