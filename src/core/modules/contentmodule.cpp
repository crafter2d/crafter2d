
#include "contentmodule.h"

#include "core/content/contentreader.h"
#include "core/content/contentwriter.h"
#include "core/defines.h"

namespace c2d
{
   ContentModule::ContentModule(const Uuid& uuid) :
      Module(eContentModule, uuid),
      mSupportedFiles(),
      mpReader(nullptr),
      mpWriter(nullptr)
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

   // - Query

   bool ContentModule::supports(const String& ext) const
   {
      return mSupportedFiles.indexOf(ext) != String::npos;
   }
}