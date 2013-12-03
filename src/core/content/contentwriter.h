
#ifndef CONTENT_WRITER_H
#define CONTENT_WRITER_H

#include "core/core_base.h"

class ContentHeader;
class DataStream;
class String;

class CORE_API ContentWriter
{
public:
   ContentWriter();

   virtual bool write(DataStream& stream, const String& filename) = 0;
};

#endif // CONTENT_WRITER_H
