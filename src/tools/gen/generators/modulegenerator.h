
#ifndef MODULE_GENERATOR_H
#define MODULE_GENERATOR_H

#include "generator.h"

class String;

namespace c2d::gen
{
   class ModuleGenerator : public Generator
   {
   public:
    // overrides
      UChar getBriefCommand() const override;
      const String& getCommand() const override;
      virtual bool generate(const CommandLine& commandline) override;

   private:

      // file generation
      bool generateContentModule(const CommandLine& commandline, const String& path, const String& name);
   };
}

#endif // MODULE_GENERATOR_H
