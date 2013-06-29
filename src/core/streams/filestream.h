
#ifndef FILE_STREAM_H
#define FILE_STREAM_H

#include "datastream.h"
#include "bufferedstream.h"

class File;

class CORE_API FileStream : public DataStream
{
public:
   FileStream(File& file);
   virtual ~FileStream() = 0;

 // get/set
   File& getFile();

 // query
   virtual int         getDataSize() const;
   virtual const char* getData() const;

private:

 // operations
   void buffer();

   BufferedStream mBuffer;
   File& mFile;
};

#endif // FILE_STREAM_H
