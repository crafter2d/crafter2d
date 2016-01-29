
#include "spritesheetwriter.h"

#include <memory>
#include <vector>
#include <tinyxml.h>

#include "core/log/log.h"
#include "core/streams/datastream.h"
#include "core/string/string.h"

#include "../xmlparser/xmlparser.h"
#include "../xmlparser/xmlparts.h"

namespace c2d
{
   ELEMENT_DEF(Sprite)

      ATTRIBUTE(std::string, n)
      ATTRIBUTE(int, x)
      ATTRIBUTE(int, y)
      ATTRIBUTE(int, w)
      ATTRIBUTE(int, h)

   ELEMENT_IMP(Sprite)

      ATTRIBUTE_INIT(n, Attribute::eMandatory, Attribute::eString, "")
      ATTRIBUTE_INIT(x, Attribute::eMandatory, Attribute::eInt, 0)
      ATTRIBUTE_INIT(y, Attribute::eMandatory, Attribute::eInt, 0)
      ATTRIBUTE_INIT(w, Attribute::eMandatory, Attribute::eInt, 0)
      ATTRIBUTE_INIT(h, Attribute::eMandatory, Attribute::eInt, 0)

   ELEMENT_END

   ELEMENT_DEF(TextureAtlas)

      ATTRIBUTE(std::string, imagePath)
      ATTRIBUTE(int, width)
      ATTRIBUTE(int, height)

      CHILD(Sprite, sprite)

   ELEMENT_IMP(TextureAtlas)

      ATTRIBUTE_INIT(imagePath, Attribute::eMandatory, Attribute::eString, "")
      ATTRIBUTE_INIT(width, Attribute::eMandatory, Attribute::eInt, 0)
      ATTRIBUTE_INIT(height, Attribute::eMandatory, Attribute::eInt, 0)

      CHILD_INIT(Sprite, sprite, Child::eOneOrAny, false)

   ELEMENT_END
}

bool c2d::SpriteSheetWriter::write(DataStream & stream, const String & filename)
{
   XmlParser parser;
   TextureAtlas atlas;

   if ( parser.parse(atlas, filename) )
   {
      // convert the atlas to c2d::SpriteSheet
      // save the sprite sheet
   }
   
   return true;
}
