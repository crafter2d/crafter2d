
#ifndef TEXTURE_READER_H
#define TEXTURE_READER_H

#include "core/content/contentreader.h"

class TextureReader : public c2d::ContentReader
{
public:

 // overrides
   virtual IContent* read(DataStream& stream) override;

};

#endif // TEXTURE_READER_H
