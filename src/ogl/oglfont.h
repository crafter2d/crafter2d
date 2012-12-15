
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
      OGLFont();
      virtual ~OGLFont();

    // query
      virtual int getBaseLine() const;

    // operations
      bool initialize(FT_Face face, int pointsize);

    // sizes
      virtual int getTextWidth(const String& text) const;
      virtual int getTextHeight(const String& text) const;

    // rendering
      virtual void render(const String& text);

   private:
      typedef std::map<char, FontChar*> Characters;

    // loading
      FontChar* loadGlyph(char character);

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
