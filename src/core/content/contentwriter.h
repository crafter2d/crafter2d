
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
      enum Result { eOk, eOkNoStore, eFail };

      ContentWriter();
      virtual ~ContentWriter();

      virtual Result write(DataStream& stream, const String& filename) = 0;
   };
}

#endif // CONTENT_WRITER_H
