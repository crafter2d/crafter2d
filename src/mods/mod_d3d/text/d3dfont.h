
#ifndef D3D_FONT_H
#define D3D_FONT_H

#include <dwrite_1.h>

#include "core/graphics/font.h"

template <class T> inline void SafeRelease(T **ppT)
{
   if ( *ppT )
   {
      (*ppT)->Release();
      *ppT = NULL;
   }
}

namespace Graphics
{
   class D3DFont : public Graphics::Font
   {
   public:
      D3DFont(IDWriteFactory* pdwritefactory);
      virtual ~D3DFont();

      // query
      virtual int      getBaseLine() const override;

      // sizes
      virtual int      getTextWidth(const String& text) const override;
      virtual int      getTextHeight(const String& text) const override;

      // operations
      bool create(const String& name, int pointsize);

      // rendering
      virtual void     render(const String& text) override;

   private:
      friend class D3DGlyphProvider;

      // data
      IDWriteFactory*   mpDWriteFactory;
      IDWriteFontFace*  mpFontFace;
   };
}

#endif // D3D_FONT_H
