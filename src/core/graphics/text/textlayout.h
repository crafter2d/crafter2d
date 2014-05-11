
#ifndef TEXT_LAYOUT_H
#define TEXT_LAYOUT_H

#include <cstdint>
#include <vector>

#include "core/math/vector.h"

class String;

namespace Graphics
{
   class Font;
   class RenderContext;

   class TextLayoutData
   {
   public:
      uint32_t glyphindex;
      Vector   pos;
   };

   typedef std::vector<TextLayoutData> TextLayoutDatas;
   typedef std::vector<uint32_t> IntVector;

   class TextLayoutInfo
   {
   public:
      uint32_t        sheetCount;
      uint32_t*       indicesPerSheet;
      uint32_t        dataCount;
      TextLayoutData* data;
   };

   class TextLayout
   {
   public:
      TextLayout();

    // get/set
      Font& getFont();

    // operations
      bool create(RenderContext& context, const Vector& position, Font& font, float fontsize, const String& text);
      void fill(TextLayoutInfo& info);

   private:
    // operations
      void sort();

    // data
      Font*             mpFont;
      TextLayoutDatas   mData;
      TextLayoutDatas   mSortedData;
      IntVector         mGlyphIndices;
      IntVector         mVerticesPerSheet;
      uint32_t          mMaxSheetIndex;
   };
}

#endif // TEXT_LAYOUT_H
