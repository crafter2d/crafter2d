
#include "tileatlasgenerator.h"

#include "core/commandline/commandline.h"
#include "core/vfs/filesystem.h"
#include "core/vfs/file.h"
#include "core/string/string.h"

#include "imagenode.h"
#include "maxrectsalgorithm.h"

namespace c2d::gen
{
   GENERATOR_IMPL(TileAtlasGenerator);

   TileAtlasGenerator::TileAtlasGenerator():
      mName(),
      mPath(),
      mOut(),
      mSheetIndex(1)
   {
   }

   UChar TileAtlasGenerator::getBriefCommand() const
   {
      return L't';
   }

   const String& TileAtlasGenerator::getCommand() const
   {
      static const String sCommand(UTEXT("tileatlas"));
      return sCommand;
   }

   bool TileAtlasGenerator::generate(const CommandLine& commandline)
   {
      setup(commandline);

      std::vector<ImageNode> nodes, usednodes;
      std::vector<String> images;
      FileSystem::getInstance().find(File::concat(mPath, UTEXT("*.png")), images, true);
      for ( auto& image : images )
      {
         nodes.emplace_back(std::move(image));
      }

      xml::XmlStreamWriter writer;
      writer.startElement(UTEXT("tileatlas"));

      do
      {
         MaxRectsAlgorithm algorithm(2048, 2048);
         algorithm.insert(nodes, usednodes);

         if ( !usednodes.empty() )
         {
            createSheet(writer, usednodes);
         }
      }
      while ( !nodes.empty() );

      writer.save(File::concat(mOut, mName + UTEXT("atlas.xml")));

      return true;
   }

   bool TileAtlasGenerator::setup(const CommandLine& commandline)
   {
      auto pargument = commandline.getArgument(UTEXT("path"));
      if ( !pargument )
         return false;
      mPath = pargument->getValue();

      pargument = commandline.getArgument(UTEXT("name"));
      if ( !pargument )
         return false;
      mName = pargument->getValue();

      pargument = commandline.getArgument(UTEXT("out"));
      if ( !pargument )
         return false;
      mOut = pargument->getValue();

      return true;
   }

   void TileAtlasGenerator::createSheet(xml::XmlStreamWriter& writer, const std::vector<ImageNode>& images)
   {
      Image sheet(2048, 2048, 4);

      String sheetname = mName + UTEXT("sheet_{0}").arg(0, mSheetIndex++);

      writer.startElement(UTEXT("tilesheet"));
      writer.setAttribute(UTEXT("name"), sheetname);
      writer.setAttribute(UTEXT("width"), 2048);
      writer.setAttribute(UTEXT("height"), 2048);

      for ( const ImageNode& node : images )
      {
         if ( node.mRotated )
            sheet.paintRotated90(node.mCoords.left, node.mCoords.top, node.mImage);
         else
            sheet.paint(node.mCoords.left, node.mCoords.top, node.mImage);

         String name = node.mFilename.right(mPath.length() + 1);

         writer.startElement(UTEXT("tile"));
         writer.setAttribute(UTEXT("name"), name);
         writer.setAttribute(UTEXT("x"), node.mCoords.left);
         writer.setAttribute(UTEXT("y"), node.mCoords.top);
         writer.setAttribute(UTEXT("w"), node.mCoords.width());
         writer.setAttribute(UTEXT("h"), node.mCoords.height());
         if ( node.mRotated )
         {
            writer.setAttribute(UTEXT("rotated"), UTEXT("true"));
         }
         writer.endElement();
      }

      writer.endElement();

      sheet.save(File::concat(mOut, sheetname + UTEXT(".png")));
   }
}
