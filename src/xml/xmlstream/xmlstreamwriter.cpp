
#include "xmlstreamwriter.h"

#include <tinyxml.h>
#include <queue>

#include "core/string/string.h"
#include "core/defines.h"

namespace c2d::xml
{
   class XmlStreamWriter::Impl
   {
   public:
      Impl() :
         doc(),
         stack(),
         pcurrentElement(nullptr)
      {
      }
      
      void save(const String& filename)
      {
         doc.SaveFile(filename.toUtf8());
      }

      void startElement(const String& name)
      {
         std::string sname = name.toUtf8();
         TiXmlElement element(sname);
         TiXmlNode* pnode = nullptr;

         if ( pcurrentElement )
            pnode = pcurrentElement->InsertEndChild(element);
         else
            pnode = doc.InsertEndChild(element);

         if ( pnode )
         {
            stack.push(pcurrentElement);
            pcurrentElement = pnode->ToElement();
            ASSERT_PTR(pcurrentElement);
         }
         else
         {
            throw std::runtime_error("could not add new element " + sname);
         }
      }

      void endElement()
      {
         pcurrentElement = stack.back();
         stack.pop();
      }

      void setAttribute(const String& name, int value)
      {
         ASSERT_PTR(pcurrentElement);
         pcurrentElement->SetAttribute(name.toUtf8(), value);
      }

      void setAttribute(const String& name, double value)
      {
         ASSERT_PTR(pcurrentElement);
         pcurrentElement->SetDoubleAttribute(name.toUtf8(), value);
      }

      void setAttribute(const String& name, const String& value)
      {
         ASSERT_PTR(pcurrentElement);
         pcurrentElement->SetAttribute(name.toUtf8(), value.toUtf8());
      }

      using ElementStack = std::queue<TiXmlElement*>;

      TiXmlDocument doc;
      ElementStack stack;
      TiXmlElement* pcurrentElement;
   };
   
   XmlStreamWriter::XmlStreamWriter():
      mpImpl(new Impl())
   {
   }

   XmlStreamWriter::~XmlStreamWriter()
   {
      delete mpImpl;
   }

   void XmlStreamWriter::startElement(const String& name)
   {
      mpImpl->startElement(name);
   }

   void XmlStreamWriter::endElement()
   {
      mpImpl->endElement();
   }

   void XmlStreamWriter::setAttribute(const String& name, int value)
   {
      mpImpl->setAttribute(name, value);
   }

   void XmlStreamWriter::setAttribute(const String& name, double value)
   {
      mpImpl->setAttribute(name, value);
   }

   void XmlStreamWriter::setAttribute(const String& name, const String& value)
   {
      mpImpl->setAttribute(name, value);
   }

   void XmlStreamWriter::save(const String& filename)
   {
      mpImpl->save(filename);
   }
}
