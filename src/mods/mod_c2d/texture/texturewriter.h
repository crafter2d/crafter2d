
#ifndef TEXTURE_WRITER_H
#define TEXTURE_WRITER_H

#include "core/content/contentwriter.h"

class TextureWriter : public c2d::ContentWriter
{
public:

 // overrides
   virtual bool write(DataStream& stream, const String& filename) override;

};

#endif // TEXTURE_WRITER_H
