
#include <stdio.h>
#include <iostream>
#include "generator.h"

#ifdef WIN32
#include <direct.h>
#elif LINUX
#include <unistd.h>
#endif

#include "core/string/string.h"
#include "core/vfs/file.h"
#include "core/defines.h"

#include "generatormanager.h"

namespace c2d::gen
{
   Generator::Generator()
   {
      GeneratorManager::getInstance().registerGenerator(*this);
   }

   Generator::~Generator()
   {
   }

   // - Query

   String Generator::getWorkingDir() const
   {
      char cCurrentPath[FILENAME_MAX];

      if ( !getcwd(cCurrentPath, sizeof(cCurrentPath)) )
      {
         return String::empty();
      }

      cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

      return String::fromUtf8(cCurrentPath);
   }

   String Generator::getTemplateFile(const String& tpl) const
   {
      return File::concat(File::concat(getWorkingDir(), UTEXT("../projects/templates")), tpl + UTEXT(".tpl"));
   }
}
