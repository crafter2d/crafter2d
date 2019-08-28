
#ifndef FILE_READER_STREAM_H
#define FILE_READER_STREAM_H

#include "bufferedstream.h"
#include "filestream.h"

class CORE_API FileReaderStream : public FileStream
{
public:
   FileReaderStream(File& file);
   virtual ~FileReaderStream();

 // query 
   virtual int         getDataSize() const override;
   virtual const char* getData() const override;
           char*       useData();
   
protected:
 // reading interface
   virtual       void  readBytes(void* pbuffer, int amount);
   virtual const char* readBytes(int amount);
   virtual       char  readByte();

private:
 // buffering
   void buffer();

 // data
   char* mpData;
   int   mSize;
   int   mPos;
};

#endif // FILE_READER_STREAM_H
