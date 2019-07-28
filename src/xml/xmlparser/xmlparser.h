
#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <xml/xml_base.h>

#include <string>

class String;
class TiXmlElement;

namespace c2d::xml
{
   class Attribute;
   class Element;

   class XML_API XmlParser
   {
   public:

      bool parse(Element& element, const String& filename);

   private:

      void parseElement(TiXmlElement* pxmlElement, Element& element);
      void parseAttribute(TiXmlElement* pxmlElement, Attribute& attribute);
   };
}

#endif // XML_PARSER_H

