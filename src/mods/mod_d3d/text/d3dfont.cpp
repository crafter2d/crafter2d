
#include "d3dfont.h"

#include "../d3dhelpers.h"

namespace Graphics
{
   D3DFont::D3DFont(D3DFontCollection& collection, IDWriteFontFace* pfontface) :
      mCollection(collection),
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
