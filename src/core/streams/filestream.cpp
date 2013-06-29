
#include "filestream.h"

#include "core/vfs/file.h"

FileStream::FileStream(File& file):
   DataStream(),
   mFile(file)
{
}

FileStream::~FileStream()
{
}

// - Get/set

File& FileStream::getFile()
{
   return mFile;
}

// - Query

// getting the data directly is not supported by file streams

int FileStream::getDataSize() const 
{
   return -1;
}

const char* FileStream::getData() const
{
   // not supported!
   return NULL;
}

// - Buffering

