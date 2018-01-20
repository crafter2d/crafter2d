
#ifndef D3D_FONT_H
#define D3D_FONT_H

#include <dwrite_1.h>

#include "core/graphics/font.h"

namespace Graphics
{
   class D3DFont : public Font
   {
   public:
      D3DFont(IDWriteFontFace* pfontface, GlyphProvider* pprovider);
      virtual ~D3DFont();
      
    // query
      virtual int      getBaseLine() const override;

   private:
      friend class D3DGlyphProvider;

      // data
      IDWriteFontFace*     mpFontFace;
   };
}

#endif // D3D_FONT_H
