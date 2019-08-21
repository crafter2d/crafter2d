#ifndef XML_TOOLS_H
#define XML_TOOLS_H

#include "xml/xml_base.h"

class String;

namespace c2d::xml::tools
{
   XML_API String rootElement(const String& filename);
}

#endif // XML_TOOLS_H
