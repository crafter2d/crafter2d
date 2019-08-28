#ifndef COMPILER_H
#define COMPILER_H

#include "core/commandline/commandline.h"
#include "core/string/string.h"

namespace c2d
{
   class ContentModule;
   class ModuleManager;
}
class DataStream;

namespace c2d::compiler
{
   class Compiler
   {
   public:
      Compiler(int argc, char* parv[]);

      int exec();

   private:

      void init();
      int compile();
      void bundle();

      String determineExtension(const String& filename);
      void save(const String& filename, const DataStream& stream);
      c2d::ContentModule& resolveModule(c2d::ModuleManager& mgr, const String& extension);
      void findFiles();

    // members
      CommandLine mCommandLine;
      std::vector<String> mFiles;
      String      mSource;
      String      mDest;
      bool        mBundle;
      bool        mMove;
   };
}

#endif // COMPILER_H
