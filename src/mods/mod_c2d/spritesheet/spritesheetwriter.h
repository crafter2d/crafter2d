
#ifndef SPRITE_SHEET_WRITER_H
#define SPRITE_SHEET_WRITER_H

#include "core/content/contentwriter.h"

namespace c2d
{
   class SpriteSheetWriter : public ContentWriter
   {
   public:

      virtual bool write(DataStream& stream, const String& filename) override;
   };
}

#endif // SPRITE_SHEET_WRITER_H

