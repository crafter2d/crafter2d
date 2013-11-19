
#ifndef COMPONENT_LOADER_H
#define COMPONENT_LOADER_H

class ComponentDefinitionProto;
class String;
class TiXmlElement;

class ComponentLoader
{
public:

   ComponentLoader();
   virtual ~ComponentLoader() = 0;

 // operation
   virtual const String& getXmlTag() const = 0;
   virtual ComponentDefinitionProto* load(const TiXmlElement& element) = 0;
};


#endif // COMPONENT_LOADER_H
