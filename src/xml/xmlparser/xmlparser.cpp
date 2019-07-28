
#include "xmlparser.h"

#include <tinyxml.h>

#include "core/string/string.h"

#include "xmlparts.h"

namespace c2d::xml
{

   bool XmlParser::parse(Element & element, const String& filename)
   {
      std::string path = filename.toUtf8();
      TiXmlDocument doc(path);
      if ( doc.LoadFile() )
      {
         TiXmlElement* pxmlElement = doc.FirstChildElement(element.getName());
         if ( pxmlElement != nullptr )
         {
            parseElement(pxmlElement, element);
         }
         else
         {
            // report error
            return false;
         }
      }

      return true;
   }

   void XmlParser::parseElement(TiXmlElement* pxmlElement, Element& element)
   {
      for ( auto pattr : element.getAttributes() )
      {
         parseAttribute(pxmlElement, *pattr);
      }

      for ( auto pchild : element.getChildren() )
      {
         TiXmlElement* pxmlChild = pxmlElement->FirstChildElement(pchild->getName());

         if ( pxmlChild == nullptr && !pchild->mAllowEmpty )
         {
            // error!
            continue;
         }

         while ( pxmlChild != nullptr )
         {
            Element& instance = pchild->createInstance();
            parseElement(pxmlChild, instance);

            pxmlChild = pxmlChild->NextSiblingElement(pchild->getName());
         }

         if ( pchild->mRelation >= Child::eOne && !pchild->hasInstances() )
         {
            std::string msg = "Relation " + pchild->getName() + " has no elements";
         }
      }
   }

   void XmlParser::parseAttribute(TiXmlElement* pxmlElement, Attribute& attribute)
   {
      int result = TIXML_SUCCESS;
      switch ( attribute.mType )
      {
      case Attribute::eString:
         result = pxmlElement->QueryStringAttribute(attribute.getName().c_str(), &attribute.asString());
         break;
      case Attribute::eInt:
         result = pxmlElement->QueryIntAttribute(attribute.getName().c_str(), &attribute.asInt());
         break;
      case Attribute::eFloat:
         result = pxmlElement->QueryFloatAttribute(attribute.getName().c_str(), &attribute.asFloat());
         break;
      case Attribute::eBool:
         result = pxmlElement->QueryBoolAttribute(attribute.getName().c_str(), &attribute.asBool());
         break;
      }

      switch ( result )
      {
      case TIXML_SUCCESS:
         // should not get here!
         break;
      case TIXML_NO_ATTRIBUTE:
         if ( attribute.mRequirement == Attribute::eMandatory )
         {
            // error!
         }
         break;
      case TIXML_WRONG_TYPE:
         // error!
         break;
      default:
         // unknown error
         break;
      }
   }
}
