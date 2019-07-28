
#include <string>

#include "themewriter.h"

#include "core/streams/datastream.h"
#include "core/string/string.h"

#include "xml/xmlparser/xmlparser.h"
#include "xml/xmlparser/xmlparts.h"

namespace c2d
{
   using namespace xml;

   namespace xml_theme_definition
   {
      ELEMENT_DEF(theme)
         ATTRIBUTE(std::string, atlas)
      ELEMENT_IMP(theme)
         ATTRIBUTE_INIT(atlas, Attribute::eMandatory, Attribute::eString, "")
      ELEMENT_END
   }

   bool ThemeWriter::write(DataStream& stream, const String& filename)
   {
      XmlParser parser;
      xml_theme_definition::theme t;

      if ( parser.parse(t, filename) )
      {
         String atlas(t.atlas);
         stream << atlas;
      }
      
      return true;
   }
}
