
#ifndef TEXTURE_WRITER_H
#define TEXTURE_WRITER_H

#ifdef C2D_EXPORT_WRITERS

#include "core/content/contentwriter.h"

class TextureWriter : public c2d::ContentWriter
{
public:

 // overrides
   virtual bool write(DataStream& stream, const String& filename) override;

};

#endif // C2D_EXPORT_WRITERS

#endif // TEXTURE_WRITER_H
