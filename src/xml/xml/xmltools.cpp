
#include "xmltools.h"

#include <tinyxml.h>

#include "core/string/string.h"

namespace c2d::xml::tools
{
   String rootElement(const String& filename)
   {
      TiXmlDocument doc(filename.toUtf8().c_str());
      if ( !doc.LoadFile() )
      {
         String error = UTEXT("File {0} is not an XML file.").arg(0, filename);
         throw std::runtime_error(error.toUtf8());
      }

      String value;
      TiXmlElement* proot = doc.FirstChildElement();
      if ( proot != nullptr )
      {
         value = String::fromUtf8(proot->Value());
      }
      return value;
   }
}
