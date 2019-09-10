
#ifndef WORLD_WRITER_H
#define WORLD_WRITER_H

#include "core/content/contentwriter.h"

class WorldWriter : public c2d::ContentWriter
{
public:

   virtual Result write(DataStream& stream, const String& filename) override;

private:

   void processWorldFile(DataStream& stream, const String& filename);
   void processWorldInfo(DataStream& stream, const String& filename);

 // helpers
   String readString(DataStream& stream);
};

#endif // WORLD_WRITER_H
