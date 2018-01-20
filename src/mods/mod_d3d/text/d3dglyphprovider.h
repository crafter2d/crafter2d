
#ifndef D3D_GLYPH_PROVIDER_H
#define D3D_GLYPH_PROVIDER_H

#include <d2d1_1.h>
#include <dwrite.h>

#include "core/graphics/text/glyphprovider.h"

class D3DTextRenderer;

/// The DirectWrite glyph provider implementation for use on Windows devices.
///
/// Links
/// https://msdn.microsoft.com/en-us/library/xwf9s90b.aspx#Y798
/// https://www.microsoft.com/typography/OTSPEC/vmtx.htm
/// Custom renderer : https://msdn.microsoft.com/en-us/library/windows/desktop/dd941787(v=vs.85).aspx
/// GetBounds : https://msdn.microsoft.com/en-us/library/windows/desktop/dd742751(v=vs.85).aspx

namespace Graphics
{
   class GlyphAtlas;

   class D3DGlyphProvider : public GlyphProvider
   {
   public:
      D3DGlyphProvider(ID2D1DeviceContext* pcontext, IDWriteFactory* pdwfactory, GlyphAtlas& atlas);
      virtual ~D3DGlyphProvider();

      bool initialize(IDWriteFontCollection* pcollection, Font& font);

      virtual uint32_t getGlyph(UChar ch) override;

   private:

    // data
      ID2D1DeviceContext*   mpD2DContext;
      IDWriteFactory*       mpDWriteFactory;
      IDWriteTextFormat*    mpTextFormat;
      D3DTextRenderer*      mpTextRenderer;
      int                   mWidth;
      int                   mHeight;
   };
}

#endif // D3D_GLYPH_PROVIDER_H
