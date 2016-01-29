
#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <string>

class String;
class TiXmlElement;

namespace c2d
{
   class Attribute;
   class Element;

   class XmlParser
   {
   public:

      bool parse(Element& element, const String& filename);

   private:

      void parseElement(TiXmlElement* pxmlElement, Element& element);
      void parseAttribute(TiXmlElement* pxmlElement, Attribute& attribute);
   };
}

#endif // XML_PARSER_H

