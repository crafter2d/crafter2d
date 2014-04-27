
#ifndef D3D_GLYPH_PROVIDER_H
#define D3D_GLYPH_PROVIDER_H

#include <d2d1_1.h>
#include <dwrite.h>

#include "core/graphics/text/glyphprovider.h"

namespace Graphics
{
   class D3DGlyphProvider : public GlyphProvider
   {
   public:
      D3DGlyphProvider(ID2D1Device* pd2ddevice, IDWriteFactory* pdwfactory);
      virtual ~D3DGlyphProvider();

      virtual bool initialize(int width, int height) override;

      virtual Glyph* getGlyph(Font& font, uint16_t glyphindex, float emsize) override;

   private:
      struct DWGlyphData
      {
         FLOAT					offsetX;
         FLOAT					offsetY;
         LONG					maxWidth;
         LONG					maxHeight;
      };

    // helpers
      void initGlyphData(const DWRITE_FONT_METRICS& fontMetrics,
                         const DWRITE_GLYPH_METRICS& glyphMetrics,
                         float fontSize,
                         DWGlyphData& outGlyphData);

    // data
      ID2D1Device*          mpD2DDevice;
      ID2D1DeviceContext*   mpD2DContext;
      ID2D1Bitmap*          mpTargetBitmap;
      ID2D1Bitmap1*         mpBitmap;
      ID2D1SolidColorBrush* mpBrush;
      IDWriteFactory*       mpDWriteFactory;
      int                   mWidth;
      int                   mHeight;
   };
}

#endif // D3D_GLYPH_PROVIDER_H
