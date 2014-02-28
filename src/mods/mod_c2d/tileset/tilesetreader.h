
#ifndef TILESET_READER_H
#define TILESET_READER_H

#include "core/content/contentreader.h"

class TileSetReader : public c2d::ContentReader
{
public:

   virtual IContent* read(DataStream& stream) override;

};

#endif // TILESET_READER_H
