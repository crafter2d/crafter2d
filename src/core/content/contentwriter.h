
#ifndef CONTENT_WRITER_H
#define CONTENT_WRITER_H

#include "core/core_base.h"

class DataStream;
class String;

namespace c2d
{
   class CORE_API ContentWriter
   {
   public:
      ContentWriter();
      virtual ~ContentWriter();

      virtual bool write(DataStream& stream, const String& filename) = 0;
   };
}

#endif // CONTENT_WRITER_H
