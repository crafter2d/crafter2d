
#include "contentmodule.h"

#include "core/streams/bufferedstream.h"
#include "core/vfs/stdiofile.h"
#include "core/defines.h"

#include "contentreader.h"
#include "contentwriter.h"

ContentModule::ContentModule():
   Module(Module::eContent),
   mUuid(),
   mSupportedFiles(),
   mpReader(NULL),
   mpWriter(NULL)
{
}

ContentModule::~ContentModule()
{
   delete mpReader;
   delete mpWriter;
}

// - Get/set

ContentReader& ContentModule::getReader()
{
   ASSERT_PTR(mpReader);
   return *mpReader;
}

void ContentModule::setReader(ContentReader* preader)
{
   mpReader = preader;
}

ContentWriter& ContentModule::getWriter()
{
   ASSERT_PTR(mpWriter);
   return *mpWriter;
}

void ContentModule::setWriter(ContentWriter* pwriter)
{
   mpWriter = pwriter;
}

const String& ContentModule::getSupportedFiles() const
{
   return mSupportedFiles;
}

void ContentModule::setSupportedFiles(const String& supportedfiles)
{
   mSupportedFiles = supportedfiles;
}

const Uuid& ContentModule::getUuid() const
{
   return mUuid;
}

void ContentModule::setUuid(const Uuid& uuid)
{
   mUuid = uuid;
}

// - Query

bool ContentModule::supports(const String& ext) const
{
   return mSupportedFiles.indexOf(ext) > -1;
}

// - Operations

void ContentModule::write(const String& source, const String& dest)
{
   ASSERT_PTR(mpWriter);

   BufferedStream stream;
   mUuid.write(stream);
   
   try
   {
      if ( mpWriter->write(stream, source) )
      {
         StdioFile file;
         file.open(dest, File::EBinary | File::EWrite);
         file.write(stream.getData(), stream.getDataSize());
         file.close();
      }
   }
   catch ( std::exception* )
   {
   }
}
