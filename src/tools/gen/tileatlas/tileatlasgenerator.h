#ifndef TILEATLAS_GENERATOR_H
#define TILEATLAS_GENERATOR_H

#include "../generators/generator.h"

namespace c2d::gen
{
   class TileAtlasGenerator : public Generator
   {
   public:

    // overrides
      UChar getBriefCommand() const override;
      const String& getCommand() const override;
      bool generate(CommandLine& commandline) override;
   };
}

#endif // TILEATLAS_GENERATOR_H
