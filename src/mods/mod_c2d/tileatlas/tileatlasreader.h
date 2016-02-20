#ifndef TILE_ATLAS_READER_H
#define TILE_ATLAS_READER_H

#include "core/content/contentreader.h"

namespace c2d
{
   class TileAtlasReader : public ContentReader
   {
   public:
      virtual IContent* read(DataStream& stream) override;
   };
}

#endif // TILE_ATLAS_READER_H

