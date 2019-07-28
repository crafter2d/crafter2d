
#include "antlrstream.h"

#include "core/vfs/file.h"
#include "core/vfs/filesystem.h"
#include "core/string/string.h"

#include "yasc/compiler/output/yasLexer.h"

// static
AntlrStream* AntlrStream::fromFile(const String& filename)
{
   std::unique_ptr<File> file(FileSystem::getInstance().open(filename, File::ERead));
   return fromFile(*file);
}

AntlrStream* AntlrStream::fromFile(File& file)
{
   if ( !file.isValid() )
   {
      return nullptr;
   }

   String code;
   int size = file.size();
   std::unique_ptr<char[]> data(new char[size+1]);
   memset(data.get(), 0, size);
   int read = file.read(data.get(), size);
   code.setToUtf8(data.get(), read);
   
   return fromString(code);
}

// static
AntlrStream* AntlrStream::fromString(const String& code)
{
   std::string data = code.toUtf8();
   return new AntlrStream(data.c_str());
}

AntlrStream::AntlrStream(const std::string& data):
   mpStream(nullptr),
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
   if ( mpStream == nullptr )
   {
      mpStream = antlr3StringStreamNew((pANTLR3_UINT8)mData.c_str(), ANTLR3_ENC_UTF8, mData.length(), (pANTLR3_UINT8)"expression");
   }

   return mpStream;
}
