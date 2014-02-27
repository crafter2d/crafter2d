
#ifndef WORLD_READER_H
#define WORLD_READER_H

#include "core/content/contentreader.h"

class WorldReader : public c2d::ContentReader
{
public:

   virtual IContent* read(DataStream& stream) override;

};

#endif // WORLD_READER_H
