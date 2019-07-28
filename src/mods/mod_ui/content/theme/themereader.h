#ifndef UI_THEME_READER_H
#define UI_THEME_READER_H

#include "core/content/contentreader.h"

namespace c2d
{
   class ThemeReader : public ContentReader
   {
   public:
      virtual IContent* read(DataStream& stream) override;
   };
}

#endif // UI_THEME_READER_H
