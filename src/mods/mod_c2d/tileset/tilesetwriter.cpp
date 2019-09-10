
#include "tilesetwriter.h"

#include <vector>

#include "core/log/log.h"
#include "core/streams/datastream.h"
#include "core/world/tileset.h"

#include "xml/xmlparser/xmlparser.h"
#include "xml/xmlparser/xmlparts.h"

using namespace c2d::xml;

namespace local
{
   ELEMENT_DEF(tile)

      ATTRIBUTE(int, id)
      ATTRIBUTE(std::string, name)
      ATTRIBUTE(float, offsetx)
      ATTRIBUTE(float, offsety)

   ELEMENT_IMP(tile)

      ATTRIBUTE_INIT(id, Attribute::eMandatory, Attribute::eInt, -1)
      ATTRIBUTE_INIT(name, Attribute::eMandatory, Attribute::eString, "")
      ATTRIBUTE_INIT(offsetx, Attribute::eOptional, Attribute::eFloat, 0)
      ATTRIBUTE_INIT(offsety, Attribute::eOptional, Attribute::eFloat, 0)

   ELEMENT_END

   ELEMENT_DEF(tileset)

      ATTRIBUTE(int, tilewidth)
      ATTRIBUTE(int, tileheight)

      CHILD(tile);

   ELEMENT_IMP(tileset)

      ATTRIBUTE_INIT(tilewidth, Attribute::eMandatory, Attribute::eInt, -1)
      ATTRIBUTE_INIT(tileheight, Attribute::eMandatory, Attribute::eInt, -1)

      CHILD_INIT(tile, Child::eOneOrAny, false)

   ELEMENT_END
}


c2d::ContentWriter::Result TileSetWriter::write(DataStream& stream, const String& filename)
{
   XmlParser parser;
   local::tileset ts;

   if ( parser.parse(ts, filename) )
   {
      stream << ts.tilewidth << ts.tileheight << (int)ts.tiles.size();
      
      for ( auto& tile : ts.tiles )
      {
         String name(tile.name);
         stream << name << tile.offsetx << tile.offsety;
      }
   }

   return eOk;
}
