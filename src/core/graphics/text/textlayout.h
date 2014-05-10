
#ifndef TEXT_LAYOUT_H
#define TEXT_LAYOUT_H

#include "core/math/vector.h"

class String;

namespace Graphics
{
   class Font;
   class RenderContext;

   struct GlyphVertex
   {
      Vector pos;
      Vector tex;
   };

   class TextLayout
   {
   public:
      TextLayout();

    // operations
      bool create(RenderContext& context, const Vector& position, Font& font, float fontsize, const String& text);

   private:

    // data
      GlyphVertex* mpVertices;
      int          mCount;
   };
}

#endif // TEXT_LAYOUT_H
