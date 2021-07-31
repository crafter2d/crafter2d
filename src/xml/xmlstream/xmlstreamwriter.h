#ifndef XML_STREAM_WRITER_H
#define XML_STREAM_WRITER_H

#include <xml/xml_base.h>

class String;

namespace c2d::xml
{
   class XML_API XmlStreamWriter final
   {
   public:
      XmlStreamWriter();
      ~XmlStreamWriter();

      void startElement(const String& name);
      void endElement();

      void setAttribute(const String& name, int value);
      void setAttribute(const String& name, double value);
      void setAttribute(const String& name, const String& value);

      void save(const String& filename);

   private:

      class Impl;

      Impl* mpImpl;
   };
}

#endif // XML_STREAM_WRITER_H
