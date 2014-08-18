
#include "antlrstream.h"

#include "core/vfs/file.h"
#include "core/vfs/filesystem.h"
#include "core/smartptr/autoptr.h"
#include "core/string/string.h"

#include "yasc/compiler/output/yasLexer.h"

// static
AntlrStream* AntlrStream::fromFile(const String& filename)
{
   AutoPtr<File> file = FileSystem::getInstance().open(filename, File::ERead);
   return fromFile(*file);
}

AntlrStream* AntlrStream::fromFile(File& file)
{
   if ( !file.isValid() )
   {
      return NULL;
   }

   String code;
   int size = file.size();
   AutoPtr<char> data = new char[size+1];
   memset(data.getPointer(), 0, size);
   int read = file.read(data.getPointer(), size);
   code.setToUtf8(data.getPointer(), read);
   
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
