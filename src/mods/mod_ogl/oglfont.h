
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
      explicit OGLFont(FT_Face face);
      virtual ~OGLFont();

    // query
      virtual int getBaseLine() const;

    // operations
      bool initialize(int pointsize);

    // sizes
      virtual int getTextWidth(const String& text) const;
      virtual int getTextHeight(const String& text) const;

    // rendering
      virtual void render(const String& text);

   private:
      typedef std::map<UChar, FontChar*> Characters;

    // loading
      FontChar* loadGlyph(UChar character);

    // helpers
      FT_BBox        measure(char c);

      unsigned char* invertMonoBitmap(FT_Bitmap& bitmap);
      unsigned char* invertBitmap(FT_Bitmap& bitmap, int width, int height);

    // members
      FT_Face     mFace;
      Characters  mCharacters;
      int         mWidth;
      int         mHeight;
      int         mBaseHeight;
   };
};

#endif // OGL_FONT_H
