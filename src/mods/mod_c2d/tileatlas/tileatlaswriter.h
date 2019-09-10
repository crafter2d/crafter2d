
#ifndef TILE_SHEET_WRITER_H
#define TILE_SHEET_WRITER_H

#include "core/content/contentwriter.h"

namespace c2d
{
   class TileAtlasWriter : public ContentWriter
   {
   public:

      virtual Result write(DataStream& stream, const String& filename) override;
   };
}

#endif // TILE_SHEET_WRITER_H

