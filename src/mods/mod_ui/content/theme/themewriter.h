#ifndef UI_THEME_WRITER_H
#define UI_THEME_WRITER_H

#include "core/content/contentwriter.h"

namespace c2d
{
   class ThemeWriter : public ContentWriter
   {
   public:
      virtual Result write(DataStream& stream, const String& filename) override;
   };
}

#endif // UI_THEME_WRITER_H
