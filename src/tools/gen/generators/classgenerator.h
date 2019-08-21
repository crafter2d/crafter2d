
#ifndef CLASS_GENERATOR_H
#define CLASS_GENERATOR_H

#include "generator.h"

namespace c2d::gen
{
   class ClassGenerator : public Generator
   {
   public:
      // overrides
      UChar getBriefCommand() const override;
      const String& getCommand() const override;
      bool generate(const CommandLine& commandline) override;

   private:
      // helpers
      bool writeFile(const String& filename, const String& output);
   };
}

#endif // CLASS_GENERATOR_H
