
#ifndef GENERATOR_H
#define GENERATOR_H

#include "core/string/char.h"

class String;
class CommandLine;

namespace c2d::gen
{
   class Generator
   {
   public:
      Generator();
      virtual ~Generator();

      // interface
      virtual UChar getBriefCommand() const = 0;
      virtual const String& getCommand() const = 0;
      virtual bool generate(const CommandLine& commandline) = 0;

   protected:

      // query
      String getWorkingDir() const;
      String getTemplateFile(const String& tpl) const;

   };
}

#define GENERATOR_IMPL(c) static c impl

#endif // GENERATOR_H
