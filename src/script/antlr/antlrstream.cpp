
#include "antlrstream.h"

#include "script/output/asLexer.h"

#include "core/vfs/file.h"
#include "core/vfs/filesystem.h"
#include "core/smartptr/autoptr.h"
#include "core/string/string.h"

// static
AntlrStream* AntlrStream::fromFile(const std::string& filename)
{
   AutoPtr<File> file = FileSystem::getInstance().open(filename, File::ERead);
   if ( !file.hasPointer() || !file->isValid() )
   {
      return NULL;
   }

   int size = file->size();
   AutoPtr<char> data = new char[size+1];
   memset(data.getPointer(), 0, size);
   file->read(data.getPointer(), size);
   String code(data.getPointer());
   
   return fromString(code);
}

// static
AntlrStream* AntlrStream::fromString(const String& code)
{
   int length = 0;
   char* pdata = code.toUtf8(length);

   pANTLR3_INPUT_STREAM input = antlr3StringStreamNew((pANTLR3_UINT8)pdata, ANTLR3_ENC_UTF8, length, (pANTLR3_UINT8)"expression");
   if ( input == NULL )
   {
      return NULL;
   }

   return new AntlrStream(input, pdata);
}

AntlrStream::AntlrStream(ANTLR3_INPUT_STREAM_struct* pstream, char* pdata):
   mpStream(pstream),
   mpData(pdata)
{
}

AntlrStream::~AntlrStream()
{
   mpStream->close(mpStream);

   delete[] mpData;
}

// - Get/set

ANTLR3_INPUT_STREAM_struct* AntlrStream::getStream() const
{
   return mpStream;
}
