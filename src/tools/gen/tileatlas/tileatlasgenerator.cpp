
#include "tileatlasgenerator.h"

#include "core/string/string.h"

namespace c2d::gen
{
   GENERATOR_IMPL(TileAtlasGenerator);

   UChar TileAtlasGenerator::getBriefCommand() const
   {
      return L't';
   }

   const String& TileAtlasGenerator::getCommand() const
   {
      static const String sCommand(UTEXT("tileatlas"));
      return sCommand;
   }

   bool TileAtlasGenerator::generate(CommandLine& commandline)
   {
      return true;
   }
}
