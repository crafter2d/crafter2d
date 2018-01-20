
#include "d3dfont.h"

#include "../d3dhelpers.h"

namespace Graphics
{
   D3DFont::D3DFont(IDWriteFontFace* pfontface, GlyphProvider* pprovider) :
      Font(pprovider),
      mpFontFace(pfontface)
   {
      mpFontFace->AddRef();
   }

   D3DFont::~D3DFont()
   {
      SafeRelease(&mpFontFace);
   }

   // - Query

   int D3DFont::getBaseLine() const
   {
      return 0;
   }
}
