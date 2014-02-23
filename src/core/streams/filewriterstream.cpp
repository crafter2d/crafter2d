
#include "filewriterstream.h"

#include "core/vfs/file.h"

FileWriterStream::FileWriterStream(File& file):
   FileStream(file)
{
}

void FileWriterStream::writeBytes(const void* pdata, int size)
{
   getFile().write(pdata, size);
}
