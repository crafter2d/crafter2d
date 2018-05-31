
#ifndef OGL_FONT_H
#define OGL_FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include "core/graphics/font.h"

namespace Graphics
{
   class FontChar;

   class OGLFont : public Font
   {
   public:
      explicit OGLFont(GlyphProvider* pprovider, FT_Face face);
      virtual ~OGLFont();

    // get/set
      FT_Face getFace() const;

    // query
      virtual int getBaseLine() const;

    // operations
      bool initialize(float pointsize);

   private:
    // helpers
      FT_BBox measure(char c);

    // members
      FT_Face     mFace;
      int         mWidth;
      int         mHeight;
      int         mBaseHeight;
   };
};

#endif // OGL_FONT_H
