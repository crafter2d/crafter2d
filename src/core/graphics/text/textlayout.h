
#ifndef TEXT_LAYOUT_H
#define TEXT_LAYOUT_H

#include <cstdint>
#include <vector>
#include <unordered_map>

#include "core/core_base.h"
#include "core/math/vector.h"

class String;

namespace Graphics
{
   class Font;
   class TextRenderer;

   class TextLayoutData
   {
   public:
      TextLayoutData() :
         glyphindex(0),
         pos()
      {
      }

      TextLayoutData(uint32_t gl, const Vector& p) :
         glyphindex(gl),
         pos(p)
      {
      }

      uint32_t glyphindex;
      Vector   pos;
   };

   using TextLayoutDatas = ::std::vector<TextLayoutData>;
   using SheetMap = ::std::unordered_map<uint32_t, TextLayoutDatas>;

   class CORE_API TextLayout
   {
   public:
               TextLayout();
      explicit TextLayout(TextRenderer& renderer);
               TextLayout(TextLayout&& other);

      TextLayout& operator=(TextLayout&& other);

    // get/set
      const Font& getFont() const {
         return *mpFont;
      }

      const SheetMap& getGlyphData() const {
         return mData;
      }

      const Vector& getPosition() const {
         return mPosition;
      }

    // operations
      bool create(const Vector& position, Font& font, const String& text);

   private:

    // data
      SheetMap      mData;
      Vector        mPosition;
      TextRenderer* mpTextRenderer;
      Font*         mpFont;
   };
}

#endif // TEXT_LAYOUT_H
