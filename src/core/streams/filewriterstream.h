
#ifndef FILE_WRITER_STREAM_H
#define FILE_WRITER_STREAM_H

#include "core/core_base.h"

#include "filestream.h"

class CORE_API FileWriterStream : public FileStream
{
public:
   FileWriterStream(File& file);

protected:
 // overrides
   virtual void  writeBytes(const void* pbuffer, int size);
};

#endif // FILE_WRITER_STREAM_H
