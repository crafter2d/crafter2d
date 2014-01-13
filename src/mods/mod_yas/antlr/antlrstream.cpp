
#include "antlrstream.h"

#include "script/output/asLexer.h"

#include "core/vfs/file.h"
#include "core/vfs/filesystem.h"
#include "core/smartptr/autoptr.h"
#include "core/string/string.h"

// static
AntlrStream* AntlrStream::fromFile(const String& filename)
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
   std::string data = code.toUtf8();
   return new AntlrStream(data.c_str());
}

AntlrStream::AntlrStream(const std::string& data):
   mpStream(NULL),
   mData(data)
{

}

AntlrStream::~AntlrStream()
{
   mpStream->close(mpStream);
}

// - Get/set

ANTLR3_INPUT_STREAM_struct* AntlrStream::getStream() const
{
   if ( mpStream == NULL )
   {
      mpStream = antlr3StringStreamNew((pANTLR3_UINT8)mData.c_str(), ANTLR3_ENC_UTF8, mData.length(), (pANTLR3_UINT8)"expression");
   }

   return mpStream;
}
