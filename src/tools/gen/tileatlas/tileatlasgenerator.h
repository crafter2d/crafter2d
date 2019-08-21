#ifndef TILEATLAS_GENERATOR_H
#define TILEATLAS_GENERATOR_H

#include <vector>

#include "xml/xmlstream/xmlstreamwriter.h"
#include "core/string/string.h"

#include "../generators/generator.h"

namespace c2d::gen
{
   class ImageNode;

   class xml::XmlStreamWriter;

   class TileAtlasGenerator : public Generator
   {
   public:
      TileAtlasGenerator();

    // overrides
      UChar getBriefCommand() const override;
      const String& getCommand() const override;
      bool generate(const CommandLine& commandline) override;

   private:

      bool setup(const CommandLine& commandline);
      void createSheet(xml::XmlStreamWriter& writer, const std::vector<ImageNode>& images);

    // members
      String mName;
      String mPath;
      String mOut;
      int mSheetIndex;
   };
}

#endif // TILEATLAS_GENERATOR_H
