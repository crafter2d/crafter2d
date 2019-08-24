
#ifndef CONTENT_MODULE_H
#define CONTENT_MODULE_H

#include <vector>

#include "core/string/string.h"
#include "core/modules/module.h"
#include "core/defines.h"

namespace c2d
{
   class ContentReader;
   class ContentWriter;

   /**
    * A content module is a collection containing a compiler, reader and writer.
    *
    * The compiler is responsible for compiling the human readable file format to the
    * Crafter 2D content file format.
    *
    * @see IContentCompiler
    */

   class CORE_API ContentModule : public Module
   {
   public:
      explicit ContentModule(const Uuid& uuid);
      virtual ~ContentModule();

      // get/set
      ContentReader& getReader();
      void           setReader(ContentReader* preader);

      ContentWriter& getWriter();
      void           setWriter(ContentWriter* pwriter);

      const String& getSupportedFiles() const;
      void          setSupportedFiles(const String& supportedfiles);

      // query
      bool supports(const String& ext) const;

   private:

      // data
      std::vector<String>  mSupportedFiles;
      ContentReader*       mpReader;
      ContentWriter*       mpWriter;
   };
}

#endif // CONTENT_MODULE_H
