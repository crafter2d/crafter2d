
#ifndef TILESET_WRITER_H
#define TILESET_WRITER_H

#include "core/content/contentwriter.h"

class TileSetWriter : public c2d::ContentWriter
{
public:

   virtual bool write(DataStream& stream, const String& filename) override;
};

#endif // TILESET_WRITER_H
