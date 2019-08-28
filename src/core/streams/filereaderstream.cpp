
#include "filereaderstream.h"

#include <cstring>

#include "core/vfs/file.h"
#include "core/defines.h"

FileReaderStream::FileReaderStream(File& file):
   FileStream(file),
   mpData(nullptr),
   mSize(file.size()),
   mPos(0)
{
   buffer();
}

FileReaderStream::~FileReaderStream()
{
   delete[] mpData;
   mpData = 0;
   mSize  = 0;
}

int FileReaderStream::getDataSize() const
{
   return mSize;
}

const char* FileReaderStream::getData() const
{
   return mpData;
}

char* FileReaderStream::useData()
{
   char* pdata = nullptr;
   std::swap(pdata, mpData);
   mSize = 0;
   mPos = 0;
   return pdata;
}

// - Operations

void FileReaderStream::buffer()
{
   mpData = new char[mSize];
   getFile().read(mpData, mSize);
}

// - Reading

void FileReaderStream::readBytes(void* pbuffer, int amount)
{
   memcpy(pbuffer, &mpData[mPos], amount);
   mPos += amount;
}

// virtual 
const char* FileReaderStream::readBytes(int amount)
{
   const char* presult = &mpData[mPos];
   mPos += amount;
   return presult;
}

char FileReaderStream::readByte()
{
   return mpData[mPos++];
}
