
#ifndef COMPONENT_LOADER_H
#define COMPONENT_LOADER_H

class ComponentDefinition;
class String;
class TiXmlElement;

class ComponentLoader
{
public:

   ComponentLoader();
   virtual ~ComponentLoader() = 0;

 // operation
   virtual const String& getId() const = 0;
   virtual ComponentDefinition* load(const TiXmlElement& element) = 0;
};


#endif // COMPONENT_LOADER_H
