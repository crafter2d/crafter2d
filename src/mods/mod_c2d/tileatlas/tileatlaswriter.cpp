
#include "tileatlaswriter.h"

#include <memory>
#include <vector>

#include "core/log/log.h"
#include "core/streams/datastream.h"
#include "core/string/string.h"

#include "xml/xmlparser/xmlparser.h"
#include "xml/xmlparser/xmlparts.h"

using namespace c2d::xml;

namespace local_atlas
{
   ELEMENT_DEF(tile)

      ATTRIBUTE(std::string, name)
      ATTRIBUTE(float, x)
      ATTRIBUTE(float, y)
      ATTRIBUTE(float, w)
      ATTRIBUTE(float, h)
      ATTRIBUTE(bool, r)

   ELEMENT_IMP(tile)

      ATTRIBUTE_INIT(name, Attribute::eMandatory, Attribute::eString, "")
      ATTRIBUTE_INIT(x, Attribute::eMandatory, Attribute::eFloat, 0)
      ATTRIBUTE_INIT(y, Attribute::eMandatory, Attribute::eFloat, 0)
      ATTRIBUTE_INIT(w, Attribute::eMandatory, Attribute::eFloat, 0)
      ATTRIBUTE_INIT(h, Attribute::eMandatory, Attribute::eFloat, 0)
      ATTRIBUTE_INIT(r, Attribute::eOptional, Attribute::eBool, false)

   ELEMENT_END

   ELEMENT_DEF(tilesheet)

      ATTRIBUTE(std::string, name)
      ATTRIBUTE(float, width)
      ATTRIBUTE(float, height)

      CHILD(tile)

   ELEMENT_IMP(tilesheet)

      ATTRIBUTE_INIT(name, Attribute::eMandatory, Attribute::eString, "")
      ATTRIBUTE_INIT(width, Attribute::eMandatory, Attribute::eFloat, 0)
      ATTRIBUTE_INIT(height, Attribute::eMandatory, Attribute::eFloat, 0)

      CHILD_INIT(tile, Child::eOneOrAny)

   ELEMENT_END

   ELEMENT_DEF(tileatlas)

      CHILD(tilesheet)

   ELEMENT_IMP(tileatlas)

      CHILD_INIT(tilesheet, Child::eOneOrAny)

   ELEMENT_END
}

c2d::ContentWriter::Result c2d::TileAtlasWriter::write(DataStream & stream, const String & filename)
{
   XmlParser parser;
   local_atlas::tileatlas atlas;

   if ( parser.parse(atlas, filename) )
   {
      stream.writeInt(atlas.tilesheets.size());
      for ( auto& sheet : atlas.tilesheets )
      {
         stream.writeString(String(sheet.name));
         stream.writeInt(sheet.tiles.size());

         for ( auto& tile : sheet.tiles )
         {
            String name(tile.name);
            float left = tile.x / sheet.width;
            float top = tile.y / sheet.height;
            float right = (tile.x + tile.w) / sheet.width;
            float bottom = (tile.y + tile.h) / sheet.height;

            stream << name << left << right << top << bottom << tile.r;
         }
      }
   }
   
   return eOk;
}
