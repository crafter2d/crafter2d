
#ifndef PROJECT_GENERATOR_H
#define PROJECT_GENERATOR_H

#include "generator.h"

class String;

namespace c2d::gen
{
   class ProjectGenerator : public Generator
   {
   public:
      ProjectGenerator();

    // overrides
      UChar getBriefCommand() const override;
      const String& getCommand() const override;
      bool generate(const CommandLine& commandline) override;

   private:
      // operations
      bool write(const String& filename, const String& output, bool force);
   };
}

#endif // PROJECT_GENERATOR_H
