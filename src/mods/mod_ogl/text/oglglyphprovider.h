
#ifndef OGL_GLYPH_PROVIDER_H
#define OGL_GLYPH_PROVIDER_H

#include "core/graphics/text/glyphprovider.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Graphics
{
   class OGLFont;

   class OGLGlyphProvider : public GlyphProvider
   {
   public:
      OGLGlyphProvider(FT_Library library, GlyphAtlas& atlas);

      void initialize(OGLFont& font);

      virtual uint32_t getGlyph(UChar ch) override;

   private:

      // data
      FT_Library mLibrary;
      OGLFont*   mpFont;
   };
}

#endif // OGL_GLYPH_PROVIDER_H
