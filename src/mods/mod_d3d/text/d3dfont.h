
#ifndef D3D_FONT_H
#define D3D_FONT_H

#include <dwrite_1.h>

#include "core/graphics/font.h"

class D3DFontCollection;

namespace Graphics
{
   class D3DFont : public Graphics::Font
   {
   public:
      D3DFont(D3DFontCollection& collection, IDWriteFontFace* pfontface);
      virtual ~D3DFont();
      
    // query
      virtual int      getBaseLine() const override;

   private:
      friend class D3DGlyphProvider;

      // data
      D3DFontCollection&   mCollection;
      IDWriteFontFace*     mpFontFace;
   };
}

#endif // D3D_FONT_H
